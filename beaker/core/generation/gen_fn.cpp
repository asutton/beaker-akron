// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <sstream>


namespace beaker {
namespace core {

using arg_iterator = llvm::Function::arg_iterator;

// If needed, adjust parameter information the return type. Returns an iterator
// referring to the first non-parameter function.
static arg_iterator
adjust_return_parm(generator& gen, const decl& d)
{
  llvm::Function* fn = gen.get_function();
  arg_iterator ai = fn->arg_begin();
 
  const type& t = get_declared_type(d);
  if (is_object_type(t)) {
    cg::type ret = generate(gen, t);
    if (ret.is_indirect()) {
      // If the return type is indirect, the first parameter is the caller's
      // return object. The first actual argument is past this return.
      llvm::Argument& arg = *ai++;
      std::string name = generate(gen, get_declaration_name(d));
      arg.setName(name);

      // Indicate that the return value is a struct return. 
      //
      // TODO: Is this actually valid on non-struct types?
      fn->addAttribute(arg.getArgNo() + 1, llvm::Attribute::StructRet);

      // Set the return value.
      gen.set_return_value(&arg);
    }
    else {
      // Allocate a local return value.
      //
      // FIXME: If the function is defined by an expression, then we don't
      // need to generate the object.
      llvm::Builder ir(gen.get_entry_block());
      cg::type ret = generate(gen, t);
      std::string name = generate(gen, get_declaration_name(d));
      cg::value ptr = ir.CreateAlloca(ret, nullptr, name);

      // Set the return value.
      gen.set_return_value(ptr);
    }
  }
  return ai;
}

static void
adjust_function_parm(generator& gen, const decl& d, arg_iterator ai)
{
  llvm::Function* fn = gen.get_function();
  llvm::Argument& arg = *ai;

  // Set the name of the argument and create a binding for it.
  std::string name = generate(gen, get_declaration_name(d));
  arg.setName(name);

  const type& t = get_declared_type(d);
  if (is_object_type(t)) {
    cg::type type = generate(gen, t);
    if (type.is_indirect()) {
      // Note that byval implies that a copy has actually been created
      // and that the calling function is free to modify it. When types
      // are passed directly, the copy is made by the callee.
      //
      // TODO: nonnull is not sufficiently strong for indirect arguments.
      // It should be dereferenceable.
      //
      // TODO: If the parameter is const, then we should be able to omit
      // the copy, because the callee is not permitted to modify it. However,
      // we don't have const types in core.
      fn->addAttribute(arg.getArgNo() + 1, llvm::Attribute::ByVal);
      fn->addAttribute(arg.getArgNo() + 1, llvm::Attribute::NonNull);

      // Bind the parameter directly to the argument.
      gen.put_value(d, &arg);
    }
    else {
      // Create local storage for the parameter variable. Note that we don't 
      // need complex initialization because the argument is passed directly.
      llvm::Builder ir(gen.get_entry_block());
      std::stringstream ss;
      cg::value ptr = ir.CreateAlloca(type, nullptr, "var." + name);
      llvm::Builder cur(gen.get_current_block());
      cur.CreateStore(&arg, ptr);
      
      // Bind the parameter to the automatic storage.
      gen.put_value(d, ptr);
    }
  }
  else if (is_reference_type(t)) {
    // Arguments passed by reference are guaranteed to be nonnull.
    //
    // TODO: See above. This is also dereferenceable.
    fn->addAttribute(arg.getArgNo() + 1, llvm::Attribute::NonNull);

    // Bind the parameter directly to the argument.
    gen.put_value(d, &arg);
  }
}


// Generate formal parameters.
static void
generate_parms(generator& gen, const fn_decl& d)
{
  // Adjust the return parameter if needed.
  auto ai = adjust_return_parm(gen, d.get_return());

  // Generate/annotate each parameter in turn.
  for (const decl& parm : d.get_parameters())
    adjust_function_parm(gen, parm, ai++);
}

// Generate a function definition that is an expression `e`. This is equivalent
// to a function whose definition is `{ return e; }`. As with return statements,
// we expect that `e` to be a void expression or an initialization of the
// return value.
static void
generate_expr_def(generator& gen, const fn_decl& decl, const expr& def)
{
  // Simply initialize the return value.
  generator::init_guard guard(gen, gen.get_return_value());
  generate(gen, def);

  // And jump to the exit block.
  llvm::Builder ir(gen.get_current_block());
  ir.CreateBr(gen.get_exit_block());
}

// Generate a function definition that is a (block) statement.
//
// When the definition is a statement, we need to allocate the return variable.
//
// FIXME: If the return value is a register, we don't need to allocate anything,
// and returns don't need to branch to the exit block.
//
// FIXME: If the return type is void, then the return generation is going to
// be a little different.
static void
generate_stmt_def(generator& gen, const fn_decl& decl, const stmt& def)
{
  // Generate the statement body.
  generate(gen, def);

  // Insert a terminator, if needed.
  llvm::BasicBlock* last = gen.get_current_block();
  if (!last->getTerminator()) {
    llvm::Builder ir(last);
    ir.CreateBr(gen.get_exit_block());
  }
}

// Choose the right kind of definition to emit.
static void
generate_body(generator& gen, const fn_decl& d)
{
  defn def = d.get_definition();
  if (def.get_kind() == fn_decl::expr_defn)
    generate_expr_def(gen, d, def.get_as<expr>());
  else if (def.get_kind() == fn_decl::stmt_defn)
    generate_stmt_def(gen, d, def.get_as<stmt>());
  else
    assert(false && "unsupported definition");
}

// Generate the final return from the function.
static void
generate_exit(generator& gen, const fn_decl& d)
{
  llvm::Builder ir(gen.get_exit_block());    

  const type& t = d.get_return_type();
  if (is_void_type(t)) {
    // For void returns, do nothing.
    ir.CreateRetVoid();
  }
  else if (is_object_type(t)) {
    // Return by value depends on whether the return is direct or indirect.
    cg::type ret = generate(gen, t);
    if (ret.is_direct()) {
      // Load the value so it can be returned directly.
      cg::value ret = ir.CreateLoad(gen.get_return_value());
      ir.CreateRet(ret);
    }
    else {
      // We've stored the return value in the caller's frame. Just exit.
      ir.CreateRetVoid();
    }
  }
  else {
    assert(false && "return by reference not implemented");
  }
}

// Generate a function definition.
//
// TODO: Don't require a separate exit block unless there is an early
// return statement.
static void
generate_fn_def(generator& gen, const fn_decl& d, llvm::Function* f)
{
  // If the function isn't defined, there's nothing to do.
  defn def = d.get_definition();
  if (def.is_remote())
    return;

  // Generate a local definition.
  gen.define_function(f);
  generate_parms(gen, d);
  generate_body(gen, d);  
  generate_exit(gen, d);
  gen.end_function();
}

// Generate an LLVM function from d.
static cg::value
generate_fn_decl(generator& gen, const fn_decl& d)
{
  std::string name = generate(gen, d.get_name());

  // Determine the linkage of the name.
  //
  // TODO: We probably want an explicit notion of name linkage so
  // that we can make better decisions here.
  llvm::Function::LinkageTypes linkage = llvm::Function::ExternalLinkage;

  // Get the function type.
  llvm::Type* type = generate(gen, d.get_type());

  // Create and declare the function.
  llvm::Function* fn = llvm::Function::Create(
    llvm::cast<llvm::FunctionType>(type), 
    linkage, 
    name, 
    &gen.get_module()
  );
  gen.put_value(d, fn);

  // Emit the definition (if needed).
  generate_fn_def(gen, d, fn);
  return fn;
}

} // namespace core
} // namespace beaker

// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/core/type.hpp>
#include <beaker/core/decl.hpp>
#include <beaker/base/generation/generation.hpp>
#include <beaker/base/printing/print.hpp>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

#include <sstream>


namespace beaker {
namespace core {

// Returns cg information for the function.
static cg::fn_info&
get_function_info(generator& gen, const fn_decl& d)
{
  cg::type type = gen.get_type(d.get_type());
  cg::fn_info* info = type.get_note<cg::fn_info>();
  assert(info != nullptr);
  return *info; 
}

// Adjust the return value when it's passed as an argument.
static void
generate_return_value(generator& gen, llvm::Argument* arg)
{
  arg->setName("result");
  gen.set_return_value(arg);
}

// Allocate local storage for the return value.
static void
generate_return_value(generator& gen, const fn_decl& fn)
{
  llvm::Builder ir(gen.get_entry_block());
  cg::type type = generate(gen, fn.get_return_type());
  cg::value ret = ir.CreateAlloca(type, nullptr, "result");
  gen.set_return_value(ret);
}

// Generate automatic storage for variable parameters. 
//
// Note that the parameter must have object type (otherwise, it would be a 
// reference declaration). A variable parameter requires a copy. If the
// type is passed indirectly, then the caller must be responsible for making
// the copy prior to the call.
//
// If the object is passed directly, then we allocate local storage for
// the parameter.
static void
generate_var_parm(generator& gen, const var_decl& parm, llvm::Argument* arg)
{
  cg::type type = gen.get_type(parm.get_type());
  
  if (type.is_indirect()) {
    // If the parameter has object type, but has indirect type, then it is
    // passed indirectly (by value). Annotate it as such.
    llvm::Context& cxt = gen.get_context();
    llvm::AttrBuilder attrs;
    attrs.addAttribute(llvm::Attribute::ByVal);
    arg->addAttr(llvm::AttributeSet::get(cxt, arg->getArgNo() + 1, attrs));
    gen.put_value(parm, arg);
  }
  else {
    // Otherwise, create local storage for the variable.
    llvm::Builder ir(gen.get_entry_block());
    std::stringstream ss;
    ss << "parm" << arg->getArgNo();
    llvm::Value* ptr = ir.CreateAlloca(type, nullptr, ss.str());
    ir.CreateStore(arg, ptr);
    gen.put_value(parm, ptr);
  }
}

// Generate code for a reference parameter.
static void
generate_ref_parm(generator& gen, const ref_decl& p, llvm::Argument* arg)
{
  assert(false && "not implemented");
}

// Generate code for a registered parameter.
//
// TODO: Set the inreg attribute?
static void
generate_reg_parm(generator& gen, const reg_decl& p, llvm::Argument* arg)
{
  assert(false && "not implemented");
}

// Generate formal parameters.
static void
generate_parms(generator& gen, const fn_decl& d, llvm::Function* fn)
{
  cg::fn_info& info = get_function_info(gen, d);
  auto iter = fn->arg_begin();
  
  // Generate/annotate the return value.
  if (info.has_return_parameter())
    generate_return_value(gen, &*iter++);
  else
    generate_return_value(gen, d);

  // Generate/annotate each parameter in turn.
  for (const decl& p : d.get_parameters()) {
    const typed_decl& parm = *p.as_typed();
    llvm::Argument* arg = &*iter++;
    
    // Set the name of the argument and bind it locally.
    std::string name = generate(gen, parm.get_name());
    arg->setName(name);

    // Bind the declared parameter to the object containing its value.
    if (const var_decl* var = as<var_decl>(&parm))
      generate_var_parm(gen, *var, arg);
    else if (const ref_decl* ref = as<ref_decl>(&parm))
      generate_ref_parm(gen, *ref, arg);
    else if (const reg_decl* reg = as<reg_decl>(&parm))
      generate_reg_parm(gen, *reg, arg);
    else
      assert(false && "unsupported parameter");
  }
}

// Generate a function definition that is an expression `e`. This is equivalent
// to a function whose definition is `{ return e; }`. As with return statements,
// we expect that `e` to be a void expression or an initialization of the
// return value.
static void
generate_expr_def(generator& gen, const fn_decl& decl, const expr& def)
{
  // FIXME: This is an initialization of the return value. We should be able to
  // replace the result of a temp expr with this object in the initializer.
  generate(gen, def);

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

  // Insert a terminator if needed.
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
//
// FIXME: The return depends on the declaration. If the return is a
// register, then we can return directly. If not, then we probably
// have to load.
static void
generate_exit(generator& gen, const fn_decl& d)
{
  llvm::Builder ir(gen.get_exit_block());    
  if (is<void_type>(d.get_return_type())) {
    // The function is void. Just return.
    ir.CreateRetVoid();
  }
  else if (is<var_decl>(d.get_return())) {
    // The function returns an object. If that object is returned in a 
    // register, load and return. Otherwise, the function is actually void.
    cg::fn_info& info = get_function_info(gen, d);
    if (!info.has_return_parameter()) {
      llvm::Value* ret = ir.CreateLoad(gen.get_return_value());
      ir.CreateRet(ret); 
    }
  }
  else {
    assert(false && "unsupported return value");
  }
}

// Generate a function definition.
//
// TODO: Don't require a separate exit block unless there is an early
// return statement.
static void
generate_fn_def(generator& gen, const fn_decl& d, llvm::Function* f)
{
  defn def = d.get_definition();
  
  if (def.is_remote())
    // The function is not defined; it is a declaration only.
    return;

  // Generate a local definition.
  gen.define_function(f);
  generate_parms(gen, d, f);
  generate_body(gen, d);  
  generate_exit(gen, d);
  gen.end_function();
}

// Generate an LLVM function from d.
llvm::Value*
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

  // Emit a definition (if local).
  generate_fn_def(gen, d, fn);
  return fn;
}

} // namespace core
} // namespace beaker

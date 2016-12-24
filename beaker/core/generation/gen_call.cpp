// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved


namespace beaker {
namespace core {

// Generate a call expression.
//
// TODO: Adjust the arguments of the call to include a (possibly) materialized
// temporary. Also, how do we elide that materialization if this call is in
// an initialization context (is that us, or is it higher-level).
//
// TODO: Consider how exceptions should be handled. If the function can
// propagate C++ exceptions, then we'll need to establish (or have established)
// a landing pad. If the language has no exceptions, then call is sufficient.
//
// TODO: Handle non-noexcept functions.
static cg::value
generate_call_expr(generator& gen, const call_expr& e)
{
  const expr& fexpr = e.get_function();
  const fn_type& ftype = cast<fn_type>(fexpr.get_type());

  llvm::Builder ir(gen.get_current_block());

  // Generate the function access.
  cg::value fn = generate(gen, e.get_function());
  fn->dump();
  return fn;

  // Build the list of arguments.
  // std::vector<llvm::Value*> args;

#if 0
  auto pii = info.get_parameters().begin();

  // If needed, materialize a temporary to use as the return value.
  //
  // TODO: If we're in an initialization context, we might be able to use
  // the initialization target as the return value.
  if (info.has_return_parameter()) {
    llvm::Builder ir(gen.get_entry_block());
    cg::type type = generate(gen, ftype.get_return_type());
    cg::value ret = ir.CreateAlloca(type);
    args.push_back(ret);
    ++pii;
  }
  
  auto ai = e.get_arguments().begin();
  auto ae = e.get_arguments().end();
  while (ai != ae) {
    // Look at the parameter to determine how it should be passed.
    //
    // FIXME: We actually need to look at the parameter declaration to
    // determine if it's intended to be passed by value or by reference.
    cg::parm_info parm = *pii;
    if (parm.is_direct()) {
      // The parameter passed directly by value.
      //
      // Note that the expression should be an initialization. However, there 
      // is no object to initialize since the value is passed in a register.
      cg::value arg = generate(gen, *ai);
      args.push_back(arg);
    } else if (parm.is_by_value()) {
      // The parameter is passed indirectly by value.
      //
      // Materialize a caller-side object and initialize. Note that the 
      // parameter and argument type must match.
      llvm::Builder ir(gen.get_entry_block());
      cg::type type = generate(gen, ai->get_type());
      cg::value arg = ir.CreateAlloca(type);
      
      // Initialize that caller-side argument.
      generator::init_guard (gen, arg);
      generate(gen, *ai);
      
      args.push_back(arg);
    } else {
      // The argument is indirectly passed by reference.
      cg::value arg = generate(gen, *ai);
      args.push_back(arg);
    }
    ++ai;
    ++pii;
  }

  return ir.CreateCall(fn, args);
  #endif
}

} // namespace core
} // namespace beaker

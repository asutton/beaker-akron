// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

namespace beaker {
namespace sys_fn {

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

  // Generate the function address.
  cg::value fn = generate(gen, e.get_function());

  // Build the list of arguments.
  std::vector<llvm::Value*> fargs;

  // If the return type is passed indirectly, then we're going to need
  // to create a local temporary to be initialized by the function.
  const type& tret = e.get_type();
  cg::type ret = generate(gen, tret);
  if (ret.is_indirect())
    fargs.push_back(gen.make_alloca(ret));

  const expr_seq& args = e.get_arguments();
  const type_seq& parms = ftype.get_parameter_types();
  auto ai = args.begin(), ae = args.end();
  auto pi = parms.begin(), pe = parms.end();
  while (ai != ae && pi != pe) {
    const expr& arg = *ai;
    const type& parm = * pi;
    cg::type ptype = generate(gen, parm);
    
    // If the type is passed indirectly, then we need to create caller-side
    // temporary and initialize that so we can pass the address. Otherwise,
    // we can pass the value directly (tmp will be null in that case).
    cg::value tmp = nullptr;
    if (ptype.is_indirect())
      tmp = gen.make_alloca(ptype);
    
    // Generate the argument initializer.
    generator::init_guard guard(gen, tmp);
    cg::value val = generate(gen, arg);
    fargs.push_back(val);

    ++ai;
    ++pi;
  }
  assert(ai == ae && pi == pe);

  llvm::Builder ir(gen.get_current_block());
  return ir.CreateCall(fn, fargs);
}

} // namespace sys_fn
} // namespace beaker

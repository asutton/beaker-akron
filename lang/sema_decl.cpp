// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

namespace bpl {

/// Simply return the module.
decl&
semantics::on_start_module()
{
  return mod;
}

/// Simply return the module.
///
/// \todo Apply all deferred actions here.
decl&
semantics::on_finish_module()
{
  return mod;
}


/// Construct and declare a function.
decl&
semantics::on_start_function(name& id, decl_seq&& parms, type& rty, locations<4> locs)
{
  decl& ret = build_fn.make_parm_decl(rty);
  type& fty = build_fn.get_fn_type(parms, ret);
  decl& cxt = current_context();
  decl& fn = build_fn.make_fn_decl(cxt, id, fty, std::move(parms), ret);

  // FIXME: Figure out how declarations should work.
  // current_scope().add(fn);

  return fn;
}

/// Returns a function declaration.
decl& 
semantics::on_finish_function(location semi)
{
  return current_context();
}

/// Attach the body to the current function defintion.
///
/// \todo Build a set of lowering functions to iteratively lower the definition
/// until we can emit IR code. Note that we can also release all nodes
/// allocated by those passes once we've finished the definition. This will
/// help keep memory low.
decl& 
semantics::on_finish_function(stmt& body)
{
  beaker::sys_fn::fn_decl& fn = cast<beaker::sys_fn::fn_decl>(current_context());
  fn.def_ = &body;

  // TODO: Lower or analyze the function?

  return fn;
}

} // namespace bpl

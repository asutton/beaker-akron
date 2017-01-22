// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "build.hpp"

/// The current global builder.
builder* global_builder::current = nullptr;

/// Create a global variable definition.
sys_var::var_decl&
builder::make_global_var_decl(name& n, type& t, expr& e)
{
  return var_.make_var_decl(*mod_, external_link, static_storage, n, t, e);
}

/// Create a global variable definition.
sys_var::var_decl&
builder::make_global_var_decl(const char* n, type& t, expr& e)
{
  return var_.make_var_decl(*mod_, external_link, static_storage, n, t, e);
}

/// Create a local variable definition.
sys_var::var_decl&
builder::make_local_var_decl(decl& cxt, name& n, type& t, expr& e)
{
  return var_.make_var_decl(cxt, no_link, automatic_storage, n, t, e);
}

/// Create a local variable definition.
sys_var::var_decl&
builder::make_local_var_decl(decl& cxt, const char* n, type& t, expr& e)
{
  return var_.make_var_decl(cxt, no_link, automatic_storage, n, t, e);
}

/// Returns a new external function.
sys_fn::fn_decl&
builder::make_fn_decl(name& n, type& t, const decl_seq& p, decl& r, stmt& s)
{
  return fn_.make_fn_decl(*mod_, n, t, p, r, s);
}

/// Returns a new external function.
sys_fn::fn_decl&
builder::make_fn_decl(const char* n, type& t, const decl_seq& p, decl& r, stmt& s)
{
  return fn_.make_fn_decl(*mod_, n, t, p, r, s);
}

/// Returns a new external function.
sys_fn::fn_decl&
builder::make_fn_decl(name& n, type& t, decl_seq&& p, decl& r, stmt& s)
{
  return fn_.make_fn_decl(*mod_, n, t, p, r, s);
}

/// Returns a new external function.
sys_fn::fn_decl&
builder::make_fn_decl(const char* n, type& t, decl_seq&& p, decl& r, stmt& s)
{
  return fn_.make_fn_decl(*mod_, n, t, p, r, s);
}

/// Returns a new function parameter.
sys_fn::parm_decl& 
builder::make_parm_decl(name& n, type& t)
{
  return fn_.make_parm_decl(n, t);
}

/// Returns a new function parameter.
sys_fn::parm_decl& 
builder::make_parm_decl(const char* n, type& t)
{
  return fn_.make_parm_decl(n, t);
}

/// Create a function of the form:
///
///   fn main() -> int32 { statement-seq }
///
/// where statements is an empty statement sequence.
decl& 
builder::make_main()
{
  decl_seq parms;
  decl& ret = make_parm_decl("ret", get_int32_type());
  type& type = get_fn_type(parms, ret);
  stmt& def = make_block_stmt();
  decl& fn = make_fn_decl("main", type, parms, ret, def);
  mod_->add_declaration(fn);
  return fn;
}

/// Makes a statement of the form:
///
///   return 0;
///
/// Where 0 has type i32.
stmt&
builder::make_return(int n)
{
  type& type = get_int32_type();
  expr& val = make_int_expr(type, 0);
  return make_ret_stmt(val);
}


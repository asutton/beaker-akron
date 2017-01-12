// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "build.hpp"

/// Create a function of the form:
///
///   fn main() -> int32 { ss }
decl& 
builder::make_main(const stmt_seq& ss)
{
  decl_seq parms;
  decl& ret = make_parm_decl("ret", get_int32_type());
  type& type = get_fn_type(parms, ret);
  stmt& def = make_block_stmt(ss);
  return make_fn_decl("main", type, parms, ret, def);
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

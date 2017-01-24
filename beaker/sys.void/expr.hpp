// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_VOID_EXPR_HPP
#define BEAKER_SYS_VOID_EXPR_HPP

#include <beaker/base/expr.hpp>


namespace beaker {
namespace sys_void {


enum {
  first_expr_kind = sys_void_lang_block,
#define def_expr(e) e ## _expr_kind,
#include "expr.def"
  last_expr_kind
};


/// The expression `nop`. The type of the expression is `void`. It has no 
/// effect. 
struct nop_expr : nullary_expr_impl<nop_expr_kind>
{
  using nullary_expr_impl<nop_expr_kind>::nullary_expr_impl;
};


/// The expression `void(e)`. The type of the expression is `void`. The value 
/// of `e` is discarded.
struct void_expr : unary_expr_impl<void_expr_kind>
{
  using unary_expr_impl<void_expr_kind>::unary_expr_impl;
};


/// Causes the program to abort, either through a trap or a call to the
/// abort() function.
struct trap_expr : nullary_expr_impl<trap_expr_kind>
{
  using nullary_expr_impl<trap_expr_kind>::nullary_expr_impl;
};


// Operations

bool is_void_expression(const expr&);

} // namespace sys_void
} // namespace beaker


#endif

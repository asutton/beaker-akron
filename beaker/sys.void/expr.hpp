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
struct nop_expr : generic_nullary_expr<nop_expr_kind>
{
  using generic_nullary_expr<nop_expr_kind>::generic_nullary_expr;
};


/// The expression `void(e)`. The type of the expression is `void`. The value 
/// of `e` is discarded.
struct void_expr : generic_unary_expr<void_expr_kind>
{
  using generic_unary_expr<void_expr_kind>::generic_unary_expr;
};


/// Causes the program to abort, either through a trap or a call to the
/// abort() function.
struct trap_expr : generic_nullary_expr<trap_expr_kind>
{
  using generic_nullary_expr<trap_expr_kind>::generic_nullary_expr;
};


// Operations

bool is_void_expression(const expr&);

} // namespace sys_void
} // namespace beaker


#endif

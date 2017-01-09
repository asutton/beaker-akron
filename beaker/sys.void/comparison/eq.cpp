// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "eq.hpp"
#include "../type.hpp"
#include "../expr.hpp"


namespace beaker {
namespace sys_void {

/// Returns true when `a` and `b` are `void`.
bool
eq_algo::operator()(const type& a, const type& b) const
{
  assert(a.get_kind() == b.get_kind());
  assert(is_void_type(a));
  return true;
}

/// Returns true when `a` and `b` are equivalent void expressions.
bool
eq_algo::operator()(const expr& a, const expr& b) const
{
  assert(a.get_kind() == b.get_kind());
  switch (a.get_kind()) {
    case nop_expr_kind:
      return true;
    case void_expr_kind:
      return equivalent_unary_expr(cast<void_expr>(a), cast<void_expr>(b));
    default:
      break;
  }
  assert(false && "not a void expression");
}

} // namespace sys_void
} // namespace beaker

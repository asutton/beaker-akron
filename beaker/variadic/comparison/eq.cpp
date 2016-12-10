// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/variadic/comparison/eq.hpp>
#include <beaker/variadic/type.hpp>
#include <beaker/variadic/expr.hpp>


namespace beaker {
namespace variadic {

/// Returns true.
bool
eq_algo::operator()(const type& a, const type& b) const
{
  assert(a.get_kind() == list_type_kind);
  return true;
}

bool
eq_algo::operator()(const expr& a, const expr& b) const
{
  switch (a.get_kind()) {
    case start_expr_kind:
    case end_expr_kind:
    case arg_expr_kind:
      return equivalent_unary_expr(cast<unary_expr>(a), cast<unary_expr>(b));
    case copy_expr_kind:
      return equivalent_binary_expr(cast<binary_expr>(a), cast<binary_expr>(b));
    default:
      break;
  }
  assert(false && "not a variadic expression");
}

} // namespace variadic
} // namespace beaker

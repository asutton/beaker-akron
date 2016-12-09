// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/variadic/type.hpp>
#include <beaker/variadic/expr.hpp>
#include <beaker/base/comparison/eq.hpp>


namespace beaker {
namespace variadic {

bool
equivalent_name(const name& a, const name& b)
{
  assert(false && "not defined");
}

/// Returns true.
bool
equivalent_type(const type& a, const type& b)
{
  assert(a.get_kind() == list_type_kind);
  return true;
}

bool
equivalent_expr(const expr& a, const expr& b)
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

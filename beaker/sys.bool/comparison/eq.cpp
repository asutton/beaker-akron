// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "eq.hpp"
#include "../type.hpp"
#include "../expr.hpp"


namespace beaker {
namespace sys_bool {

/// Returns true when a and b are of type bool.
bool
eq_algo::operator()(const type& a, const type& b) const
{
  assert(a.get_kind() == b.get_kind());
  assert(a.get_kind() == bool_type_kind);
  return true;
}


/// Returns true when a and b have the same truth value.
static inline bool
eq_bool_expr(const bool_expr& a, const bool_expr& b)
{
  return a.get_boolean() == b.get_boolean();
}

bool
eq_algo::operator()(const expr& a, const expr& b) const
{
  assert(a.get_kind() == b.get_kind());
  switch (a.get_kind()) {
    case bool_expr_kind:
      return eq_bool_expr(cast<bool_expr>(a), cast<bool_expr>(b));
    
    case and_expr_kind:
    case or_expr_kind:
    case imp_expr_kind:
    case eq_expr_kind:
      return equivalent_binary_expr(cast<binary_expr>(a), cast<binary_expr>(b));
    
    case not_expr_kind: 
      return equivalent_unary_expr(cast<unary_expr>(a), cast<unary_expr>(b));

    default:
      break;
  }
  assert(false && "not a boolean expression");
}

} // namespace sys_bool
} // namespace beaker

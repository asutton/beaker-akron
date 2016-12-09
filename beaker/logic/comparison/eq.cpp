// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/logic/type.hpp>
#include <beaker/logic/expr.hpp>
#include <beaker/base/comparison/eq.hpp>


namespace beaker {
namespace logic {

// All bool types are equivalent.
bool 
eq_type(const type& a, const type& b)
{
  assert(a.get_kind() == bool_type_kind);
  return true;
}

static inline bool
eq_bool_expr(const bool_expr& a, const bool_expr& b)
{
  return a.get_boolean() == b.get_boolean();
}

// Returns true when a and b are equivalent logical expressions.
bool 
eq_expr(const expr& a, const expr& b)
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
  assert(false && "not a logical expression");
}

} // namespace logic
} // namespace beaker

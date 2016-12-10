// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/logic/comparison/hash.hpp>
#include <beaker/logic/type.hpp>
#include <beaker/logic/expr.hpp>


namespace beaker {
namespace logic {

// Hashes no additional behavior.
void
hash_algo::operator()(hasher& h, const type& t) const
{
  assert(t.get_kind() == bool_type_kind);
}

// Hash the boolean value of e into h.
static void
hash_bool_expr(hasher& h, const bool_expr& e)
{
  hash(h, e.get_boolean());
}

// Returns true when a and b are equivalent logical expressions.
void
hash_algo::operator()(hasher& h, const expr& e) const
{
  switch (e.get_kind()) {
    case bool_expr_kind:
      return hash_bool_expr(h, cast<bool_expr>(e));
    case and_expr_kind:
    case or_expr_kind:
    case imp_expr_kind:
    case eq_expr_kind:
      return hash_binary_expr(h, cast<binary_expr>(e));
    case not_expr_kind:
      return hash_unary_expr(h, cast<unary_expr>(e));
    default:
      break;
  }
  assert(false && "not a logical expression");
}

} // namespace logic
} // namespace beaker

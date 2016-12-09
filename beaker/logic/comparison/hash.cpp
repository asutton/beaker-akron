// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/logic/type.hpp>
#include <beaker/logic/expr.hpp>
#include <beaker/base/comparison/hash.hpp>


namespace beaker {
namespace logic {

void
hash_name(hasher& h, const name& n)
{
  assert(false && "invalid name");
}

// Nothing else to hash.
void
hash_type(hasher& h, const type& t)
{
  assert(t.get_kind() == bool_type_kind);
}

static void
hash_bool_expr(hasher& h, const bool_expr& e)
{
  hash(h, e.get_value().get_integer());
}

// Returns true when a and b are equivalent logical expressions.
void
hash_expr(hasher& h, const expr& e)
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

// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/variadic/comparison/hash.hpp>
#include <beaker/variadic/type.hpp>
#include <beaker/variadic/expr.hpp>


namespace beaker {
namespace variadic {

void
hash_algo::operator()(hasher& h, const type& t) const
{
  assert(t.get_kind() == list_type_kind);
}

void
hash_algo::operator()(hasher& h, const expr& t) const
{
  switch (t.get_kind()) {
    case start_expr_kind:
    case end_expr_kind:
    case arg_expr_kind:
      return hash_unary_expr(h, cast<unary_expr>(t));
    case copy_expr_kind:
      return hash_binary_expr(h, cast<binary_expr>(t));
    default:
      break;
  }
  assert(false && "not a variadic expression");
}

} // namespace variadic
} // namespace beaker

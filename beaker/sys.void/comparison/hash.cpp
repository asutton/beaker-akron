// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "hash.hpp"
#include "../type.hpp"
#include "../expr.hpp"


namespace beaker {
namespace sys_void {

// Hashes the `void` type `t`.
void
hash_algo::operator()(hasher& h, const type& t) const
{
  assert(is_void_type(t));
}

// Hash the `void` expression `e`.
void
hash_algo::operator()(hasher& h, const expr& e) const
{
  switch (e.get_kind()) {
    case nop_expr_kind:
      return;
    case void_expr_kind:
      return hash_unary_expr(h, cast<void_expr>(e));
    default:
      break;
  }
  assert(false && "not a void expression");
}

} // namespace sys_void
} // namespace beaker

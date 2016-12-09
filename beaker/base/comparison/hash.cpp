// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "hash.hpp"
#include <beaker/base/name.hpp>
#include <beaker/base/type.hpp>
#include <beaker/base/expr.hpp>


namespace beaker {

void
hash(hasher& h, const name& n) 
{
  hash(h, typeid(n));
  return language::get_feature(n).hash(h, n);
}

void
hash(hasher& h, const type& t) 
{
  hash(h, typeid(t));
  return language::get_feature(t).hash(h, t);
}

void
hash(hasher& h, const expr& e) 
{
  hash(h, typeid(e));
  return language::get_feature(e).hash(h, e);
}

/// Hash a unary expression e into h.
void
hash_unary_expr(hasher& h, const unary_expr& e)
{
  hash(h, e.get_operand());
}

/// Hash a binary expression e into h.
void
hash_binary_expr(hasher& h, const binary_expr& e)
{
  hash(h, e.get_lhs());
  hash(h, e.get_rhs());
}

} // namespace
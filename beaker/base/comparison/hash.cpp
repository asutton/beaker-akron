// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "hash.hpp"
#include "../expr.hpp"


namespace beaker {

/// Hash a expression literal value.
void
hash_expr(hasher& h, const literal_expr& e)
{
  hash(h, e.get_value());
}

/// Hash a unary expression e into h.
void
hash_expr(hasher& h, const unary_expr& e)
{
  hash(h, e.get_first());
}

/// Hash a binary expression e into h.
void
hash_expr(hasher& h, const binary_expr& e)
{
  hash(h, e.get_first());
  hash(h, e.get_second());
}

/// Hash a ternary expression e into h.
void
hash_expr(hasher& h, const ternary_expr& e)
{
  hash(h, e.get_first());
  hash(h, e.get_second());
  hash(h, e.get_third());
}

} // namespace
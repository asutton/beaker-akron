// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "hash.hpp"
#include "../type.hpp"
#include "../expr.hpp"


namespace beaker {
namespace sys_fn {

// Append the hash of t's parameter and return types to h.
static void
hash_fn_type(hasher& h, const fn_type& t)
{
  hash(h, t.get_parameter_types());
  hash(h, t.get_return_type());
}

// Append the hash of common type t to h.
void
hash_algo::operator()(hasher& h, const type& t) const
{
  assert(is<fn_type>(t));
  hash_fn_type(h, cast<fn_type>(t));
}

/// TODO: Implement this function.
void
hash_algo::operator()(hasher& h, const expr& t) const
{
  assert(false && "not implemented");
}

} // namespace sys_fn
} // namespace beaker

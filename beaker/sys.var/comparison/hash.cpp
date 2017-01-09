// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "hash.hpp"
#include "../type.hpp"
#include "../expr.hpp"


namespace beaker {
namespace sys_var {

// Append the hash of the object type of t to h.
static void
hash_ref_type(hasher& h, const ref_type& t)
{
  hash(h, t.get_object_type());
}

// Append the hash of common type t to h.
void
hash_algo::operator()(hasher& h, const type& t) const
{
  assert(is<ref_type>(t));
  return hash_ref_type(h, cast<ref_type>(t));
}

/// TODO: Implement this function.
void
hash_algo::operator()(hasher& h, const expr& t) const
{
  assert(false && "not implemented");
}

} // namespace sys_var
} // namespace beaker

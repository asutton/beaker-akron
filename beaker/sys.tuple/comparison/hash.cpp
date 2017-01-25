// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "hash.hpp"
#include "../type.hpp"
#include "../expr.hpp"


namespace beaker {
namespace sys_tuple {

static void
hash_tuple_type(hasher& h, const tuple_type& t)
{
  hash(h, t.get_element_types());
}

void
hash_algo::operator()(hasher& h, const type& t) const
{
  hash_tuple_type(h, cast<tuple_type>(t));
}

void
hash_algo::operator()(hasher& h, const expr& n) const
{
  assert(false && "not implemented");
}

} // namespace sys_tuple
} // namespace beaker

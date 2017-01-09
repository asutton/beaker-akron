// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "hash.hpp"
#include "../type.hpp"
#include "../expr.hpp"


namespace beaker {
namespace sys_int {

/// Appends the precision of numeric type `t` to `h`.
void
hash_algo::operator()(hasher& h, const type& t) const
{
  switch (t.get_kind()) {
#define def_type(T) \
    case T ## _type_kind: \
      return hash(h, cast<T ## _type>(t).get_precision());
#include "../type.def"
    default:
      break;
  }
  assert(false && "not a numeric type");
}

void
hash_algo::operator()(hasher& h, const expr& n) const
{
  assert(false && "not implemented");
}

} // namespace sys_int
} // namespace beaker

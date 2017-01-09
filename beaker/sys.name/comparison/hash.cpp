// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "hash.hpp"
#include "../name.hpp"
#include <beaker/util/symbol.hpp>


namespace beaker {
namespace sys_name {

/// Hash the name `n` into `h`.
void
hash_algo::operator()(hasher& h, const name& n) const
{
  switch (n.get_kind()) {
    case basic_name_kind:
      return hash(h, cast<basic_name>(n).get_symbol());
    case internal_name_kind:
      return hash(h, cast<internal_name>(n).get_id());
    default:
      break;
  }
  assert(false && "name not a system name");
}

} // namespace sys_name
} // namespace beaker

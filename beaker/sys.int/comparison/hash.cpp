// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "hash.hpp"
#include "../type.hpp"


namespace beaker {
namespace sys_int {

/// Appends the precision of numeric type `t` to `h`.
void
hash_integral_type(hasher& h, const integral_type& t)
{
  hash(h, t.get_precision());
}

} // namespace sys_int
} // namespace beaker

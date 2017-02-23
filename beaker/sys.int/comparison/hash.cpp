// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "hash.hpp"
#include "../type.hpp"


namespace beaker {

/// Appends the precision of numeric type `t` to `h`.
void
hash_type(hasher& h, const sys_int::integral_type& t)
{
  hash(h, t.get_precision());
}

} // namespace beaker

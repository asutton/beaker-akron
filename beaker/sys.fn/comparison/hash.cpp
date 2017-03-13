// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "hash.hpp"
#include "../type.hpp"


namespace beaker {

// Append the hash of t's parameter and return types to h.
void
hash_type(hasher& h, const sys_fn::fn_type& t)
{
  hash(h, t.get_parameter_types());
  hash(h, t.get_return_type());
}

} // namespace beaker

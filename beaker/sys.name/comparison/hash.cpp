// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "hash.hpp"
#include "../name.hpp"

#include <beaker/util/symbol.hpp>


namespace beaker {

void 
hash(hasher& h, const sys_name::basic_name& n)
{
  hash(h, n.get_symbol());
}

void 
hash(hasher& h, const sys_name::internal_name& n)
{
  hash(h, n.get_id());
}

} // namespace beaker

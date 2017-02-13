// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_INT_COMPARISON_HASH_HPP
#define BEAKER_SYS_INT_COMPARISON_HASH_HPP

#include <beaker/sys.int/fwd.hpp>

#include <beaker/base/comparison/hash.hpp>


namespace beaker {

void hash(hasher& h, const sys_int::integral_type&);

} // namespace beaker


#endif

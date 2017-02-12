// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_INT_COMPARISON_HASH_HPP
#define BEAKER_SYS_INT_COMPARISON_HASH_HPP

#include <beaker/sys.int/fwd.hpp>

#include <beaker/base/comparison/hash.hpp>


namespace beaker {
namespace sys_int {

struct integral_type;

void hash_integral_type(hasher& h, const integral_type&);

} // namespace sys_int
} // namespace beaker


#endif

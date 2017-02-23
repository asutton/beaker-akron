// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_NAME_COMPARISON_HASH_HPP
#define BEAKER_SYS_NAME_COMPARISON_HASH_HPP

#include <beaker/sys.name/fwd.hpp>

#include <beaker/base/comparison/hash.hpp>


namespace beaker {

void hash_name(hasher& h, const sys_name::basic_name&);
void hash_name(hasher& h, const sys_name::internal_name&);

} // namespace beaker


#endif

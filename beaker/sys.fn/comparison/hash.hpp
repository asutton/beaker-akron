// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_FN_COMPARISON_HASH_HPP
#define BEAKER_SYS_FN_COMPARISON_HASH_HPP

#include <beaker/sys.fn/fwd.hpp>

#include <beaker/base/comparison/hash.hpp>


namespace beaker {

void hash_type(hasher& h, const sys_fn::fn_type&);

} // namespace beaker


#endif

// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_NAME_COMPARISON_EQUAL_HPP
#define BEAKER_SYS_NAME_COMPARISON_EQUAL_HPP

#include <beaker/sys.name/fwd.hpp>

#include <beaker/base/comparison/equal.hpp>


namespace beaker {

bool equal(const sys_name::basic_name&, const sys_name::basic_name&);
bool equal(const sys_name::internal_name&, const sys_name::internal_name&);

} // namespace beaker


#endif

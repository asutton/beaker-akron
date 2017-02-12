// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_INT_COMPARISON_EQ_HPP
#define BEAKER_SYS_INT_COMPARISON_EQ_HPP

#include <beaker/base/comparison/equal.hpp>


namespace beaker {
namespace sys_int {

struct integral_type;

bool equal(const integral_type&, const integral_type&);

} // namespace sys_int
} // namespace beaker


#endif

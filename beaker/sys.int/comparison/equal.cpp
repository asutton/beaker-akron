// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "equal.hpp"
#include "../type.hpp"


namespace beaker {
namespace sys_int {

/// Returns true when numeric types `a` and `b` have the same precision.
bool 
equal_integral_type(const integral_type& a, const integral_type& b)
{
  return a.get_precision() == b.get_precision();
}

} // namespace sys_int
} // namespace beaker

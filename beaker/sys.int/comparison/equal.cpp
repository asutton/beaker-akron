// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "equal.hpp"
#include "../type.hpp"


namespace beaker {

/// Returns true when numeric types `a` and `b` have the same precision.
bool 
equal_type(const sys_int::integral_type& a, const sys_int::integral_type& b)
{
  return a.get_precision() == b.get_precision();
}

} // namespace beaker

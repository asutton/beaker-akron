// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "expr.hpp"
#include "type.hpp"


namespace beaker {
namespace sys_float {

/// Returns true if e has floating point type.
bool
is_floating_point_expression(const expr& e)
{
  return is_floating_point_type(e.get_type());
}

} // namespace sys_float
} // namespace beaker

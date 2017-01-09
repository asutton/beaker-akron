// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "expr.hpp"
#include "type.hpp"


namespace beaker {
namespace sys_int {

/// Returns true if e has integral type.
bool
is_integral_expression(const expr& e)
{
  return is_integral_type(e.get_type());
}

} // namespace sys_int
} // namespace beaker

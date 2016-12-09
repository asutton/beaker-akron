// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "expr.hpp"
#include "type.hpp"


namespace beaker {
namespace numeric {

/// Returns true if e has integral type.
bool
is_integral_expression(const expr& e)
{
  return is_integral_type(e.get_type());
}

/// Returns true if e has floating point type.
bool
is_floating_point_expression(const expr& e)
{
  return is_floating_point_type(e.get_type());
}

/// Returns true if e has arithmetic type.
bool
is_arithmetic_expression(const expr& e)
{
  return is_arithmetic_type(e.get_type());
}

} // namespace numeric
} // namespace beaker

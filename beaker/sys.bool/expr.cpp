// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "expr.hpp"
#include "type.hpp"

#include <iostream>

namespace beaker {
namespace sys_bool {

/// Returns true when `e` has type `bool`.
bool
is_boolean_expression(const expr& e)
{
  return is<bool_type>(e.get_type());
}

} // namespace sys_bool
} // namespace beaker

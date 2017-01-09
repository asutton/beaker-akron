// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "expr.hpp"
#include "type.hpp"

#include <iostream>

namespace beaker {
namespace logic {

/// Returns true when `e` has type `bool`.
bool
is_boolean_expression(const expr& e)
{
  return is<bool_type>(e.get_type());
}

} // namespace logic
} // namespace beaker

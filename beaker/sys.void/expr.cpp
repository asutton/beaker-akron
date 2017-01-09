// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "expr.hpp"
#include "type.hpp"

#include <iostream>

namespace beaker {
namespace sys_void {

/// Returns true when `e` has type `void`.
bool
is_void_expr(const expr& e)
{
  return is_void_type(e.get_type());
}

} // namespace sys_void
} // namespace beaker

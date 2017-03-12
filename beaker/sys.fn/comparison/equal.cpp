// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "equal.hpp"
#include "../type.hpp"
#include "../expr.hpp"


namespace beaker {

// Returns true if a and b have parameter and return types.
bool
equal(const sys_fn::fn_type& a, const sys_fn::fn_type& b)
{
  return equal(a.get_return_type(), b.get_return_type()) &&
         equal(a.get_parameter_types(), b.get_parameter_types());
}

} // namespace beaker

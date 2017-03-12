// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "eq.hpp"
#include "../type.hpp"
#include "../expr.hpp"


namespace beaker {
namespace sys_fn {

// Returns true if a and b have parameter and return types.
static bool
eq_fn_type(const fn_type& a, const fn_type& b)
{
  return equivalent(a.get_return_type(), b.get_return_type()) &&
         equivalent(a.get_parameter_types(), b.get_parameter_types());
}

bool
eq_algo::operator()(const type& a, const type& b) const
{
  assert(is<fn_type>(a));
  return eq_fn_type(cast<fn_type>(a), cast<fn_type>(b));
}


/// \todo Implement me.
bool
eq_algo::operator()(const expr& a, const expr& b) const
{
  assert(false && "not implemented");
}

} // namespace sys_fn
} // namespace beaker

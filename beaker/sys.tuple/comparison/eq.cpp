// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "eq.hpp"
#include "../type.hpp"
#include "../expr.hpp"


namespace beaker {
namespace sys_tuple {

static bool
eq_tuple_type(const tuple_type& a, const tuple_type& b)
{
  return equivalent(a.get_element_types(), b.get_element_types());
}

bool
eq_algo::operator()(const type& a, const type& b) const
{
  return eq_tuple_type(cast<tuple_type>(a), cast<tuple_type>(b));
}

bool
eq_algo::operator()(const expr& a, const expr& b) const
{
  assert(false && "not implemented");
}

} // namespace sys_tuple
} // namespace beaker

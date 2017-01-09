// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "eq.hpp"
#include "../type.hpp"
#include "../expr.hpp"


namespace beaker {
namespace sys_var {


// Returns true if a and b have equivalent object types.
static inline bool
eq_ref_type(const ref_type& a, const ref_type& b)
{
  return equivalent(a.get_object_type(), b.get_object_type());
}

bool
eq_algo::operator()(const type& a, const type& b) const
{
  assert(is<ref_type>(a));
  return eq_ref_type(cast<ref_type>(a), cast<ref_type>(b));
}


/// \todo Implement me.
bool
eq_algo::operator()(const expr& a, const expr& b) const
{
  assert(false && "not implemented");
}

} // namespace sys_var
} // namespace beaker

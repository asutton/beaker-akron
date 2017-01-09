// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "eq.hpp"
#include "../name.hpp"


namespace beaker {
namespace sys_name {

// Returns true when a and b have the same symbol.
static inline bool
eq_basic_name(const basic_name& a, const basic_name& b)
{
  return &a.get_symbol() == &b.get_symbol();
}

// Returns true when a and b have the same internal id.
static inline bool
eq_internal_name(const internal_name& a, const internal_name& b)
{
  return a.get_id() == b.get_id();
}

bool
eq_algo::operator()(const name& a, const name& b) const
{
  switch (a.get_kind()) {
    case basic_name_kind:
      return eq_basic_name(cast<basic_name>(a), cast<basic_name>(b));
    case internal_name_kind:
      return eq_internal_name(cast<internal_name>(a), cast<internal_name>(b));
    default:
      break;
  }
  assert(false && "not a system name");
}

} // namespace sys_name
} // namespace beaker

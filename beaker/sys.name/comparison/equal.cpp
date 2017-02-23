// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "equal.hpp"
#include "../name.hpp"


namespace beaker {

bool
equal(const sys_name::basic_name& a, const sys_name::basic_name& b)
{ 
  return &a.get_symbol() == &b.get_symbol();
}

bool
equal(const sys_name::internal_name& a, const sys_name::internal_name& b)
{
  return a.get_id() == b.get_id();
}

} // namespace beaker

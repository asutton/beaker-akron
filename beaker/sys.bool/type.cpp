// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "type.hpp"

#include <iostream>

#if 0
namespace beaker {
namespace logic {

// All bool types are equivalent.
bool 
equivalent_type(const type& a, const type& b)
{
  assert(a.get_kind() == bool_type_kind);
  return true;
}

// There is no additional information to hash.
void
append_type(hasher& h, const type& t)
{ }


// Pretty print the bool type.
void
print_type(std::ostream& os, const bool_type& t)
{
  os << "bool";
}

// Pretty print a logical type.
void
print_type(std::ostream& os, const type& t)
{
  assert(t.get_kind() == bool_type_kind);
  print_type(os, cast<bool_type>(t));
}


} // namespace logic
} // namespace beaker
#endif
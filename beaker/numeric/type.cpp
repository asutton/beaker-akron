// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "type.hpp"

#include <iostream>


namespace beaker {
namespace numeric {

// Returns the precision of the arithmetic type t.
template<typename T>
static inline int
get_precision(const type& t) 
{
  return cast<T>(t).get_precision();
}

// Returns the precision of the arithmetic type t.
int
get_precision(const type& t)
{
  switch (t.get_kind()) {
#define def_type(T) \
    case T##_type_kind: \
      return cast<T##_type>(t).get_precision();
#include "type.def"
    default:
      break;
  }
  assert(false && "not a numeric type");
}

} // namespace numeric
} // namespace beaker

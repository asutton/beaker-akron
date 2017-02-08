// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "value.hpp"

#include <beaker/util/hash.hpp>

#include <iostream>


namespace beaker {

void
hash(hasher& h, const value& v)
{
  hash(h, (int)v.get_kind()); // FIXME: Shouldn't have to cast.
  switch (v.get_kind()) {
    case void_value_kind:
      return;
    case int_value_kind:
      return hash(h, v.get_int());
    case float_value_kind:
      return hash(h, v.get_float());
  }
  assert(false && "invalid value kind");
}


std::ostream&
operator<<(std::ostream& os, const value& val)
{
  switch (val.get_kind()) {
    case void_value_kind:
      return os << "<void>";
    case int_value_kind:
      return os << "<int " << val.get_int() << '>';
    case float_value_kind:
      return os << "<float " << val.get_float() << '>';
  }
  assert(false && "invalid value kind");
}

} // namespace beaker

// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "value.hpp"

#include <beaker/util/hash.hpp>


namespace beaker {

void
hash(hasher& h, const value& v)
{
  hash(h, (int)v.get_kind()); // FIXME: Shouldn't have to cast.
  switch (v.get_kind()) {
    case void_value_kind:
      return;
    case int_value_kind:
      return hash(h, v.get_integer());
    case float_value_kind:
      return hash(h, v.get_float());
  }
  assert(false && "invalid value kind");
}

} // namespace beaker

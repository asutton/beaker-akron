// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/numeric/type.hpp>
#include <beaker/numeric/expr.hpp>
#include <beaker/base/comparison/hash.hpp>


namespace beaker {
namespace numeric {

void
hash_name(hasher& h, const name& n)
{
  assert(false && "not defined");
}

/// Appends the precision of `t` to `h`.
void
hash_type(hasher& h, const type& t)
{
  switch (t.get_kind()) {
#define def_type(T) \
    case T##_type_kind: \
      return hash(h, cast<T##_type>(t).get_precision());
#include "../type.def"
    default:
      break;
  }
  assert(false && "not a numeric type");
}

void
hash_expr(hasher& h, const expr& n)
{
  assert(false && "not defined");
}

} // namespace numeric
} // namespace beaker

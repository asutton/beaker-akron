// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/numeric/type.hpp>
#include <beaker/numeric/expr.hpp>
#include <beaker/base/comparison/eq.hpp>


namespace beaker {
namespace numeric {

bool 
equivalent_name(const name& a, const name& b)
{
  assert(false && "not defined");
}

/// Returns true when `a` and `b` have the same precision.
bool
equivalent_type(const type& a, const type& b)
{
  switch (a.get_kind()) {
#define def_type(T) \
    case T##_type_kind: \
      return cast<T##_type>(a).get_precision() == cast<T##_type>(b).get_precision();
#include "../type.def"
    default:
      break;
  }
  assert(false && "not a numeric type");
}

/// \todo Implement me.
bool
equivalent_expr(const expr& a, const expr& b)
{
  assert(false && "not implemented");
}

} // namespace numeric
} // namespace beaker

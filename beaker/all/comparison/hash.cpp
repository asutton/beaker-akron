// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include <beaker/base/comparison/hash.hpp>

#include "../ast.hpp"


namespace beaker {

/// Appends the kind of t's hash code and dispatch to an appropriate overload.
void
hash(hasher& h, const type& t)
{
  hash(h, t.get_kind());
  switch (t.get_kind()) {
#define def_type(NS, T, B) \
    case NS::T ## _type_kind: \
      return hash(h, cast<NS::T ## _type>(t));
#include <beaker/all/type.def>
  }
  assert(false && "invalid type");
}

/// Returns true if a and b denote the same computations.
void
hash(hasher& h, const expr& e)
{
  hash(h, e.get_kind());
  switch (e.get_kind()) {
#define def_expr(NS, E, B) \
    case NS::E ## _expr_kind: \
      return hash(h, cast<NS::E ## _expr>(e));
#include <beaker/all/expr.def>
  }
  assert(false && "invalid expression");
}

} // namespace
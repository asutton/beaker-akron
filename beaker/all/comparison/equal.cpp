// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "equal.hpp"
#include "../ast.hpp"


namespace beaker {

/// Returns true if a and b are the same name.
bool
equal(const name& a, const name& b)
{
  if (&a == &b)
    return true;
  if (a.get_kind() != b.get_kind())
    return false;
  switch (a.get_kind()) {
#define def_name(NS, N) \
    case NS::N ## _name_kind: \
      return equal_name(cast<NS::N ## _name>(a), cast<NS::N ## _name>(b));
#include <beaker/all/name.def>
  }
  assert(false && "invalid type");
}

/// Returns true if a and b are the same type.
bool
equal(const type& a, const type& b)
{
  if (&a == &b)
    return true;
  if (a.get_kind() != b.get_kind())
    return false;
  switch (a.get_kind()) {
#define def_type(NS, T) \
    case NS::T ## _type_kind: \
      return equal_type(cast<NS::T ## _type>(a), cast<NS::T ## _type>(b));
#include <beaker/all/type.def>
  }
  assert(false && "invalid type");
}

/// Returns true if a and b denote the same computations.
bool
equal(const expr& a, const expr& b)
{
  if (&a == &b)
    return true;
  if (a.get_kind() != b.get_kind())
    return false;
  switch (a.get_kind()) {
#define def_expr(NS, E) \
    case NS::E ## _expr_kind: \
      return equal_expr(cast<NS::E ## _expr>(a), cast<NS::E ## _expr>(b));
#include <beaker/all/expr.def>
  }
  assert(false && "invalid expression");
}

} // namespace beaker
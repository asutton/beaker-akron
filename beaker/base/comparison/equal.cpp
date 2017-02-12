// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "equal.hpp"
#include "../expr.hpp"


namespace beaker {

/// Returns true when the literals `a` and `b` have equal values.
bool 
equal(const literal_expr& a, const literal_expr& b)
{
  return a.get_value() == b.get_value();
}

/// Returns true when `a` and `b` have equal operands.
bool 
equal(const unary_expr& a, const unary_expr& b)
{
  return equal(a.get_first(), b.get_first());
}

/// Returns true when `a` and `b` have equal operands.
bool 
equal(const binary_expr& a, const binary_expr& b)
{
  return equal(a.get_first(), b.get_first()) &&
         equal(a.get_second(), b.get_second());
}

/// Returns true when `a` and `b` have equal operands.
bool 
equal(const ternary_expr& a, const ternary_expr& b)
{
  return equal(a.get_first(), b.get_first()) &&
         equal(a.get_second(), b.get_second()) &&
         equal(a.get_third(), b.get_third());
}

} // namespace beaker
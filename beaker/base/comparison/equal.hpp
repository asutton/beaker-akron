// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_COMPARISON_EQUAL_HPP
#define BEAKER_BASE_COMPARISON_EQUAL_HPP

#include <beaker/base/lang.hpp>
#include <beaker/base/seq.hpp>


namespace beaker {

// -------------------------------------------------------------------------- //
// Equality of names

bool equal(const name&, const name&);


// -------------------------------------------------------------------------- //
// Equality of types

struct base_type;
struct object_type;

bool equal(const type&, const type&);

/// The same base types are always equal.
constexpr bool 
equal(const base_type&, const base_type&)
{
  return true;
}

/// The same object types are always equal. This must be overriden if the
/// object types have properties that cause their values to differ.
constexpr bool 
equal(const object_type&, const object_type&)
{
  return true;
}


// -------------------------------------------------------------------------- //
// Equality of expressions

struct literal_expr;
struct nullary_expr;
struct unary_expr;
struct binary_expr;
struct ternary_expr;

bool equal(const expr&, const expr&);

/// The same nullary expressions are always equal.
constexpr bool 
equal(const nullary_expr&, const nullary_expr&)
{
  return true;
}

bool equal(const literal_expr&, const literal_expr&);
bool equal(const unary_expr&, const unary_expr&);
bool equal(const binary_expr&, const binary_expr&);
bool equal(const ternary_expr&, const ternary_expr&);


// -------------------------------------------------------------------------- //
// Equality of supporting types

/// Compares two sequences for equality.
template<typename T>
bool 
equal(const seq<T>& a, const seq<T>& b)
{
  auto cmp = [](const T& x, const T& y) {
    return equal(x, y);
  };
  return std::equal(a.begin(), a.end(), b.begin(), b.end(), cmp);
}

// -------------------------------------------------------------------------- //
// Functional

/// A function object for term comparison that can be used with standard
/// algorithms and containers.
struct term_equal
{
  template<typename T>
  bool operator()(const T& a, const T& b) const noexcept
  {
    return equal(a, b);
  }
};

} // namespace beaker


#endif

// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_COMPARISON_EQUAL_HPP
#define BEAKER_BASE_COMPARISON_EQUAL_HPP

#include <beaker/base/lang.hpp>
#include <beaker/base/seq.hpp>


namespace beaker {

// -------------------------------------------------------------------------- //
// Primary interface

bool equal(const name&, const name&);
bool equal(const type&, const type&);
bool equal(const expr&, const expr&);

/// Returns truen if `a` and `b` have equal elements.
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
// Dispatch interface

struct base_type;
struct object_type;

struct literal_expr;
struct nullary_expr;
struct unary_expr;
struct binary_expr;
struct ternary_expr;

/// Returns true if two names are equal. Behavior is undefined if overload
/// resolution selects this function.
inline bool
equal_name(const name&, const name&)
{
  assert(false && "function not defined");
}

/// Returns true if two types are equal. Behavior is undefined if overload
/// resolution selects this function.
inline bool
equal_type(const type&, const type&)
{
  assert(false && "function not defined");
}

/// Returns true if two expressions are equal. Behavior is undefined if 
/// overload resolution selects this function.
inline bool
equal_expr(const expr&, const expr&)
{
  assert(false && "function not defined");
}

/// The same base types are always equal.
constexpr bool equal_type(const base_type&, const base_type&) { return true; }

/// The same object types are always equal. This must be overriden if the
/// object types have properties that cause their values to differ.
constexpr bool equal_type(const object_type&, const object_type&) { return true; }

/// The same nullary expressions are always equal.
constexpr bool equal_expr(const nullary_expr&, const nullary_expr&) { return true; }

bool equal_expr(const literal_expr&, const literal_expr&);
bool equal_expr(const unary_expr&, const unary_expr&);
bool equal_expr(const binary_expr&, const binary_expr&);
bool equal_expr(const ternary_expr&, const ternary_expr&);


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

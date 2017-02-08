// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_COMPARISON_EQUAL_HPP
#define BEAKER_BASE_COMPARISON_EQUAL_HPP

#include <beaker/base/lang.hpp>
#include <beaker/base/seq.hpp>

#include <memory>


namespace beaker {

struct name;
struct type;

/// The equal algorithm is used to determine when two terms are equal. Two
/// terms are equal when they have the same abstract syntax, and references to
/// declarations are the same.
///
/// Despite the fact that this algorithm has two "virtual" parameters, it is
/// not, in fact, a multiple dispatch algorithm. Two terms must have the same
/// kind in order to be equal.
///
/// Note that equivalence is a weaker form of equality.
struct equal_algorithm : algorithm
{
  using name_table = dispatch_table<bool(const language&, const name&, const name&)>;
  using type_table = dispatch_table<bool(const language&, const type&, const type&)>;
  using expr_table = dispatch_table<bool(const language&, const expr&, const expr&)>;

  equal_algorithm(language&);
  
  std::unique_ptr<name_table> names;
  std::unique_ptr<type_table> types;
  std::unique_ptr<expr_table> exprs;
};

bool equal(const language&, const name&, const name&);
bool equal(const language&, const type&, const type&);
bool equal(const language&, const expr&, const expr&);

/// Compares two sequences for equality.
template<typename T>
bool 
equal(const language& lang, const seq<T>& a, const seq<T>& b)
{
  auto cmp = [&lang](const T& x, const T& y) {
    return equal(lang, x, y);
  };
  return std::equal(a.begin(), a.end(), b.begin(), b.end(), cmp);
}

struct base_type;
struct object_type;

/// The same base types are always equal.
inline bool 
equal_base_type(const language&, const base_type&, const base_type&)
{
  return true;
}

/// The same object types are always equal. This must be overriden if the
/// object types have properties that cause their values to differ.
inline bool 
equal_object_type(const language&, const object_type&, const object_type&)
{
  return true;
}

struct literal_expr;
struct nullary_expr;
struct unary_expr;
struct binary_expr;
struct ternary_expr;

/// The same nullary expressions are always equal.
inline bool 
equal_nullary_expr(const language&, const nullary_expr&, const nullary_expr&)
{
  return true;
}

bool equal_literal_expr(const language&, const literal_expr&, const literal_expr&);
bool equal_unary_expr(const language&, const unary_expr&, const unary_expr&);
bool equal_binary_expr(const language&, const binary_expr&, const binary_expr&);
bool equal_ternary_expr(const language&, const ternary_expr&, const ternary_expr&);

} // namespace beaker


#endif

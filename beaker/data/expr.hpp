// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_DATA_EXPR_HPP
#define BEAKER_DATA_EXPR_HPP

#include <beaker/base/expr.hpp>


namespace beaker {
namespace data {

// TODO: Add support for array-to-sequence conversions.
enum 
{
  first_expr_kind = data_lang,
#define def_expr(e) e ## _expr_kind,
#include "expr.def"
  last_expr_kind = data_lang,
};


// Represents a tuple expression `{ e1, e2, ... en }`.
//
// The type of the expression is the tuple type `{ t1, t2, ..., tn }` where
// each `ti` is the type of the corresponding expression.
//
// NOTE: The design of this expression allows tuples to capture and reference
// to local variables. To avoid this, reference elements must first be 
// converted to objects.
struct tuple_expr : expr
{
  static constexpr int node_kind = tuple_expr_kind;

  tuple_expr(type&, const expr_seq&);
  tuple_expr(type&, expr_seq&&);

  const expr_seq& get_elements() const;
  expr_seq& get_elements();

  expr_seq elems_;
};

// Initialize the tuple.
inline 
tuple_expr::tuple_expr(type& t, const expr_seq& e)
  : expr(node_kind, t), elems_(e)
{ }

// Initialize the tuple.
inline 
tuple_expr::tuple_expr(type& t, expr_seq&& e)
  : expr(node_kind, t), elems_(std::move(e))
{ }

// Returns the elements of the tuple.
inline const expr_seq& tuple_expr::get_elements() const { return elems_; }

// Returns the elements of the tuple.
inline expr_seq& tuple_expr::get_elements() { return elems_; }


// Represents an array expression `[ e1, e2, ... en ]`.
//
// All elements of the array must have the same type `t`, and the type of
// the expression is the array type `t[n]`.
//
// NOTE: The design of this expression allows arrays of references if all
// subexpressions have the same type reference-to-t. This makes it easy to
// return references to local variables. To avoid this, ensure that all
// reference elements are first converted to objects.
struct array_expr : expr
{
  static constexpr int node_kind = array_expr_kind;

  array_expr(type&, const expr_seq&);
  array_expr(type&, expr_seq&&);

  const expr_seq& get_elements() const;
  expr_seq& get_elements();

  expr_seq elems_;
};

// Initialize the tuple.
inline 
array_expr::array_expr(type& t, const expr_seq& e)
  : expr(node_kind, t), elems_(e)
{ }

// Initialize the tuple.
inline 
array_expr::array_expr(type& t, expr_seq&& e)
  : expr(node_kind, t), elems_(std::move(e))
{ }

// Returns the elements of the tuple.
inline const expr_seq& array_expr::get_elements() const { return elems_; }

// Returns the elements of the tuple.
inline expr_seq& array_expr::get_elements() { return elems_; }


// Represents a tuple access expression `e.n`.
//
// In this expression, `e` must be a tuple or a reference to a tuple. If
// e is a reference to a tuple, then the type of the expression is a reference
// to the type of the `n`th element. If not, the the type is that of the `n`th
// element.
struct elem_expr : expr
{
  static constexpr int node_kind = elem_expr_kind;

  elem_expr(type&, expr&, int);

  const expr& get_tuple() const;
  expr& get_tuple();

  int get_index() const;

  expr* tup_;
  int elem_;
};

inline
elem_expr::elem_expr(type& t, expr& e, int n)
  : expr(node_kind, t), tup_(&e), elem_(n)
{ }

// Returns the tuple being indexed.
inline const expr& elem_expr::get_tuple() const { return *tup_; }

// Returns the tuple being indexed.
inline expr& elem_expr::get_tuple() { return *tup_; }

// Returns the index of the tuple element.
inline int elem_expr::get_index() const { return elem_; }


// Represents an array subscript expression e1[e2].
//
// In this expression `e1` must either be an array reference and `e2` must
// be an integer expression. If the value of `e2` exceeds the bounds of
// the array, behavior is undefined. The result of the expression is a
// reference to the element in `e1` denoted by the index `e2`.
struct index_expr : generic_binary_expr<index_expr_kind>
{
  using generic_binary_expr<index_expr_kind>::generic_binary_expr;

  const expr& get_array() const;
  expr& get_array();

  const expr& get_index() const;
  expr& get_index();
};

// Returns the array being indexed.
inline const expr& index_expr::get_array() const { return get_lhs(); }

// Returns the array being indexed.
inline expr& index_expr::get_array() { return get_lhs(); }

// Returns the index of the array element.
inline const expr& index_expr::get_index() const { return get_rhs(); }

// Returns the index of the array element.
inline expr& index_expr::get_index() { return get_rhs(); }


} // namespace data
} // namespace beaker


#endif

// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_TUPLE_EXPR_HPP
#define BEAKER_SYS_TUPLE_EXPR_HPP

#include <beaker/base/expr.hpp>


namespace beaker {
namespace sys_tuple {

enum {
  first_expr_kind = sys_tuple_lang_block,
#define def_expr(e) e ## _expr_kind,
#include "expr.def"
  last_expr_kind
};


// -------------------------------------------------------------------------- //
// Expressions

/// Represents a (possibly empty) sequence of expressions of the form 
/// `{e1, e2, ...}`. The expression produces an object whose sub-objects 
/// contain the values of each expression. The type of the expression is the 
/// tuple type `{t1, t2, ...}` where each element type corresponds to the 
/// object type of its corresponding expression (an object type cannot have 
/// reference sub-objects).
///
/// \todo Each sub-object copy initialized by its corresponding operand.
struct tuple_expr : expr
{
  static constexpr int node_kind = tuple_expr_kind;

  tuple_expr(type&, const expr_seq&);
  tuple_expr(type&, expr_seq&&);

  const expr_seq& get_elements() const;
  expr_seq& get_elements();

  expr_seq elems_;
};

inline
tuple_expr::tuple_expr(type& t, const expr_seq& es)
  : expr(node_kind, t), elems_(es)
{ }

inline
tuple_expr::tuple_expr(type& t, expr_seq&& es)
  : expr(node_kind, t), elems_(std::move(es))
{ }

// Returns the sequence of elemens in the tuple expression.
inline const expr_seq& tuple_expr::get_elements() const { return elems_; }

// Returns the sequence of elemens in the tuple expression.
inline expr_seq& tuple_expr::get_elements() { return elems_; }


/// Represents the projection `e.n` where `e` is denotes a tuple and `n`
/// is the nth element requested.
///
/// \todo The abstract syntax does not permint `n` to be a constant expression,
/// and trying to use `e.n` as concrete syntax also does not work. I think that
/// the operator `e.[n]` would satisfy both requirements (and work for arrays
/// also). It would also support generalized projection (see below).
///
/// \todo I think that if `e` is has object type, then `e.n` would produce
/// an expiring reference. Otherwise, I believe the expression should be an
/// lvalue. Note that we don't have expiring references, so the result of
/// the expression would be an rvalue (i.e., requiring a copy).
///
/// \todo Make a generalized version of this algorithm that projects a sequence
/// of elements (e.g., `e.[3, 1, 0]`). That seems like it could be useful.
struct proj_expr : expr
{
  static constexpr int node_kind = proj_expr_kind;

  proj_expr(type&, expr&, int);

  const expr& get_object() const;
  expr& get_object();

  int get_element() const;

  expr& obj_;
  int elem_;
};

inline
proj_expr::proj_expr(type& t, expr& e, int n)
  : expr(node_kind, t), obj_(e), elem_(n)
{ }

/// Returns the tuple object of the expression.
inline const expr& proj_expr::get_object() const { return obj_; }

/// Returns the tuple object of the expression.
inline expr& proj_expr::get_object() { return obj_; }

/// Returns the requested element.
inline int proj_expr::get_element() const { return elem_; }

} // namespace sys_tuple
} // namespace beaker


#endif


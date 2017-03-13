// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_FN_EXPR_HPP
#define BEAKER_SYS_FN_EXPR_HPP

#include <beaker/base/expr.hpp>


namespace beaker {
namespace sys_fn {

enum 
{
  first_expr_kind = sys_fn_lang_block,
#define def_expr(NS, E) E ## _expr_kind,
#include "expr.def"
  last_expr_kind
};


// -------------------------------------------------------------------------- //
// Expressions

/// Represents the expression `f(e1, e2, ..., en).
///
/// This calls the function designated by the expression `f` with the arguments
/// `e1`, `e2, etc.
struct call_expr : expr 
{
  static constexpr int node_kind = call_expr_kind;

  call_expr(type&, expr&, const expr_seq&);
  call_expr(type&, expr&, expr_seq&&);

  const expr& get_function() const;
  expr& get_function();

  const expr_seq& get_arguments() const;
  expr_seq& get_arguments();

  expr* fn_;
  expr_seq args_;
};

inline
call_expr::call_expr(type& t, expr& f, const expr_seq& a)
  : expr(node_kind, t), fn_(&f), args_(a)
{ }

inline
call_expr::call_expr(type& t, expr& f, expr_seq&& a)
  : expr(node_kind, t), fn_(&f), args_(std::move(a))
{ }

// Returns the called function.
inline const expr& call_expr::get_function() const { return *fn_; }

// Returns the called function.
inline expr& call_expr::get_function() { return *fn_; }

// Returns the sequence of call arguments.
inline const expr_seq& call_expr::get_arguments() const { return args_; }

// Returns the sequence of call arguments.
inline expr_seq& call_expr::get_arguments() { return args_; }


/// Represents the expression `e1 == e2`.
///
/// The operands `e1` and `e2` shall have the same function type. The type of 
/// the expression shall be `bool`.
///
/// The value of the expression is `true` when the values of `e1` and `e2` 
/// denote the same function (symbol).
struct eq_expr : binary_expr_impl<eq_expr_kind> 
{
  using binary_expr_impl<eq_expr_kind>::binary_expr_impl;
};


/// Represents the expression `e1 != e2`.
///
/// The operands `e1` and `e2` shall have the same function type. The type of 
/// the expression shall be `bool`.
///
/// The value of the expression is `true` when the values of `e1` and `e2` 
/// denote the same function (symbol).
struct ne_expr : binary_expr_impl<ne_expr_kind> 
{
  using binary_expr_impl<ne_expr_kind>::binary_expr_impl;
};


} // namespace sys_fn
} // namespace beaker


#endif

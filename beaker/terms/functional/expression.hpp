// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_TERM_FUNCTIONAL_EXPRESSION_HPP
#define BEAKER_TERM_FUNCTIONAL_EXPRESSION_HPP

#include <beaker/common/language.hpp>
#include <beaker/common/expression.hpp>
#include <beaker/common/declaration.hpp>


namespace beaker {
namespace functional {

enum {
  first_expr_kind = functional_lang,
  lambda_expr_kind,
  call_expr_kind,
  let_expr_kind,
  seq_expr_kind
};


// Represents unnamed functions of the form `lambda (p*) -> p => e`. 
//
// Internally, this is an anonymous function.
//
// TODO: Implement lambda capture. This might require some notion of
// a record type.
struct lambda_expr : expr
{
  static constexpr int node_kind = lambda_expr_kind;
  
  lambda_expr(type&, decl&);

  const fn_decl& get_function() const;
  fn_decl& get_function();

  const decl_seq& get_parameters() const;
  decl_seq& get_parameters();

  const decl& get_return() const;
  decl& get_return();

  const expr& get_definition() const;
  expr& get_definition();

  decl* fn_;
};

inline
lambda_expr::lambda_expr(type& t, decl& f)
  : expr(node_kind, t), fn_(&f)
{ }


// Represents expressions of the form `let x = e1 in e2`.
//
// The name binding is represented as a local variable. Note that x could
// be any kind of value declaration: variable, register, or constant.
struct let_expr : expr 
{
  static constexpr int node_kind = let_expr_kind;

  let_expr(type&, decl&, expr&);

  const decl& get_variable() const;
  decl& get_variable();

  const expr& get_expression() const;
  expr& get_expression();

  decl* var_;
  expr* expr_;
};

inline 
let_expr::let_expr(type& t, decl& d, expr& e)
  : expr(node_kind, t), var_(&d), expr_(&e)
{ }

// Returns the declared variable.
inline const decl& let_expr::get_variable() const { return *var_; }

// Returns the declared variable.
inline decl& let_expr::get_variable() { return *var_; }

// Returns the expression in which the variable is bound.
inline const expr& let_expr::get_expression() const { return *expr_; }

// Returns the expression in which the variable is bound.
inline expr& let_expr::get_expression() { return *expr_; }


// Represents sequential execution of the form `e1 ; e2`.
struct seq_expr : binary_expr<seq_expr_kind>
{
  using binary_expr<seq_expr_kind>::binary_expr;
};


// Operations

void print_expr(std::ostream&, const expr&);

} // namespace functional
} // namespace beaker


#endif

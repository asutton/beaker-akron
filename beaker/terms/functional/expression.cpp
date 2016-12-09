// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "expression.hpp"

#include <beaker/common/type.hpp>
#include <beaker/common/declaration.hpp>

#include <iostream>


namespace beaker {
namespace functional {

// Returns the underlying function of the lambda.
const fn_decl& lambda_expr::get_function() const { return cast<fn_decl>(*fn_); }

// Returns the underlying function of the lambda.
fn_decl& lambda_expr::get_function() { return cast<fn_decl>(*fn_); }

// Returns the list of parameters of the lambda.
const decl_seq& lambda_expr::get_parameters() const { return get_function().get_parameters(); }

// Returns the list of parameters of the lambda.
decl_seq& lambda_expr::get_parameters() { return get_function().get_parameters(); }

// Returns the declared return object.
const decl& lambda_expr::get_return() const { return get_function().get_return(); }

// Returns the declared return object.
decl& lambda_expr::get_return() { return get_function().get_return(); }

// Returns the body of the lambda expression.
const expr& lambda_expr::get_definition() const { return get_function().get_definition(); }

// Returns the body of the lambda expression.
expr& lambda_expr::get_definition() { return get_function().get_definition(); }


static void
print_lambda_expr(std::ostream& os, const lambda_expr& e)
{
  // FIXME: Print the parameter specification, etc.
  os << "lambda" << ' ';
  os << '(';
  const decl_seq& parms = e.get_parameters();
  for (auto iter = parms.begin(); iter != parms.end(); ++iter) {
    print(os, *iter);
    if (std::next(iter) != parms.end())
      os << ',';
  }
  os << ')';
  os << " -> ";
  print(os, e.get_type());
  os << " => ";
  print(os, e.get_definition());
}

static void
print_let_expr(std::ostream& os, const let_expr& e)
{
  os << "let" << ' '; 
  print(os, e.get_variable());
  os << " in ";
  print(os, e.get_expression());
}

static void
print_seq_expr(std::ostream& os, const seq_expr& e)
{
  os << '(';
  print(os, e.get_left());
  os << ';';
  print(os, e.get_right());
  os << ')';
}

void
print_expr(std::ostream& os, const expr& e)
{
  switch (e.get_kind()) {
    case lambda_expr_kind:
      return print_lambda_expr(os, cast<lambda_expr>(e));
    case let_expr_kind:
      return print_let_expr(os, cast<let_expr>(e));
    case seq_expr_kind:
      return print_seq_expr(os, cast<seq_expr>(e));
    default:
      break;
  }
  assert(false && "not a functional expression");
}


} // namespace functional
} // namespace beaker

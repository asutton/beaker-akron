// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "parser.hpp"

#include <iostream>
#include <stdexcept>


namespace icalc {

/// Optionally consume a token of kind k.
token*
parser::match(int k)
{
  if (next_token_is(k))
    return consume();
  else
    return nullptr;
}

// Require that a token to be consumed has kind k.
token*
parser::require(int k)
{
  if (token* tok = match(k))
    return tok;
  else
    throw std::runtime_error("parse error");
}

/// Parse an expression.
///
///   expression -> conditional-expression
expr&
parser::expression()
{
  return unary_expression();
}

/// Parse a conditional expression.
///
///   conditional-expression -> logical-or-expression
///                           | logical-or-expression '?' expression ':' expression
expr&
parser::conditional_expression()
{
  expr& e1 = logical_or_expression();
  if (token* q = match(question_tok)) {
    expr& e2 = expression();
    token* c = require(colon_tok);
    expr& e3 = expression();
    return on_condition(e1, q, e2, c, e3);
  }
  return e1;
}

/// Parse a logical-or expression.
///
///   logical-or-expression -> logical-and-expression
///                          | logical-or-expression '||' logical-and-expression
expr&
parser::logical_or_expression()
{
  expr* e1 = &logical_and_expression();
  while (true) {
    if (token* tok = match(bar_bar_tok)) {
      expr* e2 = &logical_and_expression();
      e1 = &on_logical_or(*e1, tok, *e2);
    }
    else {
      // FIXME: Isn't this an error?
      break;
    }
  }
  return *e1;
}

expr&
parser::logical_and_expression()
{
  return unary_expression();
}

expr&
parser::bitwise_or_expression()
{
  assert(false && "not implemented");
}

expr&
parser::bitwise_xor_expression()
{
  assert(false && "not implemented");
}

expr&
parser::bitwise_and_expression()
{
  assert(false && "not implemented");
}

expr&
parser::equality_expression()
{
  assert(false && "not implemented");
}

expr& 
parser::relational_expression()
{
  assert(false && "not implemented");
}

expr&
parser::additive_expression()
{
  assert(false && "not implemented");
}

expr&
parser::multiplicative_expression()
{
  assert(false && "not implemented");
}

/// Parse a unary expression.
///
///   unary-expression -> primary-expression
///                     | '-' unary-expression
///                     | '~' unary-expression -- FIXME: not implemented
///                     | '!' unary-expression
///
/// \todo Add language support for one's complement.
expr&
parser::unary_expression()
{
  switch (lookahead()) {
    case minus_tok: {
      token* op = consume();
      expr& e = unary_expression();
      return on_negation(op, e);
    }
    case bang_tok: {
      token* op = consume();
      expr& e = unary_expression();
      return on_logical_not(op, e);
    }    
    default:
      break;
  }
  return primary_expression();
}

/// Parse a primary expression.
///
///   primary-expression -> boolean-literal
///                       | integer-literal
///                       | '(' expression ')'
///
/// \todo Implement good brace matching for diagnostics.
expr&
parser::primary_expression()
{
  switch (lookahead()) {
    case bool_tok:
      return boolean_literal();
    case int_tok:
      return integer_literal();
    case lparen_tok: {
      require(lparen_tok);
      expr& e = expression();
      require(rparen_tok);
      return e;
    }
    default:
      break;
  }
  throw std::runtime_error("expected primary expression");
}

/// Parses a boolean literal.
///
///   boolean-literal -> 'true' | 'false'
expr&
parser::boolean_literal()
{
  return on_boolean_literal(consume());
}

/// Parses a boolean literal.
///
///   boolean-literal -> 'true' | 'false'
expr&
parser::integer_literal()
{
  return on_integer_literal(consume());
}

// -------------------------------------------------------------------------- //
// Semantics

expr&
parser::on_condition(expr& e1, token* q, expr& e2, token* c, expr& e3)
{
  return build.make_if_expr(e1, e2, e3);
}

expr&
parser::on_logical_or(expr& e1, token* tok, expr& e2)
{
  return build.make_or_else_expr(e1, e2);
}

expr&
parser::on_negation(token* tok, expr& e1)
{
  return build.make_neg_expr(e1);
}

expr&
parser::on_logical_not(token* tok, expr& e1)
{
  return build.make_neg_expr(e1);
}

expr&
parser::on_boolean_literal(token* tok)
{
  bool_token* tok1 = static_cast<bool_token*>(tok);
  if (tok1->get_value())
    return build.make_true_expr();
  else
    return build.make_false_expr();
}

expr&
parser::on_integer_literal(token* tok)
{
  int_token* tok1 = static_cast<int_token*>(tok);
  return build.make_int_expr(build.get_int32_type(), tok1->get_value());
}

} // namespace icalc

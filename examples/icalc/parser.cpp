// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "parser.hpp"

#include <stdexcept>


namespace icalc {

token*
parser::match(int k)
{
  if (next_token_is(k))
    return consume();
  else
    return nullptr;
}

// FIXME: Improve diagnostics.
token*
parser::require(int k)
{
  if (token* tok = match(k))
    return tok;
  else
    throw std::runtime_error("parse error");
}

expr&
parser::expression()
{
  return primary_expression();
}

/// Parse a primary expression.
///
///   primary-expression:
///     literal
///     '(' expression ')'
///     
expr&
parser::primary_expression()
{
  switch (lookahead()->get_kind()) {
    case bool_tok:
      return boolean_literal();
    case int_tok:
      return integer_literal();
    case lparen_tok:
      return grouped_expression();
  }
  throw std::runtime_error("expected primary expression");
}

expr&
parser::boolean_literal()
{
  bool_token* tok = static_cast<bool_token*>(consume());
  if (tok->get_value())
    return build.make_true_expr();
  else
    return build.make_false_expr();
}

expr&
parser::integer_literal()
{
  int_token* tok = static_cast<int_token*>(consume());
  return build.make_int_expr(build.get_int32_type(), tok->get_value());
}

expr&
parser::grouped_expression()
{
  require(lparen_tok);
  expr& e = expression();
  require(rparen_tok);
  return e;
}

} // namespace icalc

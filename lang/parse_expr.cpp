// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

namespace bpl {

/// Parse an expression.
///
///   expression -> conditional-expression
expr&
parser::expression()
{
  return conditional_expression();
}

/// Parse a conditional expression.
///
///   conditional-expression -> logical-or-expression
///                           | logical-or-expression '?' expression ':' expression
expr&
parser::conditional_expression()
{
  expr& e1 = logical_or_expression();
  if (token q = accept(question_tok)) {
    expr& e2 = expression();
    token c = expect(colon_tok);
    expr& e3 = expression();
    return act.on_condition(e1, q, e2, c, e3);
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
    if (token tok = accept(bar_bar_tok)) {
      expr* e2 = &logical_and_expression();
      e1 = &act.on_logical_or(*e1, tok, *e2);
    }
    else {
      break;
    }
  }
  return *e1;
}

/// Parse a logical-and expression.
///
///   logical-and-expression -> bitwise-or-expression
///                           | logical-and-expression '&&' bitwise-or-expression
expr&
parser::logical_and_expression()
{
  expr* e1 = &bitwise_or_expression();
  while (true) {
    if (token tok = accept(amp_amp_tok)) {
      expr* e2 = &bitwise_or_expression();
      e1 = &act.on_logical_and(*e1, tok, *e2);
    }
    else {
      break;
    }
  }
  return *e1;
}

/// Parse a bitwise-or expression.
///
///   bitwise-or-expression -> bitwise-xor-expression
///                          | bitwise-or-expression '|' bitwise-xor-expression
expr&
parser::bitwise_or_expression()
{
  expr* e1 = &bitwise_xor_expression();
  while (true) {
    if (token tok = accept(bar_tok)) {
      expr* e2 = &bitwise_xor_expression();
      e1 = &act.on_bitwise_or(*e1, tok, *e2);
    }
    else {
      break;
    }
  }
  return *e1;
}

/// Parse a bitwise-xor expression.
///
///   bitwise-xor-expression -> bitwise-and-expression
///                           | bitwise-xor-expression '^' bitwise-and-expression
expr&
parser::bitwise_xor_expression()
{
  expr* e1 = &bitwise_and_expression();
  while (true) {
    if (token tok = accept(caret_tok)) {
      expr* e2 = &bitwise_and_expression();
      e1 = &act.on_bitwise_xor(*e1, tok, *e2);
    }
    else {
      break;
    }
  }
  return *e1;
}

/// Parse a bitwise-and expression.
///
///   bitwise-and-expression -> equality-expression
///                           | bitwise-and-expression '||' equality-expression
expr&
parser::bitwise_and_expression()
{
  expr* e1 = &equality_expression();
  while (true) {
    if (token tok = accept(amp_tok)) {
      expr* e2 = &equality_expression();
      e1 = &act.on_bitwise_and(*e1, tok, *e2);
    }
    else {
      break;
    }
  }
  return *e1;
}

/// Parse an equality-expression.
///
///   equality-expression -> relational-expression
///                        | equality-expression '==' relational-expression
///                        | equality-expression '!=' relational-expression
expr&
parser::equality_expression()
{
  expr* e1 = &relational_expression();
  while (true) {
    if (token tok = accept(eq_eq_tok)) {
      expr* e2 = &relational_expression();
      e1 = &act.on_equal(*e1, tok, *e2);
    }
    if (token tok = accept(bang_eq_tok)) {
      expr* e2 = &relational_expression();
      e1 = &act.on_not_equal(*e1, tok, *e2);
    }
    else {
      break;
    }
  }
  return *e1;
}

/// Parse an relational-expression.
///
///   relational-expression -> additive-expression
///                          | relational-expression '<' additive-expression
///                          | relational-expression '>' additive-expression
///                          | relational-expression '<=' additive-expression
///                          | relational-expression '>=' additive-expression
expr& 
parser::relational_expression()
{
  expr* e1 = &additive_expression();
  while (true) {
    if (token tok = accept(lt_tok)) {
      expr* e2 = &additive_expression();
      e1 = &act.on_less(*e1, tok, *e2);
    }
    if (token tok = accept(gt_tok)) {
      expr* e2 = &additive_expression();
      e1 = &act.on_greater(*e1, tok, *e2);
    }
    if (token tok = accept(lt_eq_tok)) {
      expr* e2 = &additive_expression();
      e1 = &act.on_less_equal(*e1, tok, *e2);
    }
    if (token tok = accept(gt_eq_tok)) {
      expr* e2 = &additive_expression();
      e1 = &act.on_greater_equal(*e1, tok, *e2);
    }
    else {
      break;
    }
  }
  return *e1;
}

/// Parse an additive-expression.
///
///   additive-expression -> multiplicative-expression
///                        | additive-expression '+' multiplicative-expression
///                        | additive-expression '-' multiplicative-expression
expr&
parser::additive_expression()
{
  expr* e1 = &multiplicative_expression();
  while (true) {
    if (token tok = accept(plus_tok)) {
      expr* e2 = &multiplicative_expression();
      e1 = &act.on_addition(*e1, tok, *e2);
    }
    if (token tok = accept(minus_tok)) {
      expr* e2 = &multiplicative_expression();
      e1 = &act.on_subtraction(*e1, tok, *e2);
    }
    else {
      break;
    }
  }
  return *e1;
}

/// Parse an equality-expression.
///
///   multiplicative-expression -> unary-expression
///                              | multiplicative-expression '*' unary-expression
///                              | multiplicative-expression '/' unary-expression
///                              | multiplicative-expression '%' unary-expression
expr&
parser::multiplicative_expression()
{
  expr* e1 = &unary_expression();
  while (true) {
    if (token tok = accept(star_tok)) {
      expr* e2 = &unary_expression();
      e1 = &act.on_multiplication(*e1, tok, *e2);
    }
    if (token tok = accept(slash_tok)) {
      expr* e2 = &unary_expression();
      e1 = &act.on_division(*e1, tok, *e2);
    }
    if (token tok = accept(percent_tok)) {
      expr* e2 = &unary_expression();
      e1 = &act.on_remainder(*e1, tok, *e2);
    }
    else {
      break;
    }
  }
  return *e1;
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
      token op = consume();
      expr& e = unary_expression();
      return act.on_negation(op, e);
    }
    case bang_tok: {
      token op = consume();
      expr& e = unary_expression();
      return act.on_logical_not(op, e);
    }    
    case tilde_tok: {
      token op = consume();
      expr& e = unary_expression();
      return act.on_bitwise_not(op, e);
    }    
    default:
      break;
  }
  return primary_expression();
}

/// Parse a primary expression.
///
///   primary-expression -> id-expression
///                       | boolean-literal
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
    case dec_int_tok:
      return integer_literal();
    case id_tok:
      return id_expression();
    case lparen_tok: {
      require(lparen_tok);
      expr& e = expression();
      expect(rparen_tok);
      return e;
    }
    default:
      break;
  }
  throw syntax_error(current_location(), "expected primary expression");
}

/// Parses an id-expression.
///
///   id-expression -> identifier
///
/// \todo Allow qualified names.
expr&
parser::id_expression()
{
  token id = require(id_tok);
  return act.on_id(get_symbol(id), get_location(id));
}

/// Parses a boolean literal.
///
///   boolean-literal -> 'true' | 'false'
expr&
parser::boolean_literal()
{
  // FIXME: Extract the value here.
  return act.on_bool(require(bool_tok));
}

/// Parses a boolean literal.
///
///   boolean-literal -> 'true' | 'false'
expr&
parser::integer_literal()
{
  // FIXME: Extract the value here.
  return act.on_int(require(dec_int_tok));
}

} // namespace bpl

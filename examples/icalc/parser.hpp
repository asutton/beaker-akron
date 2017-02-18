// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef ICALC_PARSER_HPP
#define ICALC_PARSER_HPP

#include "lang.hpp"
#include "lexer.hpp"

namespace icalc {

/// Parsers a sequence of tokens to produce a tree.
///
/// \todo The tokens (and pointers to them) should be non-modifiable.
///
/// \todo Factor semantic actions out of the parser? It's not strictly 
/// necessary, but it does help keep the implementation clean. Also, we could
/// pass the default builder through that separation (i.e., use a translation
/// context that binds a default builder, module, and language object.)
struct parser
{
  parser(builder& b, token* f, token* l)
    : build(b), curr(f), last(l)
  { }

  // Stream control
  bool eof() const;
  int lookahead() const;
  int lookahead(int) const;
  token match(int);
  token require(int);
  token consume();
  void consume(int);

  int next_token() const;
  bool next_token_is(int) const;
  bool next_token_is_not(int) const;

  // Grammar productions
  expr& expression();
  expr& conditional_expression();
  expr& logical_or_expression();
  expr& logical_and_expression();
  expr& bitwise_or_expression();
  expr& bitwise_xor_expression();
  expr& bitwise_and_expression();
  expr& equality_expression();
  expr& relational_expression();
  expr& additive_expression();
  expr& multiplicative_expression();
  expr& unary_expression();
  expr& primary_expression();
  expr& boolean_literal();
  expr& integer_literal();

  // Semantics
  expr& on_condition(expr&, token, expr&, token, expr&);
  expr& on_logical_or(expr&, token, expr&);

  expr& on_negation(token, expr&);
  expr& on_bitwise_not(token, expr&);
  expr& on_logical_not(token, expr&);

  expr& on_boolean_literal(token);
  expr& on_integer_literal(token);

  builder& build;
  token* curr;
  token* last;
};

/// Returns true if the at the end of input.
inline bool parser::eof() const { return curr == last; }

/// Returns the current lookahead token.
inline int
parser::lookahead() const
{
  if (eof())
    return eof_tok;
  else
    return (*curr)->get_kind();
}

/// Consumes the current lookahead token.
inline token
parser::consume()
{
  if (curr == last)
    return nullptr;
  return *curr++;
}

/// Returns true if the next token has kind `k`.
inline bool
parser::next_token_is(int k) const
{
  if (eof())
    return false;
  else
    return lookahead() == k;
}

/// Return true if the next token is not `k`.
inline bool
parser::next_token_is_not(int k) const
{
  return !next_token_is(k);
}

} // namespace icalc

#endif


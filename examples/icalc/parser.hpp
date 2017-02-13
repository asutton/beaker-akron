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
  parser(builder& b, token** f, token** l)
    : build(b), first(f), last(l)
  { }

  // Stream control
  bool eof() const;
  token* lookahead() const;
  token* lookahead(int) const;
  token* match(int);
  token* require(int);
  token* consume();
  void consume(int);

  bool next_token_is(int) const;
  bool next_token_is_not(int) const;

  // Grammar productions
  expr& expression();

  expr& primary_expression();
  expr& boolean_literal();
  expr& integer_literal();
  expr& grouped_expression();

  builder& build;
  token** first;
  token** last;
};

/// Returns true if the at the end of input.
inline bool parser::eof() const { return first == last; }

/// Returns the current lookahead token.
inline token* 
parser::lookahead() const
{
  return *first;
}

/// Consumes the current lookahead token.
inline token*
parser::consume()
{
  if (first == last)
    return nullptr;
  return *first++;
}

/// Returns true if the next token has kind `k`.
inline bool
parser::next_token_is(int k) const
{
  if (eof())
    return false;
  else
    return lookahead()->get_kind() == k;
}

/// Return true if the next token is not `k`.
inline bool
parser::next_token_is_not(int k) const
{
  return !next_token_is(k);
}

} // namespace icalc

#endif


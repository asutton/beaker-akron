// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef ICALC_LEXER_HPP
#define ICALC_LEXER_HPP

#include "token.hpp"

#include <cassert>
#include <cctype>
#include <stdexcept>
#include <string>


namespace icalc {

// Kinds of tokens.
enum token_kind 
{
  eof_tok,

  lparen_tok,
  rparen_tok,
  
  plus_tok,
  minus_tok,
  star_tok,
  slash_tok,
  percent_tok,
  amp_tok,
  pipe_tok,
  caret_tok,

  eq_eq_tok,
  bang_eq_tok,
  lt_tok,
  gt_tok,
  lt_eq_tok,
  gt_eq_tok,

  amp_amp_tok,
  pipe_pipe_tok,
  bang_tok,

  bool_tok,
  int_tok,
};


/// The lexer is responsible for transforming a sequence of characters into
/// a sequence of tokens.
///
/// \todo This needs to be a token store.
struct lexer : token_store
{
  lexer(const char* f, const char* l)
    : first(f), last(l)
  { }
  
  token* next();

  // Stream manipulation
  bool eof() const;
  char lookahead() const;
  char lookahead(int) const;
  char consume();
  void consume(int);

  // Token constructors
  
  /// Make an token over a sequence of characters. Consume the current token.
  ///
  /// \todo The 'n' refers to the number of characters the token occupies.
  /// Note that the lookahead is always pointing at the last character in
  /// the lexeme.
  token *make_basic_token(int n, int k) 
  {
    return make<basic_token>(k);
  }

  token* lparen();
  token* rparen();
  
  token* plus();
  token* minus();
  token* star();
  token* slash();
  token* percent();
  token* amp();
  token* pipe();
  token* caret();
  
  token* eq();
  token* eq_eq();
  token* bang_eq();
  token* lt();
  token* gt();
  token* lt_eq();
  token* gt_eq();
  token* pipe_pipe();
  token* amp_amp();
  token* bang();

  token* word();
  token* number();

  bool match(char);
  template<typename P> bool match_if(P);

  bool require(char);
  template<typename P> bool require_if(P);

  // State transitions.
  bool letter();
  bool digit();
  bool ident();

  const char* first;
  const char* last;
};

/// Returns true if the stream is at its end.
inline bool lexer::eof() const { return first == last; }

/// Returns the lookahead character.
inline char 
lexer::lookahead() const
{
  if (eof())
    return 0;
  else
    return *first;
}

/// Returns the nth character past the lookahead.
inline char
lexer::lookahead(int n) const
{
  if (last - first <= n)
    return 0;
  else
    return *(first + n);
}

/// Returns the current character and advances the stream.
inline char
lexer::consume()
{
  if (eof())
    return 0;
  return *first++;
}

/// Consume at most n characters.
inline void
lexer::consume(int n)
{
  if (last - first <= n)
    first = last;
  else
    first += n;
}

inline bool
lexer::match(char c)
{
  if (lookahead() == c) {
    consume();
    return true;
  }
  return false;
}

template<typename P>
inline bool
lexer::match_if(P pred)
{
  if (pred(lookahead())) {
    consume();
    return true;
  }
  return false;
}

inline bool
lexer::require(char c)
{
  assert(lookahead() == c);
  consume();
  return true;
}

template<typename P>
inline bool
lexer::require_if(P pred)
{
  assert(pred(lookahead()));
  consume();
  return true;
}

} // namespace icalc

#endif

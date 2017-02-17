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
  bar_tok,
  caret_tok,
  tilde_tok,

  eq_eq_tok,
  bang_eq_tok,
  lt_tok,
  gt_tok,
  lt_eq_tok,
  gt_eq_tok,

  amp_amp_tok,
  bar_bar_tok,
  bang_tok,

  question_tok,
  colon_tok,

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
    : curr(f), last(l)
  { }
  
  token* next();

  // Stream manipulation
  bool eof() const;
  char lookahead() const;
  char lookahead(int) const;
  char consume();
  void consume(int);
  char ignore();
  void ignore(int);

  // Token constructors
  
  /// Make an token over a sequence of characters. Consume the current token.
  ///
  /// \todo The 'n' refers to the number of characters the token occupies.
  /// Note that the lookahead is always pointing at the last character in
  /// the lexeme.
  token *make_basic_token(int k) 
  {
    return make<basic_token>(k);
  }

  void space();

  token* lparen();
  token* rparen();
  
  token* plus();
  token* minus();
  token* star();
  token* slash();
  token* percent();
  token* amp();
  token* bar();
  token* caret();
  token* tilde();
  
  token* eq();
  token* eq_eq();
  token* bang_eq();
  token* lt();
  token* gt();
  token* lt_eq();
  token* gt_eq();
  token* bang();

  token* question();
  token* colon();

  token* word();
  token* number();

  void error(const std::string&);

  bool match(char);
  template<typename P> bool match_if(P);

  bool require(char);
  template<typename P> bool require_if(P);

  // State transitions.
  bool letter();
  bool digit();
  bool ident();

  const char* curr;  // The current character.
  const char* last;  // Past the last character.
  std::string buf;   // The text of the current symbol.
};

/// Returns true if the stream is at its end.
inline bool lexer::eof() const { return curr == last; }

/// Returns the lookahead character.
inline char 
lexer::lookahead() const
{
  if (eof())
    return 0;
  else
    return *curr;
}

/// Returns the nth character past the lookahead.
inline char
lexer::lookahead(int n) const
{
  if (last - curr <= n)
    return 0;
  else
    return *(curr + n);
}

/// Buffers and returns the current character. Advances the stream.
inline char
lexer::consume()
{
  if (eof())
    return 0;
  buf += *curr++;
  return buf.back();
}

/// Ingores the current character and advances the stream.
inline char
lexer::ignore()
{
  if (eof())
    return 0;
  return *curr++;
}

/// Buffers at most n characters and advances the stream.
inline void
lexer::consume(int n)
{
  if (last - curr <= n) {
    buf.append(curr, last);
    curr = last;
  }
  else {
    buf.append(curr, curr + n);
    curr += n;
  }
}

/// Ignore at most n characters.
inline void
lexer::ignore(int n)
{
  if (last - curr <= n)
    curr = last;
  else
    curr += n;
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


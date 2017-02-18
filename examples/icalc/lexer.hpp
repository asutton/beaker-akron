// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef ICALC_LEXER_HPP
#define ICALC_LEXER_HPP

#include "token.hpp"

#include <beaker/util/stream.hpp>

#include <cassert>
#include <cctype>
#include <stdexcept>
#include <string>


namespace icalc {

/// The lexer is responsible for transforming a sequence of characters into
/// a sequence of tokens. This is designed as a function object; calling
/// the function yields the next token.
struct lexer
{
  using stream_type = beaker::input_stream<char>;

  lexer(stream_type& s) : cs(s) { }
  
  token operator()();

  // Stream manipulation
  bool eof() const;
  char lookahead() const;
  char lookahead(int) const;
  char consume();
  void consume(int);
  void ignore();
  void ignore(int);

  // Token constructors
  void space();
  token end();
  token lparen();
  token rparen();
  token plus();
  token minus();
  token star();
  token slash();
  token percent();
  token amp();
  token bar();
  token caret();
  token tilde();
  token eq();
  token lt();
  token gt();
  token bang();
  token question();
  token colon();
  token word();
  token number();

  void error(const std::string&);

  bool match(char);
  template<typename P> bool match_if(P);

  bool require(char);
  template<typename P> bool require_if(P);

  // State transitions.
  bool letter();
  bool digit();
  bool ident();

  stream_type& cs; // The underlying character stream.
  std::string buf; // The text of the current symbol.
};

/// Returns true if the stream is at its end.
inline bool lexer::eof() const { return cs.eof(); }

/// Returns the lookahead character.
inline char lexer::lookahead() const { return cs.peek(); }

/// Returns the nth character past the lookahead.
inline char lexer::lookahead(int n) const { return cs.peek(n); }

/// Buffers and returns the current character. Advances the stream.
inline char lexer::consume()
{
  if (eof())
    return 0;
  char c = cs.get();
  buf += c;
  return c;
}

/// Buffers at most n characters and advances the stream.
inline void
lexer::consume(int n)
{
  while (!eof() && n) {
    buf += cs.get();
    --n;
  }
}

/// Ignores the current character and advances the stream.
inline void lexer::ignore() { cs.ignore(); }

/// Ignore at most n characters.
inline void lexer::ignore(int n) { cs.ignore(n); }

/// If the lookead is equal to `c`, this consumes the token and returns true. 
/// Otherwise, returns false without advancing.
inline bool
lexer::match(char c)
{
  if (lookahead() == c) {
    consume();
    return true;
  }
  return false;
}

/// If the lookead satisfies `pred`, this consumes the token and returns true. 
/// Otherwise, returns false without advancing.
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


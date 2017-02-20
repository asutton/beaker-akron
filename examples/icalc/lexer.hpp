// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef ICALC_LEXER_HPP
#define ICALC_LEXER_HPP

#include "token.hpp"

#include <beaker/util/stream.hpp>
#include <beaker/util/location.hpp>

#include <cassert>
#include <cctype>
#include <stdexcept>
#include <string>


namespace icalc {

/// Represents a lexical error.
struct lexical_error : std::runtime_error
{
  lexical_error(location, const char*);

  location get_location() const;

  location loc;
};

inline 
lexical_error::lexical_error(location loc, const char* msg)
  : std::runtime_error(msg), loc(loc)
{ }

inline location lexical_error::get_location() const { return loc; }


/// The lexer is responsible for transforming a sequence of characters into
/// a sequence of tokens. This is designed as a function object; calling
/// the function yields the next token.
struct lexer
{
  using stream_type = beaker::input_stream<char>;

  lexer(stream_type& s) : cs(s), line(1), col(1) { }
  
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

  [[noreturn]] void error(location, const std::string&);

  bool match(char);
  template<typename P> bool match_if(P);

  bool require(char);
  template<typename P> bool require_if(P);

  // State transitions.
  bool letter();
  bool digit();
  bool ident();

  location make_location();

  stream_type& cs; // The underlying character stream.
  std::string buf; // The text of the current symbol.
  
  int line; // The current line.
  int col; // The current column.
  location loc; // The start location of the current token.
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
  ++col;
  return c;
}

/// Buffers at most n characters and advances the stream.
inline void
lexer::consume(int n)
{
  while (!eof() && n != 0) {
    consume();
    --n;
  }
}

/// Ignores the current character and advances the stream.
inline void lexer::ignore()
{
  cs.ignore(); 
  ++col;
}

/// Ignore at most n characters. Note that this doesn't use the stream's
/// ignore(n) because we're adjusting the column for each character.
inline void lexer::ignore(int n) 
{ 
  while (!eof() && n != 0)
    ignore();
}

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

/// Sets the current location the current line and column offset, returning it.
inline location
lexer::make_location()
{
  return (loc = location(line, col));
}

} // namespace icalc

#endif


// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_LEX_WORD_HPP
#define BEAKER_LEX_WORD_HPP

#include <cassert>
#include <cctype>


namespace beaker {

/// A facility for lexing C-style keywords, reserved words, and identifiers
/// of the form:
///
///   [a-zA-Z_][a-ZA-Z0-9_]*
///
/// Note that it is the user's responsibility to determine the meaning of the
/// lexed word.
struct c_word_lexer
{
  c_word_lexer(const char* f, const char* l)
    : start(nullptr), finish(nullptr)
  { 
    lex(f, l); 
  }

  explicit operator bool() const { return ok(); }

  bool ok() const { return start != finish; }

  const char* word_begin() const { return start; }
  const char* word_end() const { return finish; }

  void lex(const char*, const char*);
  bool is_word_start(char);
  bool is_word_rest(char);

  const char* start; // Starting character of the start
  const char* finish; // Ending character of the
};

/// Returns true if c is in the set [a-zA-Z_].
inline bool
c_word_lexer::is_word_start(char c)
{
  return std::isalpha(c) | (c == '_'); // Avoid a branch.
}

/// Returns true if c is the set [a-zA-Z0-9_].
inline bool
c_word_lexer::is_word_rest(char c)
{
  return std::isalpha(c) | std::isdigit(c) | (c == '_'); // Avoid a branch.
}

inline void
c_word_lexer::lex(const char* first, const char* limit)
{
  if (first == limit)
    return;
  if (!is_word_start(*first))
    return;
  start = first;
  ++first;
  while (first != limit && is_word_rest(*first))
    ++first;
  finish = first;
}

} // namespace beaker

#endif

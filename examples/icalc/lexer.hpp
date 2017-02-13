// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef ICALC_LEXER_HPP
#define ICALC_LEXER_HPP

#include <cctype>
#include <list>
#include <stdexcept>
#include <string>


namespace icalc {

struct token;

/// Provides storage for token lists.
///
/// \todo Use a better allocator.
struct token_store : std::list<token*>
{
  ~token_store();

  template<typename T, typename... Args>
  T* make(Args&&... args)
  {
    T* tok = new T(std::forward<Args>(args)...);
    push_back(tok);
    return tok;
  }
};


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


/// Represents an abstract symbol in the source language.
struct token
{
  explicit token(int);
  virtual ~token() { }

  int get_kind() const;
  
  int kind_;
};

inline token::token(int k)
  : kind_(k)
{ }

/// Returns the token's kind.
inline int token::get_kind() const { return kind_; }


/// Basic tokens have are simple symbols in the language; they have no
/// additional attributes.
struct basic_token : token
{
  using token::token;
};

/// A token whose attribute is a built-in value. This is a helper class for
/// other kinds of tokens.
template<typename T>
struct literal_token : token
{
  literal_token(int, T);

  T get_value() const;
  
  T value_;
};

template<typename T>
inline literal_token<T>::literal_token(int k, T v) : token(k), value_(v) { }

template<typename T>
inline T literal_token<T>::get_value() const { return value_; }


/// Represents the boolean tokens `true` and `false`.
struct bool_token : literal_token<bool>
{
  using literal_token<bool>::literal_token;
};


/// Represents the tokens of integer literals.
struct int_token : literal_token<int>
{
  using literal_token<int>::literal_token;
};


/// Returns true if c is an identifier character.
inline bool
isident(char c)
{
  return std::isalpha(c) || std::isdigit(c);
}


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
  
  // Make an token over a sequence of characters.
  token *make_basic_token(int n, int k) 
  {
    consume(n);
    return make<basic_token>(k);
  }

  token* lparen() { return make_basic_token(1, lparen_tok); }
  token* rparen() { return make_basic_token(1, rparen_tok); }
  token* plus() { return make_basic_token(1, plus_tok); }
  token* minus() { return make_basic_token(1, minus_tok); }
  token* star() { return make_basic_token(1, star_tok); }
  token* slash() { return make_basic_token(1, slash_tok); }
  token* percent() { return make_basic_token(1, percent_tok); }
  token* amp() { return make_basic_token(1, amp_tok); }
  token* pipe() { return make_basic_token(1, pipe_tok); }
  token* caret() { return make_basic_token(1, caret_tok); }

  token* eq_eq() { return make_basic_token(2, eq_eq_tok); }
  token* bang_eq() { return make_basic_token(2, bang_eq_tok); }
  token* lt() { return make_basic_token(1, lt_tok); }
  token* gt() { return make_basic_token(1, gt_tok); }
  token* lt_eq() { return make_basic_token(2, lt_eq_tok); }
  token* gt_eq() { return make_basic_token(2, gt_eq_tok); }

  token* amp_amp() { return make_basic_token(2, amp_amp_tok); }
  token* pipe_pipe() { return make_basic_token(2, pipe_pipe_tok); }
  token* bang() { return make_basic_token(1, bang_tok); }

  token* word();
  token* number();

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

} // namespace icalc

#endif


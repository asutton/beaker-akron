// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef ICALC_TOKEN_HPP
#define ICALC_TOKEN_HPP

#include <list>


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


} // namespace icalc

#endif


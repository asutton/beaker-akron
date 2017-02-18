// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef ICALC_TOKEN_HPP
#define ICALC_TOKEN_HPP

#include <vector>
#include <memory>


namespace beaker {

/// Represents an abstract symbol in the source language. A token is defined
/// by its kind (an integer value) and an optional attribute. 
///
/// Note that the token kind 0 is reserved for a null token. This is typically
/// used to represent the end-of-input.
///
/// \todo Define == for tokens? This requires another virtual function on the
/// attribute type.
///
/// \todo Consider using type erasure or std::any to store attributes. That
/// would eliminate the need for a base class and let us store practically
/// anything as an attribute of the token.
struct token
{
  /// The base class of all token attributes. Certain kinds of tokens have
  /// attributes of different types. Note that there is a 1-to-1 correspondence
  /// between the token kind and the derived type of the attribute class.
  struct attr 
  {
    virtual ~attr() = default;
    virtual attr* clone() const = 0;
  };

  static constexpr int null = 0;
  
private:
  explicit token(int);
  token(int, attr*);

public:
  token();
  token(const token&);
  token(token&&);

  token& operator=(const token&);
  token& operator=(token&&);

  static token make(int);

  template<typename T, typename... Args>
  static token make(int, Args&&...);

  explicit operator bool() const;

  int get_kind() const;

  bool has_attribute() const;
  const attr& get_attribute() const;
  attr& get_attribute();
  
  int kind_;
  std::unique_ptr<attr> attr_;
};

/// Initialize a null token.
inline token::token() : token(null) { }

/// Initialize a non-attributed token of kind `k`.
inline token::token(int k) : token(k, nullptr) { }

/// Initialize an attributed token of kind `k`.
inline token::token(int k, attr* a) : kind_(k), attr_(a) { }

/// Initialize this token as a copy of `tok`.
inline 
token::token(const token& tok) 
  : kind_(tok.kind_), attr_(tok.attr_ ? tok.attr_->clone() : nullptr)
{ }

/// Initialize this token to assume the value of `tok`.
inline
token::token(token&& tok)
  : kind_(tok.kind_), attr_(std::move(tok.attr_))
{ }

/// Assign this token to be a copy of `tok`.
inline token& 
token::operator=(const token& tok)
{
  kind_ = tok.kind_;
  attr_.reset(tok.attr_->clone());
  return *this;
}

/// Assign this token to assume the value of `tok`.
inline token& 
token::operator=(token&& tok)
{
  kind_ = tok.kind_;
  attr_ = std::move(tok.attr_);
  return *this;
}

/// Converts to true when the token is not the end-of-file token.
inline token::operator bool() const { return kind_ != null; }

/// Returns the token's kind.
inline int token::get_kind() const { return kind_; }

/// Returns true if the token has an associated attribute.
inline bool token::has_attribute() const { return (bool)attr_; }

/// Returns the token's associated attribute.
inline const token::attr& token::get_attribute() const { return *attr_; }

/// Returns the token's associated attribute.
inline token::attr& token::get_attribute() { return *attr_; }

/// Returns a new token with no attribute.
inline token
token::make(int k)
{
  return token(k);
}

/// Returns a new token with an attribute of type T, initialized with the
/// given arguments.
template<typename T, typename... Args>
inline token
token::make(int k, Args&&... args)
{
  return token(k, new T(std::forward<Args>(args)...));
}


// -------------------------------------------------------------------------- //
// Token ttributes

/// A token attribute that holds particular value. This is a helper class for
/// other kinds of tokens.
template<typename T>
struct value_attr : token::attr
{
  value_attr(T);
  ~value_attr() = default;

  value_attr* clone() const override;

  T get_value() const;
  
  T value_;
};

template<typename T>
inline value_attr<T>::value_attr(T v) : value_(v) { }

template<typename T>
inline value_attr<T>* value_attr<T>::clone() const { return new value_attr<T>(*this); }

template<typename T>
inline T value_attr<T>::get_value() const { return value_; }


/// Represents the value of boolean literal.
using bool_attr = value_attr<bool>;


/// Represents the value of integer literals.
using int_attr = value_attr<int>;


/// Returns the boolean attribute for a token. Behavior is undefined if the
/// token does not have an attribute of that kind.
inline const bool_attr& 
get_bool_attribute(const token& tok)
{
  return static_cast<const bool_attr&>(tok.get_attribute());
}

/// Returns the boolean attribute for a token. Behavior is undefined if the
/// token does not have an attribute of that kind.
inline const int_attr& 
get_int_attribute(const token& tok)
{
  return static_cast<const int_attr&>(tok.get_attribute());
}

// -------------------------------------------------------------------------- //
// Token strings

/// A string of tokens.
///
/// \todo Actually use std::string?
using token_seq = std::vector<token>;

} // namespace beaker


#endif


// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_TYPE_HPP
#define BEAKER_BASE_TYPE_HPP

#include <beaker/base/lang.hpp>
#include <beaker/base/seq.hpp>
#include <beaker/util/cast.hpp>


namespace beaker {

// -------------------------------------------------------------------------- //
// Type base class

// Represents the set of types in a language. Specific kinds of types are
// defined as derived classes.
struct type 
{
  explicit type(int);
  virtual ~type() = default;

  int get_feature() const;
  int get_kind() const;

  int kind_;
};

// Construct a type with kind k.
inline type::type(int k) : kind_(k) { }

// Returns the language pack in which the type is defined.
inline int type::get_feature() const { return get_language(kind_); }

// Returns the kind of type.
inline int type::get_kind() const { return kind_; }


// -------------------------------------------------------------------------- //
// Type sequence

// A sequence of types.
using type_seq = seq<type>;


// -------------------------------------------------------------------------- //
// Base types

// A base type is a fundamental, non composite, type of a language.
template<int K>
struct base_type : type
{
  static constexpr int node_kind = K;
  
  base_type();
};

// Initialize the base type.
template<int K> 
inline base_type<K>::base_type() : type(K) { }

} // namespace


#endif

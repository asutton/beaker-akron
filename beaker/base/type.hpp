// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_TYPE_HPP
#define BEAKER_BASE_TYPE_HPP

#include <beaker/base/lang.hpp>
#include <beaker/base/seq.hpp>
#include <beaker/util/cast.hpp>


namespace beaker {

struct type;
struct object_type;
struct reference_type;
struct function_type;

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

  const object_type* as_object_type() const;
  object_type* as_object_type();

  const reference_type* as_reference_type() const;
  reference_type* as_reference_type();

  const function_type* as_function_type() const;
  function_type* as_function_type();

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
struct generic_base_type : type
{
  static constexpr int node_kind = K;
  
  generic_base_type();
};

// Initialize the base type.
template<int K> 
inline generic_base_type<K>::generic_base_type() : type(K) { }


// -------------------------------------------------------------------------- //
// Object types

/// An object type describes stored values.
///
/// \todo All object types have a size and alignment. Provide a way of 
/// accessing that information here.
struct object_type : type
{
  object_type(int);
};

inline object_type::object_type(int k) : type(k) { }


// A base type is a fundamental, non composite, type of a language.
template<int K>
struct generic_object_type : object_type
{
  static constexpr int node_kind = K;
  
  generic_object_type();
};

// Initialize the base type.
template<int K> 
inline generic_object_type<K>::generic_object_type() : object_type(K) { }


// -------------------------------------------------------------------------- //
// Reference types

/// A reference type describes references to objects. This class provides a
/// method of accessing the type of the underlying object.
struct reference_type : type
{
  reference_type(int);

  virtual const type& get_object_type() const = 0;
  virtual type& get_object_type() = 0;
};

inline reference_type::reference_type(int k) : type(k) { }


// A base type is a fundamental, non composite, type of a language.
template<int K>
struct generic_reference_type : reference_type
{
  static constexpr int node_kind = K;
  
  generic_reference_type();
};

// Initialize the base type.
template<int K> 
inline generic_reference_type<K>::generic_reference_type() : reference_type(K) { }


// -------------------------------------------------------------------------- //
// Mapping types

/// A mapping type describes a mapping from input types to output types.
///
/// \todo Are there any operations that we need to provide for mappings?
struct function_type : type
{
  function_type(int);
};

inline function_type::function_type(int k) : type(k) { }


/// A helper class for defining function types.
template<int K>
struct function_type_impl : function_type
{
  static constexpr int node_kind = K;
  
  function_type_impl();
};

template<int K> 
inline function_type_impl<K>::function_type_impl() : function_type(K) { }


// -------------------------------------------------------------------------- //
// Operations

/// Returns true if t is an object type.
inline bool 
is_object_type(const type& t)
{
  return dynamic_cast<const object_type*>(&t);
}

/// Returns true if t is a reference type.
inline bool 
is_reference_type(const type& t)
{
  return dynamic_cast<const reference_type*>(&t);
}

/// Returns true if t is a function type.
inline bool 
is_function_type(const type& t)
{
  return dynamic_cast<const function_type*>(&t);
}


/// Returns a type u that is guaranteed to be an object type for t. If t is a 
/// reference type, this returns the type of the referenced object. Otherwise, 
/// this returns t.
inline const type&
get_object_type(const type& t)
{
  if (const reference_type* ref = t.as_reference_type())
    return ref->get_object_type();
  return t;
}

/// Returns a type u that is guaranteed to be an object type for t. If t is a 
/// reference type, this returns the type of the referenced object. Otherwise, 
/// this returns t.
inline type&
get_object_type(type& t)
{
  if (reference_type* ref = t.as_reference_type())
    return ref->get_object_type();
  return t;
}

} // namespace


#endif

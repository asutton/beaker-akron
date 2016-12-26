// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_CORE_TYPE_HPP
#define BEAKER_CORE_TYPE_HPP

#include <beaker/base/type.hpp>


namespace beaker {
namespace core {

enum 
{
  first_type_kind = core_lang_block,
#define def_type(e) e ## _type_kind,
#include "type.def"
  last_type_kind
};


/// Represents the type `void`.
struct void_type : base_type<void_type_kind>
{
  using base_type<void_type_kind>::base_type;
};


/// A helper class for defining reference types.
template<int K>
struct basic_reference_type : type
{
  static constexpr int node_kind = K;

  basic_reference_type(type& t);

  const type& get_object_type() const;
  type& get_object_type();

  type* type_;
};

template<int K>
basic_reference_type<K>::basic_reference_type(type& t)
  : type(K), type_(&t)
{ }

/// Returns the type of the referenced object.
template<int K>
inline const type& basic_reference_type<K>::get_object_type() const { return *type_; }

/// Returns the type of the referenced object.
template<int K>
inline type& basic_reference_type<K>::get_object_type() { return *type_; }


/// Represents the type `ref t`. A reference refers to an object.
struct ref_type : basic_reference_type<ref_type_kind>
{
  using basic_reference_type<ref_type_kind>::basic_reference_type;
};


/// Represents function types `(t1, t2, ..., tn) -> t`. A function type 
/// describes entities that map inputs to outputs.
///
/// A variadic function type accepts a variadic argument list after its
/// last declared parameter.
struct fn_type : type
{
  static constexpr int node_kind = fn_type_kind;

  fn_type(const type_seq&, type&);
  fn_type(type_seq&&, type&);

  const type_seq& get_parameter_types() const;
  type_seq& get_parameter_types();

  const type& get_return_type() const;
  type& get_return_type();

  bool is_noexcept() const;
  bool is_variadic() const;

  type_seq parms_;
  type* ret_;
  bool noexcept_ : 1;
  bool variadic_ : 1;
};

// Initialize the function type with parameters p and return type t.
inline
fn_type::fn_type(const type_seq& p, type& t)
  : type(node_kind), parms_(p), ret_(&t), noexcept_(false), variadic_(false)
{ }

// Initialize the function type with parameters p and return type t.
inline
fn_type::fn_type(type_seq&& p, type& t)
  : type(node_kind), parms_(std::move(p)), ret_(&t), noexcept_(false), variadic_(false)
{ }

// Returns the sequence of parameter types.
inline const type_seq& fn_type::get_parameter_types() const { return parms_; }

// Returns the sequence of parameter types.
inline type_seq& fn_type::get_parameter_types() { return parms_; }

// Returns the return type.
inline const type& fn_type::get_return_type() const { return *ret_; }

// Returns the return type.
inline type& fn_type::get_return_type() { return *ret_; }

/// Returns true if the function is noexcept.
inline bool fn_type::is_noexcept() const { return noexcept_; }

/// Returns true if the function type is variadic.
inline bool fn_type::is_variadic() const { return variadic_; }


// -------------------------------------------------------------------------- //
// Operations

/// Returns true if `t` is void.
inline bool
is_void_type(const type& t)
{
  return t.get_kind() == void_type_kind;
}

/// Returns true if `t` is a function type.
inline bool
is_function_type(const type& t)
{
  return t.get_kind() == fn_type_kind;
}

/// Returns true if t is a reference type. The reference types are `ref t`, 
/// `in t`, and `out t`.
inline bool
is_reference_type(const type& t)
{
  return t.get_kind() == ref_type_kind;
}

/// Returns true if t is an object type. The object types are all types that 
/// are not `void`, reference types or function types.
inline bool 
is_object_type(const type& t)
{
  return !is_void_type(t) && !is_function_type(t) && !is_reference_type(t);
}

/// Returns a type u that is guaranteed to be an object type for t. If t is a 
/// reference type, this returns the type of the referenced object. Otherwise, 
/// this returns t.
inline const type&
get_object_type(const type& t)
{
  if (const ref_type* ref = as<ref_type>(&t))
    return ref->get_object_type();
  return t;
}

inline type&
get_object_type(type& t)
{
  if (ref_type* ref = as<ref_type>(&t))
    return ref->get_object_type();
  return t;
}

} // namespace core
} // namespace beaker


#endif

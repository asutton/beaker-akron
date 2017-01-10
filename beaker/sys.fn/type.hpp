// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_FN_TYPE_HPP
#define BEAKER_SYS_FN_TYPE_HPP

#include <beaker/base/type.hpp>


namespace beaker {
namespace sys_fn {

enum 
{
  first_type_kind = sys_fn_lang_block,
#define def_type(e) e ## _type_kind,
#include "type.def"
  last_type_kind
};


/// Represents function types `(t1, t2, ..., tn) -> t`. A function type 
/// describes entities that map inputs to outputs.
struct fn_type : function_type_impl<fn_type_kind>
{
  fn_type(const type_seq&, type&);
  fn_type(type_seq&&, type&);

  const type_seq& get_parameter_types() const;
  type_seq& get_parameter_types();

  const type& get_return_type() const;
  type& get_return_type();

  type_seq parms_;
  type* ret_;
};

// Initialize the function type with parameters p and return type t.
inline
fn_type::fn_type(const type_seq& p, type& t)
  : function_type_impl<node_kind>(), parms_(p), ret_(&t)
{ }

// Initialize the function type with parameters p and return type t.
inline
fn_type::fn_type(type_seq&& p, type& t)
  : function_type_impl<node_kind>(), parms_(std::move(p)), ret_(&t)
{ }

// Returns the sequence of parameter types.
inline const type_seq& fn_type::get_parameter_types() const { return parms_; }

// Returns the sequence of parameter types.
inline type_seq& fn_type::get_parameter_types() { return parms_; }

// Returns the return type.
inline const type& fn_type::get_return_type() const { return *ret_; }

// Returns the return type.
inline type& fn_type::get_return_type() { return *ret_; }


// -------------------------------------------------------------------------- //
// Operations

/// Returns the return type of t, where t is required to be a function type.
inline const type&
get_return_type(const type& t)
{
  assert(is_function_type(t));
  return cast<fn_type>(t).get_return_type();
}

inline type&
get_return_type(type& t)
{
  assert(is_function_type(t));
  return cast<fn_type>(t).get_return_type();
}

} // namespace sys_fn
} // namespace beaker


#endif

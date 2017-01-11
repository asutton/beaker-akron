// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_VAR_TYPE_HPP
#define BEAKER_SYS_VAR_TYPE_HPP

#include <beaker/base/type.hpp>


namespace beaker {
namespace sys_var {

enum 
{
  first_type_kind = sys_var_lang_block,
#define def_type(e) e ## _type_kind,
#include "type.def"
  last_type_kind
};


// -------------------------------------------------------------------------- //
// Types

/// Represents the type `ref t`.
///
/// The type `t` is required to be an object type. References to `void`, other
/// references, and functions are not allowed.
///
/// Note that "reference collapsing" is a feature of type aliasing and type 
/// substitution. Those rules may select how references compose in those
/// contexts. Those rules are not present in the core language (yet).
struct ref_type : reference_type_impl<ref_type_kind>
{
  ref_type(type& t);

  const type& get_object_type() const override;
  type& get_object_type() override;

  type* type_;
};

/// Construct the type `t&`.
inline 
ref_type::ref_type(type& t)
  : reference_type_impl<node_kind>(), type_(&t) 
{ }

/// Returns the type of the referenced object.
inline const type& ref_type::get_object_type() const { return *type_; }

/// Returns the type of the referenced object.
inline type& ref_type::get_object_type() { return *type_; }


} // namespace sys_var
} // namespace beaker


#endif

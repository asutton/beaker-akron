// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "decl.hpp"
#include "type.hpp"


namespace beaker {
namespace core {

/// Returns the type of the function.
const fn_type& fn_decl::get_type() const
{ 
  return cast<fn_type>(mapping_decl::get_type()); 
}

/// Returns the type of the function.
fn_type& fn_decl::get_type()
{ 
  return cast<fn_type>(mapping_decl::get_type()); 
}

/// Returns true if the function has variadic type.
bool fn_decl::is_variadic() const { return get_type().is_variadic(); }


/// Returns true if the parameter is stored in a variable.
bool parm_decl::is_var_parm() const { return is_object_type(get_type()); }

/// Returns true if the parameter is a mutable (read/write) parameter.
bool parm_decl::is_ref_parm() const { return is<ref_type>(get_type()); }

/// Returns true if the parameter is an input parameter.
bool parm_decl::is_in_parm() const { return is<in_type>(get_type()); }

/// Returns true if the parameter is an output parameter.
bool parm_decl::is_out_parm() const { return is<out_type>(get_type()); }

} // namespace core
} // namespace beaker


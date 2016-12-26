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


} // namespace core
} // namespace beaker


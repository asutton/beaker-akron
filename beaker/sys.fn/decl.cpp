// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "decl.hpp"
#include "type.hpp"

#include <beaker/sys.var/decl.hpp>


namespace beaker {
namespace sys_fn {

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


/// Returns true if `d` is a variable or parameter.
///
/// For most analyses, variables and function parameters are treated the
/// same, except that a return parameter can be void.
///
/// FIXME: Do we want a generic notion of variables like we do with type
/// categories? That would move these functions up to base.
bool 
is_variable(const decl& d)
{
  return is<sys_var::var_decl>(d) || is<parm_decl>(d);
}

/// Returns true if `d` is a function.
bool
is_function(const decl& d)
{
  return d.get_kind() == fn_decl_kind;
}

/// Returns true if `d` is a parameter.
bool 
is_parameter(const decl& d)
{
  return d.get_kind() == parm_decl_kind;
}

} // namespace sys_fn
} // namespace beaker


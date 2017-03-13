// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_FN_DECL_HPP
#define BEAKER_SYS_FN_DECL_HPP

#include <beaker/base/decl.hpp>


namespace beaker {
namespace sys_fn {

struct fn_type;

enum 
{
  first_decl_kind = sys_fn_lang_block,
#define def_decl(NS, D) D ## _decl_kind,
#include "decl.def"
  last_decl_kind
};


// -------------------------------------------------------------------------- //
// Declarations

/// Represents the declaration of a name as a function.
///
/// Function parameters are limited to variables are references.
///
/// A variadic function accepts a variadic argument list after its last 
/// declared parameter.
struct fn_decl : mapping_decl_impl<fn_decl_kind>
{
  using mapping_decl_impl<fn_decl_kind>::mapping_decl_impl;

  fn_type const& get_type() const;
  fn_type& get_type();
};


/// Represents the declaration of (formal) function parameter. Parameters, in
/// the definition of a function are essentially local variables (see below).
///
/// \todo Support default arguments.
struct parm_decl : value_decl_impl<parm_decl_kind>
{
  parm_decl(uid, name&, type&);
};

/// Initialize the parameter. Note that the declaration context of a parameter 
/// is null until the parameter is added to a function.
inline
parm_decl::parm_decl(uid id, name& n, type& t)
  : value_decl_impl<parm_decl_kind>(id, dc(), automatic_storage, n, t)
{ }


/// Represents a local variable declaration. Local variables have no linkage 
/// and automatic storage.
struct var_decl : value_decl_impl<var_decl_kind>
{
  var_decl(uid, dc, name& n, type& t, expr& e);
};

/// Initialize the variable with with the given name, type, and initializer.
/// Note that local variables are always initialized, although this may be by
/// trivial initialization.
inline
var_decl::var_decl(uid id, dc cxt, name& n, type& t, expr& e)
  : value_decl_impl<var_decl_kind>(id, cxt, no_link, automatic_storage, n, t, e)
{ }


// -------------------------------------------------------------------------- //
// Operations


/// Returns true if `d` is a function declaration.
inline bool
is_function(const decl& d)
{
  return d.get_kind() == fn_decl_kind;
}

/// Returns true if `d` is a parameter declaration.
inline bool 
is_parameter(const decl& d)
{
  return d.get_kind() == parm_decl_kind;
}

/// Returns true if `d` is a variable or parameter. 
///
/// \todo Do we want a generic notion of variables like we do with type
/// categories (object, reference, etc.)? That would move these functions up 
/// to the base declaration module?
inline bool 
is_variable(const decl& d)
{
  return is<var_decl>(d) || is<parm_decl>(d);
}

} // namespace sys_fn
} // namespace beaker


#endif

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
#define def_decl(e) e ## _decl_kind,
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

/// Initialize the parameter. Note that the declaration context is null
/// until the parameter is added to a function.
inline
parm_decl::parm_decl(uid id, name& n, type& t)
  : value_decl_impl<parm_decl_kind>(id, dc(), automatic_storage, n, t)
{ }


/// Represents a local variable declaration. Local variables have no linkage 
/// and automatic storage.
struct var_decl : value_decl_impl<var_decl_kind>
{
  using value_decl_impl<var_decl_kind>::value_decl_impl;
};


// -------------------------------------------------------------------------- //
// Operations

bool is_function(const decl&);
bool is_variable(const decl&);
bool is_parameter(const decl&);

} // namespace sys_fn
} // namespace beaker


#endif

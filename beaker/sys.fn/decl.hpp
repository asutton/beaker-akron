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


/// Represents the declaration of (formal) function parameter.
///
/// A parameter represent the way in which objects and references are passed
/// to a function. In general, parameters are like variables in the sense that
/// they are names that refer to local storage. However, that storage may be
/// allocated by the calling function or the caller, depending on the type of
/// the parameter.
struct parm_decl : value_decl_impl<parm_decl_kind>
{
  using value_decl_impl<parm_decl_kind>::value_decl_impl;
};


// -------------------------------------------------------------------------- //
// Operations

bool is_variable(const decl&);
bool is_function(const decl&);
bool is_parameter(const decl&);

} // namespace sys_fn
} // namespace beaker


#endif

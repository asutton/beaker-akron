// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_CORE_DECL_HPP
#define BEAKER_CORE_DECL_HPP

#include <beaker/base/decl.hpp>

namespace beaker {
namespace core {

struct fn_type;
struct parm_type;

enum 
{
  first_decl_kind = core_lang_block,
#define def_decl(e) e ## _decl_kind,
#include "decl.def"
  last_decl_kind
};


// Represents the declaration of name as a stored object.
//
// The storage of a variable may be static, thread, or automatic. This
// class only represents the declaration of the object, it does not
// maintain its storage.
struct var_decl : generic_value_decl<var_decl_kind>
{
  using generic_value_decl<var_decl_kind>::generic_value_decl;
};


/// Represents the declaration of a name as a function.
///
/// Function parameters are limited to variables are references.
///
/// A variadic function accepts a variadic argument list after its last 
/// declared parameter.
struct fn_decl : generic_mapping_decl<fn_decl_kind>
{
  using generic_mapping_decl<fn_decl_kind>::generic_mapping_decl;

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
struct parm_decl : generic_value_decl<parm_decl_kind>
{
  using generic_value_decl<parm_decl_kind>::generic_value_decl;
};

// -------------------------------------------------------------------------- //
// Operations

/// Returns true if `d` is a variable or parameter.
///
/// For most analyses, variables and function parameters are treated the
/// same, except that a return parameter can be void.
inline bool
is_variable(const decl& d)
{
  return d.get_kind() == var_decl_kind || d.get_kind() == parm_decl_kind;
}

/// Returns true if `d` is a function.
inline bool
is_function(const decl& d)
{
  return d.get_kind() == fn_decl_kind;
}

/// Returns true if `d` is a parameter.
inline bool 
is_parameter(const decl& d)
{
  return d.get_kind() == parm_decl_kind;
}

} // namespace core
} // namespace beaker


#endif

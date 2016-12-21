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

  bool is_variadic() const;
};


/// Represents the declaration of function parameter.
///
/// A parameter represent the way in which objects and references are passed
/// to a function. Parameters are similar to variables except that they may
/// not be directly associated with storage.
struct parm_decl : generic_value_decl<parm_decl_kind>
{
  using generic_value_decl<parm_decl_kind>::generic_value_decl;

  bool is_var_parm() const;
  bool is_ref_parm() const;
  bool is_in_parm() const;
  bool is_out_parm() const;
};

} // namespace core
} // namespace beaker


#endif

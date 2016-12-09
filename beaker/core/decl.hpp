// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_CORE_DECL_HPP
#define BEAKER_CORE_DECL_HPP

#include <beaker/base/decl.hpp>

namespace beaker {
namespace core {

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


// Represents the declaration of a name as a reference to a stored entity.
//
// References can refer to objects or functions with any storage class.
struct ref_decl : generic_value_decl<ref_decl_kind>
{
  using generic_value_decl<ref_decl_kind>::generic_value_decl;
};


// Represents the declaration of a name as a registered object.
//
// A registered object can be initialized with a value, but never modified.
// In general, these should not be used because they are highly irregular,
// but we support them as a low-level feature (e.g., parameter passing).
struct reg_decl : generic_value_decl<reg_decl_kind>
{
  using generic_value_decl<reg_decl_kind>::generic_value_decl;
};


// Represents the declaration of a name as a constant.
//
// Note that constants never have storage.
struct const_decl : generic_value_decl<const_decl_kind>
{
  using generic_value_decl<const_decl_kind>::generic_value_decl;
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

  bool is_variadic() const;
};

} // namespace core
} // namespace beaker


#endif

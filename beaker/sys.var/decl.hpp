// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_VAR_DECL_HPP
#define BEAKER_SYS_VAR_DECL_HPP

#include <beaker/base/decl.hpp>

namespace beaker {
namespace sys_var {

enum 
{
  first_decl_kind = sys_var_lang_block,
#define def_decl(e) e ## _decl_kind,
#include "decl.def"
  last_decl_kind
};


// Represents the declaration of name as a stored object.
//
// The storage of a variable may be static, thread, or automatic. This
// class only represents the declaration of the object, it does not
// maintain its storage.
struct var_decl : value_decl_impl<var_decl_kind>
{
  using value_decl_impl<var_decl_kind>::value_decl_impl;
};


} // namespace sys_var
} // namespace beaker


#endif

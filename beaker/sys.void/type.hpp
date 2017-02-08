// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_VOID_TYPE_HPP
#define BEAKER_SYS_VOID_TYPE_HPP

#include <beaker/base/type.hpp>


namespace beaker {
namespace sys_void {

enum 
{
  first_type_kind = sys_void_lang_block,
#define def_type(E, B) E ## _type_kind,
#include "type.def"
  last_type_kind
};

// -------------------------------------------------------------------------- //
// Types

/// Represents the type `void`. The void type describes expressions and
/// functions that do not have values.
struct void_type : base_type_impl<void_type_kind> 
{
  using base_type_impl<void_type_kind>::base_type_impl;
};


// -------------------------------------------------------------------------- //
// Operations

/// Returns true if `t` is the `void` type.
inline bool
is_void_type(const type& t)
{
  return t.get_kind() == void_type_kind;
}

} // namespace sys_void
} // namespace beaker


#endif

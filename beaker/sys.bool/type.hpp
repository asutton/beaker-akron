// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_BOOL_TYPE_HPP
#define BEAKER_SYS_BOOL_TYPE_HPP

#include <beaker/base/type.hpp>


namespace beaker {
namespace sys_bool {

enum 
{
  first_type_kind = sys_bool_lang_block,
#define def_type(NS, T, B) T ## _type_kind,
#include "type.def"
  last_type_kind
};

// -------------------------------------------------------------------------- //
// Types

/// Represents the type `bool`. There are two boolean values, true and false.
struct bool_type : object_type_impl<bool_type_kind> 
{
  using object_type_impl<bool_type_kind>::object_type_impl;
};


// -------------------------------------------------------------------------- //
// Operations

/// Returns true if t is the boolean type.
inline bool
is_boolean_type(const type& t)
{
  return t.get_kind() == bool_type_kind;
}

} // namespace sys_bool
} // namespace beaker


#endif

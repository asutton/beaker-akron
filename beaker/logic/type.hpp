// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_LOGIC_TYPE_HPP
#define BEAKER_LOGIC_TYPE_HPP

#include <beaker/base/type.hpp>


namespace beaker {
namespace logic {

enum 
{
  first_type_kind = logic_lang_block,
#define def_type(e) e ## _type_kind,
#include "type.def"
  last_type_kind
};


/// Represents the type `bool`. There are two boolean values, true and false.
struct bool_type : base_type<bool_type_kind> 
{
  using base_type<bool_type_kind>::base_type;
};


// Operations

/// Returns true if t is the boolean type.
inline bool
is_boolean_type(const type& t)
{
  return t.get_kind() == bool_type_kind;
}

} // namespace logic
} // namespace beaker


#endif

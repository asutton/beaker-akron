// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_VARIADIC_TYPE_HPP
#define BEAKER_VARIADIC_TYPE_HPP

#include <beaker/base/type.hpp>


namespace beaker {
namespace variadic {

enum 
{
  first_type_kind = variadic_lang_block,
#define def_type(e) e ## _type_kind,
#include "type.def"
  last_type_kind
};


/// Represents the type of a variadic argument list.
///
/// In C, C++, and LLVM, this type is traditionally spelled va_list. Note
/// that the actual structure of this type depends on the target operating
/// system.
struct list_type : base_type<list_type_kind> 
{
  using base_type<list_type_kind>::base_type;
};

} // namespace variadic
} // namespace beaker


#endif

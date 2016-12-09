// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "decl.hpp"
#include "type.hpp"


namespace beaker {
namespace core {

/// Returns true if the function is variadic.
///
/// This is the case when the function's type is variadic.
bool fn_decl::is_variadic() const 
{ 
  return cast<fn_type>(get_type()).is_variadic();
}

} // namespace core
} // namespace beaker


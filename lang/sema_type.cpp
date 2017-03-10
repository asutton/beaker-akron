// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

namespace bpl {

/// Returns the void type.
type&
semantics::on_void_type(location loc)
{
  return build.get_void_type();
}

/// Returns the bool type.
type&
semantics::on_bool_type(location loc)
{
  return build.get_bool_type();
}

/// Returns the default integer type.
type&
semantics::on_int_type(location loc)
{
  return build.get_int32_type();
}

} // namespace bpl

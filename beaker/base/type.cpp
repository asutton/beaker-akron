// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "type.hpp"

namespace beaker {

/// Returns this type as an object type if it has that dynamic type or 
/// null otherwise.
const object_type* 
type::as_object_type() const
{
  return dynamic_cast<const object_type*>(this);
}

/// Returns this type as an object type if it has that dynamic type or 
/// null otherwise.
object_type* 
type::as_object_type()
{
  return dynamic_cast<object_type*>(this);
}

/// Returns this type as a reference type if it has that dynamic type or 
/// null otherwise.
const reference_type* 
type::as_reference_type() const
{
  return dynamic_cast<const reference_type*>(this);
}

/// Returns this type as a reference type if it has that dynamic type or 
/// null otherwise.
reference_type* 
type::as_reference_type()
{
  return dynamic_cast<reference_type*>(this);
}

/// Returns this type as a function type if it has that dynamic type or 
/// null otherwise.
const function_type* 
type::as_function_type() const
{
  return dynamic_cast<const function_type*>(this);
}

/// Returns this type as a function type if it has that dynamic type or 
/// null otherwise.
function_type* 
type::as_function_type()
{
  return dynamic_cast<function_type*>(this);
}


} // namespace beaker
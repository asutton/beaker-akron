// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "expr.hpp"
#include "type.hpp"

#include "comparison/equal.hpp"


namespace beaker {

// Returns the kind of the expression's type.
int expr::get_type_kind() const { return type_->get_kind(); }


/// Returns true if `e` has the given type.
bool
has_type(const expr& e, const type& t)
{
  return equal(e.get_type(), t);
}


/// Returns true if `e` has object type.
bool
is_object_expression(const expr& e)
{
  return is_object_type(e.get_type());
}

/// Returns true if `e` has reference type.
bool
is_reference_expression(const expr& e)
{
  return is_reference_type(e.get_type());
}

/// Returns true if `e` has function type.
bool
is_function_expression(const expr& e)
{
  return is_function_type(e.get_type());
}



} // namespace beaker

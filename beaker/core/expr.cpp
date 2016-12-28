// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "expr.hpp"
#include "type.hpp"
#include <beaker/base/decl.hpp>

namespace beaker {
namespace core {

// Returns the referenced declaration.
const typed_decl& ref_expr::get_declaration() const { return *decl_->as_typed(); }

// Returns the referenced declaration.
typed_decl& ref_expr::get_declaration() { return *decl_->as_typed(); }

// Returns the name used by the reference.
const name& ref_expr::get_name() const { return get_declaration().get_name(); }

// Returns the name used by the reference.
name& ref_expr::get_name() { return get_declaration().get_name(); }


/// Returns true if the expression has type `void`.
bool
is_void_expression(const expr& e)
{
  return is_void_type(e.get_type());
}

/// Returns true if the type of the expression is an object type
bool
is_object_expression(const expr& e)
{
  return is_object_type(e.get_type());
}

/// Returns true if the type of the expression is `t&`.
bool
is_reference_expression(const expr& e)
{
  return is_reference_type(e.get_type());
}

} // namespace core
} // namespace beaker

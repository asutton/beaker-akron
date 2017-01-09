// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "expr.hpp"
#include "type.hpp"
#include <beaker/base/decl.hpp>

namespace beaker {
namespace sys_var {

// Returns the referenced declaration.
const typed_decl& ref_expr::get_declaration() const { return *decl_->as_typed(); }

// Returns the referenced declaration.
typed_decl& ref_expr::get_declaration() { return *decl_->as_typed(); }

// Returns the name used by the reference.
const name& ref_expr::get_name() const { return get_declaration().get_name(); }

// Returns the name used by the reference.
name& ref_expr::get_name() { return get_declaration().get_name(); }


} // namespace sys_var
} // namespace beaker

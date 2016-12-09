// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "expr.hpp"
#include "type.hpp"

namespace beaker {

// Returns the kind of the expression's type.
int expr::get_type_kind() const { return type_->get_kind(); }

} // namespace beaker

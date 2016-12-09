// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "builder.hpp"

#include <beaker/base/module.hpp>


namespace beaker {

// Returns the allocator for the object.
allocator& builder_base::get_allocator() { return mod_->get_allocator(); }


} // namespace beaker

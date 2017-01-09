// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "builder.hpp"

#include <beaker/base/lang.hpp>
#include <beaker/base/module.hpp>


namespace beaker {

/// Returns the language in which the module is written.
language&
builder_base::get_language()
{
  return mod_->get_language();
}

// Returns the allocator for the language.
allocator& 
builder_base::get_language_allocator() 
{ 
  return get_language().get_allocator(); 
}

// Returns the allocator for the module.
allocator& 
builder_base::get_module_allocator() 
{ 
  return mod_->get_allocator(); 
}


} // namespace beaker

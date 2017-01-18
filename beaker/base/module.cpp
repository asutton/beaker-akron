// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "module.hpp"
#include <beaker/util/memory.hpp>
#include <beaker/util/symbol_table.hpp>


namespace beaker {

/// Initialize the module. 
module::module(language& lang)
  : decl(-1),
    lang_(&lang),
    build_(), 
    name_(nullptr), 
    decls_()
{
  for (const feature* f : language::get_instance().get_features()) {
    feature::build_fn make = f->get_builder_factory();
    build_.put(f->get_id(), make(*this));
  }
}

module::~module()
{
}

} // namespace beaker


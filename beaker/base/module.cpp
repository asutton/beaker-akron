// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "module.hpp"
#include <beaker/util/memory.hpp>
#include <beaker/util/symbol_table.hpp>


namespace beaker {

/// Initialize the module. 
module::module(language& lang)
  : decl(node_kind),
    lang_(&lang),
    name_(nullptr), 
    decls_()
{
  for (const feature* f : lang_->get_features())
    add_builder(typeid(*f), f->make_builder(*this));
}

} // namespace beaker


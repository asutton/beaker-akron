// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "builder.hpp"
#include "../name.hpp"
#include <beaker/base/module.hpp>
#include <beaker/util/symbol_table.hpp>


namespace beaker {
namespace sys_name {

builder::builder(module& m)
  : basic_builder<sys_name_lang>(m), 
    name_(&get_language().make_canonical_set<basic_name>()),
    current_id_()
{ }

/// Returns the name corresponding to str.
basic_name& 
builder::get_name(const char* str) 
{
  symbol_table& syms = mod_->get_symbol_table();
  const symbol& sym = syms.insert(str, 0);
  return name_->get(sym);
}

/// Returns a unique internal name.
internal_name& 
builder::get_name() 
{
  return make<internal_name>(current_id_++);
}

} // namespace sys_name
} // namespace beaker


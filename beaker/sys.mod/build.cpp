// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "build.hpp"
#include "decl.hpp"

#include <beaker/sys.name/ast.hpp>


namespace beaker {
namespace sys_mod {

static inline sys_name::builder& 
get_name_builder(builder& b)
{
  return b.get_module().get_builder<sys_name::builder>();
}

builder::builder(module& m)
  : basic_builder<sys_mod_lang>(m)
{ }

/// Returns a new name.
sys_name::basic_name&
builder::get_name(const char* str)
{
  return get_name_builder(*this).get_name(str);
}

/// Returns a new module declaration. This also updates the module with
/// the declared value.
module_decl&
builder::make_module_decl(name& n)
{
  module_decl& ret = make<module_decl>(generate_id(), *mod_, n);
  get_module().set_module_name(n);
  return ret;
}

/// Returns a new module declaration.
module_decl&
builder::make_module_decl(const char* n)
{
  return make_module_decl(get_name(n));
}

/// Returns a new import declaration.
import_decl&
builder::make_import_decl(module& m)
{
  return make<import_decl>(generate_id(), *mod_, m);
}


} // namespace sys_mod
} // namespace beaker

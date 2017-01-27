// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_MOD_BUILD_HPP
#define BEAKER_SYS_MOD_BUILD_HPP

#include <beaker/sys.mod/fwd.hpp>
#include <beaker/sys.name/fwd.hpp>

#include <beaker/base/build.hpp>


namespace beaker {
namespace sys_mod {

/// Provides access to resources needed to construct, validate, and
/// evaluate logical terms.
struct builder : basic_builder<sys_mod_lang>
{
  builder(module&);

  // Names
  sys_name::basic_name& get_name(const char*);

  // Declarations
  module_decl& make_module_decl(name&);
  module_decl& make_module_decl(const char*);

  import_decl& make_import_decl(name&);
  import_decl& make_import_decl(const char*);

  module* mod_;
};

} // namespace sys_mod
} // namespace beaker


#endif

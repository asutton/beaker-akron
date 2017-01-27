// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_MOD_DECL_HPP
#define BEAKER_SYS_MOD_DECL_HPP

#include <beaker/base/decl.hpp>

namespace beaker {
namespace sys_mod {

enum 
{
  first_decl_kind = sys_mod_lang_block,
#define def_decl(e) e ## _decl_kind,
#include "decl.def"
  last_decl_kind
};


/// Declares the current translation unit to be a module with the given name.
/// At most, one such declaration exists in a translation unit.
///
/// Note that this is not a named_decl since it does not denote a conventional
/// program symbol.
///
/// \todo Define a distinct module_name? Currently, this is just expected
/// to be a simple identifier, although it may have non-identifier characters
/// in it (e.g., '.').
struct module_decl : decl
{
  static constexpr int node_kind = module_decl_kind;

  module_decl(uid, dc, name&);

  const name& get_name() const;
  name& get_name();

  name* name_;
};

inline
module_decl::module_decl(uid id, dc cxt, name& n)
  : decl(node_kind, id, cxt), name_(&n)
{ }

/// Returns the name of the declared module.
inline const name& module_decl::get_name() const { return *name_; }

/// Returns the name of the declared module.
inline name& module_decl::get_name() { return *name_; }


/// Represents the importation of exported names into the current module.
struct import_decl : decl
{
  static constexpr int node_kind = import_decl_kind;

  import_decl(uid, dc, module&);

  const module& get_import() const;
  module& get_import();

  module* imp_;
};

inline
import_decl::import_decl(uid id, dc cxt, module& m)
  : decl(node_kind, id, cxt), imp_(&m)
{ }

/// Returns the imported module.
inline const module& import_decl::get_import() const { return *imp_; }

/// Returns the imported module.
inline module& import_decl::get_import() { return *imp_; }


} // namespace sys_mod
} // namespace beaker


#endif

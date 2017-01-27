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


/// A helper class for defining module-related declarations.
template<int K>
struct module_decl_impl : decl
{
  static constexpr int node_kind = K;

  module_decl_impl(uid, dc, name&);

  const name& get_name() const;
  name& get_name();

  name* name_;
};

template<int K>
inline
module_decl_impl<K>::module_decl_impl(uid id, dc cxt, name& n)
  : decl(node_kind, id, cxt), name_(&n)
{ }

/// Returns the name of the declared module.
template<int K>
inline const name& module_decl_impl<K>::get_name() const { return *name_; }

/// Returns the name of the declared module.
template<int K>
inline name& module_decl_impl<K>::get_name() { return *name_; }


/// Declares the current translation unit to be a module with the given name.
/// At most, one such declaration exists in a translation unit.
///
/// Note that this is not a named_decl since it does not denote a conventional
/// program symbol.
///
/// \todo Define a distinct module_name? Currently, this is just expected
/// to be a simple identifier, although it may have non-identifier characters
/// in it (e.g., '.').
struct module_decl : module_decl_impl<module_decl_kind>
{
  using module_decl_impl<module_decl_kind>::module_decl_impl;
};


/// Represents the importation of exported names into the current module.
struct import_decl : module_decl_impl<import_decl_kind>
{
  using module_decl_impl<import_decl_kind>::module_decl_impl;
};


} // namespace sys_mod
} // namespace beaker


#endif

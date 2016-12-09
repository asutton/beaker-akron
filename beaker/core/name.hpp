// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_CORE_NAME_HPP
#define BEAKER_CORE_NAME_HPP

#include <beaker/base/name.hpp>


namespace beaker {

struct symbol;

namespace core {

enum 
{
  first_name_kind = core_lang_block,
#define def_name(e) e ## _name_kind,
#include "name.def"
  last_name_kind
};


/// A basic name is represented by a single symbol.
struct basic_name : name
{
  static constexpr int node_kind = basic_name_kind;

  basic_name(const symbol&);

  const symbol& get_symbol() const;

  const symbol* sym_;
};

/// Initialize the basic name.
inline basic_name::basic_name(const symbol& s) : name(node_kind), sym_(&s) { }

// Returns the symbol associated with the name.
inline const symbol& 
basic_name::get_symbol() const { return *sym_; }


/// Represents a name used internally by the language.
///
/// Internal names are often provided for declarations when a name is not
/// explicitly given. These are represented by a integer identifier.
struct internal_name : name
{
  static constexpr int node_kind = internal_name_kind;

  explicit internal_name(int);

  int get_id() const;
  
  int id_;
};

/// Initialize the internal name.
inline internal_name::internal_name(int n) : name(node_kind), id_(n) { }

/// Returns the unique id of the internal name.
inline int internal_name::get_id() const { return id_; }


} // namespace core
} // namespace beaker


#endif

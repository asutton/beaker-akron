// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_COMMON_MODULE_HPP
#define BEAKER_COMMON_MODULE_HPP

#include <beaker/base/node.hpp>
#include <beaker/base/type.hpp>
#include <beaker/base/decl.hpp>

#include <unordered_map>


namespace beaker {

struct allocator;
struct symbol_table;
struct language;
struct name;
struct builder;


// -------------------------------------------------------------------------- //
// Term builders

/// A builder set is a collection of functions that allocate builder objects, 
/// as needed.
///
/// \todo: Can we unify this with the feature set in some way?
struct builder_set
{
  using map_type = std::unordered_map<std::type_index, factory*>;

  void add_builder(std::type_index, factory&);

  template<typename T>
  void add_builder(factory&);

  factory& get_builder(std::type_index) const;

  template<typename T>
  typename T::builder_type& get_builder() const;

  map_type map_;
};

/// Install a builder factory for the given type id. This function has no effect
/// if a factory is already installed.
inline void
builder_set::add_builder(std::type_index ti, factory& b)
{
  map_.emplace(ti, &b);
}

/// Install a builder factory for the feature T. This function has no effect
/// if a factory is already installed.
template<typename T>
inline void
builder_set::add_builder(factory& b)
{
  add_builder(typeid(T), b);
}

inline factory&
builder_set::get_builder(std::type_index ti) const
{
  return *map_.find(ti)->second;
}

/// Returns a builder object for the feature T.
template<typename T>
inline typename T::builder_type&
builder_set::get_builder() const
{
  return static_cast<typename T::builder_type&>(get_builder(typeid(T)));
}


// -------------------------------------------------------------------------- //
// Id generators

/// A helper class used to generate unique ids for declarations.
///
/// \todo Consider using random numbers so that ids are independent of
/// declaration order.
struct id_generator
{
  int operator()() { return ++next; }

  // Stores the next id to generate. Note that 0 is reserved for the
  // module itself. This matters for the purpose of serialization.
  int next = 1;
};


/// Represents a named collection of types, values, and functions. A module
/// is the root container of declarations for a translation; it is equivalent
/// to a translation unit in C/C++.
struct module : decl, builder_set, node_store
{
  static constexpr int node_kind = -1;

  module(language&);

  const language& get_language() const;
  language& get_language();
  
  const symbol_table& get_symbol_table() const;
  symbol_table& get_symbol_table();
  
  int generate_id();

  const name& get_module_name() const;
  name& get_module_name();
  void set_module_name(name&);

  const decl_seq& get_declarations() const;
  decl_seq& get_declarations();

  void add_declaration(decl&);

  language* lang_;
  id_generator gen_;
  name* name_;
  decl_seq decls_;
};

/// Returns the language of the module.
inline const language& module::get_language() const { return *lang_; }

/// Returns the language of the module.
inline language& module::get_language() { return *lang_; }

/// Returns the symbol table used by the module.
inline const symbol_table& module::get_symbol_table() const { return lang_->get_symbol_table(); }

/// Returns the symbol table used by the module.
inline symbol_table& module::get_symbol_table() { return lang_->get_symbol_table(); }

/// Generate a unique id.
inline int module::generate_id() { return gen_(); }

/// Returns the name of the module.
inline const name& module::get_module_name() const { return *name_; }

/// Returns the name of the module.
inline name& module::get_module_name() { return *name_; }

/// Sets the module name.
inline void module::set_module_name(name& n) { name_ = &n; }

/// Returns the declarations in the module.
inline const decl_seq& module::get_declarations() const { return decls_; }

/// Returns the declarations in the module.
inline decl_seq& module::get_declarations() { return decls_; }

/// Adds a declaration to the module.
inline void module::add_declaration(decl& d) { decls_.push_back(d); }

} // namespace beaker


#endif

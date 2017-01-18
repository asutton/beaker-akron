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


/// A builder set is a collection of builder objects. 
///
/// The concrete types of these objects are only known by the language pack 
/// that defines them.
struct builder_set
{
  void put(int, void*);
  void put(std::initializer_list<std::pair<int, void*>>);

  template<typename T>
  T& get();

  std::unordered_map<int, void*> set_;
};

/// Install a builder b for the language pack identified by l.
///
/// The builder for l shall have previously been nullptr.
inline void
builder_set::put(int f, void* b)
{
  assert(set_.count(f) == 0);
  set_.emplace(f, b);
}

/// Install a list of language/builder pairs.
inline void
builder_set::put(std::initializer_list<std::pair<int, void*>> list)
{
  for (auto i = list.begin(); i != list.end(); ++i)
    put(i->first, i->second);
}

/// Returns the builder of type T.
template<typename T>
inline T&
builder_set::get()
{
  assert(set_.count(T::feature) != 0);
  void* p = set_.find(T::feature)->second;
  return *reinterpret_cast<T*>(p);
}


/// Represents a named collection of types, values, and functions.
struct module : decl, node_store
{
  module(language&);
  ~module();

  language& get_language();
  symbol_table& get_symbol_table();
  
  builder_set& get_builders();
  template<typename T> T& get_builder();

  const name& get_module_name() const;
  name& get_module_name();

  const decl_seq& get_declarations() const;
  decl_seq& get_declarations();

  void add_declaration(decl&);

  language* lang_;
  builder_set build_;
  name* name_;
  decl_seq decls_;
};

/// Returns the language of the module.
inline language& module::get_language() { return *lang_; }

/// Returns the symbol table used by the module.
inline symbol_table& module::get_symbol_table() { return lang_->get_symbol_table(); }

/// Returns the set of term builders for the module.
inline builder_set& module::get_builders() { return build_; }

/// Returns the builder of the given type.
template<typename T>
inline T& module::get_builder() { return build_.template get<T>(); }

/// Returns the name of the module.
inline const name& module::get_module_name() const { return *name_; }

/// Returns the name of the module.
inline name& module::get_module_name() { return *name_; }

/// Returns the declarations in the module.
inline const decl_seq& module::get_declarations() const { return decls_; }

/// Returns the declarations in the module.
inline decl_seq& module::get_declarations() { return decls_; }

/// Adds a declaration to the module.
inline void module::add_declaration(decl& d) { decls_.push_back(d); }

} // namespace


#endif

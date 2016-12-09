// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_COMMON_MODULE_HPP
#define BEAKER_COMMON_MODULE_HPP

#include <beaker/base/type.hpp>
#include <beaker/base/decl.hpp>

#include <array>


namespace beaker {

struct allocator;
struct symbol_table;
struct name;


/// A builder set is a collection of builder objects. 
///
/// The concrete types of these objects are only known by the language pack 
/// that defines them.
struct builder_set
{
  builder_set();

  void put(int, void*);
  void put(std::initializer_list<std::pair<int, void*>>);

  template<typename T>
  T& get(int);

  template<typename T>
  T& get();

  std::array<void*, last_lang> arr_;
};

/// Initialize all builders to null.
inline builder_set::builder_set() { arr_.fill(nullptr); }

/// Install a builder b for the language pack identified by l.
///
/// The builder for l shall have previously been nullptr.
inline void
builder_set::put(int l, void* b)
{
  assert(arr_[l] == nullptr);
  arr_[l] = b;
}

/// Install a list of language/builder pairs.
inline void
builder_set::put(std::initializer_list<std::pair<int, void*>> list)
{
  for (auto i = list.begin(); i != list.end(); ++i)
    put(i->first, i->second);
}

/// Returns the builder, cast as T, for the language l.
///
/// Behavior is undefined if there is no builder for l.
///
/// \deprecated Use the other version of get builder.
template<typename T>
inline T&
builder_set::get(int l)
{
  assert(arr_[l] != nullptr);
  return *reinterpret_cast<T*>(arr_[l]);
}

/// Returns the builder of type T.
template<typename T>
inline T&
builder_set::get()
{
  assert(arr_[T::lang] != nullptr);
  return *reinterpret_cast<T*>(arr_[T::lang]);
}


/// Represents a named collection of types, values, and functions.
///
/// A module is the owner of all terms constructed in a particular translation
/// or transformation.
struct module
{
  module(allocator&, symbol_table&);

  allocator& get_allocator();
  symbol_table& get_symbol_table();
  
  builder_set& get_builders();
  template<typename T> T& get_builder(int);
  template<typename T> T& get_builder();

  const name& get_module_name() const;
  name& get_module_name();

  const decl_seq& get_declarations() const;
  decl_seq& get_declarations();

  void add_declaration(decl&);

  allocator* alloc_;
  symbol_table* syms_;
  builder_set build_;
  name* name_;
  decl_seq decls_;
};

/// Returns the allocator for the module.
inline allocator& module::get_allocator() { return *alloc_; }

/// Returns the symbol table used by the module.
inline symbol_table& module::get_symbol_table() { return *syms_; }

/// Returns the set of term builders for the module.
inline builder_set& module::get_builders() { return build_; }

/// Returns the builder for the language l.
///
/// \deprecated. Use the other version of get builder.
template<typename T>
inline T& module::get_builder(int l) { return build_.template get<T>(l); }

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

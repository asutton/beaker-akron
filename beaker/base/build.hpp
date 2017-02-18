// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_BUILD_HPP
#define BEAKER_BASE_BUILD_HPP

#include <beaker/base/lang.hpp>
#include <beaker/base/module.hpp>
#include <beaker/base/name.hpp>
#include <beaker/base/type.hpp>
#include <beaker/base/expr.hpp>
#include <beaker/base/decl.hpp>
#include <beaker/base/stmt.hpp>
#include <beaker/base/comparison/equal.hpp>
#include <beaker/base/comparison/hash.hpp>
#include <beaker/util/singleton_set.hpp>
#include <beaker/util/canonical_set.hpp>


namespace beaker {

struct module;

template<typename T>
using singleton_term_set = singleton_set<T>;

template<typename T>
using canonical_term_set = canonical_set<T, term_hash, term_equal>;


/// The base class of all builder objects. This class provides access to the
/// owning module (and hence language) as well also a local allocator. All 
/// objects allocated to this builder (or rather the derived object) are 
/// destroyed when it goes out of scope.
struct factory : node_store
{
  factory(module&);

  allocator& get_allocator();
  term_equal get_equal_fn() const;
  term_hash get_hash_fn() const;

  const language& get_language() const;
  language& get_language();
  
  allocator& get_language_allocator();

  module& get_module();
  allocator& get_module_allocator();

  int generate_id();

  template<typename T>
  singleton_term_set<T>& make_singleton_set(allocator&);

  template<typename T>
  canonical_term_set<T>& make_canonical_set(allocator&);

  template<typename T, typename... Args>
  T& make(Args&&... args);

  module* mod_;
  term_equal eq_;
  term_hash hash_;
};

/// Initialize the builder object.
inline factory::factory(module& m) : mod_(&m), eq_(), hash_() { }

/// Returns the equality comparison function.
inline term_equal factory::get_equal_fn() const { return eq_; }

/// Returns the hash function.
inline term_hash factory::get_hash_fn() const { return hash_; }

/// Returns the language in which the module is written.
inline const language& factory::get_language() const { return mod_->get_language(); }

/// Returns the language in which the module is written.
inline language& factory::get_language() { return mod_->get_language(); }

/// Returns the allocator for the object.
inline module& factory::get_module() { return *mod_; }

/// Returns the allocator for the language.
inline allocator& factory::get_language_allocator() { return get_language().get_allocator(); }

/// Returns the allocator for the module.
inline allocator& factory::get_module_allocator() { return mod_->get_allocator(); }

/// Generate a unique identifier from the module.
inline int factory::generate_id() { return mod_->generate_id(); }

/// Create a singleton set whose allocator is gi
template<typename T>
singleton_term_set<T>& 
factory::make_singleton_set(allocator& alloc)
{
  return *new singleton_term_set<T>(alloc);
}

template<typename T>
canonical_term_set<T>& 
factory::make_canonical_set(allocator& alloc)
{
  return *new canonical_term_set<T>(hash_, eq_, alloc);
}

/// Construct an object from b, using the given arguments.
///
/// This is intended for use internally within builder objects to allocate
/// and construct AST nodes.
template<typename T, typename... Args>
T& 
factory::make(Args&&... args)
{
  typed_allocator<T> alloc(alloc_);
  T* ptr = alloc.allocate(1);
  return *new (ptr) T(std::forward<Args>(args)...);
}


} // namespace


#endif

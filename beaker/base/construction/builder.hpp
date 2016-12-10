// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_CONSTRUCTION_BUILDER_HPP
#define BEAKER_BASE_CONSTRUCTION_BUILDER_HPP

#include <beaker/util/memory.hpp>
#include <beaker/util/singleton_set.hpp>
#include <beaker/util/canonical_set.hpp>
#include <beaker/base/name.hpp>
#include <beaker/base/type.hpp>
#include <beaker/base/expr.hpp>
#include <beaker/base/decl.hpp>
#include <beaker/base/stmt.hpp>
#include <beaker/base/comparison/hash.hpp>


namespace beaker {

struct module;

/// The base class of all builder objects.
struct builder_base
{
  builder_base(module&);

  module& get_module();
  allocator& get_allocator();

  template<typename T, typename... Args>
  T& make(Args&&... args);

  module* mod_;
};

/// Initialize the builder object.
inline builder_base::builder_base(module& m) : mod_(&m) { }

/// Returns the allocator for the object.
inline module& builder_base::get_module() { return *mod_; }

/// Construct an object from b, using the given arguments.
///
/// This is intended for use internally within builder objects to allocate
/// and construct AST nodes.
template<typename T, typename... Args>
T& 
builder_base::make(Args&&... args)
{
  typed_allocator<T> alloc = get_allocator();
  T* ptr = alloc.allocate(1);
  return *new (ptr) T(std::forward<Args>(args)...);
}


/// A base class for builders that provides a static feature index.
template<int N>
struct basic_builder : builder_base
{
  enum { feature = N };

  using builder_base::builder_base;
};

} // namespace


#endif

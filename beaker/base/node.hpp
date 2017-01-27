// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_NODE_HPP
#define BEAKER_BASE_NODE_HPP

#include <beaker/util/memory.hpp>

#include <unordered_map>


namespace beaker {

struct allocator;
template<typename T> struct singleton_set;
template<typename T> struct canonical_set;

/// Provides a uniform store for allocated nodes.
///
/// This also includes facilities for registering language- and module-level 
/// canonical sets for specific kinds of nodes.
///
/// A set registry provides contains its own sequential allocator. This is
/// used to provide memory for all canonically allocated terms.
struct node_store
{
  allocator& get_allocator();

  template<typename T> singleton_set<T>& make_singleton_set();
  template<typename T> canonical_set<T>& make_canonical_set();

  sequential_allocator<> alloc_;
  std::unordered_map<int, void*> map_;
};

/// Returns the allocator for the node store.
inline allocator& node_store::get_allocator() { return alloc_; }

/// Register a new singleton set for the given node id. If such a set already
/// exists, then return the existing set.
template<typename T>
singleton_set<T>&
node_store::make_singleton_set()
{
  constexpr int k = T::node_kind;
  auto result = map_.emplace(k, nullptr);
  if (result.second)
    result.first->second = new singleton_set<T>(alloc_);
  void* p = result.first->second;
  return *reinterpret_cast<singleton_set<T>*>(p);
}

/// Register a new canonical set for the given node id. If such a set already
/// exists, then return the existing set.
template<typename T>
canonical_set<T>&
node_store::make_canonical_set()
{
  constexpr int k = T::node_kind;
  auto result = map_.emplace(k, nullptr);
  if (result.second)
    result.first->second = new canonical_set<T>(alloc_);
  void* p = result.first->second;
  return *reinterpret_cast<canonical_set<T>*>(p);
}

} // namespace


#endif

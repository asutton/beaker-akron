// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_UTIL_CANONICAL_SET_HPP
#define BEAKER_UTIL_CANONICAL_SET_HPP

#include <beaker/util/memory.hpp>
#include <beaker/util/hash.hpp>
#include <beaker/util/cast.hpp>

#include <unordered_set>


namespace beaker {

/// A canonical set guarantees the uniqueness of objects based on their 
/// attributes.
///
/// Canonical terms cannot be modified after construction. Doing so will
/// result in undefined behavior.
///
/// \todo This uses an extra indirection as a compiler firewall due to the
/// fact that libstdc++'s hash table requires T to be complete type.
template<typename T>
struct canonical_set
{
  using alloc_type = typed_allocator<T>;
  using set_type = std::unordered_set<T, universal_hash, universal_eq, alloc_type>;

  canonical_set();
  canonical_set(allocator&);
  ~canonical_set();

  template<typename... Args> 
  T& get(Args&&...);

  allocator* alloc_;
  set_type* set_;
};

template<typename T>
inline 
canonical_set<T>::canonical_set() 
  : alloc_(&default_allocator()), set_(new set_type(alloc_type(*alloc_)))
{ }

template<typename T>
inline 
canonical_set<T>::canonical_set(allocator& a) 
  : alloc_(&a), set_(new set_type(alloc_type(*alloc_)))
{ }

template<typename T>
inline 
canonical_set<T>::~canonical_set() 
{ 
  delete set_; 
}

/// Get the canonical value of T for args.
template<typename T>
template<typename... Args> 
inline T& 
canonical_set<T>::get(Args&&... args)
{
  auto iter = set_->emplace(std::forward<Args>(args)...).first;
  return modifiable(*iter);
}

} // namespace beaker


#endif

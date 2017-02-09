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
/// \note This uses an extra indirection as a compiler firewall due to the
/// fact that libstdc++'s hash table requires T to be complete type. We would
/// very much like to use this with only forward declarations available.
template<typename T, typename H, typename C>
struct canonical_set
{
  using hash_fn = H;
  using comp_fn = C;
  using alloc_type = typed_allocator<T>;
  using set_type = std::unordered_set<T, hash_fn, comp_fn, alloc_type>;

  canonical_set(const H&, const C&);
  canonical_set(const H&, const C&, allocator&);
  ~canonical_set();

  template<typename... Args> 
  T& get(Args&&...);

  set_type* set_;
};

template<typename T, typename H, typename C>
inline 
canonical_set<T, H, C>::canonical_set(const H& hash, const C& cmp)
  : set_(new set_type(7, hash, cmp, alloc_type(default_allocator())))
{ }

template<typename T, typename H, typename C>
inline 
canonical_set<T, H, C>::canonical_set(const H& hash, const C& cmp, allocator& a) 
  : set_(new set_type(7, hash, cmp, alloc_type(a)))
{ }

template<typename T, typename H, typename C>
inline 
canonical_set<T, H, C>::~canonical_set() 
{ 
  delete set_; 
}

/// Get the canonical value of T for args.
template<typename T, typename H, typename C>
template<typename... Args> 
inline T& 
canonical_set<T, H, C>::get(Args&&... args)
{
  auto iter = set_->emplace(std::forward<Args>(args)...).first;
  return modifiable(*iter);
}

} // namespace beaker


#endif

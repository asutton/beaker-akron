// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_UTIL_SINGLETON_SET_HPP
#define BEAKER_UTIL_SINGLETON_SET_HPP

#include <beaker/util/memory.hpp>

#include <utility>


namespace beaker {

/// A singleton set stores at most one object of a given type.
///
/// The object type may be incomplete.
///
/// \todo Use an allocator.
template<typename T>
struct singleton_set
{
  singleton_set();
  singleton_set(allocator&);
  ~singleton_set();

  template<typename... Args>
  T& get(Args&&... args);

  allocator* alloc_;
  T* obj_;
};

template<typename T>
inline 
singleton_set<T>::singleton_set() 
  : alloc_(&default_allocator()), obj_(nullptr)
{ }

template<typename T>
inline 
singleton_set<T>::singleton_set(allocator& a) 
  : alloc_(&a), obj_(nullptr)
{ }

template<typename T>
inline singleton_set<T>::~singleton_set() 
{ 
  typed_allocator<T> alloc(*alloc_);
  if (obj_) {
    alloc.destroy(obj_);
    alloc.deallocate(obj_);
  }
}

/// Get the canonical value of T for args.
template<typename T>
template<typename... Args> 
inline T& 
singleton_set<T>::get(Args&&... args)
{
  typed_allocator<T> alloc(*alloc_);
  if (!obj_) {
    obj_ = alloc.allocate();
    alloc.construct(obj_, std::forward<Args>(args)...);
  }
  return *obj_;
}

} // namespace beaker


#endif

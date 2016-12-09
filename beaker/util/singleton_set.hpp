// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_UTIL_SINGLETON_SET_HPP
#define BEAKER_UTIL_SINGLETON_SET_HPP

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
  ~singleton_set();

  template<typename... Args>
  T& get(Args&&... args);

  T* obj_;
};

template<typename T>
inline singleton_set<T>::singleton_set() : obj_(nullptr) { }

template<typename T>
inline singleton_set<T>::~singleton_set() { delete obj_; }

/// Get the canonical value of T for args.
template<typename T>
template<typename... Args> 
inline T& 
singleton_set<T>::get(Args&&... args)
{
  if (!obj_)
    obj_ = new T(std::forward<Args>(args)...);
  return *obj_;
}

} // namespace beaker


#endif

// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_UTIL_CAST_HPP
#define BEAKER_UTIL_CAST_HPP

#include <cassert>
#include <type_traits>


namespace beaker {

/// Returns true if and only if t has dynamic type R.
template<typename R, typename T>
bool is(const T& t)
{
  return t.get_kind() == R::node_kind;
}

/// Returns true if and only if t has dynamic type R.
template<typename R, typename T>
bool is(T& t)
{
  return t.get_kind() == R::node_kind;
}

/// Returns true if and only if t has dynamic type R.
template<typename R, typename T>
bool is(const T* t)
{
  return t->get_kind() == R::node_kind;
}

/// Returns true if and only if t has dynamic type R.
template<typename R, typename T>
bool is(T* t)
{
  return t->get_kind() == R::node_kind;
}


/// Returns t cast as its dynamic type or nullptr if t's dynamic type is not R.
/// Note that t may be null.
template<typename R, typename T>
const R* as(const T* t)
{
  if (t && is<R>(t))
    return static_cast<const R*>(t);
  else
    return nullptr;
}

/// Returns t cast as its dynamic type or nullptr if t's dynamic type is not R.
/// Note that t may be null.
template<typename R, typename T>
R* as(T* t)
{
  if (t && is<R>(t))
    return static_cast<R*>(t);
  else
    return nullptr;
}

/// Returns t cast as it's dynamic type R.
///
/// TODO: Re-enable casting checks when T and R are terms (i.e., have a 
/// static node_kind member).
template<typename R, typename T>
const R& cast(const T& t)
{
  // assert(is<R>(t));
  return static_cast<const R&>(t); 
}

/// Returns t cast as it's dynamic type R.
template<typename R, typename T>
R& cast(T& t)
{
  // assert(is<R>(t));
  return static_cast<R&>(t); 
}

/// Returns t cast as it's dynamic type R.
template<typename R, typename T>
const R* cast(const T* t)
{
  // assert(is<R>(t));
  return static_cast<const R*>(t); 
}

/// Returns t cast as it's dynamic type R.
template<typename R, typename T>
R* cast(T* t)
{
  // assert(is<R>(t));
  return static_cast<R*>(t); 
}


/// Returns t as a modifiable object.
template<typename T>
T& modifiable(const T& t)
{
  return const_cast<T&>(t);
}

/// Returns t as a pointer to a modifiable object.
template<typename T>
T* modifiable(const T* t)
{
  return const_cast<T*>(t);
}

} // namespace beaker

#endif

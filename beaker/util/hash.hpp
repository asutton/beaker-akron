// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_UTIL_HASH_HPP
#define BEAKER_UTIL_HASH_HPP

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <typeinfo>


namespace beaker {

namespace fnv1_detail {

constexpr std::uint32_t prime_32() { return 16777619u; }
constexpr std::uint32_t basis_32() { return 2166136261u; }

constexpr std::uint64_t prime_64() { return 1099511628211ul; }
constexpr std::uint64_t basis_64() { return 14695981039346656037ul; }

// FIXME: This is a hack
#ifndef BEAKER_ARCH
#  define BEAKER_ARCH 64
#endif

#if BEAKER_ARCH == 32
constexpr std::size_t prime() { return prime_32(); }
constexpr std::size_t basis() { return basis_32(); }
#elif BEAKER_ARCH == 64
constexpr std::size_t prime() { return prime_64(); }
constexpr std::size_t basis() { return basis_64(); }
#else 
#  error Unknown target architecture
#endif

} // namespace fnv1_detail


// The fnv1a hash algorithm is the default hasher.
struct hasher
{
  hasher()
    : code(fnv1_detail::basis())
  { }

  void operator()(const void* p, int n) 
  {
    unsigned char const* first = static_cast<unsigned char const*>(p);
    unsigned char const* limit = first + n;
    for ( ; first != limit; ++first)
      code = (code ^ *first) * fnv1_detail::prime();
  }

  operator std::size_t() const 
  {
    return code;
  }

  std::size_t code;
};


// A std-compatible hash function using fnv1a and append.
struct universal_hash
{
  template<typename T>
  std::size_t operator()(const T& t) const {
    hasher h;
    hash(h, t);
    return h;
  }
};


// A universal comparator for equivalence.
struct universal_eq
{
  template<typename T>
  bool operator()(const T& a, const T& b) const
  {
    return equivalent(a, b);
  }
};


// A std-compatible hash function using fnv1a and append.
struct indirect_hash
{
  template<typename T>
  std::size_t operator()(T const* t) const {
    hasher h;
    hash(h, *t);
    return h;
  }
};


// A universal equivalence comparator.
struct indirect_eq
{
  template<typename T>
  bool operator()(const T* a, const T* b) const
  {
    return equivalent(*a, *b);
  }
};


// Hash for trivially comparable T.
template<typename T>
inline typename std::enable_if<std::is_integral<T>::value, void>::type
hash(hasher& h, T const& t)
{
  h(&t, sizeof(t));
}

// Hash for floating point T. Guarantee that 0 and -0 have the same 
// hash code since 0 == -0.
template<typename T>
inline typename std::enable_if<std::is_floating_point<T>::value, void>::type
hash(hasher& h, T t)
{
  if (t == 0)
    t = 0;
  h(&t, sizeof(t));
}

// Hash for pointers. This just hashes the bits of the address.
template<typename T>
inline void
hash(hasher& h, const T* p)
{
  h(&p, sizeof(p));
}

// Hash append for nullptr.
inline void
hash(hasher& h, std::nullptr_t p)
{
  h(&p, sizeof(p));
}

// Hash for typeids.
inline void
hash(hasher& h, const std::type_info& ti) 
{
  hash(h, ti.hash_code());
}

} // namespace beaker


#endif

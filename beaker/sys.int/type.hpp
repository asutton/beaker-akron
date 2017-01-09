// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_INT_TYPE_HPP
#define BEAKER_SYS_INT_TYPE_HPP

#include <beaker/base/type.hpp>


namespace beaker {
namespace sys_int {

enum 
{
  first_type_kind = sys_int_lang_block,
#define def_type(e) e ## _type_kind,
#include "type.def"
  last_type_kind
};


// A base class of all integral types.
//
// The precision of a natural number type is the number of bits in its value 
// representation. In full generality, this can be any non-negative value
// however, languages with byte-oriented objects may not support unaligned
// integer sizes.
struct integral_type : object_type
{
  integral_type(int, int);

  int get_precision() const;

  int prec_;
};

inline
integral_type::integral_type(int k, int p) 
  : object_type(k), prec_(p)
{ }

// Returns the precision of the numeric type.
inline int integral_type::get_precision() const { return prec_; }


/// A helper class for defining integral types.
template<int K>
struct integral_type_impl : integral_type
{
  static constexpr int node_kind = K;

  explicit integral_type_impl(int);
};

template<int K>
inline 
integral_type_impl<K>::integral_type_impl(int p)
  : integral_type(K, p) 
{ }


// -------------------------------------------------------------------------- //
// Types

/// Represents natural number types with k bits of precision.
///
/// The values of a natural number type range from [0, 2^k).
struct nat_type : integral_type_impl<nat_type_kind> 
{
  using integral_type_impl<nat_type_kind>::integral_type_impl;
};


/// Represents integer types with k bits of precision.
///
/// The values of an integer type range from [-2^(k-1), 2^(k-1)).
struct int_type : integral_type_impl<int_type_kind> 
{
  using integral_type_impl<int_type_kind>::integral_type_impl;
};


/// Represents integers mod k where k is the number of bits of precision.
///
/// The values of an integer mod k range from [0, 2^k).
struct mod_type : integral_type_impl<mod_type_kind> 
{
  using integral_type_impl<mod_type_kind>::integral_type_impl;
};

// -------------------------------------------------------------------------- //
// Operations

/// Returns true if `t` is an integral type.
///
/// The integral types are the types `natN`, `intN`, and`modN` for all 
/// valid values of N.
inline bool
is_integral_type(const type& t)
{
  return dynamic_cast<const integral_type*>(&t);
}

/// Returns true if `t` is a signed type.
///
/// The signed types are `intN` and `floatN`.
inline bool
is_signed_type(const type& t) 
{
  return t.get_kind() == int_type_kind;
}

/// Returns true if `t` is an unsigned type.
///
/// The unsigned types are `charN`, `natN` and `modN`.
inline bool
is_unsigned_type(const type& t) 
{
  return t.get_kind() == nat_type_kind ||
         t.get_kind() == mod_type_kind;
}

/// Returns the precision of the type `t`.
inline int 
get_precision(const type& t)
{
  assert(is_integral_type(t));
  return static_cast<const integral_type&>(t).get_precision();
}

} // namespace sys_int
} // namespace beaker


#endif

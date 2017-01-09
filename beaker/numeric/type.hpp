// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_NUMERIC_TYPE_HPP
#define BEAKER_NUMERIC_TYPE_HPP

#include <beaker/base/type.hpp>


namespace beaker {
namespace numeric {

enum 
{
  first_type_kind = numeric_lang_block,
#define def_type(e) e ## _type_kind,
#include "type.def"
  last_type_kind
};


// A base class of all numeric types.
//
// The precision of a natural number type is the number of bits in its value 
// representation. In full generality, this can be any non-negative value
// however, languages with byte-oriented objects may not support unaligned
// integer sizes.
template<int K>
struct numeric_type : generic_object_type<K>
{
  explicit numeric_type(int);

  int get_precision() const;

  int prec_;
};

template<int K>
inline
numeric_type<K>::numeric_type(int p) 
  : generic_object_type<K>(), prec_(p)
{ }

// Returns the precision of the numeric type.
template<int K>
inline int numeric_type<K>::get_precision() const { return prec_; }


// Represents character or code point values with k bits of precision.
//
// The values of a natural number type range from [0, 2^k).
struct char_type : numeric_type<char_type_kind> 
{
  using numeric_type<char_type_kind>::numeric_type;
};


// Represents natural number types with k bits of precision.
//
// The values of a natural number type range from [0, 2^k).
struct nat_type : numeric_type<nat_type_kind> 
{
  using numeric_type<nat_type_kind>::numeric_type;
};


// Represents integer types with k bits of precision.
//
// The values of an integer type range from [-2^(k-1), 2^(k-1)).
struct int_type : numeric_type<int_type_kind> 
{
  using numeric_type<int_type_kind>::numeric_type;
};


// Represents integers mod k where k is the number of bits of precision.
//
// The values of an integer mod k range from [0, 2^k).
struct mod_type : numeric_type<mod_type_kind> 
{
  using numeric_type<mod_type_kind>::numeric_type;
};


// Represents IEEE 754 floating point types with k bits of precision.
//
// The precision of a floating point type must be either 32 or 64.
//
// TODO: It could be different than 32 or 64.
struct float_type : numeric_type<float_type_kind> 
{
  using numeric_type<float_type_kind>::numeric_type;
};


// Operations

/// Returns true if t is a signed type.
///
/// The signed types are `intN` and `floatN`.
inline bool
is_signed_type(const type& t) 
{
  return t.get_kind() == int_type_kind ||
         t.get_kind() == float_type_kind;
}

/// Returns true if `t` is an unsigned type.
///
/// The unsigned types are `charN`, `natN` and `modN`.
inline bool
is_unsigned_type(const type& t) 
{
  return t.get_kind() == char_type_kind ||
         t.get_kind() == nat_type_kind ||
         t.get_kind() == mod_type_kind;
}

/// Returns true if `t` is an integral type.
///
/// The integral types are the types `natN`, `intN`, and`modN` for all 
/// valid values of N.
inline bool
is_integral_type(const type& t)
{
  return t.get_kind() == char_type_kind ||
         t.get_kind() == nat_type_kind ||
         t.get_kind() == int_type_kind ||
         t.get_kind() == mod_type_kind;
}

/// Returns true if `t` is a floating point type.
///
/// The floating point types the types`floatN` for all valid values of N.
inline bool
is_floating_point_type(const type& t)
{
  return t.get_kind() == float_type_kind;
}

/// Returns true if `t` is an arithmetic type.
///
/// The arithmetic types are the integral types and floating point types.
inline bool
is_arithmetic_type(const type& t)
{
  return is_integral_type(t) || is_floating_point_type(t);
}


int get_precision(const type&);

} // namespace numeric
} // namespace beaker


#endif

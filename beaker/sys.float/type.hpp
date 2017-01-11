// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_FLOAT_TYPE_HPP
#define BEAKER_SYS_FLOAT_TYPE_HPP

#include <beaker/base/type.hpp>


namespace beaker {
namespace sys_float {

enum 
{
  first_type_kind = sys_float_lang_block,
#define def_type(e) e ## _type_kind,
#include "type.def"
  last_type_kind
};

// -------------------------------------------------------------------------- //
// Types

/// Represents IEEE 754 floating point types with k bits of precision.
///
/// The precision of a floating point type must be either 32 or 64, including
/// positive and negative infinities and NaN.
///
/// \todo Precision could be different than 32 or 64.
///
/// \todo How should we represent ordered floating point values? Are they
/// fundamentally different types (e.g., real_type?).
struct float_type : object_type_impl<float_type_kind> 
{
  explicit float_type(int);

  int get_precision() const;

  int prec_;
};

inline 
float_type::float_type(int p)
  : object_type_impl<node_kind>(), prec_(p)
{ }

/// Returns the precision of the floating point type.
inline int float_type::get_precision() const { return prec_; }


// -------------------------------------------------------------------------- //
// Operations

/// Returns true if `t` is a floating point type.
///
/// The floating point types the types`floatN` for all valid values of N.
inline bool
is_floating_point_type(const type& t)
{
  return t.get_kind() == float_type_kind;
}

} // namespace sys_float
} // namespace beaker


#endif

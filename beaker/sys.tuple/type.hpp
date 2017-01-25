// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_TUPLE_TYPE_HPP
#define BEAKER_SYS_TUPLE_TYPE_HPP

#include <beaker/base/type.hpp>


namespace beaker {
namespace sys_tuple {

enum 
{
  first_type_kind = sys_tuple_lang_block,
#define def_type(e) e ## _type_kind,
#include "type.def"
  last_type_kind
};


// -------------------------------------------------------------------------- //
// Types

/// Represents a fixed-length sequence of (probably heterogeneous, possibly
/// empty) object types of the form `{t1, t2, ..., }`.
///
/// \todo Can a tuple element have function type? Presumably since we're
/// modeling their values as function pointers.
struct tuple_type : object_type_impl<tuple_type_kind> 
{
  tuple_type(const type_seq&);
  tuple_type(type_seq&&);

  const type_seq& get_element_types() const;
  type_seq& get_element_types();

  type_seq elems_;
};

inline 
tuple_type::tuple_type(const type_seq& ts)
  : object_type_impl<tuple_type_kind>(), elems_(ts)
{ }

inline 
tuple_type::tuple_type(type_seq&& ts)
  : object_type_impl<tuple_type_kind>(), elems_(std::move(ts))
{ }

/// Returns the sequence of element types.
inline const type_seq& tuple_type::get_element_types() const { return elems_; }

// Returns the sequence of element types.
inline type_seq& tuple_type::get_element_types() { return elems_; }

} // namespace sys_tuple
} // namespace beaker


#endif

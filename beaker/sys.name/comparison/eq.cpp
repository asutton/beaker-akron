// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/core/comparison/eq.hpp>
#include <beaker/core/name.hpp>
#include <beaker/core/type.hpp>
#include <beaker/core/expr.hpp>


namespace beaker {
namespace core {


// Returns true when a and b have the same symbol.
static inline bool
eq_basic_name(const basic_name& a, const basic_name& b)
{
  return &a.get_symbol() == &b.get_symbol();
}

// Returns true when a and b have the same internal id.
static inline bool
eq_internal_name(const internal_name& a, const internal_name& b)
{
  return a.get_id() == b.get_id();
}

/// This feature does not define names.
bool
eq_algo::operator()(const name& a, const name& b) const
{
  switch (a.get_kind()) {
    case basic_name_kind:
      return eq_basic_name(cast<basic_name>(a), cast<basic_name>(b));
    case internal_name_kind:
      return eq_internal_name(cast<internal_name>(a), cast<internal_name>(b));
    default:
      break;
  }
  assert(false && "not a core name");
}

// There is only one void type.
static inline bool
eq_void_type(const void_type& a, const void_type& b)
{
  return true;
}

// Returns true if a and b have equivalent object types.
static inline bool
eq_ref_type(const ref_type& a, const ref_type& b)
{
  return equivalent(a.get_object_type(), b.get_object_type());
}

// Returns true if a and b have parameter and return types.
static bool
eq_fn_type(const fn_type& a, const fn_type& b)
{
  return equivalent(a.get_return_type(), b.get_return_type()) &&
         equivalent(a.get_parameter_types(), b.get_parameter_types());
}

bool
eq_algo::operator()(const type& a, const type& b) const
{
  assert(a.get_kind() == b.get_kind());
  switch (a.get_kind()) {
    case void_type_kind:
      return eq_void_type(cast<void_type>(a), cast<void_type>(b));
    case ref_type_kind:
      return eq_ref_type(cast<ref_type>(a), cast<ref_type>(b));
    case fn_type_kind:
      return eq_fn_type(cast<fn_type>(a), cast<fn_type>(b));
    default:
      break;
  }
  assert(false && "not a common type");
}


/// \todo Implement me.
bool
eq_algo::operator()(const expr& a, const expr& b) const
{
  assert(false && "not implemented");
}

} // namespace core
} // namespace beaker

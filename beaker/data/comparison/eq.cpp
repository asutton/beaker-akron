// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/data/type.hpp>
#include <beaker/base/comparison/eq.hpp>


namespace beaker {
namespace data {

bool
equivalent_name(const name& a, const name& b)
{
  assert(false && "not defined");
}

// Returns true when a and b have equivalent element types.
static inline bool 
equivalent_tuple_type(const tuple_type& a, const tuple_type& b)
{
  return equivalent(a.get_element_types(), b.get_element_types());
}

// Returns true when a and b have the same extent and element type.
static inline bool 
equivalent_array_type(const array_type& a, const array_type& b)
{
  return a.get_extent() == b.get_extent() &&
         equivalent(a.get_element_type(), b.get_element_type());
}

// Returns true when a and b have equivalent element types.
static inline bool 
equivalent_seq_type(const seq_type& a, const seq_type& b)
{
  return equivalent(a.get_element_type(), b.get_element_type());
}

// Returns true when data types a and b are equivalent.
bool 
equivalent_type(const type& a, const type& b)
{
  assert(a.get_kind() == b.get_kind());
  switch (a.get_kind()) {
    case tuple_type_kind:
      return equivalent_tuple_type(cast<tuple_type>(a), cast<tuple_type>(b));
    case array_type_kind:
      return equivalent_array_type(cast<array_type>(a), cast<array_type>(b));
    case seq_type_kind:
      return equivalent_seq_type(cast<seq_type>(a), cast<seq_type>(b));
    default:
      break;
  }
  assert(false && "not a data type");
}

bool
equivalent_expr(const expr& a, const expr& b)
{
  assert(false && "not implemented");
}

} // namespace data
} // namespace beaker

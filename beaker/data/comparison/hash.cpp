// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/data/type.hpp>
#include <beaker/base/comparison/hash.hpp>

#include <iostream>


namespace beaker {
namespace data {

void
hash_name(hasher& h, const name& t)
{
  assert(false && "not defined");
}

// Append the hash of t's element types to h.
static inline void
hash_tuple_type(hasher& h, const tuple_type& t)
{
  hash(h, t.get_element_types());
}

// Append the hash of t's extent and element type to h.
static inline void
hash_array_type(hasher& h, const array_type& t)
{
  hash(h, t.get_extent());
  hash(h, t.get_element_type());
}

// Append the hash of t's element type to h.
static inline void
hash_seq_type(hasher& h, const seq_type& t)
{
  hash(h, t.get_element_type());
}

// Append the hash of data type to h.
void 
hash_type(hasher& h, const type& t)
{
  switch (t.get_kind()) {
    case tuple_type_kind:
      return hash_tuple_type(h, cast<tuple_type>(t));
    case array_type_kind:
      return hash_array_type(h, cast<array_type>(t));
    case seq_type_kind:
      return hash_seq_type(h, cast<seq_type>(t));
    default:
      break;
  }
  assert(false && "not a data type");
}

void
hash_expr(hasher& h, const expr& e)
{
  assert(false && "not implemented");
}

} // namespace data
} // namespace beaker

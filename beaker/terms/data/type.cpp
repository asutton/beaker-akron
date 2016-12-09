// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "type.hpp"

#include <iostream>


namespace beaker {
namespace data {

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


// Append the hash of t's element types to h.
static inline void
append_tuple_type(hasher& h, const tuple_type& t)
{
  append(h, t.get_element_types());
}

// Append the hash of t's extent and element type to h.
static inline void
append_array_type(hasher& h, const array_type& t)
{
  append(h, t.get_extent());
  append(h, t.get_element_type());
}

// Append the hash of t's element type to h.
static inline void
append_seq_type(hasher& h, const seq_type& t)
{
  append(h, t.get_element_type());
}

// Append the hash of data type to h.
void 
append_type(hasher& h, const type& t)
{
  switch (t.get_kind()) {
    case tuple_type_kind:
      return append_tuple_type(h, cast<tuple_type>(t));
    case array_type_kind:
      return append_array_type(h, cast<array_type>(t));
    case seq_type_kind:
      return append_seq_type(h, cast<seq_type>(t));
    default:
      break;
  }
  assert(false && "not a data type");
}


// Pretty priont the tuple type t.
void
print_tuple_type(std::ostream& os, const tuple_type& t)
{
  os <<  "tuple" << ' ';
  os << '{';
  const type_seq& elems = t.get_element_types();
  for (auto iter = elems.begin(); iter != elems.end(); ++iter) {
    print(os, *iter);
    if (std::next(iter) != elems.end())
      os << ',';
  }
  os << '}';
}

// Petty print the array type t.
void
print_array_type(std::ostream& os, const array_type& t)
{
  print(os, t.get_element_type());
  os << '[' << t.get_extent() << ']';
}

// Pretty print the sequence type t.
void
print_seq_type(std::ostream& os, const seq_type& t)
{
  print(os, t.get_element_type());
  os << "[]";
}

// Pretty print the data type t.
void
print_type(std::ostream& os, const type& t)
{
  switch (t.get_kind()) {
    case tuple_type_kind:
      return print_tuple_type(os, cast<tuple_type>(t));
    case array_type_kind:
      return print_array_type(os, cast<array_type>(t));
    case seq_type_kind:
      return print_seq_type(os, cast<seq_type>(t));
    default:
      break;
  }
  assert(false && "not a data type");
}

} // namespace data
} // namespace beaker

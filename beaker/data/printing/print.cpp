// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/data/printing/print.hpp>
#include <beaker/data/expr.hpp>

#include <iostream>


namespace beaker {
namespace data {

void
print_algo::operator()(std::ostream& os, const type&) const
{
  assert(false && "not implemented");
}

// Pretty print the tuple expression e.
void 
print_tuple_expr(std::ostream& os, const tuple_expr& e)
{
  os << '{';
  const expr_seq& elems = e.get_elements();
  for (auto iter = elems.begin(); iter != elems.end(); ++iter) {
    print(os, *iter);
    if (std::next(iter) != elems.begin())
      os << ',';
  }
  os << '}';
}

// Pretty print the array expression e.
void 
print_array_expr(std::ostream& os, const array_expr& e)
{
  os << '[';
  const expr_seq& elems = e.get_elements();
  for (auto iter = elems.begin(); iter != elems.end(); ++iter) {
    print(os, *iter);
    if (std::next(iter) != elems.begin())
      os << ',';
  }
  os << ']';
}

// Pretty print the element expression e.
void 
print_elem_expr(std::ostream& os, const elem_expr& e)
{
  print(os, e.get_tuple());
  os << '.';
  os << e.get_index();
}

// Pretty print the index expression e.
void 
print_index_expr(std::ostream& os, const index_expr& e)
{
  print(os, e.get_array());
  os << '[';
  print(os, e.get_index());
  os << ']';
}

// Pretty print the data expression e.
void 
print_algo::operator()(std::ostream& os, const expr& e) const
{
  switch (e.get_kind()) {
    case tuple_expr_kind:
      return print_tuple_expr(os, cast<tuple_expr>(e));
    case array_expr_kind:
      return print_tuple_expr(os, cast<tuple_expr>(e));
    case elem_expr_kind:
      return print_tuple_expr(os, cast<tuple_expr>(e));
    case index_expr_kind:
      return print_tuple_expr(os, cast<tuple_expr>(e));
    default:
      break;
  }
  assert(false && "not a data expression");
}

} // namespace data
} // namespace beaker

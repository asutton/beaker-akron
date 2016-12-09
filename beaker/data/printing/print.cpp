// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/data/expr.hpp>
#include <beaker/base/printing/print.hpp>

#include <iostream>


namespace beaker {
namespace data {

void
print_name(std::ostream& os, const name&)
{
  assert(false && "not defined");
}

void
print_type(std::ostream& os, const type&)
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
print_expr(std::ostream& os, const expr& e)
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


void
print_decl(std::ostream& os, const decl& d)
{
  assert(false && "not defined");
}

void
print_stmt(std::ostream& os, const stmt& s)
{
  assert(false && "not defined");
}

} // namespace data
} // namespace beaker

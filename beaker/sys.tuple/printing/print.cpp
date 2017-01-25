// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "print.hpp"
#include "../type.hpp"
#include "../expr.hpp"

#include <iostream>


namespace beaker {
namespace sys_tuple {

void
print_tuple_type(std::ostream& os, const tuple_type& t)
{
  print(os, '{');
  print_comma_separated(os, t.get_element_types());
  print(os, '}');
}

void 
print_algo::operator()(std::ostream& os, const type& t) const
{
  print_tuple_type(os, cast<tuple_type>(t));
}


void
print_tuple_expr(std::ostream& os, const tuple_expr& e)
{
  print(os, '{');
  print_comma_separated(os, e.get_elements());
  print(os, '}');
}

void
print_proj_expr(std::ostream& os, const proj_expr& e)
{
  print(os, e.get_object());
  print(os, '.');
  print(os, e.get_element());
}

void
print_algo::operator()(std::ostream& os, const expr& e) const
{
  switch (e.get_kind()) {
    case tuple_expr_kind:
      return print_tuple_expr(os, cast<tuple_expr>(e));
    case proj_expr_kind:
      return print_proj_expr(os, cast<proj_expr>(e));
    default:
      break;
  }
  assert(false && "not an tuple expression");
}

} // namespace sys_tuple
} // namespace beaker



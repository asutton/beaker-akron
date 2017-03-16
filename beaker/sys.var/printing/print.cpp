// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "print.hpp"
#include "../type.hpp"
#include "../expr.hpp"

#include <iostream>


namespace beaker {

/// \todo Do we need to enclose the object type in parens? Probably if the
/// object type turns out to be a function.
void
print_type(pretty_printer& pp, const sys_var::ref_type& t)
{
  print(pp, t.get_object_type());
  pp.print('&');
}

/// \todo Actually show the reference?
void
print_expr(pretty_printer& pp, const sys_var::ref_expr& e)
{
  print(pp, e.get_name());
}

void
print_expr(pretty_printer& pp, const sys_var::val_expr& e)
{
  print(pp, e.get_source());
}

void
print_expr(pretty_printer& pp, const sys_var::assign_expr& e)
{
  print_infix_expr(pp, e, "=");
}

void
print_expr(pretty_printer& pp, const sys_var::nop_init& e)
{
  assert(false && "not implemented");
}

void
print_expr(pretty_printer& pp, const sys_var::zero_init& e)
{
  assert(false && "not implemented");
}

void
print_expr(pretty_printer& pp, const sys_var::copy_init& e)
{
  print(pp, e.get_expression());
}

void
print_expr(pretty_printer& pp, const sys_var::ref_init& e)
{
  print(pp, e.get_expression());
}

} // namespace beaker

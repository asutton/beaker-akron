// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "print.hpp"
#include "../type.hpp"
#include "../expr.hpp"

#include <iostream>


namespace beaker {
namespace sys_var {

/// \todo Do we need to enclose the object type in parens? Probably if the
/// object type turns out to be a function.
void
print_type(pretty_printer& pp, const sys_var::ref_type& t)
{
  print(pp, t.get_object_type());
  pp.print('&');
}

void
print_expr(pretty_printer& pp, const sys_var::ref_expr& e)
{
  assert(false && "not implemented");
}

void
print_expr(pretty_printer& pp, const sys_var::deref_expr& e)
{
  assert(false && "not implemented");
}

void
print_expr(pretty_printer& pp, const sys_var::assign_expr& e)
{
  assert(false && "not implemented");
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
  assert(false && "not implemented");
}

void
print_expr(pretty_printer& pp, const sys_var::ref_init& e)
{
  assert(false && "not implemented");
}

} // namespace sys_var
} // namespace beaker

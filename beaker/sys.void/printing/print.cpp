// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "print.hpp"
#include "../type.hpp"
#include "../expr.hpp"

#include <iostream>


namespace beaker {

void
print_type(pretty_printer& pp, const sys_void::void_type& t)
{
  pp.print("void");
}

void
print_expr(pretty_printer& pp, const sys_void::nop_expr& e)
{
  pp.print("nop");
}

void
print_expr(pretty_printer& pp, const sys_void::void_expr& e)
{
  print_builtin_call_expr(pp, "void", e.get_operand());
}

void
print_expr(pretty_printer& pp, const sys_void::trap_expr& e)
{
  pp.print("trap");
}

} // namespace beaker


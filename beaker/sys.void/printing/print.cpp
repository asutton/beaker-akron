// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "print.hpp"
#include "../type.hpp"
#include "../expr.hpp"

#include <iostream>


namespace beaker {
namespace sys_void {

void
print(pretty_printer& pp, const void_type& t)
{
  pp.print("void");
}

void
print(pretty_printer& pp, const nop_expr& e)
{
  pp.print("nop");
}

void
print(pretty_printer& pp, const void_expr& e)
{
  print_builtin_call_expr(pp, "void", e.get_operand());
}

void
print(pretty_printer& pp, const trap_expr& e)
{
  pp.print("trap");
}

} // namespace sys_void
} // namespace beaker


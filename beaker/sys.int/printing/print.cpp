// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "print.hpp"
#include "../type.hpp"
#include "../expr.hpp"

#include <iostream>


namespace beaker {
namespace sys_int {

void 
print(pretty_printer& pp, const nat_type& t)
{
  pp.print("nat");
  pp.print((std::uintmax_t)t.get_precision()); // FIXME: Remove this cast
}

void 
print(pretty_printer& pp, const int_type& t)
{
  pp.print("int");
  pp.print((std::intmax_t)t.get_precision()); // FIXME: Remove this cast
}

void 
print(pretty_printer& pp, const mod_type& t)
{
  pp.print("mod");
  pp.print((std::uintmax_t)t.get_precision()); // FIXME: Remove this cast
}

void
print(pretty_printer& pp, const int_expr& e)
{
  pp.print(e.get_integer());
}

void
print(pretty_printer& pp, const eq_expr& e)
{
  print_infix_expr(pp, e, "==");
}

void
print(pretty_printer& pp, const ne_expr& e)
{
  print_infix_expr(pp, e, "!=");
}

void
print(pretty_printer& pp, const lt_expr& e)
{
  print_infix_expr(pp, e, "<");
}

void
print(pretty_printer& pp, const gt_expr& e)
{
  print_infix_expr(pp, e, ">");
}

void
print(pretty_printer& pp, const le_expr& e)
{
  print_infix_expr(pp, e, "<=");
}

void
print(pretty_printer& pp, const ge_expr& e)
{
  print_infix_expr(pp, e, ">=");
}

void
print(pretty_printer& pp, const add_expr& e)
{
  print_infix_expr(pp, e, "+");
}

void
print(pretty_printer& pp, const sub_expr& e)
{
  print_infix_expr(pp, e, "-");
}

void
print(pretty_printer& pp, const mul_expr& e)
{
  print_infix_expr(pp, e, "*");
}

void
print(pretty_printer& pp, const quo_expr& e)
{
  print_infix_expr(pp, e, "/");
}

void
print(pretty_printer& pp, const rem_expr& e)
{
  print_infix_expr(pp, e, "%");
}

void
print(pretty_printer& pp, const neg_expr& e)
{
  print_prefix_expr(pp, e, "-");
}

void
print(pretty_printer& pp, const rec_expr& e)
{
  print_prefix_expr(pp, e, "/");
}

} // namespace sys_int
} // namespace beaker



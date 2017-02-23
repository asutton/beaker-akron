// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "print.hpp"
#include "../type.hpp"
#include "../expr.hpp"

#include <iostream>


namespace beaker {

void 
print_type(pretty_printer& pp, const sys_int::nat_type& t)
{
  pp.print("nat");
  pp.print((std::uintmax_t)t.get_precision()); // FIXME: Remove this cast
}

void 
print_type(pretty_printer& pp, const sys_int::int_type& t)
{
  pp.print("int");
  pp.print((std::intmax_t)t.get_precision()); // FIXME: Remove this cast
}

void 
print_type(pretty_printer& pp, const sys_int::mod_type& t)
{
  pp.print("mod");
  pp.print((std::uintmax_t)t.get_precision()); // FIXME: Remove this cast
}

void
print_expr(pretty_printer& pp, const sys_int::int_expr& e)
{
  pp.print(e.get_integer());
}

void
print_expr(pretty_printer& pp, const sys_int::eq_expr& e)
{
  print_infix_expr(pp, e, "==");
}

void
print_expr(pretty_printer& pp, const sys_int::ne_expr& e)
{
  print_infix_expr(pp, e, "!=");
}

void
print_expr(pretty_printer& pp, const sys_int::lt_expr& e)
{
  print_infix_expr(pp, e, "<");
}

void
print_expr(pretty_printer& pp, const sys_int::gt_expr& e)
{
  print_infix_expr(pp, e, ">");
}

void
print_expr(pretty_printer& pp, const sys_int::le_expr& e)
{
  print_infix_expr(pp, e, "<=");
}

void
print_expr(pretty_printer& pp, const sys_int::ge_expr& e)
{
  print_infix_expr(pp, e, ">=");
}

void
print_expr(pretty_printer& pp, const sys_int::add_expr& e)
{
  print_infix_expr(pp, e, "+");
}

void
print_expr(pretty_printer& pp, const sys_int::sub_expr& e)
{
  print_infix_expr(pp, e, "-");
}

void
print_expr(pretty_printer& pp, const sys_int::mul_expr& e)
{
  print_infix_expr(pp, e, "*");
}

void
print_expr(pretty_printer& pp, const sys_int::quo_expr& e)
{
  print_infix_expr(pp, e, "/");
}

void
print_expr(pretty_printer& pp, const sys_int::rem_expr& e)
{
  print_infix_expr(pp, e, "%");
}

void
print_expr(pretty_printer& pp, const sys_int::neg_expr& e)
{
  print_prefix_expr(pp, e, "-");
}

void
print_expr(pretty_printer& pp, const sys_int::rec_expr& e)
{
  print_prefix_expr(pp, e, "/");
}

} // namespace beaker



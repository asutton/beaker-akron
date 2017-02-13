// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "print.hpp"
#include "../type.hpp"
#include "../expr.hpp"


namespace beaker {

/// Pretty print the type `bool`.
void
print(pretty_printer& pp, const sys_bool::bool_type& t)
{
  pp.print("bool");
}

// Pretty print the boolean expression e.
void
print(pretty_printer& pp, const sys_bool::bool_expr& e)
{
  if (e.get_boolean())
    pp.print("true");
  else
    pp.print("false");
}

// Pretty print the expression `e1 & e2`.
void
print(pretty_printer& pp, const sys_bool::and_expr& e)
{
  print_infix_expr(pp, e, "&");
}

// Pretty print the expression `e1 | e2`.
void
print(pretty_printer& pp, const sys_bool::or_expr& e)
{
  print_infix_expr(pp, e, "|");
}

// Pretty print the expression `e1 ^ e2`.
void
print(pretty_printer& pp, const sys_bool::xor_expr& e)
{
  print_infix_expr(pp, e, "^");
}

// Pretty print the expression `!e`.
void
print(pretty_printer& pp, const sys_bool::not_expr& e)
{
  print_prefix_expr(pp, e, "!");
}

// Pretty print the expression `e1 => e2`.
void
print(pretty_printer& pp, const sys_bool::imp_expr& e)
{
  print_infix_expr(pp, e, "=>");
}

// Pretty print the expression `e1 <=> e2`.
void
print(pretty_printer& pp, const sys_bool::eq_expr& e)
{
  print_infix_expr(pp, e, "<=>");
}

// Pretty print the expression `e1 ? e2 : e3`.
void
print(pretty_printer& pp, const sys_bool::if_expr& e)
{
  print_grouped_expr(pp, e.get_condition());
  print_binary_op(pp, '?');
  print_grouped_expr(pp, e.get_true_value());
  print_binary_op(pp, ':');
  print_grouped_expr(pp, e.get_false_value());
}

// Pretty print the expression `e1 && e2`.
void
print(pretty_printer& pp, const sys_bool::and_then_expr& e)
{
  print_infix_expr(pp, e, "&&");
}

// Pretty print the expression `e1 || e2`.
void
print(pretty_printer& pp, const sys_bool::or_else_expr& e)
{
  print_infix_expr(pp, e, "||");
}

void
print(pretty_printer& pp, const sys_bool::assert_expr& e)
{
  print_builtin_call_expr(pp, "assert", e.get_operand());
}

} // namespace beaker


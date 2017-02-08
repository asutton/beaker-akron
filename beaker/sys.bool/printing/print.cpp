// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "print.hpp"
#include "../type.hpp"
#include "../expr.hpp"


namespace beaker {
namespace sys_bool {

/// Pretty print the type `bool`.
void
print_bool_type(pretty_printer& pp, const bool_type& t)
{
  pp.print("bool");
}

// Pretty print the boolean expression e.
void
print_bool_expr(pretty_printer& pp, const bool_expr& e)
{
  if (e.get_boolean())
    pp.print("true");
  else
    pp.print("false");
}

// Pretty print the expression `e1 & e2`.
void
print_and_expr(pretty_printer& pp, const and_expr& e)
{
  print_infix_expr(pp, e, "&");
}

// Pretty print the expression `e1 | e2`.
void
print_or_expr(pretty_printer& pp, const or_expr& e)
{
  print_infix_expr(pp, e, "|");
}

// Pretty print the expression `e1 ^ e2`.
void
print_xor_expr(pretty_printer& pp, const xor_expr& e)
{
  print_infix_expr(pp, e, "^");
}

// Pretty print the expression `!e`.
void
print_not_expr(pretty_printer& pp, const not_expr& e)
{
  print_prefix_expr(pp, e, "!");
}

// Pretty print the expression `e1 => e2`.
void
print_imp_expr(pretty_printer& pp, const imp_expr& e)
{
  print_infix_expr(pp, e, "=>");
}

// Pretty print the expression `e1 <=> e2`.
void
print_eq_expr(pretty_printer& pp, const eq_expr& e)
{
  print_infix_expr(pp, e, "<=>");
}

// Pretty print the expression `e1 ? e2 : e3`.
void
print_if_expr(pretty_printer& pp, const if_expr& e)
{
  print_grouped_expr(pp, e.get_condition());
  pp.print_space();
  pp.print('?');
  pp.print_space();
  print_grouped_expr(pp, e.get_true_value());
  pp.print_space();
  pp.print(':');
  pp.print_space();
  print_grouped_expr(pp, e.get_false_value());
}

// Pretty print the expression `e1 && e2`.
void
print_and_then_expr(pretty_printer& pp, const and_then_expr& e)
{
  print_infix_expr(pp, e, "&&");
}

// Pretty print the expression `e1 || e2`.
void
print_or_else_expr(pretty_printer& pp, const or_else_expr& e)
{
  print_infix_expr(pp, e, "||");
}

void
print_assert_expr(pretty_printer& pp, const assert_expr& e)
{
  print_builtin_call_expr(pp, "assert", e.get_operand());
}

} // namespace sys_bool
} // namespace beaker


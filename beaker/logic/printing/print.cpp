// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/logic/printing/print.hpp>
#include <beaker/logic/type.hpp>
#include <beaker/logic/expr.hpp>

#include <iostream>


namespace beaker {
namespace logic {

void
print_algo::operator()(std::ostream& os, const type& t) const
{
  assert(is_boolean_type(t));
  os << "bool";
}


// Pretty print the boolean expression e.
static void
print_bool_expr(std::ostream& os, const bool_expr& e)
{
  if (e.get_boolean())
    os << "true";
  else
    os << "false";
}

// Pretty print the expression `e1 && e2`.
static inline void
print_and_expr(std::ostream& os, const and_expr& e)
{
  print_infix_expr(os, e, "&&");
}

// Pretty print the expression `e1 || e2`.
static inline void
print_or_expr(std::ostream& os, const or_expr& e)
{
  print_infix_expr(os, e, "||");
}

// Pretty print the expression `!e`.
static inline void
print_not_expr(std::ostream& os, const not_expr& e)
{
  print_prefix_expr(os, e, "!");
}

// Pretty print the expression `e1 => e2`.
static inline void
print_imp_expr(std::ostream& os, const imp_expr& e)
{
  print_infix_expr(os, e, "=>");
}

// Pretty print the expression `e1 <=> e2`.
static inline void
print_eq_expr(std::ostream& os, const eq_expr& e)
{
  print_infix_expr(os, e, "<=>");
}

void
print_algo::operator()(std::ostream& os, const expr& e) const
{
  switch (e.get_kind()) {
#define def_expr(E) \
    case E ## _expr_kind: \
      return print_ ## E ## _expr(os, cast<E ## _expr>(e));
#include "../expr.def"
    default:
      break;
  }
  assert(false && "not a logic expression");
}

} // namespace logic
} // namespace beaker


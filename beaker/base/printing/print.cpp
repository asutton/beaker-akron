// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "print.hpp"

#include <beaker/base/name.hpp>
#include <beaker/base/type.hpp>
#include <beaker/base/expr.hpp>
#include <beaker/base/decl.hpp>
#include <beaker/base/stmt.hpp>

#include <iostream>


namespace beaker {

void
print(const name& n)
{
  print(std::cout, n);
  std::cout << '\n';
}

void
print(const type& t)
{
  print(std::cout, t);
  std::cout << '\n';
}

void
print(const expr& e)
{
  print(std::cout, e);
  std::cout << '\n';
}

void
print(const decl& d)
{
  print(std::cout, d);
  std::cout << '\n';
}

void
print(const stmt& s)
{
  print(std::cout, s);
  std::cout << '\n';
}

/// Pretty print the name n.
void
print(std::ostream& os, const name& n)
{
  language::get_feature(n).print(os, n);
}

/// Pretty print the type t.
void
print(std::ostream& os, const type& t)
{
  language::get_feature(t).print(os, t);
}

/// Pretty print the expression e.
void
print(std::ostream& os, const expr& e)
{
  language::get_feature(e).print(os, e);
}

/// Pretty print the declaration d.
void
print(std::ostream& os, const decl& d)
{
  language::get_feature(d).print(os, d);
}

/// Pretty print the statement s.
void
print(std::ostream& os, const stmt& s)
{
  language::get_feature(s).print(os, s);
}

// Print an expression with explicit grouping.
void
print_grouped_expr(std::ostream& os, const expr& e)
{
  os << '(';
  print(os, e);
  os << ')';
}

// Pretty print a prefix unary operator.
void
print_prefix_expr(std::ostream& os, const unary_expr& e, const char* op)
{
  os << op;
  print_grouped_expr(os, e.get_operand());
}

// Pretty print an infix binary expression.
void
print_infix_expr(std::ostream& os, const binary_expr& e, const char* op)
{
  print_grouped_expr(os, e.get_lhs());
  os << ' ' << op << ' ';
  print_grouped_expr(os, e.get_rhs());
}


} // namespace beaer
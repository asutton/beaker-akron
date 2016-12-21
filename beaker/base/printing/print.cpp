// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "print.hpp"

#include <beaker/base/name.hpp>
#include <beaker/base/type.hpp>
#include <beaker/base/expr.hpp>
#include <beaker/base/decl.hpp>
#include <beaker/base/stmt.hpp>
#include <beaker/base/module.hpp>

#include <iostream>


namespace beaker {

/// The default behavior is not defined.
void
print_algorithm::operator()(std::ostream& os, const name& n) const
{
  assert(false && "not defined");
}

/// The default behavior is not defined.
void
print_algorithm::operator()(std::ostream& os, const type& t) const
{
  assert(false && "not defined");
}

/// The default behavior is not defined.
void
print_algorithm::operator()(std::ostream& os, const expr& e) const
{
  assert(false && "not defined");
}

/// The default behavior is not defined.
void
print_algorithm::operator()(std::ostream& os, const decl& d) const
{
  assert(false && "not defined");
}

/// The default behavior is not defined.
void
print_algorithm::operator()(std::ostream& os, const stmt& s) const
{
  assert(false && "not defined");
}


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

/// Pretty print the module m.
void
print(const module& m)
{
  print(std::cout, m);
}

// Returns the printing algorithm associated with the node t.
template<typename T>
static inline const print_algorithm&
get_print(const T& t)
{
  feature& feat = language::get_feature(t);
  return feat.template get_algorithm<print_algorithm>();
}

/// Pretty print the name n.
void
print(std::ostream& os, const name& n)
{
  get_print(n)(os, n);
}

/// Pretty print the type t.
void
print(std::ostream& os, const type& t)
{
  get_print(t)(os, t);
}

/// Pretty print the expression e.
void
print(std::ostream& os, const expr& e)
{
  get_print(e)(os, e);
}

/// Pretty print the declaration d.
void
print(std::ostream& os, const decl& d)
{
  get_print(d)(os, d);
}

/// Pretty print the statement s.
void
print(std::ostream& os, const stmt& s)
{
  get_print(s)(os, s);
}

/// Pretty print the module m.
void
print(std::ostream& os, const module& m)
{
  for (const decl& d : m.get_declarations())
    print(os, d);
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
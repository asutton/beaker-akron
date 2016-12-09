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

#define def_lang(l) \
namespace l { \
  extern void print_name(std::ostream&, const name&); \
  extern void print_type(std::ostream&, const type&); \
  extern void print_expr(std::ostream&, const expr&); \
  extern void print_decl(std::ostream&, const decl&); \
  extern void print_stmt(std::ostream&, const stmt&); \
}
#include "../lang.def"

// Pretty print the name n.
void
print(std::ostream& os, const name& n)
{
  switch (n.get_feature()) {
#define def_lang(l) \
    case l ## _lang: \
      return l::print_name(os, n);
#include "../lang.def"
    default:
      break;
  }
  assert(false && "unsupported feature");
}

// Pretty print the type t.
void
print(std::ostream& os, const type& t)
{
  switch (t.get_feature()) {
#define def_lang(l) \
    case l ## _lang: \
      return l::print_type(os, t);
#include "../lang.def"
    default:
      break;
  }
  assert(false && "unsupported feature");
}

void
print(std::ostream& os, const expr& e)
{
  switch (e.get_feature()) {
#define def_lang(l) \
    case l ## _lang: \
      return l::print_expr(os, e);
#include "../lang.def"
    default:
      break;
  }
  assert(false && "unsupported feature");
}

void
print(std::ostream& os, const decl& d)
{
  switch (d.get_feature()) {
#define def_lang(l) \
    case l ## _lang: \
      return l::print_decl(os, d);
#include "../lang.def"
    default:
      break;
  }
  assert(false && "unsupported feature");
}

void
print(std::ostream& os, const stmt& s)
{
  switch (s.get_feature()) {
#define def_lang(l) \
    case l ## _lang: \
      return l::print_stmt(os, s);
#include "../lang.def"
    default:
      break;
  }
  assert(false && "unsupported feature");
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
// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "print.hpp"
#include "../type.hpp"
#include "../expr.hpp"
#include "../decl.hpp"

#include <iostream>


namespace beaker {
namespace sys_var {


// Pretty print a reference type.
static void
print_ref_type(std::ostream& os, const ref_type& t)
{
  print(os, t.get_object_type());
  os << '&';
}

void
print_algo::operator()(std::ostream& os, const type& t) const
{
  assert(is<ref_type>(t));
  return print_ref_type(os, cast<ref_type>(t));
}


static void
print_deref_expr(std::ostream& os, const deref_expr& e)
{
  os << "deref(";
  print(os, e.get_source());
  os << ')';
}

static void
print_nop_init(std::ostream& os, const nop_init& e)
{
  os << "= nop";
}

static void
print_zero_init(std::ostream& os, const zero_init& e)
{
  os << "= zero";
}

static void
print_copy_init(std::ostream& os, const copy_init& e)
{
  os << "= copy ";
  print(os, e.get_expression());
}

static void
print_ref_init(std::ostream& os, const ref_init& e)
{
  os << "= ref ";
  print(os, e.get_expression());
}

// FIXME: This is incomplete.
void
print_algo::operator()(std::ostream& os, const expr& e) const
{
  switch (e.get_kind()) {
    case ref_expr_kind:
      return print(os, cast<ref_expr>(e).get_name());
    case deref_expr_kind:
      return print_deref_expr(os, cast<deref_expr>(e));
    case assign_expr_kind:
      return print_infix_expr(os, cast<assign_expr>(e), "=");

    case nop_init_kind:
      return print_nop_init(os, cast<nop_init>(e));
    case zero_init_kind:
      return print_zero_init(os, cast<zero_init>(e));
    case copy_init_kind:
      return print_copy_init(os, cast<copy_init>(e));
    case ref_init_kind:
      return print_ref_init(os, cast<ref_init>(e));
    default:
      break;
  }
  assert(false && "not a variable expression");
}


// Pretty print a value declaration.
static void
print_var_decl(std::ostream& os, const var_decl& d)
{
  os << "var ";
  print(os, d.get_type());
  os << ' ';
  print(os, d.get_name());
  if (d.has_initializer()) {
    os << ' ';
    print(os, d.get_initializer().template get_as<expr>());
  }
  os << ";\n";
}


// FIXME: Allow language packs to add declarations.
void
print_algo::operator()(std::ostream& os, const decl& d) const
{
  assert(is<var_decl>(d));
  return print_var_decl(os, cast<var_decl>(d));
}

} // namespace sys_var
} // namespace beaker

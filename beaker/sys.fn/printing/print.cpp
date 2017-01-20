// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "print.hpp"
#include "../type.hpp"
#include "../expr.hpp"
#include "../decl.hpp"
#include "../stmt.hpp"

#include <iostream>


namespace beaker {
namespace sys_fn {

// Pretty print a function type.
static void
print_fn_type(std::ostream& os, const fn_type& t)
{
  os << '(';
  const type_seq& parms = t.get_parameter_types();
  for (auto iter = parms.begin(); iter != parms.end(); ++iter) {
    print(os, *iter);
    if (std::next(iter) != parms.end())
      os << ',';
  }
  os << ')';
  os << "->";
  print(os, t.get_return_type());
}

// Pretty print a common type.
void
print_algo::operator()(std::ostream& os, const type& t) const
{
  assert(is<fn_type>(t));
  return print_fn_type(os, cast<fn_type>(t));
}

static void
print_call_expr(std::ostream& os, const call_expr& e)
{
  print(os, e.get_function());
  os << '(';
  const expr_seq& args = e.get_arguments();
  for (auto iter = args.begin(); iter != args.end(); ++iter) {
    print(os, *iter);
    if (std::next(iter) != args.end())
      os << ',';
  }
  os << ')';
}

void
print_algo::operator()(std::ostream& os, const expr& e) const
{
  switch (e.get_kind()) {
    case call_expr_kind:
      return print_call_expr(os, cast<call_expr>(e));
    case eq_expr_kind:
      return print_infix_expr(os, static_cast<const binary_expr&>(e), "==");
    case ne_expr_kind:
      return print_infix_expr(os, static_cast<const binary_expr&>(e), "!=");
    default:
      break;
  }
  assert(false && "not a function expression");
}

// Pretty print a function declaration.
static void
print_fn_decl(std::ostream& os, const fn_decl& d)
{
  os << "fn" << ' ';
  print(os, d.get_name());
  os << '(';
  decl_seq const& parms = d.get_parameters();
  for (auto iter = parms.begin(); iter != parms.end(); ++iter) {
    print(os, *iter);
    if (std::next(iter) != parms.end())
      os << ',';
  }
  os << ')';
  os <<  " -> ";
  print(os, d.get_return());

  if (d.has_definition()) {
    os << ' ';
    print(os, d.get_definition());
  }
  os << '\n';
}

void
print_parm_decl(std::ostream& os, const parm_decl& d)
{
  print(os, d.get_type());
  os << ' ';
  print(os, d.get_name());
}

void
print_algo::operator()(std::ostream& os, const decl& d) const
{
  switch (d.get_kind()) {
    case fn_decl_kind:
      return print_fn_decl(os, cast<fn_decl>(d));
    case parm_decl_kind:
      return print_parm_decl(os, cast<parm_decl>(d));
    default:
      break;
  }
  assert(false && "not a function declaration");
}


static void
print_block_stmt(std::ostream& os, const block_stmt& s)
{
  os << "{\n";
  for (const stmt& s1 : s.get_statements())
    print(os, s1);
  os << "}\n";
}

static void
print_decl_stmt(std::ostream& os, const decl_stmt& s)
{
  print(os, s.get_declaration());
}

static void
print_expr_stmt(std::ostream& os, const expr_stmt& s)
{
  print(os, s.get_expression());
  os << ";\n";
}

static void
print_ret_stmt(std::ostream& os, const ret_stmt& s)
{
  os << "return ";
  print(os, s.get_return());
  os << ";\n";
}

void
print_algo::operator()(std::ostream& os, const stmt& s) const
{
  switch (s.get_kind()) {
    case block_stmt_kind:
      return print_block_stmt(os, cast<block_stmt>(s));
    case decl_stmt_kind:
      return print_decl_stmt(os, cast<decl_stmt>(s));
    case expr_stmt_kind:
      return print_expr_stmt(os, cast<expr_stmt>(s));
    case ret_stmt_kind:
      return print_ret_stmt(os, cast<ret_stmt>(s));
    default:
      break;
  }
  assert(false && "not a function statement");
}

} // namespace sys_fn
} // namespace beaker

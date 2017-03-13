// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "print.hpp"
#include "../type.hpp"
#include "../expr.hpp"
#include "../decl.hpp"
#include "../stmt.hpp"


namespace beaker {

/// Pretty print a function type.
void
print_type(pretty_printer& pp, const sys_fn::fn_type& t)
{
  pp.print('(');
  const type_seq& parms = t.get_parameter_types();
  for (auto iter = parms.begin(); iter != parms.end(); ++iter) {
    print(pp, *iter);
    if (std::next(iter) != parms.end())
      pp.print(',');
  }
  pp.print(')');
  pp.print("->");
  print(pp, t.get_return_type());
}

/// Pretty print a call expression.
void
print_expr(pretty_printer& pp, const sys_fn::call_expr& e)
{
  print(pp, e.get_function());
  pp.print('(');
  const expr_seq& args = e.get_arguments();
  for (auto iter = args.begin(); iter != args.end(); ++iter) {
    print(pp, *iter);
    if (std::next(iter) != args.end())
      pp.print(',');
  }
  pp.print(')');
}

void
print_expr(pretty_printer& pp, const sys_fn::eq_expr& e)
{
  print_infix_expr(pp, e, "==");
}

void
print_expr(pretty_printer& pp, const sys_fn::ne_expr& e)
{
  print_infix_expr(pp, e, "!=");
}

void
print_decl(pretty_printer& pp, const sys_fn::fn_decl& d)
{
  pp.print("def");
  pp.print_space();
  print(pp, d.get_name());
  pp.print('(');
  decl_seq const& parms = d.get_parameters();
  for (auto iter = parms.begin(); iter != parms.end(); ++iter) {
    print(pp, *iter);
    if (std::next(iter) != parms.end())
      pp.print(',');
  }
  pp.print(')');
  print_binary_op(pp, "->");
  print(pp, d.get_return());

  if (d.has_definition()) {
    pp.print_space();
    print(pp, d.get_definition());
  } 
  else {
    pp.print(';');
  }
  pp.print_newline();
}

void
print_decl(pretty_printer& pp, const sys_fn::parm_decl& d)
{
  print(pp, d.get_type());
  pp.print_space();
  print(pp, d.get_name());
}

void
print_decl(pretty_printer& pp, const sys_fn::var_decl& d)
{
  pp.print("var");
  pp.print_space();
  print(pp, d.get_type());
  pp.print_space();
  print(pp, d.get_name());
  print_binary_op(pp, "=");
  print(pp, d.get_initializer());
  pp.print(';');
}

// FIXME: Manage indentations.
void
print_stmt(pretty_printer& pp, const sys_fn::block_stmt& s)
{
  pp.print('{');
  pp.indent();
  pp.print_newline();
  const stmt_seq& ss = s.get_statements();
  for (auto iter = ss.begin(); iter != ss.end(); ++iter) {
    print(pp, *iter);
    if (std::next(iter) == ss.end())
      pp.undent();
    pp.print_newline();

  }
  pp.print('}');
}

void
print_stmt(pretty_printer& pp, const sys_fn::decl_stmt& s)
{
  print(pp, s.get_declaration());
}

void
print_stmt(pretty_printer& pp, const sys_fn::expr_stmt& s)
{
  print(pp, s.get_expression());
  pp.print(';');
}

void
print_stmt(pretty_printer& pp, const sys_fn::ret_stmt& s)
{
  pp.print("return");
  pp.print_space();
  print(pp, s.get_return());
  pp.print(';');
}

} // namespace beaker

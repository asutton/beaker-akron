// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "print.hpp"
#include "../name.hpp"
#include "../type.hpp"
#include "../expr.hpp"
#include "../decl.hpp"
#include "../stmt.hpp"

#include <iostream>


namespace beaker {

// -------------------------------------------------------------------------- //
// Pretty printer

pretty_printer::pretty_printer(const language& lang)
  : pretty_printer(lang, std::cout)
{ }

pretty_printer::pretty_printer(const language& lang, std::ostream& os)
  : lang(lang), os(os)
{ }

/// Print a single character.
void
pretty_printer::print(char c)
{
  os << c;
}

/// Print a string.
void
pretty_printer::print(const char* str)
{
  os << str;
}

/// Print an integer value
void
pretty_printer::print(int n)
{
  os << n;
}

/// Prints a newline character.
void
pretty_printer::print_newline()
{
  os << '\n';
}

void
pretty_printer::print_space()
{
  os << ' ';
}

// -------------------------------------------------------------------------- //
// Disaptch

// Returns the equality algorithm associated with the node t.
static inline const print_algorithm&
get_algorithm(const language& lang)
{
  return lang.get_algorithm<print_algorithm>();
}

// Generates the dispatch table for the algorithm
print_algorithm::print_algorithm(language& lang)
  : names(new name_table(lang.get_names())),
    types(new type_table(lang.get_types())),
    exprs(new expr_table(lang.get_expressions())),
    decls(new decl_table(lang.get_declarations())),
    stmts(new stmt_table(lang.get_statements()))
{ }

/// Pretty print the name `n`.
void
print(pretty_printer& pp, const name& n)
{
  const auto& tab = *get_algorithm(pp.lang).names;
  auto fn = tab.get_overrider(n);
  fn(pp, n);
}

/// Pretty print the type `t`.
void
print(pretty_printer& pp, const type& t)
{
  const auto& tab = *get_algorithm(pp.lang).types;
  auto fn = tab.get_overrider(t);
  fn(pp, t);
}

/// Pretty print the expression `e`.
void
print(pretty_printer& pp, const expr& e)
{
  const auto& tab = *get_algorithm(pp.lang).exprs;
  auto fn = tab.get_overrider(e);
  fn(pp, e);
}

/// Pretty print the declaration `d`.
void
print(pretty_printer& pp, const decl& d)
{
  const auto& tab = *get_algorithm(pp.lang).decls;
  auto fn = tab.get_overrider(d);
  fn(pp, d);
}

/// Pretty print the statement `s`.
void
print(pretty_printer& pp, const stmt& s)
{
  const auto& tab = *get_algorithm(pp.lang).stmts;
  auto fn = tab.get_overrider(s);
  fn(pp, s);
}

/// Print an expression enclosed by the given characters.
void
print_enclosed_expr(pretty_printer& pp, const expr& e, char l, char r)
{
  pp.print(l);
  print(pp, e);
  pp.print(r);
}

/// Print an expression that resembles a call to a builtin function.
void
print_builtin_call_expr(pretty_printer& pp, const char* fn, const expr& e)
{
  pp.print(fn);
  print_enclosed_expr(pp, e, '(', ')');
}

/// Print an expression enclosed by parentheses.
void
print_grouped_expr(pretty_printer& pp, const expr& e)
{
  if (is_terminal_expression(e))
    print(pp, e);
  else
    print_enclosed_expr(pp, e, '(', ')');
}

/// Pretty print a prefix unary operator spelled by `op`. The operand is
/// printed with enclosed parentheses.
void
print_prefix_expr(pretty_printer& pp, const unary_expr& e, const char* op)
{
  pp.print(op);
  print_grouped_expr(pp, e.get_operand());
}

// Pretty print an infix binary expression.
void
print_infix_expr(pretty_printer& pp, const binary_expr& e, const char* op)
{
  print_grouped_expr(pp, e.get_lhs());
  pp.print_space();
  pp.print(op);
  pp.print_space();
  print_grouped_expr(pp, e.get_rhs());
}


// -------------------------------------------------------------------------- //
// Additional functions

void
print(const language& lang, const name& n)
{
  pretty_printer pp(lang, std::cout);
  print(pp, n);
  pp.print_newline();
}

void
print(const language& lang, const type& t)
{
  pretty_printer pp(lang, std::cout);
  print(pp, t);
  pp.print_newline();
}

void
print(const language& lang, const expr& e)
{
  pretty_printer pp(lang, std::cout);
  print(pp, e);
  pp.print_newline();
}

void
print(const language& lang, const decl& d)
{
  pretty_printer pp(lang, std::cout);
  print(pp, d);
  pp.print_newline();
}

void
print(const language& lang, const stmt& s)
{
  pretty_printer pp(lang, std::cout);
  print(pp, s);
  pp.print_newline();
}


} // namespace beaer
// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "print.hpp"

#include <beaker/all/ast.hpp>

#include <beaker/util/symbol.hpp>

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

/// Print an integer value.
void
pretty_printer::print(std::intmax_t n)
{
  os << n;
}

/// Print an unsigned integer value.
void
pretty_printer::print(std::uintmax_t n)
{
  os << n;
}

/// Prints a newline character.
void
pretty_printer::print_newline()
{
  os << '\n';
}

/// Print a single space character.
void
pretty_printer::print_space()
{
  os << ' ';
}


// -------------------------------------------------------------------------- //
// Primary interface

/// Print the text of a symbol.
void
print(pretty_printer& pp, const symbol& sym)
{
  pp.print(sym.c_str());
}


// -------------------------------------------------------------------------- //
// Print helpers

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
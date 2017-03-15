// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_PRINTING_PRINT_HPP
#define BEAKER_BASE_PRINTING_PRINT_HPP

#include <beaker/base/lang.hpp>
#include <beaker/base/seq.hpp>

#include <cstdint>
#include <iosfwd>


namespace beaker {

struct symbol;


/// The pretty printer object provides context for printing terms of the
/// languages. This wraps an output stream and helps manages indentation and
/// other printing options.
///
/// \todo: Should we allow the language object or not? Is syntax always
/// self-describing (probably not).
struct pretty_printer
{
  pretty_printer();
  pretty_printer(std::ostream&);

  pretty_printer(const language&);
  pretty_printer(const language&, std::ostream&);

  const language& get_language() const;

  void print(char);
  void print(const char*);
  void print(std::intmax_t);
  void print(std::uintmax_t);

  void print_space();
  void print_newline();

  void indent() { ++depth; }
  void undent() { --depth; }

  const language* lang;
  std::ostream& os;
  int depth;
};

/// Returns the language used for printing.
inline const language& pretty_printer::get_language() const { return *lang; }


void print(pretty_printer&, const symbol&);
void print(pretty_printer&, const name&);
void print(pretty_printer&, const type&);
void print(pretty_printer&, const expr&);
void print(pretty_printer&, const decl&);
void print(pretty_printer&, const stmt&);

void print(const language&, const name&);
void print(const language&, const type&);
void print(const language&, const expr&);
void print(const language&, const decl&);
void print(const language&, const stmt&);


// -------------------------------------------------------------------------- //
// Dispatch interface

void print_name(pretty_printer&, const name&) = delete;
void print_type(pretty_printer&, const type&) = delete;
void print_expr(pretty_printer&, const expr&) = delete;
void print_decl(pretty_printer&, const decl&) = delete;
void print_stmt(pretty_printer&, const stmt&) = delete;

void print_decl(pretty_printer&, const module&);


// -------------------------------------------------------------------------- //
// Helper functions

struct unary_expr;
struct binary_expr;

/// Prints a binary operator offset by spaces.
///
/// \todo Make the spaces a configuration option for the printer.
inline void 
print_binary_op(pretty_printer& pp, char op)
{
  pp.print_space();
  pp.print(op);
  pp.print_space();
}

/// Prints a binary operator offset by spaces.
///
/// \todo Make the spaces a configuration option for the printer.
inline void 
print_binary_op(pretty_printer& pp, const char* op)
{
  pp.print_space();
  pp.print(op);
  pp.print_space();
}

void print_enclosed_expr(pretty_printer&, const expr&, char, char);
void print_builtin_call_expr(pretty_printer&, const char*, const expr&);
void print_grouped_expr(pretty_printer&, const expr&);
void print_prefix_expr(pretty_printer&, const unary_expr&, const char*);
void print_suffix_expr(pretty_printer&, const unary_expr&, const char*);
void print_infix_expr(pretty_printer&, const binary_expr&, const char*);

template<typename T>
void 
print_comma_separated(pretty_printer& pp, const seq<T>& s)
{
  for (auto iter = s.begin(); iter != s.end(); ++iter) {
    print(pp, *iter);
    if (std::next(iter) != s.end()) {
      pp.print(',');
      pp.print_space();
    }
  }
}


// -------------------------------------------------------------------------- //
// Iostream integration

template<typename T>
struct pretty_print_term
{
  const language* lang;
  const T& term;
};

template<typename T>
inline auto pretty(const T& t) { return pretty_print_term<T>{nullptr, t}; }

template<typename T>
inline auto pretty(const language& l, const T& t) { return pretty_print_term<T>{&l, t}; }

template<typename C, typename T, typename U>
std::basic_ostream<C, T>&
operator<<(std::basic_ostream<C, T>& os, pretty_print_term<U> t)
{
  if (t.lang) {
    pretty_printer pp(*t.lang, os);
    print(pp, t.term);
  }
  else {
    pretty_printer pp(os);
    print(pp, t.term);
  }
  return os;
}

} // namespace beaker


#endif

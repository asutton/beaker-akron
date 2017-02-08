// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_PRINTING_PRINT_HPP
#define BEAKER_BASE_PRINTING_PRINT_HPP

#include <beaker/base/lang.hpp>
#include <beaker/base/seq.hpp>

#include <iosfwd>


namespace beaker {

struct name;
struct type;
struct expr;
struct unary_expr;
struct binary_expr;
struct decl;
struct stmt;

/// The pretty printer object provides context for printing terms of the
/// languages. This wraps an output stream and helps manages indentation and
/// other printing options.
///
/// \todo Implement indentation.
struct pretty_printer
{
  pretty_printer(const language&);
  pretty_printer(const language&, std::ostream&);

  void print(char);
  void print(const char*);
  void print(int);

  void print_space();
  void print_newline();

  const language& lang;
  std::ostream& os;
};

/// The print algorithm is use to pretty print an unambiguous internal 
/// representation of a program.
///
/// \todo Stop using an ostream because it doesn't inherently support pretty
/// printing (although we could almost certainly abuse it to do so).
struct print_algorithm : algorithm
{
  using name_table = dispatch_table<void(pretty_printer&, const name&)>;
  using type_table = dispatch_table<void(pretty_printer&, const type&)>;
  using expr_table = dispatch_table<void(pretty_printer&, const expr&)>;
  using decl_table = dispatch_table<void(pretty_printer&, const decl&)>;
  using stmt_table = dispatch_table<void(pretty_printer&, const stmt&)>;

  print_algorithm(language&);
  
  std::unique_ptr<name_table> names;
  std::unique_ptr<type_table> types;
  std::unique_ptr<expr_table> exprs;
  std::unique_ptr<decl_table> decls;
  std::unique_ptr<stmt_table> stmts;
};

void print(pretty_printer&, const name&);
void print(pretty_printer&, const type&);
void print(pretty_printer&, const expr&);
void print(pretty_printer&, const decl&);
void print(pretty_printer&, const stmt&);

// -------------------------------------------------------------------------- //
// Helper functions

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
// Additional functions

void print(const language&, const name&);
void print(const language&, const type&);
void print(const language&, const expr&);
void print(const language&, const decl&);
void print(const language&, const stmt&);

} // namespace beaker


#endif

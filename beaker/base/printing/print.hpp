// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_PRINTING_PRINT_HPP
#define BEAKER_BASE_PRINTING_PRINT_HPP

#include <beaker/base/lang.hpp>

#include <iosfwd>


namespace beaker {

struct name;
struct type;
struct expr;
struct unary_expr;
struct binary_expr;
struct decl;
struct stmt;

/// Defines the dispatch signature for hash algorithms.
///
/// \todo Stop using an ostream because it doesn't inherently support pretty
/// printing (although we could almost certainly abuse it to do so).
struct print_algorithm : algorithm
{
  struct tag { };

  virtual void operator()(std::ostream&, const name&) const;
  virtual void operator()(std::ostream&, const type&) const;
  virtual void operator()(std::ostream&, const expr&) const;
  virtual void operator()(std::ostream&, const decl&) const;
  virtual void operator()(std::ostream&, const stmt&) const;
};

void print(const name&);
void print(const type&);
void print(const expr&);
void print(const decl&);
void print(const stmt&);
void print(const module&);

void print(std::ostream&, const name&);
void print(std::ostream&, const type&);
void print(std::ostream&, const expr&);
void print(std::ostream&, const decl&);
void print(std::ostream&, const stmt&);
void print(std::ostream&, const module&);

void print_grouped_expr(std::ostream&, const expr&);
void print_prefix_expr(std::ostream&, const unary_expr&, const char*);
void print_suffix_expr(std::ostream&, const unary_expr&, const char*);
void print_infix_expr(std::ostream&, const binary_expr&, const char*);

} // namespace beaker


#endif

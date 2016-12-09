// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_PRINTING_PRINT_HPP
#define BEAKER_BASE_PRINTING_PRINT_HPP

#include <iosfwd>


namespace beaker {

struct name;
struct type;
struct expr;
struct unary_expr;
struct binary_expr;
struct decl;
struct stmt;

// FIXME: Stop using an ostream. Support structured (and colorized!) printing.

void print(const name&);
void print(const type&);
void print(const expr&);
void print(const decl&);
void print(const stmt&);

void print(std::ostream&, const name&);
void print(std::ostream&, const type&);
void print(std::ostream&, const expr&);
void print(std::ostream&, const decl&);
void print(std::ostream&, const stmt&);

void print_grouped_expr(std::ostream&, const expr&);
void print_prefix_expr(std::ostream&, const unary_expr&, const char*);
void print_suffix_expr(std::ostream&, const unary_expr&, const char*);
void print_infix_expr(std::ostream&, const binary_expr&, const char*);

} // namespace beaker


#endif

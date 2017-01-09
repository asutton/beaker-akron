// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_BOOL_CONSTRUCTION_BUILDER_HPP
#define BEAKER_SYS_BOOL_CONSTRUCTION_BUILDER_HPP

#include <beaker/base/construction/builder.hpp>


namespace beaker {
namespace sys_bool {

struct bool_type;
struct bool_expr;
struct and_expr;
struct or_expr;
struct not_expr;
struct imp_expr;
struct eq_expr;
struct if_expr;
struct and_then_expr;
struct or_else_expr;

struct assert_decl;

/// Provides access to resources needed to construct, validate, and
/// evaluate logical terms.
struct builder : basic_builder<sys_bool_lang>
{
  builder(module&);

  // Canonical types
  bool_type& get_bool_type();

  // Expressions
  bool_expr& make_true_expr();
  bool_expr& make_false_expr();
  bool_expr& make_bool_expr(const value&);
  and_expr& make_and_expr(expr&, expr&);
  or_expr& make_or_expr(expr&, expr&);
  not_expr& make_not_expr(expr&);
  imp_expr& make_imp_expr(expr&, expr&);
  eq_expr& make_eq_expr(expr&, expr&);
  if_expr& make_if_expr(expr&, expr&, expr&);
  and_then_expr& make_and_then_expr(expr&, expr&);
  or_else_expr& make_or_else_expr(expr&, expr&);

  // Declarations
  assert_decl& make_assert_decl(expr&);

  module* mod_;
  singleton_set<bool_type>* bool_;
};

} // namespace sys_bool
} // namespace beaker


#endif

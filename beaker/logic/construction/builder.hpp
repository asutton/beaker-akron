// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_LOGIC_CONSTRUCTION_BUILDER_HPP
#define BEAKER_LOGIC_CONSTRUCTION_BUILDER_HPP

#include <beaker/base/construction/builder.hpp>


namespace beaker {

struct module;
struct value;

struct type;
struct expr;

namespace logic {

struct bool_type;
struct bool_expr;
struct and_expr;
struct or_expr;
struct not_expr;
struct imp_expr;
struct eq_expr;

struct assert_decl;

/// Provides access to resources needed to construct, validate, and
/// evaluate logical terms.
struct builder : basic_builder<logic_lang>
{
  builder(module&);

  // Canonical types
  bool_type& get_bool_type();

  // General types.
  bool_type& make_bool_type();

  // Canonical values
  bool_expr& get_true_expr();
  bool_expr& get_false_expr();
  bool_expr& get_bool_expr(const value&);

  // Expressions
  bool_expr& make_true_expr();
  bool_expr& make_false_expr();
  bool_expr& make_bool_expr(const value&);
  bool_expr& make_bool_expr(value&&);
  and_expr& make_and_expr(expr&, expr&);
  or_expr& make_or_expr(expr&, expr&);
  not_expr& make_not_expr(expr&);
  imp_expr& make_imp_expr(expr&, expr&);
  eq_expr& make_eq_expr(expr&, expr&);

  // Declarations
  assert_decl& make_assert_decl(expr&);

  module* mod_;
  bool_type* bool_;
  bool_expr* true_;
  bool_expr* false_;
};

/// Returns the bool type.
inline bool_type& builder::get_bool_type() { return *bool_; }

/// Returns the literal `true`.
///
/// The type of the expression is `bool`.
inline bool_expr& builder::get_true_expr() { return *true_; }

/// Returns the literal `false`.
///
/// The type of the expression is `bool`.
inline bool_expr& builder::get_false_expr() { return *false_; }

} // namespace logic
} // namespace beaker


#endif

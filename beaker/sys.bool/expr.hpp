// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_BOOL_EXPR_HPP
#define BEAKER_SYS_BOOL_EXPR_HPP

#include <beaker/base/expr.hpp>


namespace beaker {
namespace sys_bool {

enum {
  first_expr_kind = sys_bool_lang_block,
#define def_expr(NS, E, B) E ## _expr_kind,
#include "expr.def"
  last_expr_kind
};


// -------------------------------------------------------------------------- //
// Expressions

/// Represents the boolean literals `true` and `false`.
///
/// The type of boolean literals shall be `bool`.
struct bool_expr : literal_expr_impl<bool_expr_kind>
{
  using literal_expr_impl<bool_expr_kind>::literal_expr_impl;

  bool get_boolean() const;
};

/// Returns the boolean value of the expression.
inline bool bool_expr::get_boolean() const { return get_value().get_int(); }


/// Represents the expression `e1 & e2`. 
///
/// The operands `e1` and `e2` shall have type `bool`. The result type of the
/// expression shall be `bool`. 
///
/// The value of the expression is `true` if and only if both `e1` and `e2` are 
/// true.
struct and_expr : binary_expr_impl<and_expr_kind> 
{
  using binary_expr_impl<and_expr_kind>::binary_expr_impl;
};


/// Represents the boolean inclusive-or expression `e1 | e2`. 
///
/// The operands `e1` and `e2` shall have type `bool` . The result type of the
/// expression shall be `bool`.
///
/// The value of the expression is `true` if `e1` is `true` `e2` is `true`, or 
/// both are `true`.
struct or_expr : binary_expr_impl<or_expr_kind> 
{
  using binary_expr_impl<or_expr_kind>::binary_expr_impl;
};


/// Represents the boolean exclusive-or expression `e1 ^ e2`. The operands shall
/// be boolean expressions. The value of the expression is `false` when the
/// operands have the same values and `true` otherwise.
struct xor_expr : binary_expr_impl<xor_expr_kind> 
{
  using binary_expr_impl<xor_expr_kind>::binary_expr_impl;
};


/// Represents the expression `!e`. 
///
/// The operand `e` shall have type `bool`. The result type of the expression
/// shall be `bool.
///
/// The value of the expression is `true` if `e` is `false` and `false` 
/// otherwise.
struct not_expr : unary_expr_impl<not_expr_kind> 
{
  using unary_expr_impl<not_expr_kind>::unary_expr_impl;
};


/// Represents the expression `e1 => e2`. 
///
/// The operands `e1` and `e2` shall have type `bool`. The type of a the
/// expression is `bool`.
///
/// The value of the expression is `true` when `e1` and `e2` are `true` or
/// when `e1` is `false`. Otherwise the value is `false`.
struct imp_expr : binary_expr_impl<imp_expr_kind> 
{
  using binary_expr_impl<imp_expr_kind>::binary_expr_impl;
};


/// Represents the expression `e1 <=> e2`.
///
/// The operands `e1` and `e2` shall have type `bool`. The type of a the
/// expression is `bool`.
///
/// The value of the expression is `true` only when `e1 and e2` are equal
/// and `false` otherwise.
struct eq_expr : binary_expr_impl<eq_expr_kind> 
{
  using binary_expr_impl<eq_expr_kind>::binary_expr_impl;
};


/// Represents the conditional expression `e1 ? e2 : e3`.
///
/// The operand `e1` shall be a boolean expression, and the types of `e2` and
/// `e3` shall be the same.
///
/// The value of the expression...
struct if_expr : ternary_expr_impl<if_expr_kind> 
{
  using ternary_expr_impl<if_expr_kind>::ternary_expr_impl;

  const expr& get_condition() const;
  expr& get_condition();

  const expr& get_true_value() const;
  expr& get_true_value();

  const expr& get_false_value() const;
  expr& get_false_value();
};

/// Returns the condition of of the expression.
inline const expr& if_expr::get_condition() const { return get_first(); }

/// Returns the condition of of the expression.
inline expr& if_expr::get_condition() { return get_first(); }

/// Returns the expression to evaluate when the condition is true.
inline const expr& if_expr::get_true_value() const { return get_second(); }

/// Returns the expression to evaluate when the condition is true.
inline expr& if_expr::get_true_value() { return get_second(); }

/// Returns the expression to evaluate when the condition is false.
inline const expr& if_expr::get_false_value() const { return get_third(); }

/// Returns the expression to evaluate when the condition is false.
inline expr& if_expr::get_false_value() { return get_third(); }


/// Represents the expression `e1 && e2`. Both `e1` and `e2` shall be boolean 
/// expressions. The expression `e1 && e2` is equivalent to `if e1 then e2 else 
/// false`.
struct and_then_expr : binary_expr_impl<and_then_expr_kind> 
{
  using binary_expr_impl<and_then_expr_kind>::binary_expr_impl;
};


/// Represents the expression `e1 || e2`. Both `e1` and `e2` shall be boolean 
/// expressions. The expression `e1 || e2` is equivalent to `if e1 then true 
/// else e2`.
struct or_else_expr : binary_expr_impl<or_else_expr_kind> 
{
  using binary_expr_impl<or_else_expr_kind>::binary_expr_impl;
};


/// Represents the expressoin `assert(e)`. The expression `e` shall have type
/// bool. If the value of `e` is `false`, the behavior of the program is 
/// undefined. The value of the expression is `true` (i.e., it has type 
/// `bool`).
struct assert_expr : unary_expr_impl<assert_expr_kind>
{
  using unary_expr_impl<assert_expr_kind>::unary_expr_impl;
};


// -------------------------------------------------------------------------- //
// Operations

bool is_boolean_expression(const expr&);

} // namespace sys_bool
} // namespace beaker


#endif

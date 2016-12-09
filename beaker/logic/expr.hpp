// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_LOGIC_EXPR_HPP
#define BEAKER_LOGIC_EXPR_HPP

#include <beaker/base/expr.hpp>


namespace beaker {
namespace logic {


enum {
  first_expr_kind = logic_lang_block,
#define def_expr(e) e ## _expr_kind,
#include "expr.def"
  last_expr_kind
};


/// Represents the boolean literals `true` and `false`.
///
/// The type of boolean literals shall be `bool`.
struct bool_expr : generic_literal_expr<bool_expr_kind>
{
  using generic_literal_expr<bool_expr_kind>::generic_literal_expr;

  bool get_boolean() const;
};

/// Returns the boolean value of the expression.
inline bool bool_expr::get_boolean() const { return get_value().get_integer(); }


/// Represents the expression `e1 && e2`. 
///
/// The operands `e1` and `e2` shall have type `bool`. The result type of the
/// expression shall be `bool`. 
///
/// The value of the expression is `true` if and only if both `e1` and `e2` are 
/// true. If `e1` is false, `e2` is not evaluated.
struct and_expr : generic_binary_expr<and_expr_kind> 
{
  using generic_binary_expr<and_expr_kind>::generic_binary_expr;
};


/// Represents the expression `e1 || e2`. 
///
/// The operands `e1` and `e2` shall have type `bool` . The result type of the
/// expression shall be `bool`.
///
/// The value of the expression is `true` if `e1` is `true` `e2` is `true`, or 
/// both are `true`. If `e1` is true, then `e2` is not evaluated.
struct or_expr : generic_binary_expr<or_expr_kind> 
{
  using generic_binary_expr<or_expr_kind>::generic_binary_expr;
};


/// Represents the expression `!e`. 
///
/// The operand `e` shall have type `bool`. The result type of the expression
/// shall be `bool.
///
/// The value of the expression is `true` if `e` is `false` and `false` 
/// otherwise.
struct not_expr : generic_unary_expr<not_expr_kind> 
{
  using generic_unary_expr<not_expr_kind>::generic_unary_expr;
};


/// Represents the expression `e1 => e2`. 
///
/// The operands `e1` and `e2` shall have type `bool`. The type of a the
/// expression is `bool`.
///
/// The value of the expression is `true` when `e1` and `e2` are `true` or
/// when `e1` is `false`. Otherwise the value is `false`.
struct imp_expr : generic_binary_expr<imp_expr_kind> 
{
  using generic_binary_expr<imp_expr_kind>::generic_binary_expr;
};


/// Represents the expression `e1 <=> e2`.
///
/// The operands `e1` and `e2` shall have type `bool`. The type of a the
/// expression is `bool`.
///
/// The value of the expression is `true` only when `e1 and e2` are equal
/// and `false` otherwise.
struct eq_expr : generic_binary_expr<eq_expr_kind> 
{
  using generic_binary_expr<eq_expr_kind>::generic_binary_expr;
};

// Operations

bool is_boolean_expression(const expr&);

} // namespace logic
} // namespace beaker


#endif

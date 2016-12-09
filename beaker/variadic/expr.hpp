// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_VARIADIC_EXPR_HPP
#define BEAKER_VARIADIC_EXPR_HPP

#include <beaker/base/expr.hpp>


namespace beaker {
namespace variadic {

enum 
{
  first_expr_kind = sample_lang_block,
#define def_expr(e) e ## _expr_kind,
#include "expr.def"
  last_expr_kind
};


/// Represents the expression `va_start(list)`.
///
/// The operand shall be a reference to a variadic list type. The result of
/// the expression is a reference to the operand.
struct start_expr : generic_unary_expr<start_expr_kind>
{
  using generic_unary_expr<start_expr_kind>::generic_unary_expr;

  const expr& get_list() const;
  expr& get_list();
};


/// Represents the expression `va_end(list)`.
///
/// The operand shall be a reference to a variadic list type. The result of
/// the expression is a reference to the operand.
struct end_expr : generic_unary_expr<end_expr_kind>
{
  using generic_unary_expr<end_expr_kind>::generic_unary_expr;

  const expr& get_list() const;
  expr& get_list();
};


/// Represents the expression `va_arg(list, t)`.
///
/// The type `t` is the type of the expression.
struct arg_expr : generic_unary_expr<arg_expr_kind>
{
  using generic_unary_expr<arg_expr_kind>::generic_unary_expr;

  const expr& get_list() const;
  expr& get_list();
};


/// Represents the expression `va_copy(dst, src)`.
///
///
/// The type of the expression denotes the type of object accessed. Both
/// operands shall be references to variadic argument lists. The result of 
/// the expression is a reference to the destination list.
struct copy_expr : generic_binary_expr<copy_expr_kind>
{
  using generic_binary_expr<copy_expr_kind>::generic_binary_expr;

  const expr& get_source() const;
  expr& get_source();

  const expr& get_destination() const;
  expr& get_destination();
};


/// Returns the variadic argument list.
inline const expr& start_expr::get_list() const { return get_operand(); }
  
/// Returns the variadic argument list.
inline expr& start_expr::get_list() { return get_operand(); }


/// Returns the variadic argument list.
inline const expr& end_expr::get_list() const { return get_operand(); }
  
/// Returns the variadic argument list.
inline expr& end_expr::get_list() { return get_operand(); }


/// Returns the variadic argument list.
inline const expr& arg_expr::get_list() const { return get_operand(); }
  
/// Returns the variadic argument list.
inline expr& arg_expr::get_list() { return get_operand(); }


/// Returns the variadic argument list.
inline const expr& copy_expr::get_source() const { return get_rhs(); }
  
/// Returns the variadic argument list.
inline expr& copy_expr::get_source() { return get_rhs(); }

/// Returns the variadic argument list.
inline const expr& copy_expr::get_destination() const { return get_lhs(); }
  
/// Returns the variadic argument list.
inline expr& copy_expr::get_destination() { return get_lhs(); }

} // namespace variadic
} // namespace beaker


#endif

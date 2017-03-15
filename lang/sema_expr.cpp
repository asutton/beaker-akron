// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

namespace bpl {

/// A helper function that assigns source locations of unary expressions
/// to an expression. This constructs the layout [tok, e1].
static inline expr&
set_locations(expr& r, token tok, expr& e1)
{
  r.set_fixed_layout(tok.get_location(), e1.get_location());
  return r;
}

/// A helper function that assigns source locations of binary expressions
/// to an expression. This constructs the layout [e1, tok, e2].
static inline expr&
set_locations(expr& r, expr& e1, token tok, expr& e2)
{
  r.set_fixed_layout(e1.get_location(), tok.get_location(), e2.get_location());
  return r;
}

// -------------------------------------------------------------------------- //
// Semantics of the conditional expression.

/// Process a new condition.
expr&
semantics::on_condition(expr& e1, token q, expr& e2, token c, expr& e3)
{
  check_bool(e1);
  check_same(e2, e3);
  expr& ret = build_bool.make_if_expr(e1, e2, e3);
  ret.set_fixed_layout(
    e1.get_location(), 
    q.get_location(), 
    e2.get_location(), 
    c.get_location(), 
    e3.get_location()
  );
  return ret;
}

// -------------------------------------------------------------------------- //
// Semantics of logical expressions

/// Process the expression `e1 || e2`.
expr&
semantics::on_logical_or(expr& e1, token tok, expr& e2)
{
  check_bool(e1, e2);
  expr& ret = build_bool.make_or_else_expr(e1, e2);
  return set_locations(ret, e1, tok, e2);;
}

/// Process the expression `e1 && e2`.
expr&
semantics::on_logical_and(expr& e1, token tok, expr& e2)
{
  check_bool(e1, e2);
  expr& ret = build_bool.make_and_then_expr(e1, e2);
  return set_locations(ret, e1, tok, e2);;
}

// -------------------------------------------------------------------------- //
// Semantics of bitwise expressions

/// A subroutine of on_bitwise_or.
static inline expr&
make_bool_or(semantics& sema, expr& e1, token tok, expr& e2)
{
  expr& ret = sema.build_bool.make_or_expr(e1, e2);
  return set_locations(ret, e1, tok, e2);;
}

/// A subroutine of on_bitwise_xor.
static inline expr&
make_bool_xor(semantics& sema, expr& e1, token tok, expr& e2)
{
  expr& ret = sema.build_bool.make_xor_expr(e1, e2);
  return set_locations(ret, e1, tok, e2);
}

/// A subroutine of on_bitwise_and.
static inline expr&
make_bool_and(semantics& sema, expr& e1, token tok, expr& e2)
{
  expr& ret = sema.build_bool.make_and_expr(e1, e2);
  return set_locations(ret, e1, tok, e2);
}

/// Process the expression `e1 | e2`.
expr&
semantics::on_bitwise_or(expr& e1, token tok, expr& e2)
{
  if (are_boolean(e1, e2))
    return make_bool_or(*this, e1, tok, e2);
  else if (are_integral(e1, e2))
    throw type_error(location(), "bitwise-or not implemented");
  throw type_error(e1.get_location(), "operands have different types");
}

/// Process the expression `e1 ^ e2`.
expr&
semantics::on_bitwise_xor(expr& e1, token tok, expr& e2)
{
  if (are_boolean(e1, e2))
    return make_bool_xor(*this, e1, tok, e2);
  else if (are_integral(e1, e2)) {
    throw type_error(location(), "bitwise-xor not implemented");
  }
  throw type_error(e1.get_location(), "operands have different types");
}

/// Process the expression `e1 & e2`.
expr&
semantics::on_bitwise_and(expr& e1, token tok, expr& e2)
{
  if (are_boolean(e1, e2))
    return make_bool_and(*this, e1, tok, e2);
  else if (are_integral(e1, e2)) {
    throw type_error(location(), "bitwise-and not implemented");
  }
  throw type_error(e1.get_location(), "operands have different types");
}


// -------------------------------------------------------------------------- //
// Semantics of equality-expressions

/// A subroutine of on_equal.
static inline expr&
make_bool_eq(semantics& sema, expr& e1, token tok, expr& e2)
{
  expr& ret = sema.build_bool.make_eq_expr(e1, e2);
  return set_locations(ret, e1, tok, e2);
}

/// A subroutine of on_ne. This returns an xor expression.
static inline expr&
make_bool_ne(semantics& sema, expr& e1, token tok, expr& e2)
{
  expr& ret = sema.build_bool.make_xor_expr(e1, e2);
  return set_locations(ret, e1, tok, e2);
}

/// A subroutine of on_equal.
static inline expr&
make_int_eq(semantics& sema, expr& e1, token tok, expr& e2)
{
  expr& ret = sema.build_int.make_eq_expr(e1, e2);
  return set_locations(ret, e1, tok, e2);
}

/// A subroutine of on_not_equal.
static inline expr&
make_int_ne(semantics& sema, expr& e1, token tok, expr& e2)
{
  expr& ret = sema.build_int.make_ne_expr(e1, e2);
  return set_locations(ret, e1, tok, e2);
}

/// Process the expression `e1 == e2`.
expr&
semantics::on_equal(expr& e1, token tok, expr& e2)
{
  if (are_boolean(e1, e2))
    return make_bool_eq(*this, e1, tok, e2);
  else if (are_integral(e1, e2)) 
    return make_int_eq(*this, e1, tok, e2);
  throw type_error(e1.get_location(), "operands have different types");
}

/// Process the expression `e1 != e2`.
expr&
semantics::on_not_equal(expr& e1, token tok, expr& e2)
{
  if (are_boolean(e1, e2))
    return make_bool_ne(*this, e1, tok, e2);
  else if (are_integral(e1, e2))
    return make_int_ne(*this, e1, tok, e2);
  throw type_error(e1.get_location(), "operands have different types");
}

// -------------------------------------------------------------------------- //
// Semantics of relational-expressions

expr&
semantics::on_less(expr& e1, token tok, expr& e2)
{
  check_int(e1, e2);
  expr& ret = build_int.make_lt_expr(e1, e2);
  return set_locations(ret, e1, tok, e2);
}

expr&
semantics::on_greater(expr& e1, token tok, expr& e2)
{
  check_int(e1, e2);
  expr& ret = build_int.make_gt_expr(e1, e2);
  return set_locations(ret, e1, tok, e2);
}

expr&
semantics::on_less_equal(expr& e1, token tok, expr& e2)
{
  check_int(e1, e2);
  expr& ret = build_int.make_le_expr(e1, e2);
  return set_locations(ret, e1, tok, e2);
}

expr&
semantics::on_greater_equal(expr& e1, token tok, expr& e2)
{
  check_int(e1, e2);
  expr& ret = build_int.make_ge_expr(e1, e2);
  return set_locations(ret, e1, tok, e2);
}

// -------------------------------------------------------------------------- //
// Semantics of arithmetic expressions

expr&
semantics::on_addition(expr& e1, token tok, expr& e2)
{
  check_int(e1, e2);
  expr& ret = build_int.make_add_expr(e1, e2);
  return set_locations(ret, e1, tok, e2);
}

expr&
semantics::on_subtraction(expr& e1, token tok, expr& e2)
{
  check_int(e1, e2);
  expr& ret = build_int.make_sub_expr(e1, e2);
  return set_locations(ret, e1, tok, e2);
}

expr&
semantics::on_multiplication(expr& e1, token tok, expr& e2)
{
  check_int(e1, e2);
  expr& ret = build_int.make_mul_expr(e1, e2);
  return set_locations(ret, e1, tok, e2);
}

expr&
semantics::on_division(expr& e1, token tok, expr& e2)
{
  check_int(e1, e2);
  expr& ret = build_int.make_quo_expr(e1, e2);
  return set_locations(ret, e1, tok, e2);
}

expr&
semantics::on_remainder(expr& e1, token tok, expr& e2)
{
  check_int(e1, e2);
  expr& ret = build_int.make_rem_expr(e1, e2);
  return set_locations(ret, e1, tok, e2);
}

/// Process the expression `-e1`.
expr&
semantics::on_negation(token tok, expr& e1)
{
  check_int(e1);
  expr& ret = build_int.make_neg_expr(e1);
  return set_locations(ret, tok, e1);
}

/// Process the expression `~e1`.
expr&
semantics::on_bitwise_not(token tok, expr& e1)
{
  throw type_error(tok.get_location(), "bitwise-not not implemented");
}

/// Process the expression `!e1`.
expr&
semantics::on_logical_not(token tok, expr& e1)
{
  check_bool(e1);
  expr& ret = build_bool.make_not_expr(e1);
  return set_locations(ret, tok, e1);
}

/// Returns a new boolean expression.
expr&
semantics::on_bool(token tok)
{
  const beaker::bool_attr& attr = get_bool_attribute(tok);
  expr* ret;
  if (attr.get_value())
    ret = &build_bool.make_true_expr();
  else
    ret = &build_bool.make_false_expr();
  ret->set_fixed_layout(tok.get_location());
  return *ret;
}

expr&
semantics::on_int(token tok)
{
  const beaker::int_attr& attr = get_int_attribute(tok);
  type& ty = build_int.get_int32_type();
  expr& ret = build_int.make_int_expr(ty, attr.get_value());
  ret.set_fixed_layout(tok.get_location());
  return ret;
}

} // namespace bpl

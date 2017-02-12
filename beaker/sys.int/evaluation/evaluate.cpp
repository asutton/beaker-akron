// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "evaluate.hpp"
#include "../type.hpp"
#include "../expr.hpp"
#include <beaker/base/printing/print.hpp>


namespace beaker {
namespace sys_int {

/// Evaluate the literal expression `e`.
value
evaluate(evaluator& eval, const int_expr& e)
{
  return e.get_value();
}

value
evaluate(evaluator& eval, const eq_expr& e)
{
  value v1 = evaluate(eval, e.get_lhs());
  value v2 = evaluate(eval, e.get_rhs());
  return value(v1.get_int() == v2.get_int());
}

value
evaluate(evaluator& eval, const ne_expr& e)
{
  value v1 = evaluate(eval, e.get_lhs());
  value v2 = evaluate(eval, e.get_rhs());
  return value(v1.get_int() != v2.get_int());
}

value
evaluate(evaluator& eval, const lt_expr& e)
{
  value v1 = evaluate(eval, e.get_lhs());
  value v2 = evaluate(eval, e.get_rhs());
  return value(v1.get_int() < v2.get_int());
}

value
evaluate(evaluator& eval, const gt_expr& e)
{
  value v1 = evaluate(eval, e.get_lhs());
  value v2 = evaluate(eval, e.get_rhs());
  return value(v1.get_int() > v2.get_int());
}

value
evaluate(evaluator& eval, const le_expr& e)
{
  value v1 = evaluate(eval, e.get_lhs());
  value v2 = evaluate(eval, e.get_rhs());
  return value(v1.get_int() <= v2.get_int());
}

value
evaluate(evaluator& eval, const ge_expr& e)
{
  value v1 = evaluate(eval, e.get_lhs());
  value v2 = evaluate(eval, e.get_rhs());
  return value(v1.get_int() >= v2.get_int());
}


// -------------------------------------------------------------------------- //
// Addition

template<typename T>
static value
add_overflow(evaluator& eval, const add_expr& e, const T& t)
{
  using int_t = typename T::rep_type;
  int_t a = evaluate(eval, e.get_lhs()).get_int();
  int_t b = evaluate(eval, e.get_rhs()).get_int();
  if (a > t.max() - b)
    throw overflow_error(e);
  return value(a + b);
}

static value
add_modular(evaluator& eval, const add_expr& e, const mod_type& t)
{
  std::uintmax_t a = evaluate(eval, e.get_lhs()).get_int();
  std::uintmax_t b = evaluate(eval, e.get_rhs()).get_int();
  return value((a + b) % t.mod());
}

value
evaluate(evaluator& eval, const add_expr& e)
{
  const type& t = e.get_type();
  switch (t.get_kind()) {
    case nat_type_kind: return add_overflow(eval, e, cast<nat_type>(t));
    case int_type_kind: return add_overflow(eval, e, cast<int_type>(t));
    case mod_type_kind: return add_modular(eval, e, cast<mod_type>(t));
  }
  assert(false && "not an integer expression");
}


// -------------------------------------------------------------------------- //
// Subtraction

template<typename T>
static value
sub_overflow(evaluator& eval, const sub_expr& e, const T& t)
{
  std::uintmax_t a = evaluate(eval, e.get_lhs()).get_int();
  std::uintmax_t b = evaluate(eval, e.get_rhs()).get_int();
  if (a < t.min() + b)
    throw overflow_error(e);
  return value(a - b);
}

static value
sub_modular(evaluator& eval, const sub_expr& e, const mod_type& t)
{
  std::uintmax_t a = evaluate(eval, e.get_lhs()).get_int();
  std::uintmax_t b = evaluate(eval, e.get_rhs()).get_int();
  // FIXME: Is this right?
  return value((a - b) % t.mod());
}

value
evaluate(evaluator& eval, const sub_expr& e)
{
  const type& t = e.get_type();
  switch (t.get_kind()) {
    case nat_type_kind: return sub_overflow(eval, e, cast<nat_type>(t));
    case int_type_kind: return sub_overflow(eval, e, cast<int_type>(t));
    case mod_type_kind: return sub_modular(eval, e, cast<mod_type>(t));
  }
  assert(false && "not an integer expression");
}


// -------------------------------------------------------------------------- //
// Multiplication

static value
mul_nat(evaluator& eval, const mul_expr& e, const nat_type& t)
{
  std::uintmax_t a = evaluate(eval, e.get_lhs()).get_int();
  std::uintmax_t b = evaluate(eval, e.get_rhs()).get_int();
  if (a > t.max() / b)
    throw overflow_error(e);
  return value(a * b);
}

static value
mul_int(evaluator& eval, const mul_expr& e, const int_type& t)
{
  std::intmax_t a = evaluate(eval, e.get_lhs()).get_int();
  std::intmax_t b = evaluate(eval, e.get_rhs()).get_int();
  if (b > 0 && a > t.max() / b)
    throw overflow_error(e);
  if (b < 0 && a < t.max() / b)
    throw overflow_error(e);
  return value(a * b);
}

static value
mul_mod(evaluator& eval, const mul_expr& e, const mod_type& t)
{
  std::uintmax_t a = evaluate(eval, e.get_lhs()).get_int();
  std::uintmax_t b = evaluate(eval, e.get_rhs()).get_int();
  // FIXME: I don't think that this is right.
  return value((a * b) % t.mod()); 
}

value
evaluate(evaluator& eval, const mul_expr& e)
{
  const type& t = e.get_type();
  switch (t.get_kind()) {
    case nat_type_kind: return mul_nat(eval, e, cast<nat_type>(t));
    case int_type_kind: return mul_int(eval, e, cast<int_type>(t));
    case mod_type_kind: return mul_mod(eval, e, cast<mod_type>(t));
  }
  assert(false && "not an integer expression");
}


// -------------------------------------------------------------------------- //
// Quotient

template<typename T>
static value
quo_unsigned(evaluator& eval, const quo_expr& e, const T& t)
{
  using int_t = typename T::rep_type;
  int_t a = evaluate(eval, e.get_lhs()).get_int();
  int_t b = evaluate(eval, e.get_rhs()).get_int();
  if (b == 0)
    throw division_error(e);
  return value(a / b);
}

static value
quo_signed(evaluator& eval, const quo_expr& e, const int_type& t)
{
  std::intmax_t a = evaluate(eval, e.get_lhs()).get_int();
  std::intmax_t b = evaluate(eval, e.get_rhs()).get_int();
  if (b == 0)
    throw division_error(e);
  if (a == t.min() && b == -1)
    throw overflow_error(e);
  return value(a / b);
}

value
evaluate(evaluator& eval, const quo_expr& e)
{
  const type& t = e.get_type();
  switch (t.get_kind()) {
    case nat_type_kind: return quo_unsigned(eval, e, cast<nat_type>(t));
    case int_type_kind: return quo_signed(eval, e, cast<int_type>(t));
    case mod_type_kind: return quo_unsigned(eval, e, cast<mod_type>(t));
  }
  assert(false && "not an integer expression");
}


// -------------------------------------------------------------------------- //
// Remainder

value
evaluate(evaluator& eval, const rem_expr& e)
{
  std::uintmax_t a = evaluate(eval, e.get_lhs()).get_int();
  std::uintmax_t b = evaluate(eval, e.get_rhs()).get_int();
  if (b == 0)
    throw division_error(e);
  return value(a % b);
}


// -------------------------------------------------------------------------- //
// Negation

static value
neg_nat(evaluator& eval, const neg_expr& e, const nat_type& t)
{
  assert(false && "negation of natural number");
}

static value
neg_int(evaluator& eval, const neg_expr& e, const int_type& t)
{
  std::intmax_t n = evaluate(eval, e.get_operand()).get_int();
  if (n == t.min())
    throw overflow_error(e);
  return value(-n);
}

static value
neg_mod(evaluator& eval, const neg_expr& e, const mod_type& t)
{
  std::uintmax_t n = evaluate(eval, e.get_operand()).get_int();
  // FIXME: Is this right?
  return value(-n % t.mod());
}

value
evaluate(evaluator& eval, const neg_expr& e)
{
  const type& t = e.get_type();
  switch (t.get_kind()) {
    case nat_type_kind: return neg_nat(eval, e, cast<nat_type>(t));
    case int_type_kind: return neg_int(eval, e, cast<int_type>(t));
    case mod_type_kind: return neg_mod(eval, e, cast<mod_type>(t));
  }
  assert(false && "not an negatable expression");
}


// -------------------------------------------------------------------------- //
// Reciprocal


template<typename T>
static value
eval_rec(evaluator& eval, const rec_expr& e, const T& t)
{
  using int_t = typename T::rep_type;
  int_t n = evaluate(eval, e.get_operand()).get_int();
  if (n == 0)
    throw division_error(e);
  return value(1 / n);
}

value
evaluate(evaluator& eval, const rec_expr& e)
{
  const type& t = e.get_type();
  switch (t.get_kind()) {
    case nat_type_kind: return eval_rec(eval, e, cast<nat_type>(t));
    case int_type_kind: return eval_rec(eval, e, cast<int_type>(t));
    case mod_type_kind: return eval_rec(eval, e, cast<mod_type>(t));
  }
  assert(false && "not an negatable expression");
}

} // namespace sys_int
} // namespace beaker

// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/numeric/type.hpp>
#include <beaker/numeric/expr.hpp>
#include <beaker/base/generation/generation.hpp>

#include <iostream>

namespace beaker {
namespace numeric {

std::string
generate_name(generator& gen, const name& n)
{
  assert(false && "not defined");
}

/// Generate an integer type. If the precision is greater than 128, the type
/// will be indirect.
template<typename T>
static inline cg::type
generate_int_type(generator& g, const T& t)
{
  int prec = t.get_precision();
  bool ref = (prec <= 128);
  return {llvm::Type::getIntNTy(g.get_context(), prec), ref};
}

static cg::type
generate_fp_type(generator& g, const type& t)
{
  int prec = cast<float_type>(t).get_precision();
  if (prec == 16)
    return llvm::Type::getHalfTy(g.get_context());
  else if (prec == 32)
    return llvm::Type::getFloatTy(g.get_context());
  else if (prec == 64)
    return llvm::Type::getDoubleTy(g.get_context());
  else
    assert(false && "invalid floating point precision");
}

// Generate a numeric type.
cg::type
generate_type(generator& gen, const type& t)
{
  switch (t.get_kind()) {
    case char_type_kind:
      return generate_int_type(gen, cast<char_type>(t));
    case nat_type_kind:
      return generate_int_type(gen, cast<nat_type>(t));
    case int_type_kind:
      return generate_int_type(gen, cast<int_type>(t));
    case mod_type_kind:
      return generate_int_type(gen, cast<mod_type>(t));
    case float_type_kind:
      return generate_fp_type(gen, cast<float_type>(t));
    default:
      break;
  }
  assert(false && "not a numeric type");
}

extern cg::value generate_int_expr(generator&, const int_expr&);
extern cg::value generate_float_expr(generator&, const float_expr&);

extern cg::value generate_eq_expr(generator&, const eq_expr&);
extern cg::value generate_ne_expr(generator&, const ne_expr&);
extern cg::value generate_lt_expr(generator&, const lt_expr&);
extern cg::value generate_gt_expr(generator&, const gt_expr&);
extern cg::value generate_le_expr(generator&, const le_expr&);
extern cg::value generate_ge_expr(generator&, const ge_expr&);

extern cg::value generate_add_expr(generator&, const add_expr&);
extern cg::value generate_sub_expr(generator&, const sub_expr&);
extern cg::value generate_mul_expr(generator&, const mul_expr&);
extern cg::value generate_div_expr(generator&, const div_expr&);
extern cg::value generate_rem_expr(generator&, const rem_expr&);
extern cg::value generate_neg_expr(generator&, const neg_expr&);
extern cg::value generate_rec_expr(generator&, const rec_expr&);

// Generate code for the numeric expression e.
//
// Code generation semantics are determined by the type of e.
cg::value
generate_expr(generator& gen, const expr& e)
{
  switch (e.get_kind()) {
    case int_expr_kind:
      return generate_int_expr(gen, cast<int_expr>(e));
    case float_expr_kind:
      return generate_float_expr(gen, cast<float_expr>(e));

    case eq_expr_kind: 
      return generate_eq_expr(gen, cast<eq_expr>(e));
    case ne_expr_kind:
      return generate_ne_expr(gen, cast<ne_expr>(e));
    case lt_expr_kind:
      return generate_lt_expr(gen, cast<lt_expr>(e));
    case gt_expr_kind:
      return generate_gt_expr(gen, cast<gt_expr>(e));
    case le_expr_kind:
      return generate_le_expr(gen, cast<le_expr>(e));
    case ge_expr_kind:
      return generate_ge_expr(gen, cast<ge_expr>(e));

    case add_expr_kind:
      return generate_add_expr(gen, cast<add_expr>(e));
    case sub_expr_kind:
      return generate_sub_expr(gen, cast<sub_expr>(e));
    case mul_expr_kind:
      return generate_mul_expr(gen, cast<mul_expr>(e));
    case div_expr_kind:
      return generate_div_expr(gen, cast<div_expr>(e));
    case rem_expr_kind:
      return generate_rem_expr(gen, cast<rem_expr>(e));
    case neg_expr_kind:
      return generate_neg_expr(gen, cast<neg_expr>(e));
    case rec_expr_kind:
      return generate_rec_expr(gen, cast<rec_expr>(e));
    
    default:
      break;
  }
  assert(false && "not a numeric expression");
}

cg::value
generate_decl(generator& gen, const decl& d)
{
  assert(false && "not defined");
}

void
generate_stmt(generator& gen, const stmt& s)
{
  assert(false && "not defined");
}


} // namespace numeric
} // namespace beaker


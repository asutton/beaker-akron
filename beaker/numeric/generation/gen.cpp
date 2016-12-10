// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/numeric/generation/gen.hpp>
#include <beaker/numeric/type.hpp>
#include <beaker/numeric/expr.hpp>


namespace beaker {
namespace numeric {


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
gen_algo::operator()(generator& gen, const type& t) const
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
gen_algo::operator()(generator& gen, const expr& e) const
{
  switch (e.get_kind()) {
#define def_expr(E) \
    case E ## _expr_kind: \
      return generate_ ## E ## _expr(gen, cast<E ## _expr>(e));
#include "../expr.def"
    default:
      break;
  }
  assert(false && "not a numeric expression");
}

} // namespace numeric
} // namespace beaker


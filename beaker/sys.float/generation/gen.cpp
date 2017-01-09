// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "gen.hpp"
#include "../type.hpp"
#include "../expr.hpp"


namespace beaker {
namespace sys_float {


static cg::type
generate_float_type(generator& g, const type& t)
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
  assert(is_floating_point_type(t));
  return generate_float_type(gen, cast<float_type>(t));
}

/// Generate a floating point literal.
static cg::value 
generate_float_expr(generator& gen, const float_expr& e)
{
  cg::type type = generate(gen, e.get_type());
  llvm::Builder ir(gen.get_current_block());
  return llvm::ConstantFP::get(type, e.get_value().get_float());
}

/// Generate an equality comparison.
static cg::value
generate_eq_expr(generator& gen, const eq_expr& e)
{
  cg::value v1 = generate(gen, e.get_lhs());
  cg::value v2 = generate(gen, e.get_rhs());
  llvm::Builder ir(gen.get_current_block());
  return ir.CreateFCmpUEQ(v1, v2);
}

/// Generate a not-equals comparison.
static cg::value
generate_ne_expr(generator& gen, const ne_expr& e)
{
  cg::value v1 = generate(gen, e.get_lhs());
  cg::value v2 = generate(gen, e.get_rhs());
  llvm::Builder ir(gen.get_current_block());
  return ir.CreateFCmpUNE(v1, v2);
}

static cg::value
generate_lt_expr(generator& gen, const lt_expr& e)
{
  cg::value v1 = generate(gen, e.get_lhs());
  cg::value v2 = generate(gen, e.get_rhs());
  llvm::Builder ir(gen.get_current_block());
  return ir.CreateFCmpULT(v1, v2);
}

static cg::value
generate_gt_expr(generator& gen, const gt_expr& e)
{
  cg::value v1 = generate(gen, e.get_lhs());
  cg::value v2 = generate(gen, e.get_rhs());
  llvm::Builder ir(gen.get_current_block());
  return ir.CreateFCmpUGT(v1, v2);
}

static cg::value
generate_le_expr(generator& gen, const le_expr& e)
{
  cg::value v1 = generate(gen, e.get_lhs());
  cg::value v2 = generate(gen, e.get_rhs());
  llvm::Builder ir(gen.get_current_block());
  return ir.CreateFCmpULE(v1, v2);
}

static cg::value
generate_ge_expr(generator& gen, const ge_expr& e)
{
  cg::value v1 = generate(gen, e.get_lhs());
  cg::value v2 = generate(gen, e.get_rhs());
  llvm::Builder ir(gen.get_current_block());
  return ir.CreateFCmpUGE(v1, v2);
}

/// Generate an add expression.
static cg::value
generate_add_expr(generator& gen, const add_expr& e)
{
  cg::value v1 = generate(gen, e.get_lhs());
  cg::value v2 = generate(gen, e.get_rhs());
  llvm::Builder ir(gen.get_current_block());
  return ir.CreateFAdd(v1, v2);
}

static cg::value
generate_sub_expr(generator& gen, const sub_expr& e)
{
  cg::value v1 = generate(gen, e.get_lhs());
  cg::value v2 = generate(gen, e.get_rhs());
  llvm::Builder ir(gen.get_current_block());
  return ir.CreateFSub(v1, v2);
}

static cg::value
generate_mul_expr(generator& gen, const mul_expr& e)
{
  cg::value v1 = generate(gen, e.get_lhs());
  cg::value v2 = generate(gen, e.get_rhs());
  llvm::Builder ir(gen.get_current_block());
  return ir.CreateFMul(v1, v2);
}

static cg::value
generate_div_expr(generator& gen, const div_expr& e)
{
  cg::value v1 = generate(gen, e.get_lhs());
  cg::value v2 = generate(gen, e.get_rhs());
  llvm::Builder ir(gen.get_current_block());
  return ir.CreateFDiv(v1, v2);
}

static cg::value
generate_neg_expr(generator& gen, const neg_expr& e)
{
  assert(false && "not implemented");
}

static cg::value
generate_rec_expr(generator& gen, const rec_expr& e)
{
  assert(false && "not implemented");
}

// Generate code for the numeric expression e.
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
  assert(false && "not a floating point expression");
}

} // namespace sys_float
} // namespace beaker


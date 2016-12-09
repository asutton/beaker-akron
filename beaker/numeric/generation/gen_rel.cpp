// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/numeric/type.hpp>
#include <beaker/numeric/expr.hpp>
#include <beaker/base/generation/generation.hpp>


namespace beaker {
namespace numeric {

cg::value
generate_eq_expr(generator& gen, const eq_expr& e)
{
  cg::value v1 = generate(gen, e.get_lhs());
  cg::value v2 = generate(gen, e.get_rhs());

  llvm::Builder ir(gen.get_current_block());
  const type& t = e.get_lhs().get_type();
  switch (t.get_kind()) {
    case nat_type_kind:
    case int_type_kind:
    case mod_type_kind:
      return ir.CreateICmpEQ(v1, v2);
    case float_type_kind:
      return ir.CreateFCmpOEQ(v1, v2);
    default:
      break;
  }
  assert(false && "not a numeric type");
}

cg::value
generate_ne_expr(generator& gen, const ne_expr& e)
{
  cg::value v1 = generate(gen, e.get_lhs());
  cg::value v2 = generate(gen, e.get_rhs());

  llvm::Builder ir(gen.get_current_block());
  const type& t = e.get_lhs().get_type();
  switch (t.get_kind()) {
    case nat_type_kind:
    case int_type_kind:
    case mod_type_kind:
      return ir.CreateICmpNE(v1, v2);
    case float_type_kind:
      return ir.CreateFCmpONE(v1, v2);
    default:
      break;
  }
  assert(false && "not a numeric type");
}

cg::value
generate_lt_expr(generator& gen, const lt_expr& e)
{
  cg::value v1 = generate(gen, e.get_lhs());
  cg::value v2 = generate(gen, e.get_rhs());

  llvm::Builder ir(gen.get_current_block());
  const type& t = e.get_lhs().get_type();
  switch (t.get_kind()) {
    case nat_type_kind:
    case mod_type_kind:
      return ir.CreateICmpULT(v1, v2);
    case int_type_kind:
      return ir.CreateICmpSLT(v1, v2);
    case float_type_kind:
      return ir.CreateFCmpOLT(v1, v2);
    default:
      break;
  }
  assert(false && "not a numeric type");
}

cg::value
generate_gt_expr(generator& gen, const gt_expr& e)
{
  cg::value v1 = generate(gen, e.get_lhs());
  cg::value v2 = generate(gen, e.get_rhs());

  llvm::Builder ir(gen.get_current_block());
  const type& t = e.get_lhs().get_type();
  switch (t.get_kind()) {
    case nat_type_kind:
    case mod_type_kind:
      return ir.CreateICmpUGT(v1, v2);
    case int_type_kind:
      return ir.CreateICmpSGT(v1, v2);
    case float_type_kind:
      return ir.CreateFCmpOGT(v1, v2);
    default:
      break;
  }
  assert(false && "not a numeric type");
}

cg::value
generate_le_expr(generator& gen, const le_expr& e)
{
  cg::value v1 = generate(gen, e.get_lhs());
  cg::value v2 = generate(gen, e.get_rhs());

  llvm::Builder ir(gen.get_current_block());
  const type& t = e.get_lhs().get_type();
  switch (t.get_kind()) {
    case nat_type_kind:
    case mod_type_kind:
      return ir.CreateICmpULE(v1, v2);
    case int_type_kind:
      return ir.CreateICmpSLE(v1, v2);
    case float_type_kind:
      return ir.CreateFCmpOLE(v1, v2);
    default:
      break;
  }
  assert(false && "not a numeric type");
}

cg::value
generate_ge_expr(generator& gen, const ge_expr& e)
{
  cg::value v1 = generate(gen, e.get_lhs());
  cg::value v2 = generate(gen, e.get_rhs());

  llvm::Builder ir(gen.get_current_block());
  const type& t = e.get_lhs().get_type();
  switch (t.get_kind()) {
    case nat_type_kind:
    case mod_type_kind:
      return ir.CreateICmpUGE(v1, v2);
    case int_type_kind:
      return ir.CreateICmpSGE(v1, v2);
    case float_type_kind:
      return ir.CreateFCmpOGE(v1, v2);
    default:
      break;
  }
  assert(false && "not a numeric type");
}


} // namespace numeric
} // namespace beaker


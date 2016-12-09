// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/numeric/type.hpp>
#include <beaker/numeric/expr.hpp>
#include <beaker/base/generation/generation.hpp>


namespace beaker {
namespace numeric {

cg::value 
generate_int_expr(generator& gen, const int_expr& e)
{
  int prec = get_precision(e.get_type());
  value val = e.get_value();
  
  llvm::Builder ir(gen.get_current_block());
  return ir.getIntN(prec, val.get_integer());
}

cg::value 
generate_float_expr(generator& gen, const float_expr& e)
{
  value val = e.get_value();

  llvm::Builder ir(gen.get_current_block());
  cg::type type = generate(gen, e.get_type());
  return llvm::ConstantFP::get(type, val.get_float());
}

cg::value
generate_add_expr(generator& gen, const add_expr& e)
{
  cg::value v1 = generate(gen, e.get_lhs());
  cg::value v2 = generate(gen, e.get_rhs());

  llvm::Builder ir(gen.get_current_block());
  const type& t = e.get_lhs().get_type();
  switch (t.get_kind()) {
    case nat_type_kind:
      return ir.CreateNUWAdd(v1, v2);
    case int_type_kind:
      return ir.CreateNSWAdd(v1, v2);
    case mod_type_kind:
      return ir.CreateAdd(v1, v2);
    case float_type_kind:
      return ir.CreateFAdd(v1, v2, "");
    default:
      break;
  }
  assert(false && "not a numeric type");
}

cg::value
generate_sub_expr(generator& gen, const sub_expr& e)
{
  cg::value v1 = generate(gen, e.get_lhs());
  cg::value v2 = generate(gen, e.get_rhs());

  llvm::Builder ir(gen.get_current_block());
  const type& t = e.get_lhs().get_type();
  switch (t.get_kind()) {
    case nat_type_kind:
      return ir.CreateNUWSub(v1, v2);
    case int_type_kind:
      return ir.CreateNSWSub(v1, v2);
    case mod_type_kind:
      return ir.CreateSub(v1, v2);
    case float_type_kind:
      return ir.CreateFSub(v1, v2, "");
    default:
      break;
  }
  assert(false && "not a numeric type");
}

cg::value
generate_mul_expr(generator& gen, const mul_expr& e)
{
  cg::value v1 = generate(gen, e.get_lhs());
  cg::value v2 = generate(gen, e.get_rhs());

  llvm::Builder ir(gen.get_current_block());
  const type& t = e.get_lhs().get_type();
  switch (t.get_kind()) {
    case nat_type_kind:
      return ir.CreateNUWMul(v1, v2);
    case int_type_kind:
      return ir.CreateNSWMul(v1, v2);
    case mod_type_kind:
      return ir.CreateMul(v1, v2);
    case float_type_kind:
      return ir.CreateFMul(v1, v2, "");
    default:
      break;
  }
  assert(false && "not a numeric type");
}

cg::value
generate_div_expr(generator& gen, const div_expr& e)
{
  cg::value v1 = generate(gen, e.get_lhs());
  cg::value v2 = generate(gen, e.get_rhs());

  llvm::Builder ir(gen.get_current_block());
  const type& t = e.get_lhs().get_type();
  switch (t.get_kind()) {
    case nat_type_kind:
    case mod_type_kind:
      return ir.CreateUDiv(v1, v2);
    case int_type_kind:
      return ir.CreateSDiv(v1, v2);
    case float_type_kind:
      return ir.CreateFDiv(v1, v2, "");
    default:
      break;
  }
  assert(false && "not a numeric type");
}

cg::value
generate_rem_expr(generator& gen, const rem_expr& e)
{
  cg::value v1 = generate(gen, e.get_lhs());
  cg::value v2 = generate(gen, e.get_rhs());

  llvm::Builder ir(gen.get_current_block());
  const type& t = e.get_lhs().get_type();
  switch (t.get_kind()) {
    case nat_type_kind:
    case mod_type_kind:
      return ir.CreateURem(v1, v2);
    case int_type_kind:
      return ir.CreateSRem(v1, v2);
    default:
      break;
  }
  assert(false && "not an integral type");
}

cg::value
generate_neg_expr(generator& gen, const neg_expr& e)
{
  assert(false && "not implemented");
}

cg::value
generate_rec_expr(generator& gen, const rec_expr& e)
{
  assert(false && "not implemented");
}

} // namespace numeric
} // namespace beaker


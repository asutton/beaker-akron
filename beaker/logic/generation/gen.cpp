// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/logic/generation/gen.hpp>
#include <beaker/logic/type.hpp>
#include <beaker/logic/expr.hpp>


namespace beaker {
namespace logic {

cg::type
gen_algo::operator()(generator& gen, const type& t) const
{
  assert(is_boolean_type(t));
  return llvm::Type::getInt1Ty(gen.get_context());
}


static cg::value 
generate_bool_expr(generator& gen, const bool_expr& e)
{
  llvm::Builder ir(gen.get_current_block());
  return ir.getInt1(e.get_boolean());
}

// FIXME: Generate short circuiting evaluation.
static cg::value 
generate_and_expr(generator& gen, const and_expr& e)
{
  llvm::Builder ir(gen.get_current_block());
  cg::value lhs = generate(gen, e.get_lhs());
  cg::value rhs = generate(gen, e.get_rhs());
  return ir.CreateAnd(lhs, rhs);
}

// FIXME: Generate short circuiting evaluation.
static cg::value 
generate_or_expr(generator& gen, const or_expr& e)
{
  llvm::Builder ir(gen.get_current_block());
  cg::value lhs = generate(gen, e.get_lhs());
  cg::value rhs = generate(gen, e.get_rhs());
  return ir.CreateOr(lhs, rhs);
}

static cg::value 
generate_not_expr(generator& gen, const not_expr& e)
{
  llvm::Builder ir(gen.get_current_block());
  cg::value op = generate(gen, e.get_operand());
  cg::value truth = ir.getTrue();
  return ir.CreateXor(op, truth);
}

static cg::value
generate_imp_expr(generator& gen, const imp_expr& e)
{
  assert(false && "not implemented");
}

static cg::value
generate_eq_expr(generator& gen, const eq_expr& e)
{
  assert(false && "not implemented");
}

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
  assert(false && "not a logic epxression");
}

} // namespace logic
} // namespace beaker


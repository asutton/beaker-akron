// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/logic/type.hpp>
#include <beaker/logic/expr.hpp>
#include <beaker/base/generation/generation.hpp>


namespace beaker {
namespace logic {

std::string
generate_name(generator& gen, const name& n)
{
  assert(false && "not defined");
}

/// Generate the LLVM type for t.
cg::type
generate_type(generator& g, const type& t)
{
  assert(t.get_kind() == bool_type_kind);
  return llvm::Type::getInt1Ty(g.get_context());
}

static cg::value 
generate_bool_expr(generator& g, const bool_expr& e)
{
  llvm::Builder ir(g.get_current_block());
  value v = e.get_value();
  return ir.getInt1(v.get_integer());
}

// FIXME: Generate short circuiting evaluation.
static cg::value 
generate_and_expr(generator& g, const and_expr& e)
{
  llvm::Builder ir(g.get_current_block());
  cg::value lhs = generate(g, e.get_lhs());
  cg::value rhs = generate(g, e.get_rhs());
  return ir.CreateAnd(lhs, rhs);
}

// FIXME: Generate short circuiting evaluation.
static cg::value 
generate_or_expr(generator& g, const or_expr& e)
{
  llvm::Builder ir(g.get_current_block());
  cg::value lhs = generate(g, e.get_lhs());
  cg::value rhs = generate(g, e.get_rhs());
  return ir.CreateOr(lhs, rhs);
}

static cg::value 
generate_not_expr(generator& g, const not_expr& e)
{
  llvm::Builder ir(g.get_current_block());
  cg::value op = generate(g, e.get_operand());
  cg::value truth = ir.getTrue();
  return ir.CreateXor(op, truth);
}

cg::value
generate_expr(generator& g, const expr& e)
{
  switch (e.get_kind()) {
    case bool_expr_kind:
      return generate_bool_expr(g, cast<bool_expr>(e));
    case and_expr_kind:
      return generate_and_expr(g, cast<and_expr>(e));
    case or_expr_kind:
      return generate_or_expr(g, cast<or_expr>(e));
    case not_expr_kind:
      return generate_not_expr(g, cast<not_expr>(e));
    case imp_expr_kind:
    case eq_expr_kind:
      assert(false && "not implemented");
    default:
      break;
  }
  assert(false && "not a logic epxression");
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

} // namespace logic
} // namespace beaker


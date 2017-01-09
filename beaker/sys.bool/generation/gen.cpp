// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "gen.hpp"
#include "../type.hpp"
#include "../expr.hpp"
#include "../decl.hpp"


namespace beaker {
namespace sys_bool {

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

static cg::value 
generate_and_expr(generator& gen, const and_expr& e)
{
  cg::value lhs = generate(gen, e.get_lhs());
  cg::value rhs = generate(gen, e.get_rhs());
  llvm::Builder ir(gen.get_current_block());
  return ir.CreateAnd(lhs, rhs);
}

static cg::value 
generate_or_expr(generator& gen, const or_expr& e)
{
  cg::value lhs = generate(gen, e.get_lhs());
  cg::value rhs = generate(gen, e.get_rhs());
  llvm::Builder ir(gen.get_current_block());
  return ir.CreateOr(lhs, rhs);
}

static cg::value 
generate_not_expr(generator& gen, const not_expr& e)
{
  cg::value op = generate(gen, e.get_operand());
  llvm::Builder ir(gen.get_current_block());
  cg::value truth = ir.getTrue();
  return ir.CreateXor(op, truth);
}

static cg::value
generate_imp_expr(generator& gen, const imp_expr& e)
{
  cg::value lhs = generate(gen, e.get_lhs());
  cg::value rhs = generate(gen, e.get_rhs());
  llvm::Builder ir(gen.get_current_block());
  cg::value truth = ir.getTrue();
  return ir.CreateOr(ir.CreateXor(lhs, truth), rhs); // !lhs or rhs
}

static cg::value
generate_eq_expr(generator& gen, const eq_expr& e)
{
  cg::value lhs = generate(gen, e.get_lhs());
  cg::value rhs = generate(gen, e.get_rhs());
  llvm::Builder ir(gen.get_current_block());
  return ir.CreateICmpEQ(lhs, rhs);
}

static cg::value
generate_if_expr(generator& gen, const if_expr& e)
{
  llvm::Context& cxt = gen.get_context();
  llvm::BasicBlock* then = llvm::BasicBlock::Create(cxt, "if.then");
  llvm::BasicBlock* other = llvm::BasicBlock::Create(cxt, "if.else");
  llvm::BasicBlock* end = llvm::BasicBlock::Create(cxt, "if.end");

  // Evaluate the condition. If true, we need to evaluate the RHS.
  cg::value v1 = generate(gen, e.get_condition());
  llvm::Builder ir(gen.get_current_block());
  ir.CreateCondBr(v1, then, other);

  // Evaluate the true branch and jump to end.
  gen.set_current_block(then);
  cg::value v2 = generate(gen, e.get_true_value());
  then = gen.get_current_block();
  ir.SetInsertPoint(then);
  ir.CreateBr(end);

  // Evaluate the false branch and jump to end.
  gen.set_current_block(other);
  cg::value v3 = generate(gen, e.get_false_value());
  other = gen.get_current_block();
  ir.SetInsertPoint(other);
  ir.CreateBr(end);

  // The type of the expression is the type of e2 and e3.
  cg::type type = generate(gen, e.get_type());

  // Join the true and false values of e2 and e3.
  gen.set_current_block(end);
  ir.SetInsertPoint(end);
  llvm::PHINode* phi = ir.CreatePHI(type, 2);
  phi->addIncoming(v2, then);
  phi->addIncoming(v3, other);
  return phi;
}

static cg::value
generate_and_then_expr(generator& gen, const and_then_expr& e)
{
  llvm::Context& cxt = gen.get_context();
  llvm::BasicBlock* then = llvm::BasicBlock::Create(cxt, "and.then");
  llvm::BasicBlock* end = llvm::BasicBlock::Create(cxt, "and.end");

  // Evaluate the LHS. If true, we need to evaluate the RHS.
  cg::value v1 = generate(gen, e.get_lhs());
  llvm::BasicBlock* init = gen.get_current_block();
  llvm::Builder ir(init);
  ir.CreateCondBr(v1, then, end);

  // Evaluate the RHS and an unconditional branch to the end block. 
  gen.set_current_block(then);
  cg::value v2 = generate(gen, e.get_rhs());
  then = gen.get_current_block();
  ir.SetInsertPoint(then);
  ir.CreateBr(end);

  cg::type type = generate(gen, e.get_type());
  cg::value vf = ir.getFalse();

  // Join the values with a phi node.
  gen.set_current_block(end);
  ir.SetInsertPoint(end);
  llvm::PHINode* phi = ir.CreatePHI(type, 2);
  phi->addIncoming(vf, init); // false from the initial block.
  phi->addIncoming(v2, then); // v2 from the and.then.
  return phi;
}

static cg::value
generate_or_else_expr(generator& gen, const or_else_expr& e)
{
  llvm::Context& cxt = gen.get_context();
  llvm::BasicBlock* init = gen.get_current_block();
  llvm::BasicBlock* then = llvm::BasicBlock::Create(cxt, "or.else");
  llvm::BasicBlock* end = llvm::BasicBlock::Create(cxt, "or.end");

  // Evaluate the LHS. If false, we need to evaluate the RHS.
  llvm::Builder ir(gen.get_current_block());
  cg::value v1 = generate(gen, e.get_lhs());
  ir.CreateCondBr(v1, end, then);

  // Evaluate the RHS and an unconditional branch to the end block. 
  gen.set_current_block(then);
  cg::value v2 = generate(gen, e.get_rhs());
  then = gen.get_current_block();
  ir.SetInsertPoint(then);
  ir.CreateBr(end);
  
  cg::type type = generate(gen, e.get_type());
  cg::value vt = ir.getTrue();

  // Join the values with a phi node.
  gen.set_current_block(end);
  ir.SetInsertPoint(end);
  llvm::PHINode* phi = ir.CreatePHI(type, 2);
  phi->addIncoming(vt, init); // true from the initial block
  phi->addIncoming(v2, then); // v2 from or.else.
  return phi;
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


/// Generate an assertion. This generates the evaluation of the assertion
/// and, if the resulting value is false, branches to a block that contains
/// a trap.
///
/// Returns the value of the condition or nullptr if not processing a function.
///
/// \todo Support a configuration mode where the assertion can be disabled.
///
/// \bug Why do declarations return values? They modify context. That's it.
static cg::value
generate_assert_decl(generator& gen, const assert_decl& d)
{
  llvm::Context& cxt = gen.get_context();
  llvm::Module& mod = gen.get_module();

  llvm::BasicBlock* fail = llvm::BasicBlock::Create(cxt, "assert.fail");
  llvm::BasicBlock* pass = llvm::BasicBlock::Create(cxt, "assert.pass");

  // Generate the condition and the branch.
  cg::value cond = generate(gen, d.get_assertion());
  llvm::Builder ir(gen.get_current_block());
  ir.CreateCondBr(cond, pass, fail);

  // Emit the 'fail' block, which contains a trap and an unreachable
  // terminator.
  //
  // FIXME: Emit either "trap" or "debugtrap" depending on configuration.
  //
  // FIXME: Jump to assert.pass to ensure that the branch isn't removed?
  gen.set_current_block(fail);
  std::vector<llvm::Value*> args;
  cg::value op = llvm::Intrinsic::getDeclaration(&mod, llvm::Intrinsic::debugtrap);
  ir.SetInsertPoint(fail);
  ir.CreateCall(op, args);
  ir.CreateUnreachable();

  // Emit all future instructions into the end block.
  gen.set_current_block(pass);

  return cond;
}

cg::value
gen_algo::operator()(generator& gen, const decl& d) const
{
  assert(is<assert_decl>(d));
  return generate_assert_decl(gen, cast<assert_decl>(d));
}

} // namespace sys_bool
} // namespace beaker


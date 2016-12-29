// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/logic/generation/gen.hpp>
#include <beaker/logic/type.hpp>
#include <beaker/logic/expr.hpp>
#include <beaker/logic/decl.hpp>


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
  llvm::Builder ir(gen.get_current_block());
  cg::value lhs = generate(gen, e.get_lhs());
  cg::value rhs = generate(gen, e.get_rhs());
  return ir.CreateICmpEQ(lhs, rhs);
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
static cg::value
generate_assert_decl(generator& gen, const assert_decl& d)
{
  // If we're not processing a function, then assertions can have no behavior.
  llvm::Function* fn = gen.get_function();
  if (!fn)
    return nullptr;

  llvm::Context& cxt = gen.get_context();
  llvm::Module& mod = gen.get_module();

  llvm::BasicBlock* fail = llvm::BasicBlock::Create(cxt, "assert.fail", fn);
  llvm::BasicBlock* pass = llvm::BasicBlock::Create(cxt, "assert.pass", fn);

  // Generate the condition and the branch.
  llvm::Builder ir(gen.get_current_block());
  cg::value cond = generate(gen, d.get_assertion());
  ir.CreateCondBr(cond, pass, fail);

  // Emit the 'fail' block, which contains a trap and an unreachable
  // terminator.
  ir.SetInsertPoint(fail);
  std::vector<llvm::Value*> args;
  cg::value op = llvm::Intrinsic::getDeclaration(&mod, llvm::Intrinsic::trap);
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

} // namespace logic
} // namespace beaker


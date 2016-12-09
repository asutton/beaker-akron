// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/procedural/stmt.hpp>
#include <beaker/base/generation/generation.hpp>


namespace beaker {
namespace procedural {

std::string
generate_name(generator& gen, const name& n)
{
  assert(false && "not implemented");
}

llvm::Type*
generate_type(generator& gen, const type& t)
{
  assert(false && "not implemented");
}

llvm::Value*
generate_expr(generator& gen, const expr& e)
{
  assert(false && "not implemented");
}

llvm::Value*
generate_decl(generator& gen, const decl& d)
{
  assert(false && "not implemented");
}


void
generate_if_then_stmt(generator& gen, const if_then_stmt& s)
{
  llvm::Builder ir(gen.get_current_block());
  llvm::Context& cxt = gen.get_context();
  llvm::Function* fn = gen.get_function();
  llvm::BasicBlock* then = llvm::BasicBlock::Create(cxt, "if.then", fn);
  llvm::BasicBlock* end = llvm::BasicBlock::Create(cxt, "if.end", fn);

  // Generate the condition and the branch.
  llvm::Value* cond = generate(gen, s.get_condition());
  ir.CreateCondBr(cond, then, end);

  // Emit the 'then' block, possibly with a terminator.
  ir.SetInsertPoint(then);
  generate(gen, s.get_then());
  then = ir.GetInsertBlock();
  if (!then->getTerminator())
    ir.CreateBr(end);

  // Emit all future instructions into the end block.
  gen.set_current_block(end);
}

void
generate_if_else_stmt(generator& gen, const if_else_stmt& s)
{
  llvm::Builder ir(gen.get_current_block());
  llvm::Context& cxt = gen.get_context();
  llvm::Function* fn = gen.get_function();
  llvm::BasicBlock* then = llvm::BasicBlock::Create(cxt, "if.then", fn);
  llvm::BasicBlock* other = llvm::BasicBlock::Create(cxt, "if.else", fn);
  llvm::BasicBlock* end = llvm::BasicBlock::Create(cxt, "if.end", fn);
  
  // Generate the condition and the branch.
  llvm::Value* cond = generate(gen, s.get_condition());
  ir.CreateCondBr(cond, then, other);

  // Emit the then block, possibly with a terminator.
  ir.SetInsertPoint(then);
  generate(gen, s.get_then());
  then = ir.GetInsertBlock();
  if (!then->getTerminator())
    ir.CreateBr(end);

  // Emit the else block, possibly with a terminator.
  ir.SetInsertPoint(other);
  generate(gen, s.get_else());
  other = ir.GetInsertBlock();
  if (!other->getTerminator())
    ir.CreateBr(end);

  // Emit all future instructions into the end block.
  gen.set_current_block(end);
}

void
generate_while_stmt(generator& gen, const while_stmt& s)
{

}

void
generate_do_while_stmt(generator& gen, const do_while_stmt& s)
{

}

void
generate_stmt(generator& gen, const stmt& s)
{
  switch (s.get_kind()) {
#define def_stmt(S) \
    case S ## _stmt_kind: \
      return generate_## S ##_stmt(gen, cast<S ##_stmt>(s));
#include "../stmt.def"
    default:
      break;
  }
  assert(false && "not a procedural statement");
}

} // namespace procedural
} // namespace beaker

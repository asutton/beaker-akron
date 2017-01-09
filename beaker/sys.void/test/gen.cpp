// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/terms/logic.hpp>

#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/IRBuilder.h>

#include <iostream>


using namespace beaker;

// Make a function for our codegen test.
llvm::Function* 
make_function(generator& gen)
{
  llvm::Type* ret = llvm::Type::getInt1Ty(gen.get_context());
  llvm::FunctionType* type = llvm::FunctionType::get(ret, false);
  llvm::Module* mod = &gen.get_module();
  llvm::Function* fn = llvm::Function::Create(type, 
                                              llvm::Function::ExternalLinkage, 
                                              "run", 
                                              mod);
  return fn;
}

int 
main()
{
  sequential_allocator<> alloc;
  logic::builder bb(alloc);

  type& b = bb.get_bool_type();

  generator gen("a.ll");
  llvm::Function* fn = make_function(gen);
  gen.define_function(fn);

  // NOTE: These are constant-folded.
  expr& t = bb.get_true_expr();
  expr& f = bb.get_false_expr();
  expr& e1 = bb.make_and_expr(b, t, f);
  expr& e2 = bb.make_not_expr(b, e1);

  llvm::Value* val = generate(gen, e2);
  llvm::IRBuilder<> llb(gen.get_current_block());
  llb.CreateRet(val);

  gen.end_function();
  gen.get_module().dump();

}

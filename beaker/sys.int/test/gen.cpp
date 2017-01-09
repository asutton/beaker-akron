// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/terms/numeric.hpp>

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
  numeric::builder cxt(alloc);

  generator gen("a.ll");
  llvm::Function* fn = make_function(gen);
  gen.define_function(fn);

  type& z = cxt.get_int32_type();
  type& f = cxt.get_float32_type();

  expr& n3 = cxt.make_int_expr(z, 3);
  expr& n10 = cxt.make_int_expr(z, 10);
  expr& e1 = cxt.make_add_expr(z, n3, n10);

  expr& f3 = cxt.make_float_expr(f, 3.14);
  expr& e2 = cxt.make_add_expr(f, f3, f3);

  llvm::Value* v1 = generate(gen, e1);
  llvm::Value* v2 = generate(gen, e2);

  llvm::IRBuilder<> b(gen.get_current_block());
  b.CreateRet(v1);
  b.CreateRet(v2);

  gen.end_function();
  gen.get_module().dump();

}

// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/terms/logic.hpp>

// TODO: Is there not a better module to include this?
#include <beaker/common/name.hpp>
#include <beaker/common/declaration.hpp>
#include <beaker/common/symbol_table.hpp>

#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/IRBuilder.h>

#include <iostream>


using namespace beaker;

int 
main()
{
  symbol_table syms;
  sequential_allocator<> alloc;
  beaker::builder bb(alloc, syms);
  logic::builder lb(alloc);

  // Types.
  type& b = lb.get_bool_type();
  type& rb = bb.get_ref_type(b);

  // Parameters and returns.
  decl_seq parms {
    &bb.make_var_parm(bb.get_name("p1"), b)
  };
  decl& ret = bb.make_var_parm(b);

  // Build a function definition.
  expr& t = lb.get_true_expr();
  expr& e1 = bb.make_ref_expr(rb, parms[0]); // ref-to-p1
  expr& e2 = bb.make_obj_expr(b, e1);        // ref-to-obj(p1)
  expr& e3 = lb.make_and_expr(b, t, e2);

  name& n1 = bb.get_name("f1");
  fn_decl& fn1 = bb.make_fn_decl(n1, parms, ret, e3);
 
  // Build a second function definition.
  expr_seq args { &t };
  expr& e4 = bb.make_ref_expr(fn1.get_type(), fn1);
  expr& e5 = bb.make_call_expr(fn1.get_return_type(), e4, args);

  name& n2 = bb.get_name("f2");
  fn_decl& fn2 = bb.make_fn_decl(n2, parms, ret, e5);
 
  generator gen("a.ll");
  generate(gen, fn1);
  generate(gen, fn2);

  gen.get_module().dump();
}

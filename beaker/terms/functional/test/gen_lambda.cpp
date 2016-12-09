// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/terms/logic.hpp>
#include <beaker/terms/functional.hpp>

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
  functional::builder fb(alloc);

  // Types.
  type& b = lb.get_bool_type();
  // type& rb = bb.get_ref_type(b);

  // Simple values.
  expr& t = lb.get_true_expr();
  expr& f = lb.get_false_expr();


  // Build the expression lambda (x : bool, y : bool) => true.
  decl_seq lambda_parms {
    &bb.make_var_parm(bb.get_name("x"), b),
    &bb.make_var_parm(bb.get_name("y"), b),
  };
  decl& lambda_ret = bb.make_var_parm(b);
  fn_decl& lambda_fn = bb.make_fn_decl(bb.get_name(), lambda_parms, lambda_ret, t);
  expr& lambda = fb.make_lambda_expr(lambda_fn.get_type(), lambda_fn);

  // Call the lambda expression
  expr_seq args { &t, &f };
  expr& call = bb.make_call_expr(b, lambda, args);


  // Parameters and returns for an outer function.
  name& fn_name = bb.get_name("f1");
  decl_seq fn_parms {
    &bb.make_var_parm(bb.get_name("p1"), b)
  };
  decl& fn_ret = bb.make_var_parm(b);
  fn_decl& fn = bb.make_fn_decl(fn_name, fn_parms, fn_ret, call);
 

  // name& n2 = bb.get_name("f2");
  // fn_decl& fn2 = bb.make_fn_decl(n2, parms, ret, e5);
 
  generator gen("a.ll");
  generate(gen, fn);

  gen.get_module().dump();
}

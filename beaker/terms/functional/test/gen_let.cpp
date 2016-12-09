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
  type& rb = bb.get_ref_type(b);

  // Simple values.
  expr& t = lb.get_true_expr();
  // expr& f = lb.get_false_expr();

  name& var_name = bb.get_name("x");
  decl& var = bb.make_auto_var(var_name, b, t); // var x = true

  expr& e1 = bb.make_ref_expr(rb, var);
  expr& e2 = bb.make_obj_expr(b, e1);
  expr& e3 = lb.make_or_expr(b, e2, t);
  expr& let = fb.make_let_expr(b, var, e3); // let x = true in false

  // Parameters and returns for an outer function.
  name& fn_name = bb.get_name("f1");
  decl_seq fn_parms {
    &bb.make_var_parm(bb.get_name("p1"), b)
  };
  decl& fn_ret = bb.make_var_parm(b);
  fn_decl& fn = bb.make_fn_decl(fn_name, fn_parms, fn_ret, let);
 

  // name& n2 = bb.get_name("f2");
  // fn_decl& fn2 = bb.make_fn_decl(n2, parms, ret, e5);
 
  generator gen("a.ll");
  generate(gen, fn);

  gen.get_module().dump();
}

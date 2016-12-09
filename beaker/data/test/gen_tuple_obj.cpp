// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/terms/logic.hpp>
#include <beaker/terms/numeric.hpp>
#include <beaker/terms/data.hpp>

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
  numeric::builder nb(alloc);
  data::builder db(alloc);

  // Types.
  type& b = lb.get_bool_type();
  type& z = nb.get_int32_type();

  // Simple values.
  expr& t = lb.get_true_expr();
  expr& f = lb.get_false_expr();
  expr& z3 = nb.make_int_expr(z, 3);

  type& t1 = db.get_tuple_type({&b, &b, &z});
  expr& tup1 = db.make_tuple_expr(t1, {&t, &f, &z3}); // {true, false, 3}

  expr& e1 = db.make_elem_expr(b, tup1, 0); // true
  expr& e2 = db.make_elem_expr(z, tup1, 2); // 3

  type& t2 = db.get_tuple_type({&b, &z});
  expr& tup2 = db.make_tuple_expr(t2, {&e1, &e2}); // {true, 3}

  // Parameters and returns for an outer function.
  name& fn_name = bb.get_name("f1");
  decl_seq fn_parms;
  decl& fn_ret = bb.make_var_parm(b);
  fn_decl& fn = bb.make_fn_decl(fn_name, fn_parms, fn_ret, tup2);
 
  generator gen("a.ll");
  generate(gen, fn);
  gen.get_module().dump();
}

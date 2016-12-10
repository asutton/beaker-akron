// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/util/memory.hpp>
#include <beaker/util/symbol_table.hpp>

#include <beaker/base/module.hpp>
#include <beaker/base/generation/generation.hpp>
#include <beaker/base/printing/print.hpp>

#include <beaker/logic/lang.hpp>
#include <beaker/logic/type.hpp>
#include <beaker/logic/expr.hpp>
#include <beaker/logic/construction/builder.hpp>

#include <beaker/numeric/lang.hpp>
#include <beaker/numeric/type.hpp>
#include <beaker/numeric/expr.hpp>
#include <beaker/numeric/construction/builder.hpp>

#include <beaker/core/lang.hpp>
#include <beaker/core/name.hpp>
#include <beaker/core/type.hpp>
#include <beaker/core/expr.hpp>
#include <beaker/core/decl.hpp>
#include <beaker/core/stmt.hpp>
#include <beaker/core/construction/builder.hpp>

#include <llvm/IR/Type.h>
#include <llvm/IR/Module.h>

#include <iostream>


using namespace beaker;

int
main(int argc, char* argv[])
{
  language lang;
  lang.add_feature<logic::feature>();
  lang.add_feature<numeric::feature>();
  lang.add_feature<core::feature>();

  module mod;
  logic::builder& lb = mod.get_builder<logic::builder>();
  numeric::builder& nb = mod.get_builder<numeric::builder>();
  core::builder& cb = mod.get_builder<core::builder>();

  type& b = lb.get_bool_type();
  type& i32 = nb.get_int_type(32);

  expr& t = lb.get_true_expr();
  expr& z0 = nb.make_int_expr(i32, 0);

  // Some declarations
  decl* vars[] {
    &cb.make_var_decl(cb.get_name("a"), i32),
    &cb.make_var_decl(cb.get_name("b"), b),
    &cb.make_var_decl(cb.get_name("r"), i32)
  };

  typed_decl* d1;
  { // f1 : (int32, bool) -> i32
    decl_seq parms {vars[0], vars[1]};
    decl& ret = *vars[2];
    type& type = cb.get_fn_type(parms, ret);

    // FIXME: Make this return a legit value.
    d1 = &cb.make_fn_decl(cb.get_name("f1"), type, parms, ret);
    mod.add_declaration(*d1);
  }
  expr& f1 = cb.make_ref_expr(d1->get_type(), *d1);

  // Pre-seed a set of expressions.
  expr_seq exprs {
    &cb.make_call_expr(i32, f1, expr_seq{&t, &z0})
  };

  stmt_seq stmts {
    &cb.make_expr_stmt(exprs[0])
  };
  stmt& block = cb.make_block_stmt(stmts);

  // main : () -> int32
  decl_seq parms;
  decl& ret = *vars[2];
  type& type = cb.get_fn_type(parms, ret);
  decl& fn = cb.make_fn_decl(cb.get_name("f"), type, parms, ret, block);
  mod.add_declaration(fn);
  // print(fn);

  // Emit LLVM.
  generator gen("out.ll");
  generate(gen, mod);
  gen.get_module().dump();
}

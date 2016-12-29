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
#include <beaker/logic/decl.hpp>
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
  lang.add_feature<core::feature>();
  lang.add_feature<logic::feature>();
  lang.add_feature<numeric::feature>();

  module mod;
  core::builder& cb = mod.get_builder<core::builder>();
  logic::builder& lb = mod.get_builder<logic::builder>();
  numeric::builder& nb = mod.get_builder<numeric::builder>();

  expr& t = lb.get_true_expr();
  decl& d1 = lb.make_assert_decl(t);
  expr& eq = lb.make_eq_expr(t, t);
  decl& d2 = lb.make_assert_decl(eq);

  stmt_seq stmts {
    &cb.make_decl_stmt(d1),
    &cb.make_decl_stmt(d2),
    &cb.make_ret_stmt(nb.make_int_expr(nb.get_int32_type(), 0))
  };
  stmt& def = cb.make_block_stmt(stmts);


  // Build a wrapper function.
  decl_seq parms;
  decl& ret = cb.make_parm_decl("ret", nb.get_int32_type());
  type& ftype = cb.get_fn_type(parms, ret);
  decl& fn = cb.make_fn_decl("main", ftype, parms, ret, def);
  mod.add_declaration(fn);

  print(fn);

  // Emit LLVM.
  generator gen("out.ll");
  generate(gen, mod);
  gen.get_module().dump();
}

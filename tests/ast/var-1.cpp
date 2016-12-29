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
  lang.add_feature<core::feature>();
  lang.add_feature<logic::feature>();
  lang.add_feature<numeric::feature>();

  module mod;
  core::builder& cb = mod.get_builder<core::builder>();
  logic::builder& lb = mod.get_builder<logic::builder>();
  numeric::builder& nb = mod.get_builder<numeric::builder>();

  type& b = lb.get_bool_type();
  type& i32 = nb.get_int32_type();
  type& i1024 = nb.get_int_type(1024);

  expr& t = lb.get_true_expr();
  expr& z1 = nb.make_int_expr(i32, 5);
  expr& z2 = nb.make_int_expr(i1024, 42);

  // Make some variables and their initializers.
  decl_seq vars {
    &cb.make_var_decl("b1", b, cb.make_zero_init(b)),
    &cb.make_var_decl("b2", b, t),
    &cb.make_var_decl("z1", i32, z1),
    &cb.make_var_decl("z2", i1024, z2),
  };

  stmt_seq stmts {
    &cb.make_decl_stmt(vars[0]),
    &cb.make_decl_stmt(vars[1]),
    &cb.make_decl_stmt(vars[2]),
    &cb.make_decl_stmt(vars[3]),
    &cb.make_ret_stmt(z1),
  };
  stmt& def = cb.make_block_stmt(stmts);

  // Build a wrapper function.
  decl_seq parms;
  decl& ret = cb.make_parm_decl("ret", i32);
  type& ftype = cb.get_fn_type(parms, ret);
  decl& fn = cb.make_fn_decl("main", ftype, parms, ret, def);
  print(fn);

  mod.add_declaration(fn);

  // Emit LLVM.
  generator gen("out.ll");
  generate(gen, mod);
  gen.get_module().dump();
}

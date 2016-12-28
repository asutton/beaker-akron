// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/util/memory.hpp>
#include <beaker/util/symbol_table.hpp>

#include <beaker/base/module.hpp>
#include <beaker/base/generation/generation.hpp>
#include <beaker/base/printing/print.hpp>

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
  lang.add_feature<numeric::feature>();

  module mod;
  core::builder& cb = mod.get_builder<core::builder>();
  numeric::builder& nb = mod.get_builder<numeric::builder>();

  type& i32 = nb.get_int32_type();
  type& i1024 = nb.get_int_type(1024);

  expr& z1 = nb.make_int_expr(i32, 42);
  expr& z2 = nb.make_int_expr(i1024, 17);

  // Make some variables and their initializers.
  decl_seq vars {
    &cb.make_var_decl("a", i32, cb.make_nop_init(i32)), // var int32 z1;
    &cb.make_var_decl("b", i32, cb.make_copy_init(z1)), // var int32 z2 = 42

    &cb.make_var_decl("c", i1024, cb.make_nop_init(i1024)), // var int1024 z3;
    &cb.make_var_decl("d", i1024, cb.make_copy_init(z2)), // var int1024 z4 = 17
  };

  // References to variables.
  expr_seq vnames {
    &cb.make_ref_expr(vars[0]),
    &cb.make_ref_expr(vars[1]),
    &cb.make_ref_expr(vars[2]),
    &cb.make_ref_expr(vars[3]),
  };

  stmt_seq stmts {
    &cb.make_decl_stmt(vars[0]),
    &cb.make_decl_stmt(vars[1]),
    &cb.make_decl_stmt(vars[2]),
    &cb.make_decl_stmt(vars[3]),
    &cb.make_expr_stmt( // z1 = z2  ~> ref z1 = deref ref 2
      cb.make_assign_expr(
        vnames[0], 
        cb.make_deref_expr(vnames[1])
      )
    ),
    &cb.make_ret_stmt( // return = copy deref ref z1
      cb.make_copy_init(
        cb.make_deref_expr(vnames[0])
      )
    )
  };
  stmt& block = cb.make_block_stmt(stmts);


  // Build a wrapper function.
  decl_seq parms;
  decl& ret = cb.make_parm_decl(cb.get_name("ret"), i32);
  type& ftype = cb.get_fn_type(parms, ret);
  decl& fn = cb.make_fn_decl("main", ftype, parms, ret, block);
  mod.add_declaration(fn);

  print(fn);

  // Emit LLVM.
  generator gen("out.ll");
  generate(gen, mod);
  gen.get_module().dump();
}

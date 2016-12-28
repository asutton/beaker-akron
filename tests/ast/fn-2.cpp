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
  type& i1024 = nb.get_int_type(1024);

  expr& t = lb.get_true_expr();
  expr& z = nb.make_int_expr(i32, 42);
  expr& big = nb.make_int_expr(i1024, 5);

  // Some declarations.
  decl* vars[] {
    &cb.make_parm_decl(cb.get_name("a"), i32),
    &cb.make_parm_decl(cb.get_name("b"), b),
    &cb.make_parm_decl(cb.get_name("c"), i1024),
    &cb.make_parm_decl(cb.get_name("r"), i32)
  };

  typed_decl* d1;
  { // f1 : (int32, bool) -> i32
    decl_seq parms {vars[0], vars[1], vars[2]};
    decl& ret = *vars[3];
    type& type = cb.get_fn_type(parms, ret);

    // expr& val = nb.make_int_expr(i32, 3); // = 3
    expr& ref = cb.make_ref_expr(parms[0]); // = ref a
    expr& val = cb.make_deref_expr(ref);
    expr& def = cb.make_copy_init(val);
    
    d1 = &cb.make_fn_decl(cb.get_name("f1"), type, parms, ret, def);
    mod.add_declaration(*d1);
  }

  // Generate the call expression.
  expr& f1 = cb.make_ref_expr(*d1);
  expr_seq args {
    &cb.make_copy_init(z),
    &cb.make_copy_init(t),
    &cb.make_copy_init(big)
  };
  expr& call = cb.make_call_expr(i32, f1, args);

  // Generate the function definition (note that copy of the call).
  stmt_seq stmts {
    &cb.make_ret_stmt(cb.make_copy_init(call))
  };
  stmt& def = cb.make_block_stmt(stmts);

  // main : () -> int32
  decl_seq parms;
  decl& ret = *vars[3];
  type& type = cb.get_fn_type(parms, ret);
  decl& fn = cb.make_fn_decl(cb.get_name("main"), type, parms, ret, def);
  mod.add_declaration(fn);
  print(mod);

  // Emit LLVM.
  generator gen("out.ll");
  generate(gen, mod);
  gen.get_module().dump();
}

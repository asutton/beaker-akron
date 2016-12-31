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

  // Get types.
  type& b = lb.get_bool_type();
  type& i32 = nb.get_int32_type();
  type& i1024 = nb.get_int_type(1024);

  // Get some initial values.
  expr& t = lb.get_true_expr();
  expr& z = nb.make_int_expr(i32, 5);

  // A function
  typed_decl* d1;
  { // f1 : (bool, int32) -> int32
    decl* vars[] {
      &cb.make_parm_decl("a", b),
      &cb.make_parm_decl("b", i32),
      &cb.make_parm_decl("r", i32)
    };

    decl_seq parms {vars[0], vars[1]};
    decl& ret = *vars[2];
    type& type = cb.get_fn_type(parms, ret);

    expr& ref = cb.make_ref_expr(parms[1]); // ref a
    expr& val = cb.make_deref_expr(ref); // deref ref a
    expr& def = cb.make_copy_init(val); // = copy deref ref a
    
    d1 = &cb.make_fn_decl(cb.get_name("f1"), type, parms, ret, def);
    mod.add_declaration(*d1);
  }


  // Make some variables and their initializers.
  decl_seq vars {
    &cb.make_var_decl( // var (bool,int32)->int32 f = ref f1
      "f", d1->get_type(), cb.make_ref_expr(*d1)
    ), 
  };

  stmt_seq stmts {
    &cb.make_decl_stmt(vars[0]),
    &cb.make_ret_stmt(z)
  };
  stmt& def = cb.make_block_stmt(stmts);


  // Build a wrapper function.
  decl_seq parms;
  decl& ret = cb.make_parm_decl("ret", cb.get_void_type());
  type& ftype = cb.get_fn_type(parms, ret);
  decl& fn = cb.make_fn_decl("main", ftype, parms, ret, def);
  mod.add_declaration(fn);

  print(fn);

  // Emit LLVM.
  generator gen("out.ll");
  generate(gen, mod);
  gen.get_module().dump();
}

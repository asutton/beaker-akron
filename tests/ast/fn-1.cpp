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

  // Define the module.
  module mod;
  
  logic::builder& lb = mod.get_builder<logic::builder>();
  numeric::builder& nb = mod.get_builder<numeric::builder>();
  core::builder& cb = mod.get_builder<core::builder>();

  type& b = lb.get_bool_type();
  type& i32 = nb.get_int_type(32);
  type& i1024 = nb.get_int_type(1024);

  // Some declarations
  decl* vars[] {
    &cb.make_parm_decl(cb.get_name("a"), i32),
    &cb.make_parm_decl(cb.get_name("b"), b),
    &cb.make_parm_decl(cb.get_name("c"), i1024),
    &cb.make_parm_decl(cb.get_name("r"), i32)
  };

  { // Beaker: f1(int32, bool, int1024) -> int32
    // LLVM: i32 f1(i32, bool, int1024*)
    decl_seq parms {vars[0], vars[1], vars[2]};
    decl& ret = *vars[3];
    type& type = cb.get_fn_type(parms, ret);

    expr& z = nb.make_int_expr(i32, 0);
    expr& c = cb.make_copy_init(z);
    stmt_seq ss {
      &cb.make_ret_stmt(c)
    };
    stmt& def = cb.make_block_stmt(ss);

    decl& fn = cb.make_fn_decl(cb.get_name("f1"), type, parms, ret, def);
    mod.add_declaration(fn);
  }

  { // Beaker: f2(bool) -> int1024
    // LLVM: void f2(int1024*, bool)
    decl_seq parms {vars[1]};
    decl& ret = *vars[2];
    type& type = cb.get_fn_type(parms, ret);

    expr& z = nb.make_int_expr(i1024, 5);
    expr& c = cb.make_copy_init(z);
    stmt_seq ss {
      &cb.make_ret_stmt(c)
    };
    stmt& def = cb.make_block_stmt(ss);

    decl& fn = cb.make_fn_decl(cb.get_name("f2"), type, parms, ret, def);
    mod.add_declaration(fn);
  }

  // Defined by expression.
  { // Beaker: f3() -> bool = true
    // LLVM: bool f3()
    decl_seq parms {};
    decl& ret = *vars[1]; // A boolean value
    type& type = cb.get_fn_type(parms, ret);

    // = copy true.
    expr& t = lb.get_true_expr();
    expr& def = cb.make_copy_init(t);

    decl& fn = cb.make_fn_decl(cb.get_name("f3"), type, parms, ret, def);
    mod.add_declaration(fn);
  }

  // std::cout << "-- input --\n";
  // print(mod);

  // Emit LLVM.
  generator gen("out.ll");
  generate(gen, mod);
  gen.get_module().dump();
}

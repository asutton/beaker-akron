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

  // Get reference types.
  type& rb = cb.get_ref_type(b);
  type& ri32 = cb.get_ref_type(i32);
  type& ri1024 = cb.get_ref_type(i1024);

  // Get some initial values.
  expr& t = lb.get_true_expr();
  expr& z1 = nb.make_int_expr(i32, 5);
  expr& z2 = nb.make_int_expr(i1024, 42);

  // Make some variables and their initializers.
  decl_seq vars {
    &cb.make_var_decl("b", b, cb.make_copy_init(t)), // var bool b = copy true
    &cb.make_var_decl("z1", i32, cb.make_copy_init(z1)), // var int32 z1 = copy 5
    &cb.make_var_decl("z2", i1024, cb.make_copy_init(z2)), // var in1024 z2 = copy 42
  };

  // For convenience... the names declared variables
  expr_seq vnames {
    &cb.make_ref_expr(vars[0]), // ref b
    &cb.make_ref_expr(vars[1]), // ref z1
    &cb.make_ref_expr(vars[2]) // ref z2
  };

  // Make some references and their initializers.
  decl_seq refs {
    &cb.make_var_decl("rb", rb, cb.make_ref_init(vnames[0])), // var bool& rb = ref b
    &cb.make_var_decl("rz1", ri32, cb.make_ref_init(vnames[1])), // var int32& rz1 = ref z1
    &cb.make_var_decl("rz2", ri1024, cb.make_ref_init(vnames[2])) // var int1024& rz2 = ref z2
  };

  // For convenience... the names of references.
  expr_seq rnames {
    &cb.make_ref_expr(refs[0]), // ref rb
    &cb.make_ref_expr(refs[1]), // ref rz1
    &cb.make_ref_expr(refs[2]) // ref rz2
  };

  // Generate the values of references.
  expr_seq exprs {
    &cb.make_deref_expr(rnames[0]), // deref ref rb
    &cb.make_deref_expr(rnames[1]), // deref ref rz1
    &cb.make_deref_expr(rnames[2]) // deref ref rz2
  };

  stmt_seq stmts {
    &cb.make_decl_stmt(vars[0]),
    &cb.make_decl_stmt(vars[1]),
    &cb.make_decl_stmt(vars[2]),

    &cb.make_decl_stmt(refs[0]),
    &cb.make_decl_stmt(refs[1]),    
    &cb.make_decl_stmt(refs[2]),

    &cb.make_expr_stmt(exprs[0]),
    &cb.make_expr_stmt(exprs[1]),
    &cb.make_expr_stmt(exprs[2]),

    &cb.make_decl_stmt(
      cb.make_var_decl(
        "x", i1024, cb.make_copy_init(exprs[2]) // var int1024 x = copy deref ref rz2
      )
    )
  };
  stmt& def = cb.make_block_stmt(stmts);


  // Build a wrapper function.
  decl_seq parms;
  decl& ret = cb.make_parm_decl("ret", cb.get_void_type());
  type& ftype = cb.get_fn_type(parms, ret);
  decl& fn = cb.make_fn_decl(cb.get_name("f1"), ftype, parms, ret, def);
  mod.add_declaration(fn);

  print(fn);

  // Emit LLVM.
  generator gen("out.ll");
  generate(gen, mod);
  gen.get_module().dump();
}

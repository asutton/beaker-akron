// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "lang.hpp"


using namespace beaker;

int 
main()
{
  symbol_table syms;
  system_lang lang(syms);
}

#if 0
#include <beaker/util/memory.hpp>
#include <beaker/util/symbol_table.hpp>

#include <beaker/base/lang.hpp>
#include <beaker/base/module.hpp>
#include <beaker/base/printing/print.hpp>
#include <beaker/base/generation/generation.hpp>

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

#include <llvm/IR/Module.h>

#include <iostream>


using namespace beaker;


static void
check_canonical_types(module& mod)
{
  logic::builder& lb = mod.get_builder<logic::builder>();

  // Check canonical types.
  type& b1 = lb.get_bool_type();
  type& b2 = lb.get_bool_type();
  assert(&b1 == &b2);
}


static void
check_canonical_values(module& mod)
{
  logic::builder& lb = mod.get_builder<logic::builder>();

  // Check canonical expressions.
  expr& t1 = lb.get_true_expr();
  expr& t2 = lb.get_true_expr();
  assert(&t1 == &t2);

  expr& f1 = lb.get_false_expr();
  expr& f2 = lb.get_false_expr();
  assert(&f1 == &f2);
}

// Build an assertion that e1 op e2 <=> r.
static stmt*
assert_eq(module& mod, int op, expr& e1, expr& e2, expr& r)
{
  logic::builder& lb = mod.get_builder<logic::builder>();
  core::builder& cb = mod.get_builder<core::builder>();

  expr* e;
  switch (op) {
    case logic::and_expr_kind:
      e = &lb.make_and_expr(e1, e2); 
      break;
    case logic::or_expr_kind:
      e = &lb.make_or_expr(e1, e2);
      break;
    case logic::imp_expr_kind:
      e = &lb.make_imp_expr(e1, e2);
      break;
    case logic::eq_expr_kind:
      e = &lb.make_eq_expr(e1, e2);
      break;
    case logic::and_then_expr_kind:
      e = &lb.make_and_then_expr(e1, e2);
      break;
    case logic::or_else_expr_kind:
      e = &lb.make_or_else_expr(e1, e2);
      break;
    default:
      assert(false && "invalid expression");
  }
  expr& eq = lb.make_eq_expr(*e, r);
  decl& d = lb.make_assert_decl(eq);
  return &cb.make_decl_stmt(d);
}

// Build an assertion that op e1 <=> r.
static stmt*
assert_eq(module& mod, int op, expr& e, expr& r)
{
  logic::builder& lb = mod.get_builder<logic::builder>();
  core::builder& cb = mod.get_builder<core::builder>();

  assert(op == logic::not_expr_kind);
  expr& eq = lb.make_eq_expr(lb.make_not_expr(e), r);
  decl& d = lb.make_assert_decl(eq);
  return &cb.make_decl_stmt(d);
}


int
main(int argc, char* argv[])
{
  language lang;
  lang.add_feature<logic::feature>();
  lang.add_feature<numeric::feature>();
  lang.add_feature<core::feature>();

  module mod;
  
  check_canonical_types(mod);
  check_canonical_values(mod);

  logic::builder& lb = mod.get_builder<logic::builder>();
  numeric::builder& nb = mod.get_builder<numeric::builder>();
  core::builder& cb = mod.get_builder<core::builder>();

  type& b = lb.get_bool_type();

  // Store true and false values as variables to suppress constant
  // folding during code generation (but not optimization).
  decl_seq vars {
    &cb.make_var_decl("t", b, lb.get_true_expr()),
    &cb.make_var_decl("f", b, lb.get_false_expr()),
  };
  expr& t = cb.make_deref_expr(cb.make_ref_expr(vars[0]));
  expr& f = cb.make_deref_expr(cb.make_ref_expr(vars[1]));

  stmt_seq stmts {
    &cb.make_decl_stmt(vars[0]),
    &cb.make_decl_stmt(vars[1]),

    // Truth table for !e
    assert_eq(mod, logic::not_expr_kind, t, f),
    assert_eq(mod, logic::not_expr_kind, f, t),

    // Truth table for e1 & e2
    assert_eq(mod, logic::and_expr_kind, t, t, t),
    assert_eq(mod, logic::and_expr_kind, t, f, f),
    assert_eq(mod, logic::and_expr_kind, f, t, f),
    assert_eq(mod, logic::and_expr_kind, f, f, f),

    // Truth table for e1 | e2
    assert_eq(mod, logic::or_expr_kind, t, t, t),
    assert_eq(mod, logic::or_expr_kind, t, f, t),
    assert_eq(mod, logic::or_expr_kind, f, t, t),
    assert_eq(mod, logic::or_expr_kind, f, f, f),

    // Truth table for e1 => e2
    assert_eq(mod, logic::imp_expr_kind, t, t, t),
    assert_eq(mod, logic::imp_expr_kind, t, f, f),
    assert_eq(mod, logic::imp_expr_kind, f, t, t),
    assert_eq(mod, logic::imp_expr_kind, f, f, t),

    // Truth table for e1 <=> e2
    assert_eq(mod, logic::eq_expr_kind, t, t, t),
    assert_eq(mod, logic::eq_expr_kind, t, f, f),
    assert_eq(mod, logic::eq_expr_kind, f, t, f),
    assert_eq(mod, logic::eq_expr_kind, f, f, t),

    // Truth table for e1 & e2
    assert_eq(mod, logic::and_then_expr_kind, t, t, t),
    assert_eq(mod, logic::and_then_expr_kind, t, f, f),
    assert_eq(mod, logic::and_then_expr_kind, f, t, f),
    assert_eq(mod, logic::and_then_expr_kind, f, f, f),

    // Truth table for e1 | e2
    assert_eq(mod, logic::or_else_expr_kind, t, t, t),
    assert_eq(mod, logic::or_else_expr_kind, t, f, t),
    assert_eq(mod, logic::or_else_expr_kind, f, t, t),
    assert_eq(mod, logic::or_else_expr_kind, f, f, f),

    &cb.make_ret_stmt(nb.make_int_expr(nb.get_int32_type(), 0)) // return 0
  };
  stmt& def = cb.make_block_stmt(stmts);

  // Build a main function.
  decl_seq parms;
  decl& ret = cb.make_parm_decl("ret", nb.get_int32_type());
  type& ftype = cb.get_fn_type(parms, ret);
  decl& fn = cb.make_fn_decl("main", ftype, parms, ret, def);
  mod.add_declaration(fn);

  print(mod);

  // Emit LLVM.
  generator gen("out.ll");
  generate(gen, mod);
  gen.get_module().dump();
}
#endif
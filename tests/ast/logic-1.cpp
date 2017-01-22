// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "lang.hpp"
#include "build.hpp"

#include <beaker/base/printing/print.hpp>
#include <beaker/base/generation/generation.hpp>
#include <beaker/base/serialization/writer.hpp>


using namespace beaker;

static void
check_canonical_types()
{
  // Check canonical types.
  builder& build = global_builder::get();
  type& b1 = build.get_bool_type();
  type& b2 = build.get_bool_type();
  assert(&b1 == &b2);
}

#if 0
// Build an assertion statement that e1 op e2 <=> r.
static stmt*
assert_eq(builder& build, int op, expr& e1, expr& e2, expr& r)
{
  expr* e;
  switch (op) {
    case sys_bool::and_expr_kind:
      e = &build.make_bool_and_expr(e1, e2); 
      break;
    case sys_bool::or_expr_kind:
      e = &build.make_bool_or_expr(e1, e2);
      break;
    case sys_bool::imp_expr_kind:
      e = &build.make_bool_imp_expr(e1, e2);
      break;
    case sys_bool::eq_expr_kind:
      e = &build.make_bool_eq_expr(e1, e2);
      break;
    case sys_bool::and_then_expr_kind:
      e = &build.make_and_then_expr(e1, e2);
      break;
    case sys_bool::or_else_expr_kind:
      e = &build.make_or_else_expr(e1, e2);
      break;
    default:
      assert(false && "invalid expression");
  }
  expr& eq = build.make_bool_eq_expr(*e, r);
  decl& d = build.make_assert_decl(eq);
  return &build.make_decl_stmt(d);
}

// Build an assertion statement that op e1 <=> r.
static stmt*
assert_eq(builder& build, int op, expr& e, expr& r)
{
  assert(op == sys_bool::not_expr_kind);
  expr& e0 = build.make_bool_not_expr(e);
  expr& eq = build.make_bool_eq_expr(e0, r);
  decl& d = build.make_assert_decl(eq);
  return &build.make_decl_stmt(d);
}
#endif

int 
main()
{
  symbol_table syms;
  system_lang lang(syms);
  module mod(lang);
  
  // Create and establish a global builder context.
  builder build(mod);
  global_builder global(build);

  check_canonical_types();

  type& b = build.get_bool_type();

  decl& main_ = build.make_main();

  out<decl> t;
  out<decl> f;
  add_stmts(main_)
    .var(t, "t", b, true_())
    .var(f, "f", b, false_())
    // // Truth table for !
    .check(!f == t)
    .check(!t == f)
    // // Truth table for &
    .check((t & t) == t)
    .check((t & f) == f)
    .check((f & t) == f)
    .check((f & f) == f)
    // Truth table for |
    .check((t | t) == t)
    .check((t | f) == t)
    .check((f | t) == t)
    .check((f | f) == f)
  ;
#if 0
  expr& t = build.make_deref_expr(build.make_ref_expr(vars[0]));
  expr& f = build.make_deref_expr(build.make_ref_expr(vars[1]));

  stmt_seq stmts {
    &build.make_decl_stmt(vars[0]),
    &build.make_decl_stmt(vars[1]),

    // Truth table for !e
    assert_eq(build, sys_bool::not_expr_kind, t, f),
    assert_eq(build, sys_bool::not_expr_kind, f, t),

    // Truth table for e1 & e2
    assert_eq(build, sys_bool::and_expr_kind, t, t, t),
    assert_eq(build, sys_bool::and_expr_kind, t, f, f),
    assert_eq(build, sys_bool::and_expr_kind, f, t, f),
    assert_eq(build, sys_bool::and_expr_kind, f, f, f),

    // Truth table for e1 | e2
    assert_eq(build, sys_bool::or_expr_kind, t, t, t),
    assert_eq(build, sys_bool::or_expr_kind, t, f, t),
    assert_eq(build, sys_bool::or_expr_kind, f, t, t),
    assert_eq(build, sys_bool::or_expr_kind, f, f, f),

    // Truth table for e1 => e2
    assert_eq(build, sys_bool::imp_expr_kind, t, t, t),
    assert_eq(build, sys_bool::imp_expr_kind, t, f, f),
    assert_eq(build, sys_bool::imp_expr_kind, f, t, t),
    assert_eq(build, sys_bool::imp_expr_kind, f, f, t),

    // Truth table for e1 <=> e2
    assert_eq(build, sys_bool::eq_expr_kind, t, t, t),
    assert_eq(build, sys_bool::eq_expr_kind, t, f, f),
    assert_eq(build, sys_bool::eq_expr_kind, f, t, f),
    assert_eq(build, sys_bool::eq_expr_kind, f, f, t),

    // Truth table for e1 & e2
    assert_eq(build, sys_bool::and_then_expr_kind, t, t, t),
    assert_eq(build, sys_bool::and_then_expr_kind, t, f, f),
    assert_eq(build, sys_bool::and_then_expr_kind, f, t, f),
    assert_eq(build, sys_bool::and_then_expr_kind, f, f, f),

    // Truth table for e1 | e2
    assert_eq(build, sys_bool::or_else_expr_kind, t, t, t),
    assert_eq(build, sys_bool::or_else_expr_kind, t, f, t),
    assert_eq(build, sys_bool::or_else_expr_kind, f, t, t),
    assert_eq(build, sys_bool::or_else_expr_kind, f, f, f),

    &build.make_return(0)
  };
  #endif

  archive_writer ar;
  write_module(ar, mod);
  ar.save("out.bkm");

  generator gen("a.ll");
  generate(gen, mod);
  gen.print();
}

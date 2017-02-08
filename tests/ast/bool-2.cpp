// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "util.hpp"

#include <beaker/sys.bool/ast.hpp>


using namespace beaker;

int 
main()
{
  symbol_table syms;
  language lang(syms, {
    new sys_bool::feature(),
  });
  module mod(lang);
  auto& bb = mod.get_builder<sys_bool::feature>();

  // Useful constants
  const value one(1);
  const value zero(0);

  // True and false.
  auto& t = bb.make_true_expr();
  auto& f = bb.make_false_expr();
  check_value(lang, t, one);
  check_value(lang, f, zero);

  // e1 & e2
  check_value(lang, bb.make_and_expr(t, t), one);
  check_value(lang, bb.make_and_expr(t, f), zero);
  check_value(lang, bb.make_and_expr(f, t), zero);
  check_value(lang, bb.make_and_expr(f, f), zero);

  // e1 | e2
  check_value(lang, bb.make_or_expr(t, t), one);
  check_value(lang, bb.make_or_expr(t, f), one);
  check_value(lang, bb.make_or_expr(f, t), one);
  check_value(lang, bb.make_or_expr(f, f), zero);

  // e1 ^ e2
  check_value(lang, bb.make_xor_expr(t, t), zero);
  check_value(lang, bb.make_xor_expr(t, f), one);
  check_value(lang, bb.make_xor_expr(f, t), one);
  check_value(lang, bb.make_xor_expr(f, f), zero);

  // !e1
  check_value(lang, bb.make_not_expr(t), zero);
  check_value(lang, bb.make_not_expr(f), one);

  // e1 => e2
  check_value(lang, bb.make_imp_expr(t, t), one);
  check_value(lang, bb.make_imp_expr(t, f), zero);
  check_value(lang, bb.make_imp_expr(f, t), one);
  check_value(lang, bb.make_imp_expr(f, f), one);

  // e1 <=> e2
  check_value(lang, bb.make_eq_expr(t, t), one);
  check_value(lang, bb.make_eq_expr(t, f), zero);
  check_value(lang, bb.make_eq_expr(f, t), zero);
  check_value(lang, bb.make_eq_expr(f, f), one);

  // e1 && e2
  check_value(lang, bb.make_and_then_expr(t, t), one);
  check_value(lang, bb.make_and_then_expr(t, f), zero);
  check_value(lang, bb.make_and_then_expr(f, t), zero);
  check_value(lang, bb.make_and_then_expr(f, f), zero);

  // e1 || e2
  check_value(lang, bb.make_or_else_expr(t, t), one);
  check_value(lang, bb.make_or_else_expr(t, f), one);
  check_value(lang, bb.make_or_else_expr(f, t), one);
  check_value(lang, bb.make_or_else_expr(f, f), zero);

  // e1 ? e2 : e3
  check_value(lang, bb.make_if_expr(t, t, f), one);
  check_value(lang, bb.make_if_expr(f, t, f), zero);

  // assert(e1)
  check_value(lang, bb.make_assert_expr(t), one);
  check_error(lang, bb.make_assert_expr(f));
}

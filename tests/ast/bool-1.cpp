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

  check_identical_terms(lang, bb.get_bool_type(), bb.get_bool_type());

  auto& t = bb.make_true_expr();
  auto& f = bb.make_false_expr();
  check_equal_terms(lang, t, bb.make_true_expr());
  check_equal_terms(lang, f, bb.make_false_expr());
  check_different_terms(lang, t, f);

  auto& and1 = bb.make_and_expr(t, f);
  check_equal_terms(lang, and1, bb.make_and_expr(t, f));
  check_different_terms(lang, and1, bb.make_and_expr(f, t));

  auto& or1 = bb.make_or_expr(t, f);
  check_equal_terms(lang, or1, bb.make_or_expr(t, f));
  check_different_terms(lang, or1, bb.make_or_expr(f, t));

  auto& xor1 = bb.make_xor_expr(t, f);
  check_equal_terms(lang, xor1, bb.make_xor_expr(t, f));
  check_different_terms(lang, xor1, bb.make_xor_expr(f, t));

  auto& imp1 = bb.make_imp_expr(t, f);
  check_equal_terms(lang, imp1, bb.make_imp_expr(t, f));
  check_different_terms(lang, imp1, bb.make_imp_expr(f, t));

  auto& eq1 = bb.make_eq_expr(t, f);
  check_equal_terms(lang, eq1, bb.make_eq_expr(t, f));
  check_different_terms(lang, eq1, bb.make_eq_expr(f, t));

  auto& not1 = bb.make_not_expr(t);
  check_equal_terms(lang, not1, bb.make_not_expr(t));
  check_different_terms(lang, not1, bb.make_not_expr(f));

  auto& if1 = bb.make_if_expr(t, t, f);
  check_equal_terms(lang, if1, bb.make_if_expr(t, t, f));
  check_different_terms(lang, if1, bb.make_if_expr(f, t, f));
  check_different_terms(lang, if1, bb.make_if_expr(t, f, t));

  auto& andif1 = bb.make_and_then_expr(t, f);
  check_equal_terms(lang, andif1, bb.make_and_then_expr(t, f));
  check_different_terms(lang, andif1, bb.make_and_then_expr(f, t));

  auto& orif1 = bb.make_or_else_expr(t, f);
  check_equal_terms(lang, orif1, bb.make_or_else_expr(t, f));
  check_different_terms(lang, orif1, bb.make_or_else_expr(f, t));
}

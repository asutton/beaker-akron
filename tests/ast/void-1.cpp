// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "util.hpp"

#include <beaker/sys.void/ast.hpp>

int 
main()
{
  symbol_table syms;
  language lang(syms, {
    new sys_void::feature(),
  });
  module mod(lang);
  auto& vb = mod.get_builder<sys_void::feature>();

  check_identical_terms(lang, vb.get_void_type(), vb.get_void_type());

  auto& nop = vb.make_nop_expr();
  auto& trap = vb.make_trap_expr();
  check_equal_terms(lang, nop, vb.make_nop_expr());
  check_equal_terms(lang, trap, vb.make_trap_expr());
  auto& void_nop = vb.make_void_expr(nop);
  check_equal_terms(lang, void_nop, vb.make_void_expr(nop));
  auto& void_trap = vb.make_void_expr(trap);
  check_different_terms(lang, void_nop, void_trap);
}

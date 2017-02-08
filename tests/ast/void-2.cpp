// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "util.hpp"

#include <beaker/sys.void/ast.hpp>


using namespace beaker;

int 
main()
{
  symbol_table syms;
  language lang(syms, {
    new sys_void::feature(),
  });
  module mod(lang);

  auto& vb = mod.get_builder<sys_void::feature>();

  // nop ~> void
  auto& nop = vb.make_nop_expr();
  check_value(lang, nop, {});

  // void(nop) ~> void
  auto& void_nop = vb.make_void_expr(nop);
  check_value(lang, void_nop, {});

  // trap ~> error
  auto& trap = vb.make_trap_expr();
  check_error(lang, trap);

  // void(trap) ~> error
  auto& void_trap = vb.make_void_expr(trap);
  check_error(lang, void_trap);
}

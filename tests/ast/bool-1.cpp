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
  check_equal_terms(lang, t, bb.make_true_expr());
  
  auto& f = bb.make_false_expr();
  check_equal_terms(lang, f, bb.make_false_expr());

  print(lang, bb.get_bool_type());
  print(lang, t);
  print(lang, f);
  // print(lang, nop);
  // print(lang, trap);
  // print(lang, void_nop);
  // print(lang, void_trap);
}

// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "util.hpp"

#include <beaker/sys.bool/ast.hpp>
#include <beaker/sys.int/ast.hpp>


int 
main()
{
  symbol_table syms;
  language lang(syms, {
    new sys_bool::feature(),
    new sys_int::feature(),
  });
  module mod(lang);
  auto& ib = mod.get_builder<sys_int::feature>();

  auto& int8 = ib.get_int8_type();
  // auto& mod8 = ib.get_mod8_type();

  // Don't allow construction of out-of-range values.
  // ib.make_int_expr(int8, 128);
  // ib.make_int_expr(int8, -129);

  auto& e1 = ib.make_add_expr( // 1 + 2 ~> 3
    ib.make_int_expr(int8, 1),
    ib.make_int_expr(int8, 2)
  );
  check_value(lang, e1, value(3));

  auto& e2 = ib.make_add_expr( // 127 + 1 ~> error
    ib.make_int_expr(int8, int8.max()),
    ib.make_int_expr(int8, 1)
  );
  check_error(lang, e2);
}

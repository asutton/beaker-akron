// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/util/memory.hpp>
#include <beaker/util/symbol_table.hpp>

#include <beaker/base/lang.hpp>
#include <beaker/base/module.hpp>
#include <beaker/base/printing/print.hpp>

#include <beaker/logic/lang.hpp>
#include <beaker/logic/type.hpp>
#include <beaker/logic/expr.hpp>
#include <beaker/logic/construction/builder.hpp>

#include <iostream>


using namespace beaker;

int
main(int argc, char* argv[])
{
  language lang;
  lang.add_feature<logic::feature>();

  sequential_allocator<> alloc;
  symbol_table syms;
  module mod(alloc, syms);
  
  logic::builder& lb = mod.get_builder<logic::builder>();

  // Check canonical types.
  type& b1 = lb.get_bool_type();
  type& b2 = lb.get_bool_type();
  assert(&b1 == &b2);
  print(b1);

  // Check canonical expressions.
  expr& t1 = lb.get_true_expr();
  expr& t2 = lb.get_true_expr();
  assert(&t1 == &t2);
  print(t1);

  expr& f1 = lb.get_false_expr();
  expr& f2 = lb.get_false_expr();
  assert(&f1 == &f2);
  print(f2);
}

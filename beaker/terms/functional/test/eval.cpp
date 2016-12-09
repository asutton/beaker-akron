// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/terms/logic.hpp>

#include <iostream>


using namespace beaker;

int 
main()
{
  logic::initialize();

  logic::context cxt;

  type& b = cxt.get_bool_type();
  expr& t = cxt.get_true_expr();
  expr& f = cxt.get_false_expr();
  expr& e1 = cxt.make_and_expr(b, t, f);
  expr& e2 = cxt.make_not_expr(b, e1);
  value v1 = logic::evaluate(e1);
  value v2 = logic::evaluate(e2);
  std::cout << v1.get_integer() << '\n';
  std::cout << v2.get_integer() << '\n';
}

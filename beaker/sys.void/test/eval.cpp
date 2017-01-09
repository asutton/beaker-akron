// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/terms/logic.hpp>

#include <iostream>


using namespace beaker;

int 
main()
{
  sequential_allocator<> alloc;
  logic::builder build(alloc);

  type& b = build.get_bool_type();
  expr& t = build.get_true_expr();
  expr& f = build.get_false_expr();
  expr& e1 = build.make_and_expr(b, t, f);
  expr& e2 = build.make_not_expr(b, e1);
  
  evaluator eval;
  value v1 = evaluate(eval, e1);
  value v2 = evaluate(eval, e2);
  std::cout << v1.get_integer() << '\n';
  std::cout << v2.get_integer() << '\n';
}

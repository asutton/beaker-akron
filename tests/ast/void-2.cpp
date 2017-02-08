// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include <beaker/base/lang.hpp>
#include <beaker/base/module.hpp>
#include <beaker/base/printing/print.hpp>
#include <beaker/base/evaluation/evaluate.hpp>
#include <beaker/util/symbol_table.hpp>

#include <beaker/sys.void/ast.hpp>


using namespace beaker;

// Check that e evaluates to the void value.
void 
check_void(evaluator& eval, const expr& e)
{
  value v = evaluate(eval, e);
  assert(v.is_void());
}

// Check that the evaluation of e fails.
void 
check_error(evaluator& eval, const expr& e)
{
  int f = false;
  try {
    evaluate(eval, e);
  } catch (evaluation_error& err) {
    f = true;
  }
  assert(f);
}

int 
main()
{
  symbol_table syms;
  language lang(syms, {
    new sys_void::feature(),
  });
  module mod(lang);
  evaluator eval(lang);

  auto& vb = mod.get_builder<sys_void::feature>();

  // nop ~> void
  auto& nop = vb.make_nop_expr();
  check_void(eval, nop);

  // void(nop) ~> void
  auto& void_nop = vb.make_void_expr(nop);
  check_void(eval, void_nop);

  // trap ~> error
  auto& trap = vb.make_trap_expr();
  check_error(eval, trap);

  // void(trap) ~> error
  auto& void_trap = vb.make_void_expr(trap);
  check_error(eval, void_trap);
}

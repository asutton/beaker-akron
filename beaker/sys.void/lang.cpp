// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "lang.hpp"
#include "type.hpp"
#include "expr.hpp"
#include "build.hpp"
#include "printing/print.hpp"
#include "evaluation/evaluate.hpp"
// #include "generation/gen.hpp"
// #include "serialization/write.hpp"


namespace beaker {
namespace sys_void {

void 
feature::add_terms(language& lang)
{
  auto& types = lang.get_types();
  types.add_derivation<base_type, void_type>();
  
  auto& exprs = lang.get_expressions();
  exprs.add_derivation<nullary_expr, nop_expr>();
  exprs.add_derivation<nullary_expr, trap_expr>();
  exprs.add_derivation<unary_expr, void_expr>();
}

void 
feature::add_semantics(language& lang)
{
  auto& print = lang.get_algorithm<print_algorithm>();
  print.types->add_overrider<void_type>(print_void_type);
  print.exprs->add_overrider<nop_expr>(print_nop_expr);
  print.exprs->add_overrider<void_expr>(print_void_expr);
  print.exprs->add_overrider<trap_expr>(print_trap_expr);

  auto& eval = lang.get_algorithm<evaluate_algorithm>();
  eval.exprs->add_overrider<nop_expr>(evaluate_nop_expr);
  eval.exprs->add_overrider<void_expr>(evaluate_void_expr);
  eval.exprs->add_overrider<trap_expr>(evaluate_trap_expr);
}

factory&
feature::make_builder(module& m) const
{
  return *new builder(m);
}


} // namespace sys_void
} // namespace beaker

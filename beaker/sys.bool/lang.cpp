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
namespace sys_bool {

void 
feature::add_terms(language& lang)
{
  auto& types = lang.get_types();
#define def_type(T, B) \
  types.add_derivation<B ## _type, T ## _type>();
#include "type.def"

  auto& exprs = lang.get_expressions();
#define def_expr(E, B) \
  exprs.add_derivation<B ## _expr, E ## _expr>();
#include "expr.def"
}

void 
feature::add_semantics(language& lang)
{
  // Add print overriders.
  auto& print = lang.get_algorithm<print_algorithm>();
#define def_type(T, B) \
  print.types->add_overrider<T ## _type>(print_ ## T ## _type);
#include "type.def"
#define def_expr(E, B) \
  print.exprs->add_overrider<E ## _expr>(print_ ## E ## _expr);
#include "expr.def"

  // Add eval overriders.
  auto& eval = lang.get_algorithm<evaluate_algorithm>();
#define def_expr(E, B) \
  eval.exprs->add_overrider<E ## _expr>(evaluate_ ## E ## _expr);
#include "expr.def"
}

beaker::builder&
feature::make_builder(module& m) const
{
  return *new builder(m);
}

} // namespace sys_bool
} // namespace beaker

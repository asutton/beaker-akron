// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "lang.hpp"
#include "type.hpp"
#include "expr.hpp"
#include "build.hpp"

// #include "printing/print.hpp"
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
}

beaker::builder&
feature::make_builder(module& m) const
{
  return *new builder(m);
}


} // namespace sys_void
} // namespace beaker

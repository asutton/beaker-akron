// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "lang.hpp"
#include "name.hpp"
#include "type.hpp"
#include "expr.hpp"
#include "decl.hpp"
#include "stmt.hpp"
#include "comparison/equal.hpp"
#include "comparison/hash.hpp"

#include <beaker/util/memory.hpp>
#include <beaker/util/symbol_table.hpp>

#include <iostream>


namespace beaker {

static void init_type_hierarchy(inheritance_hierarchy&);
static void init_expr_hierarchy(inheritance_hierarchy&);
static void init_decl_hierarchy(inheritance_hierarchy&);
static void init_equal_algorithm(language&);
static void init_hash_algorithm(language&);

language::language(symbol_table& syms, const feature_list& feats)
  : algorithm_set(), feature_set(feats), node_store(), syms_(&syms)
{
  init_type_hierarchy(types_);
  init_expr_hierarchy(exprs_);
  init_decl_hierarchy(decls_);

  // Add the terms defined by the features to class hierarchies.
  for (feature* f : get_features())
    f->add_terms(*this);

  // Define the initial set of algorithms.
  init_equal_algorithm(*this);
  init_hash_algorithm(*this);

  // Add the semantics for terms defined by features.
  for (feature* f : get_features())
    f->add_terms(*this);
}

void
init_type_hierarchy(inheritance_hierarchy& hier)
{
  hier.add_derivation<type, base_type>();
  hier.add_derivation<type, object_type>();
  hier.add_derivation<type, reference_type>();
  hier.add_derivation<type, function_type>();
}

void 
init_expr_hierarchy(inheritance_hierarchy& hier)
{
  hier.add_derivation<expr, literal_expr>();
  hier.add_derivation<expr, nullary_expr>();
  hier.add_derivation<expr, unary_expr>();
  hier.add_derivation<expr, binary_expr>();
  hier.add_derivation<expr, ternary_expr>();
  hier.add_derivation<expr, ternary_expr>();
  hier.add_derivation<expr, init>();
  hier.add_derivation<expr, nullary_init>();
  hier.add_derivation<expr, unary_init>();
  // FIXME: add conversions.
}

void
init_decl_hierarchy(inheritance_hierarchy& hier)
{
  hier.add_derivation<decl, named_decl>();
  hier.add_derivation<decl, typed_decl>();
  hier.add_derivation<decl, value_decl>();
  hier.add_derivation<decl, mapping_decl>();
}

void
init_equal_algorithm(language& lang)
{
  auto& algo = lang.add_algorithm<equal_algorithm>(lang);
  algo.types->add_overrider<base_type>(equal_base_type);
  algo.exprs->add_overrider<nullary_expr>(equal_nullary_expr);
  algo.exprs->add_overrider<unary_expr>(equal_unary_expr);
  algo.exprs->add_overrider<binary_expr>(equal_binary_expr);
  algo.exprs->add_overrider<ternary_expr>(equal_ternary_expr);
}

void
init_hash_algorithm(language& lang)
{
  auto& algo = lang.add_algorithm<hash_algorithm>(lang);
  algo.types->add_overrider<base_type>(hash_base_type);
  algo.exprs->add_overrider<nullary_expr>(hash_nullary_expr);
  algo.exprs->add_overrider<unary_expr>(hash_unary_expr);
  algo.exprs->add_overrider<binary_expr>(hash_binary_expr);
  algo.exprs->add_overrider<ternary_expr>(hash_ternary_expr);
}

} // namespace beaker

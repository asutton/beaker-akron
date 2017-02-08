// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "hash.hpp"
#include <beaker/base/name.hpp>
#include <beaker/base/type.hpp>
#include <beaker/base/expr.hpp>


namespace beaker {

// Returns the equality algorithm associated with the node t.
static inline const hash_algorithm&
get_algorithm(const language& lang)
{
  return lang.get_algorithm<hash_algorithm>();
}

// Generates the dispatch table for the algorithm
hash_algorithm::hash_algorithm(language& lang)
  : names(new name_table(lang.get_names())),
    types(new type_table(lang.get_types())),
    exprs(new expr_table(lang.get_expressions()))
{ }

void
hash(const language& lang, hasher& h, const name& n) 
{
  hash(h, n.get_kind());
  const auto& tab = *get_algorithm(lang).names;
  auto fn = tab.get_overrider(n);
  fn(lang, h, n);
}

void
hash(const language& lang, hasher& h, const type& t)
{
  hash(h, t.get_kind());
  const auto& tab = *get_algorithm(lang).types;
  auto fn = tab.get_overrider(t);
  fn(lang, h, t);
}

void
hash(const language& lang, hasher& h, const expr& e) 
{
  hash(h, e.get_kind());
  const auto& tab = *get_algorithm(lang).exprs;
  auto fn = tab.get_overrider(e);
  fn(lang, h, e);
}

// -------------------------------------------------------------------------- //
// Overrides

/// Hash a literal 
void
hash_literal_expr(const language& lang, hasher& h, const literal_expr& e)
{
  hash(h, e.get_value());
}

/// Hash a unary expression e into h.
void
hash_unary_expr(const language& lang, hasher& h, const unary_expr& e)
{
  hash(lang, h, e.get_first());
}

/// Hash a binary expression e into h.
void
hash_binary_expr(const language& lang, hasher& h, const binary_expr& e)
{
  hash(lang, h, e.get_first());
  hash(lang, h, e.get_second());
}

/// Hash a ternary expression e into h.
void
hash_ternary_expr(const language& lang, hasher& h, const ternary_expr& e)
{
  hash(lang, h, e.get_first());
  hash(lang, h, e.get_second());
  hash(lang, h, e.get_third());
}

} // namespace
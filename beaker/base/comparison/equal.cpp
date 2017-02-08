// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "equal.hpp"
#include "../name.hpp"
#include "../type.hpp"
#include "../expr.hpp"


namespace beaker {

// Returns the equality algorithm associated with the node t.
static inline const equal_algorithm&
get_algorithm(const language& lang)
{
  return lang.get_algorithm<equal_algorithm>();
}

// Generates the dispatch table for the algorithm
equal_algorithm::equal_algorithm(language& lang)
  : names(new name_table(lang.get_names())),
    types(new type_table(lang.get_types())),
    exprs(new expr_table(lang.get_expressions()))
{ }

bool
equal(const language& lang, const name& a, const name& b) 
{
  if (a.get_kind() != b.get_kind())
    return false;
  const auto& tab = *get_algorithm(lang).names;
  auto fn = tab.get_overrider(a);
  return fn(lang, a, b);
}

bool
equal(const language& lang, const type& a, const type& b) 
{
  if (a.get_kind() != b.get_kind())
    return false;
  const auto& tab = *get_algorithm(lang).types;
  auto fn = tab.get_overrider(a);
  return fn(lang, a, b);
}

bool
equal(const language& lang, const expr& a, const expr& b) 
{
  if (a.get_kind() != b.get_kind())
    return false;
  const auto& tab = *get_algorithm(lang).exprs;
  auto fn = tab.get_overrider(a);
  return fn(lang, a, b);
}


// -------------------------------------------------------------------------- //
// Overrides

bool 
equal_unary_expr(const language& lang, const unary_expr& a, const unary_expr& b)
{
  return equal(lang, a.get_first(), b.get_first());
}

bool 
equal_binary_expr(const language& lang, const binary_expr& a, const binary_expr& b)
{
  return equal(lang, a.get_first(), b.get_first()) &&
         equal(lang, a.get_second(), b.get_second());
}

bool 
equal_ternary_expr(const language& lang, const ternary_expr& a, const ternary_expr& b)
{
  return equal(lang, a.get_first(), b.get_first()) &&
         equal(lang, a.get_second(), b.get_second()) &&
         equal(lang, a.get_third(), b.get_third());
}

} // namespace beaker
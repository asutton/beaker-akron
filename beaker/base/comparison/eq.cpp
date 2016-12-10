// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "eq.hpp"
#include <beaker/base/name.hpp>
#include <beaker/base/type.hpp>
#include <beaker/base/expr.hpp>

namespace beaker {

/// The default behavior is undefined.
bool
equivalence_algorithm::operator()(const name& a, const name& b) const
{
  assert(false && "not defined");
}

/// The default behavior is undefined.
bool
equivalence_algorithm::operator()(const type& a, const type& b) const
{
  assert(false && "not defined");
}

/// The default behavior is undefined.
bool
equivalence_algorithm::operator()(const expr& a, const expr& b) const
{
  assert(false && "not defined");
}

// Returns the equality algorithm associated with the node t.
template<typename T>
static inline const equivalence_algorithm&
get_eq(const T& t)
{
  feature& feat = language::get_feature(t);
  return feat.template get_algorithm<equivalence_algorithm>();
}

bool
equivalent(const name& a, const name& b) 
{
  if (a.get_kind() != b.get_kind())
    return false;
  return get_eq(a)(a, b);
}

bool
equivalent(const type& a, const type& b) 
{
  if (a.get_kind() != b.get_kind())
    return false;
  return get_eq(a)(a, b);
}

bool
equivalent(const expr& a, const expr& b) 
{
  if (a.get_kind() != b.get_kind())
    return false;
  return get_eq(a)(a, b);
}


bool
equivalent_unary_expr(const unary_expr& a, const unary_expr& b)
{
  assert(a.get_kind() == b.get_kind());
  return equivalent(a.get_operand(), b.get_operand());
}

bool
equivalent_binary_expr(const binary_expr& a, const binary_expr& b)
{
  assert(a.get_kind() == b.get_kind());
  return equivalent(a.get_lhs(), b.get_lhs()) 
      && equivalent(a.get_rhs(), b.get_rhs());
}

} // namespace
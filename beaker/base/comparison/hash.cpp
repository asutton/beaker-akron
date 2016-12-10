// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "hash.hpp"
#include <beaker/base/name.hpp>
#include <beaker/base/type.hpp>
#include <beaker/base/expr.hpp>


namespace beaker {

/// The default behavior is undefined.
void
hash_algorithm::operator()(hasher& h, const name& n) const
{
  assert(false && "not defined");
}

/// The default behavior is undefined.
void
hash_algorithm::operator()(hasher& h, const type& t) const
{
  assert(false && "not defined");
}

/// The default behavior is undefined.
void
hash_algorithm::operator()(hasher& h, const expr& e) const
{
  assert(false && "not defined");
}

// Returns the equality algorithm associated with the node t.
template<typename T>
static inline const hash_algorithm&
get_hash(const T& t)
{
  feature& feat = language::get_feature(t);
  return feat.template get_algorithm<hash_algorithm>();
}

void
hash(hasher& h, const name& n) 
{
  hash(h, typeid(n));
  return get_hash(n)(h, n);
}

void
hash(hasher& h, const type& t) 
{
  hash(h, typeid(t));
  return get_hash(t)(h, t);
}

void
hash(hasher& h, const expr& e) 
{
  hash(h, typeid(e));
  return get_hash(e)(h, e);
}

/// Hash a unary expression e into h.
void
hash_unary_expr(hasher& h, const unary_expr& e)
{
  hash(h, e.get_operand());
}

/// Hash a binary expression e into h.
void
hash_binary_expr(hasher& h, const binary_expr& e)
{
  hash(h, e.get_lhs());
  hash(h, e.get_rhs());
}

} // namespace
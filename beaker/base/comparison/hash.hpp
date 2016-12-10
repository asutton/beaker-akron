// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_COMPARISON_HASH_HPP
#define BEAKER_BASE_COMPARISON_HASH_HPP

#include <beaker/util/hash.hpp>
#include <beaker/base/lang.hpp>
#include <beaker/base/comparison/eq.hpp>

namespace beaker {

struct name;
struct type;
struct expr;
struct unary_expr;
struct binary_expr;

/// Defines the dispatch signature for hash algorithms.
struct hash_algorithm : algorithm
{
  struct tag { };

  virtual void operator()(hasher&, const name&) const;
  virtual void operator()(hasher&, const type&) const;
  virtual void operator()(hasher&, const expr&) const;
};

void hash(hasher&, const name&);
void hash(hasher&, const type&);
void hash(hasher&, const expr&);

void hash_unary_expr(hasher&, const unary_expr&);
void hash_binary_expr(hasher&, const binary_expr&);

} // namespace beaker


#endif

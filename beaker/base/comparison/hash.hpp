// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_COMPARISON_HASH_HPP
#define BEAKER_BASE_COMPARISON_HASH_HPP

#include <beaker/base/lang.hpp>
#include <beaker/base/seq.hpp>

#include <beaker/util/hash.hpp>


namespace beaker {

struct name;
struct type;
struct expr;
struct unary_expr;
struct binary_expr;

/// The hash algorithm is used to hash elements of a term into an accumulating
/// hash algorithm.
struct hash_algorithm : algorithm
{
  using name_table = dispatch_table<void(const language&, hasher&, const name&)>;
  using type_table = dispatch_table<void(const language&, hasher&, const type&)>;
  using expr_table = dispatch_table<void(const language&, hasher&, const expr&)>;

  hash_algorithm(language&);
  
  std::unique_ptr<name_table> names;
  std::unique_ptr<type_table> types;
  std::unique_ptr<expr_table> exprs;
};

void hash(const language&, hasher&, const name&);
void hash(const language&, hasher&, const type&);
void hash(const language&, hasher&, const expr&);


/// Hash the elements of s into h.
template<typename T>
inline void
hash(const language& lang, hasher& h, const seq<T>& s) 
{
  for (const T& t : s)
    hash(lang, h, t);
  hash(lang, h, s.size());
}

struct base_type;

/// Append no additional information for base types.
inline void
hash_base_type(const language&, hasher&, const base_type&)
{ }

struct nullary_expr;
struct unary_expr;
struct binary_expr;
struct ternary_expr;

/// Append no additional information for nullary expressions.
inline void
hash_nullary_expr(const language&, hasher&, const nullary_expr&)
{ }

void hash_unary_expr(const language&, hasher&, const unary_expr&);
void hash_binary_expr(const language&, hasher&, const binary_expr&);
void hash_ternary_expr(const language&, hasher&, const ternary_expr&);

} // namespace beaker


#endif

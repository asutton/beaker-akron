// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_COMPARISON_HASH_HPP
#define BEAKER_BASE_COMPARISON_HASH_HPP

#include <beaker/base/lang.hpp>
#include <beaker/base/seq.hpp>

#include <beaker/util/hash.hpp>


namespace beaker {

// -------------------------------------------------------------------------- //
// Hashing for names

struct name;

void hash(hasher&, const name&);

// -------------------------------------------------------------------------- //
// Hashing for types

struct type;
struct base_type;
struct object_type;

void hash(hasher&, const type&);

/// Appends no additional information for base types.
constexpr void hash(hasher&, const base_type&) { }

/// Appends no additional information for base types. This must be overriden
/// if the object type has additional properties.
constexpr void hash(hasher&, const object_type&) { }


// -------------------------------------------------------------------------- //
// Hashing for expressions

struct expr;
struct literal_expr;
struct nullary_expr;
struct unary_expr;
struct binary_expr;
struct ternary_expr;

void hash(hasher&, const expr&);

/// Append no additional information for nullary expressions.
constexpr void hash(hasher&, const nullary_expr&) { }

void hash(hasher&, const literal_expr&);
void hash(hasher&, const unary_expr&);
void hash(hasher&, const binary_expr&);
void hash(hasher&, const ternary_expr&);


// -------------------------------------------------------------------------- //
// Hashing for supporting types

/// Hash the elements of s into h.
template<typename T>
inline void
hash(hasher& h, const seq<T>& s) 
{
  for (const T& t : s)
    hash(h, t);
  hash(h, s.size());
}


// -------------------------------------------------------------------------- //
// Functional

/// A function object for term hashing that can be used with standard
/// algorithms and containers.
struct term_hash
{
  template<typename T>
  std::size_t operator()(const T& t) const noexcept
  {
    hash(algo, t);
    return algo;
  }

  mutable hasher algo;
};

} // namespace beaker


#endif

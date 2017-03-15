// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_COMPARISON_HASH_HPP
#define BEAKER_BASE_COMPARISON_HASH_HPP

#include <beaker/base/lang.hpp>
#include <beaker/base/seq.hpp>

#include <beaker/util/hash.hpp>


namespace beaker {

// -------------------------------------------------------------------------- //
// Primary interface.

struct name;
struct type;
struct expr;

void hash(hasher&, const name&);
void hash(hasher&, const type&);
void hash(hasher&, const expr&);

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
// Dispatch interface

struct base_type;
struct object_type;

struct literal_expr;
struct nullary_expr;
struct unary_expr;
struct binary_expr;
struct ternary_expr;

/// Hash the given name. Behavior is undefined if overload resolution selects
/// this function.
inline void 
hash_name(hasher&, const name&)
{
  assert(false && "function not defined");
}

/// Hash the given type. Behavior is undefined if overload resolution selects
/// this function.
inline void 
hash_type(hasher&, const type&)
{
  assert(false && "function not defined");
}

/// Hash the given expression. Behavior is undefined if overload resolution 
/// selects this function.
inline void 
hash_expr(hasher&, const expr&)
{
  assert(false && "function not defined");
}


/// Appends no additional information for base types.
constexpr void hash_type(hasher&, const base_type&) { }

/// Appends no additional information for base types. This must be overriden
/// if the object type has additional properties.
constexpr void hash_type(hasher&, const object_type&) { }

/// Append no additional information for nullary expressions.
constexpr void hash_expr(hasher&, const nullary_expr&) { }

void hash_expr(hasher&, const literal_expr&);
void hash_expr(hasher&, const unary_expr&);
void hash_expr(hasher&, const binary_expr&);
void hash_expr(hasher&, const ternary_expr&);


// -------------------------------------------------------------------------- //
// Functional

/// A function object for term hashing that can be used with standard
/// algorithms and containers.
struct term_hash
{
  template<typename T>
  std::size_t operator()(const T& t) const noexcept
  {
    hasher algo;
    hash(algo, t);
    return algo;
  }
};

} // namespace beaker


#endif

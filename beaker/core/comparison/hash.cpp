// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/core/comparison/hash.hpp>
#include <beaker/core/name.hpp>
#include <beaker/core/type.hpp>
#include <beaker/core/expr.hpp>
#include <beaker/util/symbol.hpp>


namespace beaker {
namespace core {

/// Hash the name `n` into `h`.
void
hash_algo::operator()(hasher& h, const name& n) const
{
  switch (n.get_kind()) {
    case basic_name_kind:
      return hash(h, cast<basic_name>(n).get_symbol());
    case internal_name_kind:
      return hash(h, cast<internal_name>(n).get_id());
    default:
      break;
  }
  assert(false && "name not a core name");
}


// Append the hash of t's referent type to h.
template<typename T>
static void
hash_reference_type(hasher& h, const T& t)
{
  hash(h, t.get_object_type());
}

// Append the hash of t's parameter and return types to h.
static void
hash_fn_type(hasher& h, const fn_type& t)
{
  hash(h, t.get_parameter_types());
  hash(h, t.get_return_type());
  hash(h, t.is_noexcept());
  hash(h, t.is_variadic());
}

// Append the hash of common type t to h.
void
hash_algo::operator()(hasher& h, const type& t) const
{
  switch (t.get_kind()) {
    case void_type_kind:
      return;
    case ref_type_kind:
      return hash_reference_type(h, cast<ref_type>(t));
    case in_type_kind:
      return hash_reference_type(h, cast<in_type>(t));
    case out_type_kind:
      return hash_reference_type(h, cast<out_type>(t));
    case fn_type_kind:
      return hash_fn_type(h, cast<fn_type>(t));
    default:
      break;
  }
  assert(false && "not a core type");
}

/// TODO: Implement this function.
void
hash_algo::operator()(hasher& h, const expr& t) const
{
  assert(false && "not implemented");
}

} // namespace core
} // namespace beaker

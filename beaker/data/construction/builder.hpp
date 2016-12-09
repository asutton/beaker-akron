// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_DATA_CONSTRUCTION_BUILDER_HPP
#define BEAKER_DATA_CONSTRUCTION_BUILDER_HPP

#include <beaker/base/construction/builder.hpp>

namespace beaker {
namespace data {

struct tuple_type;
struct array_type;
struct seq_type;

struct tuple_expr;
struct array_expr;
struct elem_expr;
struct index_expr;


// Provides access to resources needed to construct, validate, and
// evaluate data terms.
//
// TODO: This should use an allocator.
struct builder : builder_base
{
  builder(module&);

  // Canonical types
  tuple_type& get_tuple_type(const type_seq&);
  tuple_type& get_tuple_type(type_seq&&);
  array_type& get_array_type(type&, int);
  seq_type& get_seq_type(type&);

  // Expressions
  tuple_expr& make_tuple_expr(type&, const expr_seq&);
  tuple_expr& make_tuple_expr(type&, expr_seq&&);
  array_expr& make_array_expr(type&, const expr_seq&);
  array_expr& make_array_expr(type&, expr_seq&&);
  elem_expr& make_elem_expr(type&, expr&, int);
  index_expr& make_index_expr(type&, expr&, expr&);

  allocator* alloc_;
  canonical_set<tuple_type> tuples_;
  canonical_set<array_type> arrays_;
  canonical_set<seq_type> seqs_;
};

inline builder::builder(module& m) : builder_base(m) { }



} // namespace data
} // namespace beaker


#endif

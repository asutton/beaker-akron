// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_VARIADIC_CONSTRUCTION_BUILDER_HPP
#define BEAKER_VARIADIC_CONSTRUCTION_BUILDER_HPP

#include <beaker/base/construction/builder.hpp>


namespace beaker {

struct module;

namespace variadic {

struct list_type;
struct start_expr;
struct end_expr;
struct arg_expr;
struct copy_expr;

/// Provides access to resources needed to construct, validate, and
/// evaluate variadic terms.
struct builder : builder_base
{
  builder(module&);

  // Canonical types
  list_type& get_list_type();

  // Expressions
  start_expr& make_start_expr(type&, expr&);
  end_expr& make_end_expr(type&, expr&);
  arg_expr& make_arg_expr(type&, expr&);
  copy_expr& make_copy_expr(type&, expr&, expr&);

  singleton_set<list_type> list_;
};


} // namespace variadic
} // namespace beaker


#endif

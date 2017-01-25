// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_TUPLE_BUILD_HPP
#define BEAKER_SYS_TUPLE_BUILD_HPP

#include <beaker/sys.tuple/fwd.hpp>

#include <beaker/base/build.hpp>


namespace beaker {
namespace sys_tuple {

struct builder : basic_builder<sys_tuple_lang>
{
  builder(module&);

  // Types
  tuple_type& get_tuple_type(const type_seq&);
  tuple_type& get_tuple_type(type_seq&&);
  tuple_type& get_tuple_type(const expr_seq&);

  // Expressions
  tuple_expr& make_tuple_expr(const expr_seq&);
  tuple_expr& make_tuple_expr(expr_seq&&);
  proj_expr& make_proj_expr(expr&, int);

  canonical_set<tuple_type>* tup_;
};


} // namespace sys_tuple
} // namespace beaker


#endif

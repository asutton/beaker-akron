// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SAMPLE_EXPR_HPP
#define BEAKER_SAMPLE_EXPR_HPP

#include <beaker/base/expr.hpp>


namespace beaker {
namespace sample {

enum 
{
  first_expr_kind = sample_lang_block,
#define def_expr(e) e ## _expr_kind,
#include "expr.def"
  last_expr_kind
};


struct sample_expr : expr 
{
  static constexpr int node_kind = sample_expr_kind;

  sample_expr(type&);
};

inline sample_expr::sample_expr(type& t) : expr(node_kind, t) { }

} // namespace sample
} // namespace beaker


#endif

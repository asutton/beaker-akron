// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SAMPLE_STMT_HPP
#define BEAKER_SAMPLE_STMT_HPP

#include <beaker/base/stmt.hpp>


namespace beaker {
namespace sample {

enum 
{
  first_stmt_kind = sample_lang_block,
#define def_stmt(e) e ## _stmt_kind,
#include "stmt.def"
  last_stmt_kind
};


struct sample_stmt : stmt
{
  static constexpr int node_kind = sample_stmt_kind;

  sample_stmt();
};

inline sample_stmt::sample_stmt() : stmt(node_kind) { }

} // namespace sample
} // namespace beaker


#endif

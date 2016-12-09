// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SAMPLE_TYPE_HPP
#define BEAKER_SAMPLE_TYPE_HPP

#include <beaker/base/type.hpp>


namespace beaker {
namespace sample {

enum 
{
  first_type_kind = sample_lang_block,
#define def_type(e) e ## _type_kind,
#include "type.def"
  last_type_kind
};


struct sample_type : type
{
  static constexpr int node_kind = sample_type_kind;

  sample_type();
};

inline sample_type::sample_type() : type(node_kind) { }

} // namespace sample
} // namespace beaker


#endif

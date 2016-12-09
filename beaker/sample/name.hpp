// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SAMPLE_NAME_HPP
#define BEAKER_SAMPLE_NAME_HPP

#include <beaker/base/name.hpp>


namespace beaker {
namespace sample {

enum 
{
  first_name_kind = sample_lang_block,
#define def_name(e) e ## _name_kind,
#include "name.def"
  last_name_kind
};


struct sample_name : name
{
  static constexpr int node_kind = sample_name_kind;

  sample_name();
};

/// Initialize the basic name.
inline sample_name::sample_name() : name(node_kind) { }

} // namespace sample
} // namespace beaker


#endif

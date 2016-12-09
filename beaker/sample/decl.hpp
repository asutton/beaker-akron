// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SAMPLE_DECL_HPP
#define BEAKER_SAMPLE_DECL_HPP

#include <beaker/base/decl.hpp>

namespace beaker {
namespace sample {

enum 
{
  first_decl_kind = sample_lang_block,
#define def_decl(e) e ## _decl_kind,
#include "decl.def"
  last_decl_kind
};


// Represents the declaration of name as a stored object.
//
// The storage of a variable may be static, thread, or automatic. This
// class only represents the declaration of the object, it does not
// maintain its storage.
struct sample_decl : decl
{
  static constexpr int node_kind = sample_decl_kind;

  sample_decl();
};

inline sample_decl::sample_decl() : decl(node_kind) { }

} // namespace sample
} // namespace beaker


#endif

// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_PROCEDURAL_GENERATION_GEN_HPP
#define BEAKER_PROCEDURAL_GENERATION_GEN_HPP

#include <beaker/base/generation/generation.hpp>


namespace beaker {
namespace procedural {

struct gen_algo : generate_algorithm
{
  void operator()(generator&, const stmt&) const;
};

} // namespace procedural
} // namespace beaker


#endif

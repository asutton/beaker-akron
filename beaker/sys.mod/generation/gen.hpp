// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_MOD_GENERATION_GEN_HPP
#define BEAKER_SYS_MOD_GENERATION_GEN_HPP

#include <beaker/base/generation/generation.hpp>


namespace beaker {
namespace sys_mod {

// Module declarations do not result in generated code.
struct gen_algo : generate_algorithm
{
  cg::value operator()(generator&, const decl&) const { return {}; }
};

} // namespace sys_mod
} // namespace beaker


#endif

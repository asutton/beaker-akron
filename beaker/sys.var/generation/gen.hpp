// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_VAR_GENERATION_GEN_HPP
#define BEAKER_SYS_VAR_GENERATION_GEN_HPP

#include <beaker/base/generation/generation.hpp>


namespace beaker {
namespace sys_var {

struct gen_algo : generate_algorithm
{
  cg::type operator()(generator&, const type&) const;
  cg::value operator()(generator&, const expr&) const;
  cg::value operator()(generator&, const decl&) const;
};

} // namespace sys_var
} // namespace beaker


#endif

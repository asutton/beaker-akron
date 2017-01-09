// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_VOID_GENERATION_GEN_HPP
#define BEAKER_SYS_VOID_GENERATION_GEN_HPP

#include <beaker/base/generation/generation.hpp>


namespace beaker {
namespace sys_void {

struct gen_algo : generate_algorithm
{
  cg::type operator()(generator&, const type&) const override;
  cg::value operator()(generator&, const expr&) const override;
};

} // namespace sys_void
} // namespace beaker


#endif

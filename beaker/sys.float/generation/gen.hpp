// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_FLOAT_GENERATION_GEN_HPP
#define BEAKER_SYS_FLOAT_GENERATION_GEN_HPP

#include <beaker/base/generation/generation.hpp>


namespace beaker {
namespace sys_float {

struct gen_algo : generate_algorithm
{
  cg::type operator()(generator&, const type&) const;
  cg::value operator()(generator&, const expr&) const;
};

} // namespace sys_float
} // namespace beaker


#endif

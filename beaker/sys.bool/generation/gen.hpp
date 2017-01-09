// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_LOGIC_GENERATION_GEN_HPP
#define BEAKER_LOGIC_GENERATION_GEN_HPP

#include <beaker/base/generation/generation.hpp>


namespace beaker {
namespace logic {

struct gen_algo : generate_algorithm
{
  cg::type operator()(generator&, const type&) const;
  cg::value operator()(generator&, const expr&) const;
  cg::value operator()(generator&, const decl&) const;
};

} // namespace logic
} // namespace beaker


#endif

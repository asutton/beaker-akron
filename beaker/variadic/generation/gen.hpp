// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_VARIADIC_GENERATION_GEN_HPP
#define BEAKER_VARIADIC_GENERATION_GEN_HPP

#include <beaker/base/generation/generation.hpp>


namespace beaker {
namespace variadic {

struct gen_algo : generate_algorithm
{
  cg::type operator()(generator&, const type&) const;
  cg::value operator()(generator&, const expr&) const;
};

} // namespace variadic
} // namespace beaker


#endif

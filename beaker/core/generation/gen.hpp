// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_CORE_GENERATION_GEN_HPP
#define BEAKER_CORE_GENERATION_GEN_HPP

#include <beaker/base/generation/generation.hpp>


namespace beaker {
namespace core {

struct gen_algo : generate_algorithm
{
  std::string operator()(generator&, const name&) const;
  cg::type operator()(generator&, const type&) const;
  cg::value operator()(generator&, const expr&) const;
  cg::value operator()(generator&, const decl&) const;
  void operator()(generator&, const stmt&) const;
};

} // namespace core
} // namespace


#endif

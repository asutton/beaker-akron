// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_NAME_GENERATION_GEN_HPP
#define BEAKER_SYS_NAME_GENERATION_GEN_HPP

#include <beaker/base/generation/generation.hpp>


namespace beaker {
namespace sys_name {

struct gen_algo : generate_algorithm
{
  std::string operator()(generator&, const name&) const;
};

} // namespace sys_name
} // namespace beaker


#endif

// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_NAME_COMPARISON_EQ_HPP
#define BEAKER_SYS_NAME_COMPARISON_EQ_HPP

#include <beaker/base/comparison/eq.hpp>


namespace beaker {
namespace sys_name {

struct eq_algo : equivalence_algorithm
{
  bool operator()(const name&, const name&) const override;
};

} // namespace sys_name
} // namespace beaker


#endif

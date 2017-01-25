// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_TUPLE_COMPARISON_EQ_HPP
#define BEAKER_SYS_TUPLE_COMPARISON_EQ_HPP

#include <beaker/base/comparison/eq.hpp>


namespace beaker {
namespace sys_tuple {

struct eq_algo : equivalence_algorithm
{
  bool operator()(const type&, const type&) const override;
  bool operator()(const expr&, const expr&) const override;
};

} // namespace sys_tuple
} // namespace beaker


#endif

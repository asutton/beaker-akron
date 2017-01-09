// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_LOGIC_COMPARISON_EQ_HPP
#define BEAKER_LOGIC_COMPARISON_EQ_HPP

#include <beaker/base/comparison/eq.hpp>


namespace beaker {
namespace logic {

struct eq_algo : equivalence_algorithm
{
  bool operator()(const type&, const type&) const override;
  bool operator()(const expr&, const expr&) const override;
};

} // namespace logic
} // namespace beaker


#endif

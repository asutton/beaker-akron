// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_NUMERIC_COMPARISON_EQ_HPP
#define BEAKER_NUMERIC_COMPARISON_EQ_HPP

#include <beaker/base/comparison/eq.hpp>


namespace beaker {
namespace numeric {

struct eq_algo : equivalence_algorithm
{
  bool operator()(const type&, const type&) const override;
  bool operator()(const expr&, const expr&) const override;
};

} // namespace numeric
} // namespace beaker


#endif

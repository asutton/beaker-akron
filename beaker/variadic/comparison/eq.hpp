// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_VARIADIC_COMPARISON_EQ_HPP
#define BEAKER_VARIADIC_COMPARISON_EQ_HPP

#include <beaker/base/comparison/eq.hpp>


namespace beaker {
namespace variadic {

struct eq_algo : equivalence_algorithm
{
  bool operator()(const type&, const type&) const override;
  bool operator()(const expr&, const expr&) const override;
};

} // namespace variadic
} // namespace beaker


#endif

// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_CORE_COMPARISON_EQ_HPP
#define BEAKER_CORE_COMPARISON_EQ_HPP

#include <beaker/base/comparison/eq.hpp>


namespace beaker {
namespace core {

struct eq_algo : equivalence_algorithm
{
  bool operator()(const name&, const name&) const override;
  bool operator()(const type&, const type&) const override;
  bool operator()(const expr&, const expr&) const override;
};

} // namespace core
} // namespace beaker


#endif

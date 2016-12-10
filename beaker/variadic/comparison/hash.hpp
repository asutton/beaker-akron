// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_VARIADIC_COMPARISON_HASH_HPP
#define BEAKER_VARIADIC_COMPARISON_HASH_HPP

#include <beaker/base/comparison/hash.hpp>


namespace beaker {
namespace variadic {

struct hash_algo : hash_algorithm
{
  void operator()(hasher&, const type&) const override;
  void operator()(hasher&, const expr&) const override;
};

} // namespace variadic
} // namespace beaker


#endif

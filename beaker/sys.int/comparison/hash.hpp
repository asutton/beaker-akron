// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_INT_COMPARISON_HASH_HPP
#define BEAKER_SYS_INT_COMPARISON_HASH_HPP

#include <beaker/base/comparison/hash.hpp>


namespace beaker {
namespace sys_int {

struct hash_algo : hash_algorithm
{
  void operator()(hasher&, const type&) const override;
  void operator()(hasher&, const expr&) const override;
};

} // namespace sys_int
} // namespace beaker


#endif

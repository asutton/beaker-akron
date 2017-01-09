// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_NAME_COMPARISON_HASH_HPP
#define BEAKER_SYS_NAME_COMPARISON_HASH_HPP

#include <beaker/base/comparison/hash.hpp>


namespace beaker {
namespace sys_name {

struct hash_algo : hash_algorithm
{
  void operator()(hasher&, const name&) const override;
};

} // namespace sys_name
} // namespace beaker


#endif

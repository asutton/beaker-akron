// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "type.hpp"


namespace beaker {
namespace sys_int {

value
nat_type::min() const
{
  return value(0);
}

value
nat_type::max() const
{
  std::uintmax_t n = (std::uintmax_t(1) << prec_) -1;
  return value(std::intmax_t(n));
}

value
int_type::min() const
{
  std::uintmax_t n = (std::uintmax_t(1) << (prec_ - 1));
  return value(std::intmax_t(n));
}

value
int_type::max() const
{
  std::uintmax_t n = (std::uintmax_t(1) << (prec_ - 1)) -1;
  return value(std::intmax_t(n));
}

value
mod_type::min() const
{
  return value(0);
}

value
mod_type::max() const
{
  std::uintmax_t n = (std::uintmax_t(1) << prec_) -1;
  return value(std::intmax_t(n));
}

} // namespace sys_int
} // namespace beaker

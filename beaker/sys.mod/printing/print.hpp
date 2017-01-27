// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_MOD_PRINTING_PRINT_HPP
#define BEAKER_SYS_MOD_PRINTING_PRINT_HPP

#include <beaker/base/printing/print.hpp>


namespace beaker {
namespace sys_mod {

struct print_algo : print_algorithm
{
  void operator()(std::ostream&, const decl&) const;
};

} // namespace sys_mod
} // namespace beaker


#endif

// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_VOID_PRINTING_PRINT_HPP
#define BEAKER_SYS_VOID_PRINTING_PRINT_HPP

#include <beaker/base/printing/print.hpp>


namespace beaker {
namespace sys_void {

struct print_algo : print_algorithm
{
  void operator()(std::ostream&, const type&) const;
  void operator()(std::ostream&, const expr&) const;
};

} // namespace sys_void
} // namespace beaker


#endif

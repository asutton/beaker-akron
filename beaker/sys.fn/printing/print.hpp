// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_FN_PRINTING_PRINT_HPP
#define BEAKER_SYS_FN_PRINTING_PRINT_HPP

#include <beaker/base/printing/print.hpp>


namespace beaker {
namespace sys_fn {

struct print_algo : print_algorithm
{
  void operator()(std::ostream&, const type&) const;
  void operator()(std::ostream&, const expr&) const;
  void operator()(std::ostream&, const decl&) const;
  void operator()(std::ostream&, const stmt&) const;
};

} // namespace sys_fn
} // namespace beaker


#endif

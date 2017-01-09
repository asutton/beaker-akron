// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_BOOL_PRINTING_PRINT_HPP
#define BEAKER_SYS_BOOL_PRINTING_PRINT_HPP

#include <beaker/base/printing/print.hpp>


namespace beaker {
namespace sys_bool {

struct print_algo : print_algorithm
{
  void operator()(std::ostream&, const type&) const;
  void operator()(std::ostream&, const expr&) const;
  void operator()(std::ostream&, const decl&) const;
};

} // namespace sys_bool
} // namespace beaker


#endif

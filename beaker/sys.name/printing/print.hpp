// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_NAME_PRINTING_PRINT_HPP
#define BEAKER_SYS_NAME_PRINTING_PRINT_HPP

#include <beaker/base/printing/print.hpp>


namespace beaker {
namespace sys_name {

struct print_algo : print_algorithm
{
  void operator()(std::ostream&, const name&) const override;
};

} // namespace sys_name
} // namespace beaker


#endif

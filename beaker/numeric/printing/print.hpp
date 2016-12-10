// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_NUMERIC_PRINTING_PRINT_HPP
#define BEAKER_NUMERIC_PRINTING_PRINT_HPP

#include <beaker/base/printing/print.hpp>


namespace beaker {
namespace numeric {

struct print_algo : print_algorithm
{
  void operator()(std::ostream&, const type&) const;
  void operator()(std::ostream&, const expr&) const;
};

} // namespace numeric
} // namespace beaker


#endif

// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_PROCEDURAL_PRINTING_PRINT_HPP
#define BEAKER_PROCEDURAL_PRINTING_PRINT_HPP

#include <beaker/base/printing/print.hpp>


namespace beaker {
namespace procedural {

struct print_algo : print_algorithm
{
  void operator()(std::ostream&, const stmt&) const;
};

} // namespace procedural
} // namespace beaker


#endif

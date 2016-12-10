// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_CORE_PRINTING_PRINT_HPP
#define BEAKER_CORE_PRINTING_PRINT_HPP

#include <beaker/base/printing/print.hpp>


namespace beaker {
namespace core {

struct print_algo : print_algorithm
{
  void operator()(std::ostream&, const name&) const;
  void operator()(std::ostream&, const type&) const;
  void operator()(std::ostream&, const expr&) const;
  void operator()(std::ostream&, const decl&) const;
  void operator()(std::ostream&, const stmt&) const;
};

} // namespace core
} // namespace beaker


#endif

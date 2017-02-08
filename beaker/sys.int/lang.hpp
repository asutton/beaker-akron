// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_INT_LANG_HPP
#define BEAKER_SYS_INT_LANG_HPP

#include <beaker/base/lang.hpp>


namespace beaker {
namespace sys_int {

struct builder;

/// Adds support for integer types and expressions.
struct feature : beaker::feature
{
  using builder_type = builder;

  void add_terms(language&) override;
  void add_semantics(language&) override;
  beaker::builder& make_builder(module&) const override;
};

} // namespace sys_int
} // namespace beaker


#endif

// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_BOOL_LANG_HPP
#define BEAKER_SYS_BOOL_LANG_HPP

#include <beaker/base/lang.hpp>


namespace beaker {
namespace sys_bool {

struct builder;

/// Adds support for boolean expressions.
struct feature : beaker::feature
{
  using builder_type = builder;

  void add_terms(language&) override;
  void add_semantics(language&) override;
  beaker::builder& make_builder(module&) const override;
};
} // namespace sys_bool
} // namespace beaker


#endif

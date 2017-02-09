// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_VOID_LANG_HPP
#define BEAKER_SYS_VOID_LANG_HPP

#include <beaker/base/lang.hpp>


namespace beaker {
namespace sys_void {

struct builder;

/// Adds support for void expressions.
struct feature : beaker::feature
{
  using builder_type = builder;

  void add_terms(language&) override;
  void add_semantics(language&) override;
  factory& make_builder(module&) const override;
};

} // namespace sys_void
} // namespace beaker


#endif

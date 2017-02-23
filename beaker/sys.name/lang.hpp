// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_NAME_LANG_HPP
#define BEAKER_SYS_NAME_LANG_HPP

#include <beaker/base/lang.hpp>


namespace beaker {
namespace sys_name {

struct builder;

/// Adds support for boolean expressions.
struct feature : beaker::feature
{
  using builder_type = builder;

  factory& make_builder(module&) const override;
};

} // namespace sys_name
} // namespace beaker


#endif

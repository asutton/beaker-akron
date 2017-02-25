// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_VAR_LANG_HPP
#define BEAKER_SYS_VAR_LANG_HPP

#include <beaker/base/lang.hpp>


namespace beaker {
namespace sys_var {

struct builder;

/// Adds core language support for the definition of local and global variables.
struct feature : beaker::feature
{
  using builder_type = builder;

  factory& make_builder(module&) const override;
};

} // namespace sys_var
} // namespace beaker


#endif

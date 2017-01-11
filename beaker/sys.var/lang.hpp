// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_VAR_LANG_HPP
#define BEAKER_SYS_VAR_LANG_HPP

#include <beaker/base/lang.hpp>


namespace beaker {
namespace sys_var {

/// Represents the feature allowing the declaration of variables.
struct feature : basic_feature<sys_var_lang>
{
  feature();
};

} // namespace sys_var
} // namespace beaker


#endif

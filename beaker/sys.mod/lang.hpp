// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_MOD_LANG_HPP
#define BEAKER_SYS_MOD_LANG_HPP

#include <beaker/base/lang.hpp>


namespace beaker {
namespace sys_mod {

struct feature : feature_impl<sys_mod_lang>
{
  feature(language&);
};

} // namespace sys_mod
} // namespace beaker


#endif

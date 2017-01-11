// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_FN_LANG_HPP
#define BEAKER_SYS_FN_LANG_HPP

#include <beaker/base/lang.hpp>


namespace beaker {
namespace sys_fn {

struct feature : feature_impl<sys_fn_lang>
{
  feature(language&);
};

} // namespace sys_fn
} // namespace beaker


#endif

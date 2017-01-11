// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_BOOL_LANG_HPP
#define BEAKER_SYS_BOOL_LANG_HPP

#include <beaker/base/lang.hpp>


namespace beaker {
namespace sys_bool {

struct feature : feature_impl<sys_bool_lang>
{
  feature(language&);
};

} // namespace sys_bool
} // namespace beaker


#endif

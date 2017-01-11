// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_NAME_LANG_HPP
#define BEAKER_SYS_NAME_LANG_HPP

#include <beaker/base/lang.hpp>


namespace beaker {
namespace sys_name {

struct feature : feature_impl<sys_name_lang>
{
  feature(language&);
};

} // namespace sys_name
} // namespace beaker


#endif

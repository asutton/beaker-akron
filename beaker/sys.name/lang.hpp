// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_NAME_LANG_HPP
#define BEAKER_SYS_NAME_LANG_HPP

#include <beaker/base/lang.hpp>


namespace beaker {
namespace sys_name {

// A feature for using working with basic system names.
struct feature : basic_feature<sys_name_lang>
{
  feature();
};

} // namespace sys_name
} // namespace beaker


#endif

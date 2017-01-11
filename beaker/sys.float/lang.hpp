// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_FLOAT_LANG_HPP
#define BEAKER_SYS_FLOAT_LANG_HPP

#include <beaker/base/lang.hpp>


namespace beaker {
namespace sys_float {

struct feature : feature_impl<sys_float_lang>
{
  feature(language&);
};

} // namespace sys_float
} // namespace beaker


#endif

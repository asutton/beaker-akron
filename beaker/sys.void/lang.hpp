// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_VOID_LANG_HPP
#define BEAKER_SYS_VOID_LANG_HPP

#include <beaker/base/lang.hpp>


namespace beaker {
namespace sys_void {

/// A feature defines an extensible set of terms that can be used with the
/// beaker language runtime. In particular, this exposes a core set of 
/// algorithms and facilities needed to operate on those terms.
struct feature : basic_feature<sys_void_lang>
{
  feature();
};

} // namespace sys_void
} // namespace beaker


#endif

// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_CORE_LANG_HPP
#define BEAKER_CORE_LANG_HPP

#include <beaker/base/lang.hpp>


namespace beaker {
namespace core {

/// A feature defines an extensible set of terms that can be used with the
/// beaker language runtime. In particular, this exposes a core set of 
/// algorithms and facilities needed to operate on those terms.
struct feature : basic_feature<core_lang>
{
  feature();
};

} // namespace core
} // namespace beaker


#endif

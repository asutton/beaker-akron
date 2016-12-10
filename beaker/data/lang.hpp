// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_DATA_LANG_HPP
#define BEAKER_DATA_LANG_HPP

#include <beaker/base/lang.hpp>


namespace beaker {
namespace data {

/// A feature defines an extensible set of terms that can be used with the
/// beaker language runtime. In particular, this exposes a core set of 
/// algorithms and facilities needed to operate on those terms.
struct feature : basic_feature<data_lang>
{
  feature();
};

} // namespace data
} // namespace beaker


#endif

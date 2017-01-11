// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "lang.hpp"
#include "construction/builder.hpp"
#include "comparison/eq.hpp"
#include "comparison/hash.hpp"
#include "printing/print.hpp"
#include "generation/gen.hpp"

#include <beaker/sys.bool/lang.hpp>

namespace beaker {
namespace sys_float {

/// Allocates a builder.
static void* make_builder(module& m) { return new builder(m); }

feature::feature(language& lang)
  : feature_impl<sys_float_lang>(lang, make_builder)
{
  lang.add_feature<sys_bool::feature>();

  add_algorithm<eq_algo>();
  add_algorithm<hash_algo>();
  add_algorithm<print_algo>();
  add_algorithm<gen_algo>();
}

} // namespace sys_float
} // namespace beaker

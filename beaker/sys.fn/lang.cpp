// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "lang.hpp"
#include "construction/builder.hpp"
#include "comparison/eq.hpp"
#include "comparison/hash.hpp"
#include "printing/print.hpp"
#include "generation/gen.hpp"

#include <cassert>


namespace beaker {
namespace sys_fn {

/// Allocate a node builder for the language feature.
static void* make_builder(module& m) { return new builder(m); }


feature::feature()
  : basic_feature<sys_fn_lang>(make_builder)
{
  add_algorithm<eq_algo>();
  add_algorithm<hash_algo>();
  add_algorithm<print_algo>();
  add_algorithm<gen_algo>();
}

} // namespace sys_fn
} // namespace beaker

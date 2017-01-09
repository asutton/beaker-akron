// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "lang.hpp"
#include <beaker/core/construction/builder.hpp>
#include <beaker/core/comparison/eq.hpp>
#include <beaker/core/comparison/hash.hpp>
#include <beaker/core/printing/print.hpp>
#include <beaker/core/generation/gen.hpp>

#include <cassert>


namespace beaker {
namespace core {

/// Allocate a node builder for the language feature.
static void* make_builder(module& m) { return new builder(m); }


feature::feature()
  : basic_feature<core_lang>(make_builder)
{
  add_algorithm<eq_algo>();
  add_algorithm<hash_algo>();
  add_algorithm<print_algo>();
  add_algorithm<gen_algo>();
}

} // namespace core
} // namespace beaker

// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "lang.hpp"
#include <beaker/logic/construction/builder.hpp>
#include <beaker/logic/comparison/eq.hpp>
#include <beaker/logic/comparison/hash.hpp>
#include <beaker/logic/printing/print.hpp>
#include <beaker/logic/generation/gen.hpp>

#include <cassert>


namespace beaker {
namespace logic {

/// Allocate a node builder for the language feature.
static void* make_builder(module& m) { return new builder(m); }

feature::feature()
  : basic_feature<logic_lang>(make_builder)
{
  add_algorithm<eq_algo>();
  add_algorithm<hash_algo>();
  add_algorithm<print_algo>();
  add_algorithm<gen_algo>();
}

} // namespace logic
} // namespace beaker

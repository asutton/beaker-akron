// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "lang.hpp"
#include <beaker/procedural/construction/builder.hpp>
#include <beaker/procedural/printing/print.hpp>
#include <beaker/procedural/generation/gen.hpp>


namespace beaker {
namespace procedural {

/// Allocate a node builder for the language feature.
static void* make_builder(module& m) { return new builder(m); }

feature::feature()
  : basic_feature<procedural_lang>(make_builder)
{
  add_algorithm<print_algo>();
  add_algorithm<gen_algo>();
}

} // namespace procedural
} // namespace beaker

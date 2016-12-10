// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "lang.hpp"
#include <beaker/variadic/construction/builder.hpp>
#include <beaker/variadic/comparison/eq.hpp>
#include <beaker/variadic/comparison/hash.hpp>
#include <beaker/variadic/printing/print.hpp>
#include <beaker/variadic/generation/gen.hpp>

#include <cassert>


namespace beaker {
namespace variadic {

/// Allocate a node builder for the language feature.
static void* make_builder(module& m) { return new builder(m); }

feature::feature()
  : basic_feature<variadic_lang>(make_builder)
{
  add_algorithm<eq_algo>();
  add_algorithm<hash_algo>();
  add_algorithm<print_algo>();
  add_algorithm<gen_algo>();
}

} // namespace variadic
} // namespace beaker

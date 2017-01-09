// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "lang.hpp"
#include "construction/builder.hpp"
#include "printing/print.hpp"
#include "generation/gen.hpp"

#include <cassert>


namespace beaker {
namespace sys_name {

/// Allocate a node builder for the language feature.
static void* make_builder(module& m) { return new builder(m); }


feature::feature()
  : basic_feature<sys_name_lang>(make_builder)
{
  add_algorithm<print_algo>();
  add_algorithm<gen_algo>();
}

} // namespace sys_name
} // namespace beaker

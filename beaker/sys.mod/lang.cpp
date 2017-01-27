// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "lang.hpp"
#include "build.hpp"
#include "printing/print.hpp"
#include "serialization/write.hpp"


namespace beaker {
namespace sys_mod {

/// Allocate a node builder for the language feature.
static void* make_builder(module& m) { return new builder(m); }

feature::feature(language& lang)
  : feature_impl<sys_mod_lang>(lang, make_builder)
{
  add_algorithm<print_algo>();
  add_algorithm<write_algo>();
}

} // namespace sys_mod
} // namespace beaker

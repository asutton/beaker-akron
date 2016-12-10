// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "lang.hpp"
#include <beaker/procedural/printing/print.hpp>
#include <beaker/procedural/generation/gen.hpp>


namespace beaker {
namespace procedural {

feature::feature()
  : basic_feature<procedural_lang>()
{
  add_algorithm<print_algo>();
  add_algorithm<gen_algo>();
}

} // namespace procedural
} // namespace beaker

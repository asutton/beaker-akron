// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "lang.hpp"
#include <beaker/numeric/comparison/eq.hpp>
#include <beaker/numeric/comparison/hash.hpp>
#include <beaker/numeric/printing/print.hpp>
#include <beaker/numeric/generation/gen.hpp>

#include <cassert>


namespace beaker {
namespace numeric {

feature::feature()
  : basic_feature<numeric_lang>()
{
  add_algorithm<eq_algo>();
  add_algorithm<hash_algo>();
  add_algorithm<print_algo>();
  add_algorithm<gen_algo>();
}

} // namespace numeric
} // namespace beaker

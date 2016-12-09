// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/data/type.hpp>
#include <beaker/data/expr.hpp>
#include <beaker/data/construction/builder.hpp>
#include <beaker/base/module.hpp>
#include <beaker/base/serialization/json_reader.hpp>

namespace beaker {
namespace data {

// // A useful function.
// static builder&
// get_builder(json_reader& r)
// {
//   return r.get_module().get_builder<builder>(sample_lang);
// }

void
initialize_reader(json_reader& r)
{
}

} // namespace data
} // namespace beaker

// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/sample/name.hpp>
#include <beaker/sample/type.hpp>
#include <beaker/sample/expr.hpp>
#include <beaker/sample/decl.hpp>
#include <beaker/sample/construction/builder.hpp>
#include <beaker/base/module.hpp>
#include <beaker/base/serialization/json_reader.hpp>

namespace beaker {
namespace sample {

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

} // namespace sample
} // namespace beaker

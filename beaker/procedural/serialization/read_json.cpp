// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/procedural/stmt.hpp>
#include <beaker/procedural/construction/builder.hpp>
#include <beaker/base/module.hpp>
#include <beaker/base/serialization/json_reader.hpp>

namespace beaker {
namespace procedural {

// A useful function.
static builder&
get_builder(json_reader& r)
{
  return r.get_module().get_builder<builder>(procedural_lang);
}

void
initialize_reader(json_reader& r)
{
  // FIXME: Implement me.
}

} // namespace procedural
} // namespace beaker

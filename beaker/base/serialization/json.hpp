// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SERIALIZATION_JSON_HPP
#define BEAKER_SERIALIZATION_JSON_HPP

#include <rapidjson/document.h>


// Inject some names into RapidJSON to make naming consistent.
namespace rapidjson {

enum {
  null_value,
  false_value,
  true_value,
  object_value,
  array_value,
  string_value,
  number_value,
  last_value // Not a kind of a value.
};

} // namespace


namespace beaker
{

/// Introduce a new nested namespace.
namespace json = rapidjson;

} // namespace beaker


#endif

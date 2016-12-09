// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_SERIALIZATION_JSON_WRITER_HPP
#define BEAKER_BASE_SERIALIZATION_JSON_WRITER_HPP

#include <beaker/base/serialization/json.hpp>


namespace beaker {

struct module;

// A utility for emitting JSON modules.
struct json_writer
{
  json::Document emit(const module& m);
};

} // namespace beaker


#endif

// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SYS_NAME_CONSTRUCTION_BUILDER_HPP
#define BEAKER_SYS_NAME_CONSTRUCTION_BUILDER_HPP

#include <beaker/base/construction/builder.hpp>


namespace beaker {
namespace sys_name {

struct basic_name;
struct internal_name;

/// Provides access to resources needed to construct, validate, and
/// evaluate sys_name terms.
struct builder : basic_builder<sys_name_lang>
{
  builder(module&);

  // Names
  basic_name& get_name(const char*);
  internal_name& get_name();

  canonical_set<basic_name>* name_;
  int current_id_;
};

} // namespace sys_name
} // namespace beaker


#endif

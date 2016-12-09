// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_SAMPLE_CONSTRUCTION_BUILDER_HPP
#define BEAKER_SAMPLE_CONSTRUCTION_BUILDER_HPP

#include <beaker/base/construction/builder.hpp>


namespace beaker {

struct module;

namespace sample {

struct sample_name;
struct sample_type;
struct sample_expr;
struct sample_decl;
struct sample_stmt;

/// Provides access to resources needed to construct, validate, and
/// evaluate sample terms.
struct builder : builder_base
{
  builder(module&);

  // Names
  sample_name& get_sample_name();

  // Canonical types
  sample_type& get_sample_type();

  // Expressions
  sample_expr& make_sample_expr(type&);

  // Declarations
  sample_decl& make_sample_decl();

  // Statement
  sample_stmt& make_sample_stmt();
};


} // namespace sample
} // namespace beaker


#endif

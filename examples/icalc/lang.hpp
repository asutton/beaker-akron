// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef ICALC_LANG_HPP
#define ICALC_LANG_HPP

#include <beaker/base/lang.hpp>
#include <beaker/base/module.hpp>

// FIXME: We shouldn't have these dependencies here. Refactor the builder
// to use a compiler firewall to prevent physical dependencies.
#include <beaker/sys.bool/ast.hpp>
#include <beaker/sys.int/ast.hpp>

#include <beaker/util/symbol_table.hpp>


namespace icalc {

using expr = beaker::expr;
using type = beaker::expr;


/// Defines the icalc language.
struct language : beaker::language
{
  language();

  beaker::symbol_table syms;
};


// Defines an icalc module.
using module = beaker::module;


// A builder for the language.
struct builder : beaker::sys_bool::builder, beaker::sys_int::builder
{
  builder(module&);
};

} // namespace icalc

#endif


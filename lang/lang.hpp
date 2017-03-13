// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BPL_LANG_HPP
#define BPL_LANG_HPP

#include <beaker/base/lang.hpp>
#include <beaker/base/module.hpp>
#include <beaker/base/symbol_table.hpp>
#include <beaker/base/name.hpp>
#include <beaker/base/type.hpp>
#include <beaker/base/expr.hpp>
#include <beaker/base/decl.hpp>
#include <beaker/base/stmt.hpp>

#include <beaker/sys.void/fwd.hpp>
#include <beaker/sys.bool/fwd.hpp>
#include <beaker/sys.int/fwd.hpp>
#include <beaker/sys.name/fwd.hpp>
#include <beaker/sys.var/fwd.hpp>
#include <beaker/sys.fn/fwd.hpp>


namespace bpl {

using beaker::is;
using beaker::as;
using beaker::cast;

using beaker::name;
using beaker::type;
using beaker::type_seq;
using beaker::expr;
using beaker::expr_seq;
using beaker::decl;
using beaker::decl_seq;
using beaker::stmt;
using beaker::stmt_seq;


// -------------------------------------------------------------------------- //
// Language

/// Defines the Beaker Programming Language..
struct language : beaker::language
{
  language();

  const beaker::symbol_table& get_symbols() const;
  beaker::symbol_table& get_symbols();  

  beaker::symbol_table syms;
};


/// Returns the symbol table for the language.
inline const beaker::symbol_table& language::get_symbols() const { return syms; }

/// Returns the symbol table for the language.
inline beaker::symbol_table& language::get_symbols() { return syms; }


// -------------------------------------------------------------------------- //
// Module

/// Defines a beaker source module.
struct module : beaker::module
{
  using beaker::module::module;

  const language& get_language() const;
  language& get_language();

  // Builders
  beaker::sys_void::builder& get_void_builder();
  beaker::sys_bool::builder& get_bool_builder();
  beaker::sys_int::builder& get_int_builder();
  beaker::sys_name::builder& get_name_builder();
  beaker::sys_var::builder& get_var_builder();
  beaker::sys_fn::builder& get_fn_builder();
};

/// Returns the language for the module.
inline const language&
module::get_language() const
{ 
  return static_cast<const language&>(beaker::module::get_language());
}
/// Returns the language for the module.
inline
language& module::get_language()
{ 
  return static_cast<language&>(beaker::module::get_language());
}


} // namespace bpl

#endif


// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BPL_LANG_HPP
#define BPL_LANG_HPP

#include <beaker/base/lang.hpp>
#include <beaker/base/module.hpp>
#include <beaker/base/symbol_table.hpp>

// FIXME: We shouldn't have these dependencies here. Refactor the builder
// to use a compiler firewall to prevent physical dependencies.
#include <beaker/sys.void/ast.hpp>
#include <beaker/sys.bool/ast.hpp>
#include <beaker/sys.int/ast.hpp>
#include <beaker/sys.name/ast.hpp>


namespace bpl {

using beaker::name;
using beaker::type;
using beaker::type_seq;
using beaker::expr;
using beaker::expr_seq;
using beaker::decl;
using beaker::decl_seq;
using beaker::stmt;
using beaker::stmt_seq;


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


/// Defines a beaker source module.
using module = beaker::module;


/// Provides a single interface for node construction.
///
/// \todo Factor this into a separate module.
struct builder : 
  beaker::sys_void::builder, 
  beaker::sys_bool::builder, 
  beaker::sys_int::builder,
  beaker::sys_name::builder
{
  builder(module&);

  const language& get_language() const;
  language& get_language();

  const beaker::factory& get_base() const;
  beaker::factory& get_base();
};

// Returns the builder for void terms.
inline const beaker::factory& builder::get_base() const 
{ 
  return static_cast<const beaker::sys_void::builder&>(*this); 
}

// Returns the builder for void terms.
inline beaker::factory& 
builder::get_base()
{ 
  return static_cast<beaker::sys_void::builder&>(*this); 
}

/// Returns the language for the builder.
inline const language& builder::get_language() const 
{ 
  return static_cast<const language&>(get_base().get_language()); 
}

/// Returns the language for the builder.
inline language& builder::get_language() 
{ 
  return static_cast<language&>(get_base().get_language()); 
}

} // namespace bpl

#endif


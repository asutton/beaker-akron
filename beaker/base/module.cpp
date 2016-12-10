// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "module.hpp"
#include <beaker/util/memory.hpp>
#include <beaker/util/symbol_table.hpp>


namespace beaker {

#define def_lang(l) \
namespace l { \
  extern void* make_builder(module&); \
}
#include "lang.def"

/// Initialize the module. The allocator and symbol table are allocated
/// and owned by the module.
module::module()
  : alloc_(new sequential_allocator<>()), 
    syms_(new symbol_table()), 
    build_(), 
    name_(nullptr), 
    decls_(),
    my_alloc_(true), 
    my_syms_(true)
{
  build_.put({
#define def_lang(l) {l##_lang, l::make_builder(*this)},
#include "lang.def"
  });
}

// Initialize the module to use the given allocator and symbol table.
module::module(allocator& a, symbol_table& s)
  : alloc_(&a), 
    syms_(&s), 
    build_(), 
    name_(nullptr), 
    decls_(),
    my_alloc_(false), 
    my_syms_(false)
{
  build_.put({
#define def_lang(l) {l##_lang, l::make_builder(*this)},
#include "lang.def"
  });
}

module::~module()
{
  if (my_syms_)
    delete syms_;
  if (my_alloc_)
    delete alloc_;
}

} // namespace beaker


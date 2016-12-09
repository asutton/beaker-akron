// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include "module.hpp"


namespace beaker {

#define def_lang(l) \
namespace l { \
  extern void* make_builder(module&); \
}
#include "lang.def"


// Initialize the module to use the given allocator and symbol table.
module::module(allocator& a, symbol_table& s)
  : alloc_(&a), syms_(&s), build_(), name_(nullptr), decls_()
{
  build_.put({
#define def_lang(l) {l##_lang, l::make_builder(*this)},
#include "lang.def"
  });
}


} // namespace beaker


// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include <beaker/base/lang.hpp>
#include <beaker/base/module.hpp>
#include <beaker/util/symbol_table.hpp>

#include <beaker/sys.void/ast.hpp>

// #include <beaker/base/printing/print.hpp>
// #include <beaker/base/generation/generation.hpp>
// #include <beaker/base/serialization/writer.hpp>


using namespace beaker;

int 
main()
{
  symbol_table syms;
  language lang(syms, {
    new sys_void::feature(),
  });
  module mod(lang);
  auto& vb = mod.get_builder<sys_void::feature>();

  type& t1 = vb.get_void_type();
  type& t2 = vb.get_void_type();
  assert(equal(lang, t1, t2));

  hasher h1;
  hash(lang, h1, t1);
  hasher h2;
  hash(lang, h2, t2);
  assert((std::size_t)h1 == (std::size_t)h2);
}

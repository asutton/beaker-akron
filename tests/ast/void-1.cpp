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

// Check that two terms are not equal.
template<typename T>
void 
check_different(const language& lang, const T& t1, const T& t2)
{
  assert(!equal(lang, t1, t2));
}

// Check if two terms are equal and hash equivalent.
template<typename T>
void 
check_equal(const language& lang, const T& t1, const T& t2)
{
  // Check equality
  assert(equal(lang, t1, t2));

  // Check hash equivalence.
  hasher h1;
  hash(lang, h1, t1);
  hasher h2;
  hash(lang, h2, t2);
  assert((std::size_t)h1 == (std::size_t)h2);
}

// Check if two terms are identical (the same object), and also equal.
template<typename T>
void
check_identical(const language& lang, const T& t1, const T& t2)
{
  assert(&t1 == &t2);
  check_equal(lang, t1, t2);
}

int 
main()
{
  symbol_table syms;
  language lang(syms, {
    new sys_void::feature(),
  });
  module mod(lang);
  auto& vb = mod.get_builder<sys_void::feature>();

  check_identical(lang, vb.get_void_type(), vb.get_void_type());

  auto& nop = vb.make_nop_expr();
  auto& trap = vb.make_trap_expr();
  check_equal(lang, nop, vb.make_nop_expr());
  check_equal(lang, trap, vb.make_trap_expr());
  check_equal(lang, vb.make_void_expr(nop), vb.make_void_expr(nop));
  check_different(lang, vb.make_void_expr(nop), vb.make_void_expr(trap));
}

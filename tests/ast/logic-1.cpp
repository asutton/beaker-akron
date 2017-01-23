// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "test.hpp"

#include <beaker/base/printing/print.hpp>
#include <beaker/base/generation/generation.hpp>
#include <beaker/base/serialization/writer.hpp>


using namespace beaker;

static void
check_canonical_types()
{
  // Check canonical types.
  builder& build = global_builder::get();
  type& b1 = build.get_bool_type();
  type& b2 = build.get_bool_type();
  assert(&b1 == &b2);
}

int 
main()
{
  symbol_table syms;
  system_lang lang(syms);
  module mod(lang);
  
  // Create and establish a global builder context.
  builder build(mod);
  global_builder global(build);

  check_canonical_types();

  type& b = build.get_bool_type();

  decl& main_ = build.make_main();

  // NOTE: GCC may not sequence the initializatoin of t and f before their
  // use if we don't declare them separately from adding them.
  out<decl> t;
  out<decl> f;
  add_stmts(main_)
    .var(t, "t", b, true_())  // var bool t = true;
    .var(f, "f", b, false_()) // var bool f = false;
  ;
  add_stmts(main_)
    // Truth table for !
    .check((!f) == t)
    .check((!t) == f)
    // Truth table for &
    .check((t & t) == t)
    .check((t & f) == f)
    .check((f & t) == f)
    .check((f & f) == f)
    // Truth table for |
    .check((t | t) == t)
    .check((t | f) == t)
    .check((f | t) == t)
    .check((f | f) == f)
    // Truth table for ^
    .check((t ^ t) == f)
    .check((t ^ f) == t)
    .check((f ^ t) == t)
    .check((f ^ f) == f)
    // Truth table for =>
    .check(imp(t, t) == t)
    .check(imp(t, f) == f)
    .check(imp(f, t) == t)
    .check(imp(f, f) == t)
    // Truth table for <=>
    .check((t == t) == t)
    .check((t == f) == f)
    .check((f == t) == f)
    .check((f == f) == t)
  ;

  archive_writer ar;
  write_module(ar, mod);
  ar.save("out.bkm");

  generator gen("a.ll");
  generate(gen, mod);
  gen.print();
}

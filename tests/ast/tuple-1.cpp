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
  type& b = build.get_bool_type();
  type& z = build.get_int32_type();
  type& t1 = build.get_tuple_type({&b, &z});
  type& t2 = build.get_tuple_type({&b, &z});
  type& t3 = build.get_tuple_type({&z, &b});
  assert(&t1 == &t2);
  assert(&t1 != &t3);
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

  type& i32 = build.get_int32_type();
  expr& e1 = build.make_true_expr();
  expr& e2 = build.make_int_expr(i32, 5);
  expr& e3 = build.make_false_expr();

  decl& main_ = build.make_main();

  // NOTE: GCC may not sequence the initializatoin of t and f before their
  // use if we don't declare them separately from adding them.
  out<decl> t;
  out<decl> f;
  add_stmts(main_)
    .run(build.make_tuple_expr({
      &build.make_copy_init(e1), 
      &build.make_copy_init(e2),
      &build.make_copy_init(e3)
    }))
  ;

  archive_writer ar;
  write_module(ar, mod);
  ar.save("out.bkm");

  generator gen("a.ll");
  generate(gen, mod);
  gen.print();
}

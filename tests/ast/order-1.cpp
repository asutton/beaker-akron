// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "test.hpp"

#include <beaker/base/printing/print.hpp>
#include <beaker/base/generation/generation.hpp>
#include <beaker/base/serialization/writer.hpp>


using namespace beaker;

int 
main()
{
  symbol_table syms;
  system_lang lang(syms);
  module mod(lang);
  
  // Create and establish a global builder context.
  builder build(mod);
  global_builder global(build);

  type& void_ = build.get_void_type();

  // TODO: Make it easier to define functions.
  // Also, this should be replaced by a lambda expression so that we
  // don't have to build this declaration separately.
  decl_seq parms;
  decl& ret = build.make_parm_decl("ret", void_);
  type& type = build.get_fn_type(parms, ret);
  stmt& def = build.make_block_stmt({});
  decl& foo = build.make_fn_decl("foo", type, parms, ret, def);

  // Create main.
  decl& main_ = build.make_main();
  add_stmts(main_)
    .run(call(foo))
  ;

  // Add foo out of order.
  mod.add_declaration(foo);

  archive_writer ar;
  write_module(ar, mod);
  ar.save("out.bkm");

  generator gen("a.ll");
  generate(gen, mod);
  gen.print();
}

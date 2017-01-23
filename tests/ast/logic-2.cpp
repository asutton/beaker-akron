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

  type& b = build.get_bool_type();

  // TODO: Make it easier to define functions.
  // Also, this should be replaced by a lambda expression so that we
  // don't have to build this declaration separately.
  decl_seq parms;
  decl& ret = build.make_parm_decl("ret", b);
  type& type = build.get_fn_type(parms, ret);
  stmt& def = build.make_block_stmt({});
  decl& fail = build.make_fn_decl("fail", type, parms, ret, def);
  mod.add_declaration(fail);
  add_stmts(fail)
    .run(trap())
  ;

  decl& main_ = build.make_main();
  out<decl> t;
  out<decl> f;
  add_stmts(main_)
    .var(t, "t", b, true_())  // var bool t = true;
    .var(f, "f", b, false_()) // var bool f = false;
  ;
  add_stmts(main_)
    // Truth table for &&
    .check((t && t) == t)
    .check((t && f) == f)
    .check((f && t) == f)
    .check((f && f) == f)
    
    // Short-circuiting &&
    .run(f && call(fail))

    // Truth table for ||
    .check((t || t) == t)
    .check((t || f) == t)
    .check((f || t) == t)
    .check((f || f) == f)
    
    // Short circuiting for ||
    .run(t || call(fail))
  ;

  archive_writer ar;
  write_module(ar, mod);
  ar.save("out.bkm");

  generator gen("a.ll");
  generate(gen, mod);
  gen.print();
}

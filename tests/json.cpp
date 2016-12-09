// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/util/memory.hpp>
#include <beaker/util/symbol_table.hpp>
#include <beaker/base/module.hpp>
#include <beaker/base/serialization/json_reader.hpp>
#include <beaker/base/generation/generation.hpp>

#include <llvm/IR/Module.h>


using namespace beaker;

int
main(int argc, char* argv[])
{
  // Define the module.
  sequential_allocator<> alloc;
  symbol_table syms;
  module mod(alloc, syms);
  
  // Load from JSON.
  json_reader reader(mod);
  reader.load(argv[1]);

  // Emit LLVM.
  generator gen("out.ll");
  generate(gen, mod);
  gen.get_module().dump();
}

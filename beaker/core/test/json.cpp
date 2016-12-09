// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/common/serialization.hpp>

#include <iostream>


using namespace beaker;

int 
main(int argc, char* argv[])
{
  if (argc < 2) {
    std::cerr << "error: no input file given\n";
    return 1;
  }

  sequential_allocator<> alloc;
  symbol_table syms;
  json_reader jr(alloc, syms);
  jr.load(argv[1]);
}

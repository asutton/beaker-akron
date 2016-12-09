// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/util/memory.hpp>
#include <beaker/util/symbol_table.hpp>

#include <beaker/base/module.hpp>
#include <beaker/base/generation/generation.hpp>
#include <beaker/base/printing/print.hpp>

#include <beaker/numeric/type.hpp>
#include <beaker/numeric/expr.hpp>
#include <beaker/numeric/construction/builder.hpp>

#include <iostream>


using namespace beaker;

#define var(T, N, K) T ## N ## _ ## K

#define check_canonical_type(T, N) \
  type& var(T,N,1) = nb.get_ ## T ## N ## _type(); \
  type& var(T,N,2) = nb.get_ ## T ## N ## _type(); \
  assert(&var(T,N,1) == &var(T,N,2)); \
  print(var(T,N,1));

#define check_extended_type(T, N) \
  type& var(T,N,1) = nb.get_ ## T ## _type(N); \
  type& var(T,N,2) = nb.get_ ## T ## _type(N); \
  assert(&var(T,N,1) == &var(T,N,2)); \
  print(var(T,N,1));

int
main(int argc, char* argv[])
{
  sequential_allocator<> alloc;
  symbol_table syms;
  module mod(alloc, syms);
  
  numeric::builder& nb = mod.get_builder<numeric::builder>();

  // Check canonical types.
  check_canonical_type(char,8);
  check_canonical_type(char,16);
  check_canonical_type(char,32);

  check_canonical_type(nat,8);
  check_canonical_type(nat,16);
  check_canonical_type(nat,32);
  check_canonical_type(nat,64);
  check_extended_type(nat,128);

  check_canonical_type(int,8);
  check_canonical_type(int,16);
  check_canonical_type(int,32);
  check_canonical_type(int,64);
  check_extended_type(int,128);

  check_canonical_type(mod,8);
  check_canonical_type(mod,16);
  check_canonical_type(mod,32);
  check_canonical_type(mod,64);
  check_extended_type(mod,128);

  check_canonical_type(float,32);
  check_canonical_type(float,64);

  generator gen("out.ll");
  cg::type t1 = generate(gen, nb.get_int32_type());
  assert(t1.is_direct());
  cg::type t2 = generate(gen, nb.get_int_type(1024));
  assert(t2.is_indirect());
}

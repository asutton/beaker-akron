// Copyright (c) 2016 Andrew Sutton
// All rights reserved

#include "memory.hpp"

#include <new>


namespace beaker
{

void*
freestore_allocator::allocate(int n)
{
  return ::operator new(n);
}

// TODO: This can be implemented using the new over-aligned allocation
// support in C++17. In the meantime, we could just allocate a block
// of memory large enough to contain n + a and then return the pointer
// to an address aligned at a.
void*
freestore_allocator::allocate(int n, int a)
{
  assert(false && "not implemented");
}

void
freestore_allocator::deallocate(void* p)
{
  ::operator delete(p);
}

allocator&
default_allocator()
{
  static freestore_allocator alloc_;
  return alloc_;
}


} // namespace beaker

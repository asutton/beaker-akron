// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_UTIL_MEMORY_HPP
#define BEAKER_UTIL_MEMORY_HPP

#include <cassert>
#include <cstdint>


namespace beaker
{

// The base class of all allocators.
struct allocator
{
  // Allocate n bytes of storage.
  virtual void* allocate(int) = 0;

  // Allocate n bytes of storage aligned to a bytes. The alignment must be
  // a power of two.
  virtual void* allocate(int, int) = 0;

  // Deallocate the indicated storage.
  virtual void deallocate(void*) = 0;
};


// Returns the default allocator.
allocator& default_allocator();


// -------------------------------------------------------------------------- //
// Freestore allocation

// An allocator that uses new and delete.
struct freestore_allocator : allocator
{
  void* allocate(int) override;
  void* allocate(int, int) override;
  void deallocate(void*) override;
};


// -------------------------------------------------------------------------- //
// Sequential allocation

// The sequential allocator allocates memory from a sequence of dynamically
// allocated blocks of memory. Memory is allocated from increasingly large
// addresses within each block. When the next allocation would exceed the 
// size current block, a new block is allocated.
//
// All memory is released when the allocator is destroyed. Individual objects
// cannot be deallocated.
//
// The internal structure of a block of memory looks like this:
//
//    +---+---+----------------------------------+
//    | B | P |       Allocatable memory         |
//    +---+---+----------------------------------+
//
// B is the "control block" portion of the memory. It essentially contains a
// pointer to the next block (or nullptr in the last case). P is the current
// allocation pointer. When memory is allocated, this is incremented to consume
// the rest of the allocatable memory.
//
// TODO: Support aligned allocation. The allocate() function would take the
// alignment as a second argument and guarantee that the pointer was a multiple
// of that value.
//
// NOTE: This is not compatible with standard allocators because it isn't typed. 
template<int S = 8192>
struct sequential_allocator : allocator
{
  struct block
  {
    block* next;
  };

  sequential_allocator()
    : sequential_allocator(default_allocator())
  { }

  sequential_allocator(allocator& a)
    : alloc_(&a), current_(0), head_(), tail_()
  { }

  ~sequential_allocator();

  void* allocate(int) override;
  void* allocate(int, int) override;
  void deallocate(void*) override;

  // Private methods.
  char* limit() const;
  int over(int) const;
  void extend();

  allocator* alloc_;
  char* current_;
  block* head_;
  block* tail_;
};

// Destroy the allocator, releasing all of it's allocated memory.
template<int S>
sequential_allocator<S>::~sequential_allocator()
{
  block* p = head_;
  while (p) {
    block* q = p->next;
    p->~block();
    alloc_->deallocate(p);
    p = q;
  }
}

// Allocate n bytes of memory from the current block.
//
// TODO: What if n is greater than the size of the block. We could actually
// allocate a "jumbo block" and link that into the chain.
template<int S>
void*
sequential_allocator<S>::allocate(int n)
{
  assert(n < S - (int)sizeof(block));
  
  // Guarantee that we have an initial block.
  if (!head_)
    extend();

  // If the allocation would exceed the current block, allocate a new
  // one and update the current pointer.
  char* limit = reinterpret_cast<char*>(tail_) + S;
  if (limit - current_ < n)
    extend();

  // Bump the pointer and return the address where the object will be
  // allocated.
  char* addr = current_;
  current_ += n;
  return addr;
}

// Allocate n bytes of memory from the current block aligned to the number
// of bytes required by align.
//
// TODO: What if n is greater than the size of the block. We could actually
// allocate a "jumbo block" and link that into the chain.
template<int S>
void*
sequential_allocator<S>::allocate(int n, int a)
{
  assert(n < S - (int)sizeof(block));
  assert(a > 0);
  
  // Guarantee that we have a block.
  if (!head_)
    extend();
  
  // Compute the adjustment needed to align the allocation. If that adjustment,
  // along with the original request exceeds the limit of the current block,
  // then extend the store and re-compute the adjustment. Then align the
  // current pointer.
  int k = over(a) ? a - over(a) : 0;
  if (limit() - current_ < n + k) {
    extend();
    k = over(a) ? a - over(a) : 0;
  }
  current_ += k;

  // Bump the pointer and return the address where the object will be
  // allocated.
  char* addr = current_;
  current_ += n;
  return addr;
}

// This function has no effect.
template<int S>
void
sequential_allocator<S>::deallocate(void* p)
{ }

// Returns a pointer past the end of the current block.
template<int S>
char*
sequential_allocator<S>::limit() const
{
  return reinterpret_cast<char*>(tail_) + S;
}

// Returns the current offset modulo an alignment value.
template<int S>
int
sequential_allocator<S>::over(int a) const
{
  return reinterpret_cast<std::uintptr_t>(current_) % a;
}

// Allocate a new block.
template<int S>
void
sequential_allocator<S>::extend()
{
  // Allocate a new block and embed the block at the front and set the
  // current allocation pointer past that.
  current_ = (char*)alloc_->allocate(S);
  block* b = new (current_) block { nullptr };
  current_ += sizeof(block);

  // Update the list of nodes.
  if (!head_)
    head_ = b;
  else
    tail_->next = b;
  tail_ = b;
}


// -------------------------------------------------------------------------- //
// Typed allocator

// A typed allocator adapts the allocator model into the standard allocator
// model. It allocates objects of specific type and defines the required
// associated traits.
template<typename T>
struct typed_allocator
{
  typed_allocator()
    : typed_allocator(default_allocator())
  { }

  typed_allocator(allocator& a)
    : alloc_(&a)
  { }

  T* allocate();
  T* allocate(int);
  void deallocate(T*);

  allocator* alloc_;
};


template<typename T>
T*
typed_allocator<T>::allocate()
{
  return static_cast<T*>(alloc_->allocate(sizeof(T), alignof(T)));
}


template<typename T>
T*
typed_allocator<T>::allocate(int n)
{
  return static_cast<T*>(alloc_->allocate(n * sizeof(T), alignof(T)));
}


} // namespace beaker


#endif

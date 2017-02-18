// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "stream.hpp"

#include <iostream>


namespace beaker {

// FIXME: This needs to be rewritten so that it works like a generator stream.
#if 0

/// Returns true if at the end of input.
bool 
istream_stream::eof() const 
{ 
  return is->eof(); 
}

static char zero = 0;

/// Returns the current character in the stream.
const char&
istream_stream::peek() const
{
  if (eof())
    return zero;
  else
    return is->peek();
}

/// Returns the nth character in the stream. This works by reading up to 
/// n characters into a buffer and then re-inserting them into the stream.
const char&
istream_stream::peek(int n) const
{
  // FIXME: Use a deque like the generator stream to buffer beyond the end.
  // Note that once we commit to this, it would be worthwhile to factor the
  // queuing facility into a base class for reuse.
  assert(false && "not implemented");
  
  // std::string buf;
  // while (!eof() && n != 0) {
  //   buf += peek();
  //   --n;
  // }
  // if (eof())
  //   return 0;
  // char ret = buf.back();
  // for (char c : buf)
  //   is->putback(c);
  // return ret;
}

/// Returns the current character in the stream and advances.
char
istream_stream::get()
{
  if (eof())
    return zero;
  else
    return is->get();
}

/// Advances the stream.
void
istream_stream::ignore()
{
  is->ignore();
}

/// Advances the stream.
void
istream_stream::ignore(int n)
{
  is->ignore(n);
}
#endif

} // namespace beaker
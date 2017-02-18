// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "stream.hpp"

#include <iostream>


namespace beaker {

/// Returns true if at the end of input.
bool 
istream_stream::eof() const 
{ 
  return is->eof(); 
}

/// Returns the current character in the stream.
char
istream_stream::peek() const
{
  if (eof())
    return 0;
  else
    return is->peek();
}

/// Returns the nth character in the stream. This works by reading up to 
/// n characters into a buffer and then re-inserting them into the stream.
char
istream_stream::peek(int n) const
{
  std::string buf;
  while (!eof() && n != 0) {
    buf += peek();
    --n;
  }
  if (eof())
    return 0;
  char ret = buf.back();
  for (char c : buf)
    is->putback(c);
  return ret;
}

/// Returns the current character in the stream and advances.
char
istream_stream::get()
{
  if (eof())
    return 0;
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

} // namespace beaker
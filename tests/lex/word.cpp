// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include <beaker/lex/word.cpp>

#include <cassert>
#include <iostream>


using namespace beaker;

word_lexer
lexer(const char *first, const char* limit)
{
  return word_lexer(first, limit);
}

/// Returns true if [first, limit) is a C word.
bool
lex(const char *first, const char* limit)
{
  word_lexer lex(first, limit);
  return lex.ok() && lex.word_begin() == first && lex.word_end() == limit;
}

/// Returns true if the first n
bool
lex(int n, const char *first, const char* limit)
{
  word_lexer lex(first, limit);
  return lex.ok() && lex.word_begin() == first && lex.word_end() == first + n;
}

template<int N>
bool 
pass(const char (&str)[N])
{
  return lex(str, str + N - 1);
}

template<int N>
bool 
pass(const char (&str)[N], int n)
{
  return lex(n, str, str + N - 1);
}

template<int N>
bool 
fail(const char (&str)[N])
{
  return !lex(str, str + N - 1);
}

int main()
{
  assert(pass("x"));
  assert(pass("x1"));
  assert(pass("_"));
  assert(pass("_x"));
  assert(pass("x_"));
  assert(pass("__"));
  assert(pass("xxx z", 3)); // identifier stops after last 'x'.
  assert(fail(""));
  assert(fail("0"));
}

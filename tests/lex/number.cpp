// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include <beaker/lex/number.cpp>

#include <cassert>


using namespace beaker;

using num_lexer = number_lexer<'_'>; // use '_' as a number separator.

num_lexer
lexer(const char *first, const char* limit)
{
  return num_lexer(first, limit);
}

/// Returns true if [first, limit) spell an integer.
std::string
lex_int(const char *first, const char* limit)
{
  num_lexer lex(first, limit);
  if (!lex.ok())
    std::cout << "error: " << lex.get_error() << '\n';
  assert(lex.ok());
  return std::string(lex.integer_begin(), lex.integer_end());
}

template<int N>
std::string 
lex_int(const char (&str)[N])
{
  return lex_int(str, str + N - 1);
}

int main()
{
  // assert(lex_int("3") == "3");
  // assert(lex_int("3a") == "3");
  // assert(lex_int("1_000") == "1_000");
  assert(lex_int("0b0000") == "0000");
  assert(lex_int("0xdead_beef") == "dead_beef");
}

// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef TEST_UTIL_HPP
#define TEST_UTIL_HPP

#include <beaker/base/lang.hpp>
#include <beaker/base/module.hpp>
#include <beaker/base/comparison/equal.hpp>
#include <beaker/base/comparison/hash.hpp>
#include <beaker/base/printing/print.hpp>

#include <beaker/util/symbol_table.hpp>


using namespace beaker;

// Check if two terms are equal and hash equivalent.
template<typename T>
void 
check_equal_terms(const language& lang, const T& t1, const T& t2)
{
  // Check equality
  assert(equal(lang, t1, t2));

  // Check hash equivalence.
  hasher h1;
  hash(lang, h1, t1);
  hasher h2;
  hash(lang, h2, t2);
  assert((std::size_t)h1 == (std::size_t)h2);
}

// Check if two terms are identical (the same object), and also equal.
template<typename T>
void
check_identical_terms(const language& lang, const T& t1, const T& t2)
{
  assert(&t1 == &t2);
  check_equal_terms(lang, t1, t2);
}

// Check that two terms are not equal.
template<typename T>
void 
check_different_terms(const language& lang, const T& t1, const T& t2)
{
  assert(!equal(lang, t1, t2));
}



#endif

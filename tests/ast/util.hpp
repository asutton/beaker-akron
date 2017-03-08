// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef TEST_UTIL_HPP
#define TEST_UTIL_HPP

#include <beaker/base/lang.hpp>
#include <beaker/base/module.hpp>
#include <beaker/base/symbol_table.hpp>
#include <beaker/base/comparison/equal.hpp>
#include <beaker/base/comparison/hash.hpp>
#include <beaker/base/printing/print.hpp>
#include <beaker/base/evaluation/evaluate.hpp>


#include <iostream>


using namespace beaker;

/// Check if two terms are equal and hash equivalent.
template<typename T>
void 
check_equal_terms(const language& lang, const T& t1, const T& t2)
{
  std::clog << pretty(lang, t1) << " ~= " << pretty(lang, t2) << '\n';
  
  // Check equality
  assert(equal(t1, t2));

  // Check hash equivalence.
  hasher h1;
  hash(h1, t1);
  hasher h2;
  hash(h2, t2);
  assert((std::size_t)h1 == (std::size_t)h2);
}

/// Check if two terms are identical (the same object), and also equal.
template<typename T>
void
check_identical_terms(const language& lang, const T& t1, const T& t2)
{
  std::clog << pretty(lang, t1) << " ~~ " << pretty(lang, t2) << '\n';

  // Check identity and equality (identity implies equality).
  assert(&t1 == &t2);
  assert(equal(t1, t2));

  // Also check hash equivalence so that we know its defined.
  hasher h1;
  hash(h1, t1);
  hasher h2;
  hash(h2, t2);
  assert((std::size_t)h1 == (std::size_t)h2);
}

/// Check that two terms are not equal.
template<typename T>
void 
check_different_terms(const language& lang, const T& t1, const T& t2)
{
  std::clog << pretty(lang, t1) << " !~= " << pretty(lang, t2) << '\n';
  assert(!equal(t1, t2));
}


/// Check that `e` evaluates to the given value.
void 
check_value(const language& lang, const expr& e, const value& v)
{
  std::clog << pretty(lang, e) << " ~> " << v << '\n';
  evaluator eval(lang);
  assert(evaluate(eval, e) == v);
}

/// Check that the evaluation of `e` fails.
///
/// \todo Should we try to detect the specific kind of error? This would
/// require including headers defining overrides.
void
check_error(const language& lang, const expr& e)
{
  std::clog << pretty(lang, e) << " ~> error\n";
  int f = false;
  evaluator eval(lang);
  try {
    evaluate(eval, e);
  } catch (evaluation_error& err) {
    f = true;
  }
  assert(f);
}


#endif

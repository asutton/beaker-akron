// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "util.hpp"

#include <beaker/sys.bool/ast.hpp>
#include <beaker/sys.int/ast.hpp>

#include <limits>


int 
main()
{
  symbol_table syms;
  language lang(syms, {
    new sys_bool::feature(),
    new sys_int::feature(),
  });
  module mod(lang);
  auto& ib = mod.get_builder<sys_int::feature>();

  // FIXME: Add a bunch more uniqueness tests.
  check_equal_terms(lang, ib.get_int8_type(), ib.get_int8_type());
  check_different_terms(lang, ib.get_int8_type(), ib.get_int16_type());


  auto& n8 = ib.get_nat8_type();
  using n8_t = std::uint8_t;
  // std::cout << (int)n8.min() << '\n';
  // std::cout << (int)n8.max() << '\n';
  assert(n8.min() == std::numeric_limits<n8_t>::min());
  assert(n8.max() == std::numeric_limits<n8_t>::max());

  auto& n16 = ib.get_nat16_type();
  using n16_t = std::uint16_t;
  // std::cout << (int)n16.min() << '\n';
  // std::cout << (int)n16.max() << '\n';
  assert(n16.min() == std::numeric_limits<n16_t>::min());
  assert(n16.max() == std::numeric_limits<n16_t>::max());

  auto& z8 = ib.get_int8_type();
  using z8_t = std::int8_t;
  // std::cout << (int)z8.min() << '\n';
  // std::cout << (int)z8.max() << '\n';
  assert(z8.min() == std::numeric_limits<z8_t>::min());
  assert(z8.max() == std::numeric_limits<z8_t>::max());

  auto& z16 = ib.get_int16_type();
  using z16_t = std::int16_t;
  // std::cout << (int)z16.min() << '\n';
  // std::cout << (int)z16.max() << '\n';
  assert(z16.min() == std::numeric_limits<z16_t>::min());
  assert(z16.max() == std::numeric_limits<z16_t>::max());

  auto& m8 = ib.get_mod8_type();
  using m8_t = std::uint8_t;
  // std::cout << (int)m8.min() << '\n';
  // std::cout << (int)m8.max() << '\n';
  assert(m8.min() == std::numeric_limits<m8_t>::min());
  assert(m8.max() == std::numeric_limits<m8_t>::max());
}

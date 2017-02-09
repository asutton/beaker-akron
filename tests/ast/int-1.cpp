// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "util.hpp"

#include <beaker/sys.bool/ast.hpp>
#include <beaker/sys.int/ast.hpp>

#include <limits>


template<typename T>
T conv(const value& v)
{
  return (T)v.get_int();
}

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

  // FIXME: This is bad.
  auto& x = ib.get_int8_type();
  auto& y = ib.get_int16_type();
  check_different_terms(lang, x, y);


  auto& n8 = ib.get_nat8_type();
  using n8_t = std::uint8_t;
  // std::cout << (int)conv<n8_t>(n8.min()) << '\n';
  // std::cout << (int)conv<n8_t>(n8.max()) << '\n';
  assert(conv<n8_t>(n8.min()) == std::numeric_limits<n8_t>::min());
  assert(conv<n8_t>(n8.max()) == std::numeric_limits<n8_t>::max());

  auto& n16 = ib.get_nat16_type();
  using n16_t = std::uint16_t;
  // std::cout << (int)conv<n16_t>(n16.min()) << '\n';
  // std::cout << (int)conv<n16_t>(n16.max()) << '\n';
  assert(conv<n16_t>(n16.min()) == std::numeric_limits<n16_t>::min());
  assert(conv<n16_t>(n16.max()) == std::numeric_limits<n16_t>::max());

  auto& z8 = ib.get_int8_type();
  using z8_t = std::int8_t;
  // std::cout << (int)conv<z8_t>(z8.min()) << '\n';
  // std::cout << (int)conv<z8_t>(z8.max()) << '\n';
  assert(conv<z8_t>(z8.min()) == std::numeric_limits<z8_t>::min());
  assert(conv<z8_t>(z8.max()) == std::numeric_limits<z8_t>::max());

  auto& z16 = ib.get_int16_type();
  using z16_t = std::int16_t;
  std::cout << z16.get_precision() << '\n';
  std::cout << (int)conv<z16_t>(z16.min()) << '\n';
  std::cout << (int)conv<z16_t>(z16.max()) << '\n';
  // assert(conv<z16_t>(z16.min()) == std::numeric_limits<z16_t>::min());
  // assert(conv<z16_t>(z16.max()) == std::numeric_limits<z16_t>::max());

  auto& m8 = ib.get_mod8_type();
  using m8_t = std::uint8_t;
  // std::cout << (int)conv<m8_t>(m8.min()) << '\n';
  // std::cout << (int)conv<m8_t>(m8.max()) << '\n';
  assert(conv<m8_t>(m8.min()) == 0);
  assert(conv<m8_t>(m8.max()) == 255);
}

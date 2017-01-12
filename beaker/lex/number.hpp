// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BEAKER_LEX_NUMBER_HPP
#define BEAKER_LEX_NUMBER_HPP

#include <cassert>
#include <cctype>

#include <iostream>
#include <string>


namespace beaker {

/// Returns true if c is in the set [0-9].
inline bool
is_decimal_digit(char c)
{
  return std::isdigit(c);
}

/// Returns true if c is in the set [0-1].
inline bool
is_binary_digit(char c)
{
  return c - '0' < 2;
}

/// Returns true if c is in the set [0-7].
inline bool
is_octal_digit(char c)
{
  return c - '0' < 8;
}

/// Returns true if c is in the set [0-9a-fA-F].
inline bool
is_hexadecimal_digit(char c)
{
  return std::isxdigit(c);
}


/// A facility for lexing common forms of integer and floating point numbers.
/// This lexer includes prefixes for binary, octal, and hex numbers and digit
/// separators. The digit separator character is given as an optional template 
/// argument.
///
/// The format for numbers generally follows the C/C++ rules for forming numbers
/// except for the octal prefix and restrictions on the writing of real numbers. 
/// This lexer requires '0o' or '0O' as the prefix for octal numbers. It also 
/// requires both integer and fraction parts of the floating point
///
///   number:
///     integer-literal
///     floating-point-literal
///
///   integer-literal:
///     decimal-integer-literal
///     binary-integer-literal
///     octal-integer-literal
///     hexadecimal-integer-literal
///
///   decimal-integer-literal:
///     digits(0-9)
///
///   binary-integer-literal:
///     0[bB] digits(0-1)
///
///   octal-integer-literal:
///     0[oO] digits(0-7)
///
///   hexadecimal-integer-literal:
///     0[xX] digits(0-9a-fA-F)
///
///   real-literal:
///     decimal-real-literal
///     hexadecimal-real-literal
///
///   decimal-real-literal
///     digits(0-9) '.' digits(0-9) decimal-exponent?
///     digits(0-9) decimal-exponent
///   
///   decimal-exponent:
///     [eE] [+-]? digits(0-9)
///
///   hexadecimal-real-literal
///     0[xX] digits(0-9a-fA-F) '.' digits(0-9a-fA-F) hexadecimal-exponent?
///     0[xX] digits(0-9a-fA-F) hexadecimal-exponent
///
///   hexadecimal-exponent:
///     [pP] [+-]? digits(0-9a-fA-F)
///
/// The pseudo-function "digits" means a sequence of digits possibly 
/// interleaved with digit separator characters.
///
/// Note that this lexer does not include suffixes for the literals that it
/// parses. The expectation is that suffixes be lexed as an adjacent sequence
/// of characters, possibly with an intervening operator. Note that the choice
/// of digit separator and suffix operator are not readily chosen.
///
///   1_100~u       -- unsigned 10.
///   0x0.000_1 f   -- floating point hex float.
///
/// \todo Add support for hex literals.
template<char Sep = 0>
struct number_lexer
{
  enum error {
    success,
    empty_string,
    invalid_base,
    invalid_number,
    empty_integer,
    invalid_integer,
    empty_fraction,
    invalid_fraction,
    invalid_exponent,
  };
  
  number_lexer(const char* f, const char* l)
  { 
    lex(f, l); 
  }

  explicit operator bool() const { return ok(); }

  /// Returns true if the lex was successesful.
  bool ok() const { return err == success; }

  /// Returns the error code generated from a failed parse.
  error get_error() const { return err; }

  bool has_prefix() const { return pre; }
  bool has_integer() const { return int_start; }
  bool has_fraction() const { return frac_start; }
  bool has_exponent() const { return exp; }
  bool has_sign() const { return sign; }

  /// Returns true if the lex resulted in an integer value. That is, only
  /// an integer part was parsed.
  bool is_integer() const { return int_start && !is_real(); }

  /// Returns true if the lex resulted in a real value. That is, either a
  /// fractional or exponential part was parsed.
  bool is_real() const { return frac_start || exp_start; }

  /// Returns the radix of the number.
  int get_radix() const { return radix; }

  /// Returns the +1/-1 sign of the exponent.
  int get_sign() const { return mul; }

  const char* prefix_begin() const { return pre; }
  const char* prefix_end() const { return pre + 2; }

  // Returns the characters in the integer part.
  const char* integer_begin() const { return int_start; }
  const char* integer_end() const { return int_end; }

  // Returns the characters in the fraction.
  const char* fraction_begin() const { return frac_start; }
  const char* fraction_end() const { return frac_end; }

  // Returns the characters in the exponent.
  const char* exponent_begin() const { return exp_start; }
  const char* exponent_end() const { return exp_end; }

  void lex(const char*, const char*);
  const char* lex_prefix(const char*, const char*);
  const char* lex_integer(const char*, const char*);
  const char* lex_fraction(const char*, const char*);
  
  const char* lex_exponent(const char*, const char*);
  template<typename P> const char* lex_exponent(const char*, const char*, const char*, P);
  const char* lex_power(const char*, const char*, const char*);
  const char* lex_sign(const char*, const char*);
  
  const char* lex_digits(const char*, const char*);
  template<typename P> const char* lex_digits(const char*, const char*, P);
  template<typename P> const char* lex_digits_nosep(const char*, const char*, P);

  void fail(error errc) { err = errc; }

  const char* pre = nullptr; // A pointer to the 2 character prefix, if given.
  const char* int_start = nullptr; // The start of the integer part
  const char* int_end = nullptr; // The end of the integer part.
  const char* dot = nullptr; // A pointer to the decimal point, if present.
  const char* frac_start = nullptr; // The start of fractional digits.
  const char* frac_end = nullptr; // The end of fractional digits.
  const char* exp = nullptr; // The start of an exponent.
  const char* sign = nullptr; // The sign of exponent.
  const char* exp_start = nullptr; // The start exponent digits.
  const char* exp_end = nullptr; // The end of exponent digits.

  int radix = 0; // The numeric radix.
  int mul = 0; // The exponent multiplier.
  error err = success; // An error code.
};

template<char C>
inline void
number_lexer<C>::lex(const char* first, const char* limit)
{
  if (first == limit) // Not much to do here.
    return fail(empty_string);
  if (!std::isdigit(*first)) // All numbers start with [0-9].
    return fail(invalid_number);

  // Lex the optional prefix. This puts the lexer in a 'radix' mode
  // for the remainder of the lex.
  first = lex_prefix(first, limit);
  if (pre) {
    if (radix == -1)
      return fail(invalid_base); // Got something like '0c'
    if (first == limit)
      return fail(invalid_number); // Have a prefix at end of input
  }

  // Lex the optional integer part.
  first = lex_integer(first, limit);
  if (!first)
    return fail(invalid_integer);
  if (first == limit)
    return;

  // Try to match the fractional part. 
  if (*first == '.') {
    if (radix == 2 || radix == 8) // Disallow binary, octal floats.
      return fail(invalid_fraction);
    dot = first;
    ++first;
    first = lex_fraction(first, limit);
    if (!first)
      return fail(empty_fraction); // Got something like '.a' or '. '.
  }

  // Lex the optional exponent.
  first = lex_exponent(first, limit);
  if (!first) {
    return fail(invalid_exponent);
  }
}

/// Lex an optional prefix. Returns a pointer to the first character beyond 
/// the optional prefix. If the first two characters do not spell a numeric 
/// prefix, then we default to a decimal number, and do not advance the lexer.
template<char C>
inline const char*
number_lexer<C>::lex_prefix(const char* first, const char* limit)
{
  if (first == limit) // There is
    return nullptr;
  if (*first != '0') { // Not a prefix, but not an error (e.g., '1')
    radix = 10;
    return first;
  }
  
  // '04' is not a prefix, but it is an integer. Note that '0c' is not
  // a hexadecimal integer at this point because we need to see '0x' in
  // order to put the lexer in "hex mode".
  const char* ptr = first + 1;
  if (std::isdigit(*ptr)) {
    radix = 10;
    return first;
  }

  // Note the start of the prefix. Note that the prefix indicator may yet
  // be invalid.
  pre = first;
  
  // Check the prefix specifier.
  switch (*ptr) {
    case 'b':
    case 'B':
      radix = 2;
      break;
    
    case 'o':
    case 'O':
      radix = 8;
      break;
    
    case 'x': 
    case 'X':
      radix = 16;
      break;
    
    default:
      // We got something like '0c', which is neither a valid prefix nor
      // integer.
      radix = -1;
      break;
  }
  return ptr + 1;
}

/// Lex the integer part, returning a pointer to the first character
/// past that part.
template<char C>
inline const char*
number_lexer<C>::lex_integer(const char* first, const char* limit)
{
  int_start = first;
  int_end = lex_digits(first, limit);
  return int_end;
}

/// Lex the fraction part, returning a pointer to the first character
/// past that part.
template<char C>
inline const char*
number_lexer<C>::lex_fraction(const char* first, const char* limit)
{
  frac_start = first;
  frac_end = lex_digits(first, limit);
  return frac_end;
}

/// Lex the exponent part, returning a pointer to the first character
/// past that part.
template<char C>
inline const char*
number_lexer<C>::lex_exponent(const char* first, const char* limit)
{
  if (radix == 10)
    return lex_exponent(first, limit, "eE", is_decimal_digit);
  else if (radix == 16)
    return lex_exponent(first, limit, "pP", is_hexadecimal_digit);
  return exp_end;
}

template<char C>
template<typename P>
const char*
number_lexer<C>::lex_exponent(const char* first, const char* limit, const char* chars, P pred) 
{
  // Lex the introducer.
  first = lex_power(first, limit, chars);
  if (!exp)
    return first;
  if (first == limit) // 'e' or 'p' followed by end of input.
    return nullptr;

  // Lex the optional sign.
  first = lex_sign(first, limit);
  if (first == limit) // 'e+' followed by end of input.
    return nullptr;
  
  // Lex the digits in the exponent.
  exp_start = first;
  exp_end = lex_digits(first, limit, pred);
  return exp_end;
}


/// Lexes the beginning of an exponent or power. If that symbol is not present
/// the lex state is unchanged. 
template<char C>
const char*
number_lexer<C>::lex_power(const char* first, const char* limit, const char* chars)
{
  assert(first != limit);
  if (*first == chars[0] || *first == chars[1]) {
    exp = first;
    ++first;
  } 
  return first;
}

template<char C>
inline const char*
number_lexer<C>::lex_sign(const char* first, const char* limit)
{
  if (*first == '+') {
    sign = first;
    mul = 1;
    ++first;
  } else if (*first == '-') {
    sign = first;
    mul = -1;
    ++first;
  }
  return first;
}



/// Lex a digit sequence in an appropriate base.
template<char C>
const char*
number_lexer<C>::lex_digits(const char* first, const char* limit) 
{
  switch (get_radix()) {
    case 2:
      return lex_digits(first, limit, is_binary_digit);
    case 8:
      return lex_digits(first, limit, is_octal_digit);
    case 10:
      return lex_digits(first, limit, is_decimal_digit);
    case 16:
      return lex_digits(first, limit, is_hexadecimal_digit);
    default:
      assert(false && "invalid base");
  }
}

/// Lex a sequence of possibly separated digits. In BNF, we would have:
///
///   digits(S):
///     digits-nosep(S)
///     digits-nosep(S) sep digits(S)
///
///   digits-nosep(S):
///     S+
///
/// Note that if sep is '\0', then digits is defined thusly:
///
///   digits(S):
///     digits-nosep(S)
///
/// Returns a pointer past the last digit in the given base. Returns nullptr
/// if the digit sequence is ill-formed.
///
/// TODO: Improve diagnostics around bad digit separators.
template<char C>
template<typename P>
const char*
number_lexer<C>::lex_digits(const char* first, const char* limit, P pred) 
{
  // Ensure a single digit.
  first = lex_digits_nosep(first, limit, pred);
  if (!first) // No valid digits
    return first;
  
  if (C == 0) // Digits can't be separated.
    return first;

  // Match sep digits(S)
  while (first != limit) {
    // Do we have a digit separator?
    if (*first != C)
      return first;
    ++first;
    
    // If so, we require a sequence of digits.
    first = lex_digits_nosep(first, limit, pred);
    if (!first) // No valid digits.
      return first;
  }
  return first;
}

/// Lex a sequence of non-separated digits.
template<char C>
template<typename P>
const char*
number_lexer<C>::lex_digits_nosep(const char* first, const char* limit, P pred) 
{
  if (first == limit) // Expected digits, got end of input.
    return nullptr;
  if (!pred(*first)) // Leading character has wrong class.
    return nullptr;
  ++first;
  while (first != limit && pred(*first))
    ++first;
  return first;
}


} // namespace beaker

#endif

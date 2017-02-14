// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "lexer.hpp"

#include <iostream>


namespace icalc {


/// Returns the next token in the sequence.
token*
lexer::next() 
{
  while (!eof()) {
    switch (lookahead()) {
      case '\n':
        // FIXME: Track line numbers.
        // Fallthrough.
      case ' ':
      case '\t':
        // Skip whitespace.
        consume();
        continue;

      case '(': return lparen();
      case ')': return rparen();
      case '+': return plus();
      case '-': return minus();
      case '*': return star();
      case '/': return slash();
      case '%': return percent();
      case '&': return amp();
      case '|': return pipe();
      case '<': return lt();
      case '>': return gt();
      case '=': return eq();
      case '!': return bang();

      default:
        if (std::isalpha(lookahead() || lookahead() == '_')) 
          return word();
        if (std::isdigit(lookahead()))
          return number();
        break;
    }
  }
  return nullptr;
}

token*
lexer::lparen()
{
  require('(');
  return make_basic_token(1, lparen_tok);
}

token*
lexer::rparen()
{
  require(')');
  return make_basic_token(1, rparen_tok);
}

token*
lexer::plus()
{
  require('+');
  return make_basic_token(1, plus_tok);
}

token*
lexer::minus()
{
  require('-');
  return make_basic_token(1, minus_tok);
}

token*
lexer::star()
{
  require('*');
  return make_basic_token(1, star_tok);
}

token*
lexer::slash()
{
  require('/');
  return make_basic_token(1, slash_tok);
}

token*
lexer::percent()
{
  require('%');
  return make_basic_token(1, percent_tok);
}

/// Process a token beginning with an '&'.
token* 
lexer::amp() 
{
  require('&');
  if (lookahead() == '&')
    amp_amp();
  return make_basic_token(1, amp_tok); 
}

/// Process the '&&' token.
token*
lexer::amp_amp() 
{
  require('&');
  return make_basic_token(2, amp_amp_tok);
}

/// Process a token beginning with a '|'.
token* 
lexer::pipe() 
{
  require('|');
  if (lookahead() == '&')
    pipe_pipe();
  return make_basic_token(1, pipe_tok); 
}

/// Process the '||' token.
token*
lexer::pipe_pipe() 
{
  require('|');
  return make_basic_token(2, pipe_pipe_tok);
}

/// Process a token beginning with a '<'.
token* 
lexer::lt() 
{
  require('<');
  if (lookahead() == '=')
    lt_eq();
  return make_basic_token(1, lt_tok); 
}

/// Process the '<=' token.
token*
lexer::lt_eq()
{
  require('=');
  return make_basic_token(2, lt_eq_tok);
}

/// Process a token beginning with a '>'.
token* 
lexer::gt()
{
  require('>');
  if (lookahead() == '=')
    gt_eq();
  return make_basic_token(1, gt_tok); 
}

/// Process the '>=' token.
token*
lexer::gt_eq()
{
  require('=');
  return make_basic_token(2, gt_eq_tok);
}

/// Process a token beginning with a '='.
token* 
lexer::eq() 
{
  require('=');
  if (lookahead() == '=')
    eq_eq();

  // FIXME: Throw a lexical error, including source location. Note 
  // that this actually diagnoses sequence of charaters '=X' where
  // 'X' is not '='.
  throw std::runtime_error("invalid character");
}

/// Process the '==' token.
token*
lexer::eq_eq() 
{
  require('=');
  return make_basic_token(2, eq_eq_tok);
}

/// Process a token beginning with '!'.
token*
lexer::bang()
{
  require('!');
  if (lookahead() == '=')
    return bang_eq();
  return make_basic_token(1, bang_tok);
}

token*
lexer::bang_eq()
{
  require('=');
  return make_basic_token(2, bang_eq_tok);
}

/// Consumes the lookahead if it is a letter and returns true. Returns
/// false otherwise.
bool
lexer::letter()
{
  return match_if([](char c) { 
    return std::isalpha(c); 
  });
}

/// Consumes the lookahead if it is a digit and returns true. Returns false
/// otherewise.
bool
lexer::digit()
{
  return match_if([](char c) { 
    return std::isdigit(c); 
  });
}

/// Returns the lookahead if it is an identifier character and returns true.
/// Otherwise, no action is performed and returns false.
bool 
lexer::ident()
{
  return match_if([](char c) {
    return std::isalpha(c) || std::isdigit(c) || c == '_';
  });
}

/// Match an identifier or keyword in the language.
token*
lexer::word()
{
  // Match the characters in the word.
  const char* iter = first;
  consume();
  while (!eof() && ident())
    ;

  // Determine if the identifier is a reserved word.
  //
  // TODO: Use a keyword hash table for full generality.
  std::string str(iter, first);
  if (str == "true")
    return make<bool_token>(bool_tok, true);
  if (str == "false")
    return make<bool_token>(bool_tok, false);
  throw std::runtime_error("invalid character sequence '" + str + "'");
}

token*
lexer::number()
{
  const char* iter = first;
  consume();
  while (!eof() && digit())
    ;

  std::string val(iter, first);
  return make<int_token>(int_tok, std::stoi(val));
}

} // namespace icalc
// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "lexer.hpp"

#include <iostream>
#include <sstream>


namespace icalc {


/// Returns the next token in the sequence.
token*
lexer::next() 
{
  while (!eof()) {
    // Perform an initial check for whitespace, consuming those characters.
    switch (lookahead()) {
      case '\n':
        newline();
        continue;
      
      case ' ':
      case '\t':
        space();
        continue;
      default:
        break;
    }

    // Marks the beginning of non-whitespace characters.
    start = curr;
    switch (lookahead()) {
      // Matches punctuators and operators.
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

      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
        return number();

      case 't': case 'f':
        // TODO: We could build very precise matches for these keywords.
        return word();

      default: {
        std::stringstream ss;
        ss << "invalid character '" << lookahead() << "'";
        error(ss.str());
        break;
      }
    }
  }
  return nullptr;
}

/// Matches horizontal whitespace.
///
///   space -> ' ' | '\t'
///
/// Ignores the space.
void
lexer::space()
{
  consume();
}

/// Matches vertical whitespace.
///
///   newline -> '\n'
///
/// Increments the line count.
///
/// \todo Manage the line count.
void
lexer::newline()
{
  consume();
}

/// Matches left-parenthesis.
///
///   lparen -> '('
///
/// Returns the corresponding token.
token*
lexer::lparen()
{
  require('(');
  return make_basic_token(1, lparen_tok);
}

/// Matches right-parenthesis.
///
///   rparen -> '('
///
/// Returns the corresponding token.
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
    return pipe_pipe();
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
    return lt_eq();
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
    return gt_eq();
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
    return eq_eq();

  // FIXME: Add the source location.
  error("expected '=' after '='");
  return nullptr;
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
  consume();
  while (!eof() && ident())
    ;

  // Determine if the identifier is a reserved word.
  //
  // TODO: Use a keyword hash table for full generality.
  std::string str(start, curr);
  if (str == "true")
    return make<bool_token>(bool_tok, true);
  if (str == "false")
    return make<bool_token>(bool_tok, false);

  // FIXME: Use a better error.
  error("invalid identifier");
  return nullptr;
}

token*
lexer::number()
{
  consume();
  while (!eof() && digit())
    ;
  std::string val(start, curr);
  return make<int_token>(int_tok, std::stoi(val));
}

/// Throws lexical error exception with the given message.
///
/// \todo Incorporate the source code location.
void
lexer::error(const std::string& msg)
{
  throw std::runtime_error(msg);
}

} // namespace icalc
// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "lexer.hpp"

#include <iostream>
#include <sstream>


namespace icalc {

/// Returns the next token in the sequence.
///
/// FIXME: Don't simply assert on invalid characters. Diagnose them and 
/// continue lexing.
token
lexer::operator()() 
{
  // Reset the symbol buffer.
  buf.clear();

  // Ignore whitespace.
  space();

  // Match a token at the current location.
  start_token();
  switch (lookahead()) {
    case 0: return end();

    // Matches punctuators and operators.
    case '(': return lparen();
    case ')': return rparen();
    case '+': return plus();
    case '-': return minus();
    case '*': return star();
    case '/': return slash();
    case '%': return percent();
    case '&': return amp();
    case '|': return bar();
    case '^': return caret();
    case '~': return tilde();
    case '<': return lt();
    case '>': return gt();
    case '=': return eq();
    case '!': return bang();
    case '?': return question();
    case ':': return colon();

    // Matches digits.
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
      return number();

    case 't': case 'f':
      // TODO: We could build very precise matches for these keywords.
      return word();

    default: {
      char ch = consume();
      note_end();
      std::stringstream ss;
      ss << "invalid character '" << ch << '\'';
      error(loc, ss.str());
      break;
    }
  }

  // FIXME: This is probably not reachable.
  assert(false && "unreachable");
}

/// Matches whitespace.
///
///   space -> ' ' | '\t' | '\n'
///
/// This simply ignores the space.
///
/// \todo Keep track of the source line.
void
lexer::space()
{
  while (!eof()) {
    switch (lookahead()) {
      case '\n':
        // Increment the line count and reset the column number.
        ++line;
        col = 1;
      case ' ':
      case '\t':
        ignore();
        continue;
      default:
        return;
    }
  }
}

/// Returns the end-of-file token.
token 
lexer::end()
{ 
  return finish_token(eof_tok); 
}

/// Matches left parenthesis.
///
///   lparen-token -> '('
///
/// Returns the corresponding token.
token
lexer::lparen()
{
  require('(');
  return finish_token(lparen_tok);
}

/// Matches right parenthesis.
///
///   rparen-token -> '('
///
/// Returns the corresponding token.
token
lexer::rparen()
{
  require(')');
  return finish_token(rparen_tok);
}

/// Matches '+' operator.
///
///   plus-operator -> '+'
///
/// Returns the corresponding token.
token
lexer::plus()
{
  require('+');
  return finish_token(plus_tok);
}

/// Matches '-' operator.
///
///   minus-operator -> '-'
///
/// Returns the corresponding token.
token
lexer::minus()
{
  require('-');
  return finish_token(minus_tok);
}

/// Matches '*' operator.
///
///   star-operator -> '*'
///
/// Returns the corresponding token.
token
lexer::star()
{
  require('*');
  return finish_token(star_tok);
}

/// Matches '/' operator.
///
///   slash-operator -> '/'
///
/// Returns the corresponding token.
token
lexer::slash()
{
  require('/');
  return finish_token(slash_tok);
}

/// Matches '%' operator.
///
///   percent-operator -> '%'
///
/// Returns the corresponding token.
token
lexer::percent()
{
  require('%');
  return finish_token(percent_tok);
}

/// Matches '&' or '&&' operators.
///
///   amp-operator -> '&'
///   amp-amp-operator -> '&&'
///
/// Returns the corresponding token.
token 
lexer::amp() 
{
  require('&');
  if (match('&'))
    return finish_token(amp_amp_tok);
  else
    return finish_token(amp_tok);
}

/// Matches the '|' or '||' operators.
///
///   bar-operator -> '|'
///   bar-bar-operator -> '||'
///
/// Returns the corresponding token.
token 
lexer::bar() 
{
  require('|');
  if (match('|'))
    return finish_token(bar_bar_tok);
  else
  return finish_token(bar_tok); 
}

/// Matches the '^' operator.
///
///   caret-operator -> '^'
///
/// Returns the corresponding token.
token
lexer::caret()
{
  require('^');
  return finish_token(caret_tok);
}

/// Matches the '~' operator.
///
///   tilde-operator -> '~'
///
/// Returns the corresponding token.
token
lexer::tilde()
{
  require('~');
  return finish_token(tilde_tok);
}

/// Matches the '<' or '<=' operators.
///
///   lt-operator -> '<'
///   lt-eq-operator -> '<='
///
/// Returns the corresponding token.
token 
lexer::lt() 
{
  require('<');
  if (match('='))
    return finish_token(lt_eq_tok);
  else
    return finish_token(lt_tok); 
}

/// Matches the '>' or '>=' operators.
///
///   gt-operator -> '>'
///   gt-eq-operator -> '>='
///
/// Returns the corresponding token.
token 
lexer::gt()
{
  require('>');
  if (match('='))
    return finish_token(gt_eq_tok);
  else
    return finish_token(gt_tok); 
}

/// Matches the '==' operator.
///
///   eq-eq-operator -> '=='
///
/// Returns the corresponding token.
token 
lexer::eq() 
{
  require('=');
  if (match('='))
    return finish_token(eq_eq_tok);

  note_start();
  char ch = consume();
  note_end();
  std::stringstream ss;
  ss << "expected '=' but got '" << ch << '\'';
  error(loc, ss.str());
}

/// Matches the '!' or '!=' operators.
///
///   bang-operator -> '!'
///   bang-eq-operator -> '!='
///
/// Returns the corresponding token.
token
lexer::bang()
{
  require('!');
  if (match('='))
    return finish_token(bang_eq_tok);
  else
    return finish_token(bang_tok);
}

/// Match the '?' token.
///
///   question -> '?'
token
lexer::question()
{
  require('?');
  return finish_token(question_tok);
}

/// Match the ':' token.
///
///   colon -> ':'
token
lexer::colon()
{
  require(':');
  return finish_token(colon_tok);
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

/// Match a boolean literal.
///
///   boolean-literal -> 'true' | 'false'
///
/// TODO: This is way more general then we need it to be.
token
lexer::word()
{
  consume();
  while (!eof() && ident())
    ;

  if (buf == "true")
    return finish_token<beaker::bool_attr>(bool_tok, true);
  if (buf == "false")
    return finish_token<beaker::bool_attr>(bool_tok, false);

  std::stringstream ss;
  ss << "invalid identifier '" << buf << '\'';
  error(loc, ss.str());
}

/// Matches an integer value.
///
///   integer-literal -> digit digit*
///
///   digit -> '0' | '1' | ... | '9'
///
/// Returns the corresponding token.
token
lexer::number()
{
  consume();
  while (!eof() && digit())
    ;
  return finish_token<beaker::int_attr>(int_tok, std::stoi(buf));
}

/// Throws lexical error exception with the given message.
///
/// \todo Incorporate the source code location.
void
lexer::error(location loc, const std::string& msg)
{
  throw lexical_error(loc, msg.c_str());
}

} // namespace icalc
// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "lexer.hpp"

#include <beaker/base/symbol_table.hpp>

#include <iostream>
#include <sstream>


namespace bpl {

// The global keyword table.
lexer::keyword_table lexer::kw;

lexer::lexer(beaker::symbol_table& syms, stream_type& s) 
  : syms(syms), cs(s), line(1), col(1) 
{
  // Build the keyword table.
  if (kw.empty()) {
#define def_token(T, S)
#define def_keyword(T, S) kw.emplace(S, T ## _kw);
#include "token.def"
  }
}

/// Returns the next token in the sequence.
///
/// FIXME: Don't simply assert on invalid characters. Diagnose them and 
/// continue lexing.
token
lexer::operator()() 
{
  while (true) {
    // Reset the symbol buffer.
    buf.clear();

    // Ignore whitespace.
    space();

    // Match a token at the current location.
    start_token();
    switch (lookahead()) {
      case 0: return end();

      // Matches punctuators and operators.
      case '{': return lbrace();
      case '}': return rbrace();
      case '(': return lparen();
      case ')': return rparen();
      case '[': return lbrack();
      case ']': return rbrack();
      case ';': return semicolon();
      case '.': return dot();
      case '+': return plus();
      case '-': return minus();
      case '*': return star();
      case '/': 
        if (lookahead(1) == '/') {
          comment();
          continue;
        }
        return slash();
      
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

      case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
      case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
      case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
      case 'v': case 'w': case 'x': case 'y': case 'z':
      case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
      case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
      case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
      case 'V': case 'W': case 'X': case 'Y': case 'Z':
      case '_':
        return word();

      default: {
        // TODO: Don't propagate the error. Just buffer it for future use
        // and then continue lexing.
        char ch = consume();
        note_end();
        std::stringstream ss;
        ss << "invalid character '" << ch << '\'';
        error(loc, ss.str());
        break;
      }
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

/// Remove a comment from the character stream.
///
///   comment -> '//' all characters until '\n'
///
/// \todo Scrape comments into a side buffer to support documentation tools.
void
lexer::comment()
{
  while (!eof() && lookahead() != '\n')
    ignore();
}

/// Returns the end-of-file token.
token lexer::end() { return finish_token(eof_tok); }

/// Matches left brace.
///
///   lbrace-token -> '('
///
/// Returns the corresponding token.
token lexer::lbrace() { return require('{'), finish_token(lbrace_tok); }

/// Matches right brace.
///
///   rbrace-token -> '('
///
/// Returns the corresponding token.
token lexer::rbrace() { require('}'); return finish_token(rbrace_tok); }

/// Matches left parenthesis.
///
///   lparen-token -> '('
///
/// Returns the corresponding token.
token lexer::lparen() { return require('('), finish_token(lparen_tok); }

/// Matches right parenthesis.
///
///   rparen-token -> '('
///
/// Returns the corresponding token.
token lexer::rparen() { return require(')'), finish_token(rparen_tok); }

/// Matches left bracket.
///
///   lbrack-token -> '('
///
/// Returns the corresponding token.
token lexer::lbrack() { return require('('), finish_token(lbrack_tok); }

/// Matches right bracket.
///
///   rbrack-token -> '('
///
/// Returns the corresponding token.
token lexer::rbrack() { return require(')'), finish_token(rbrack_tok); }

/// Matches a semicolon.
///
///   semicolon-token -> ';'
///
/// Returns the corresponding token.
token lexer::semicolon() { return require(';'), finish_token(semicolon_tok); }

/// Matches a dot.
///
///   dot-token -> ';'
///
/// Returns the corresponding token.
token lexer::dot() { return require(';'), finish_token(dot_tok); }

/// Matches '+' operator.
///
///   plus-operator -> '+'
///
/// Returns the corresponding token.
token lexer::plus() { return require('+'), finish_token(plus_tok); }

/// Matches '-' operator or '->' punctuator/
///
///   minus-operator -> '-'
///   arrow-punctuator -> '->'
///
/// Returns the corresponding token.
token 
lexer::minus()
{ 
  require('-');
  if (match('>'))
    return finish_token(arrow_tok);
  else
    return finish_token(minus_tok); 
}

/// Matches '*' operator.
///
///   star-operator -> '*'
///
/// Returns the corresponding token.
token lexer::star() { return require('*'), finish_token(star_tok); }

/// Matches '/' operator or the '//' comment delimiter.
///
///   slash-operator -> '/'
token 
lexer::slash() { return require('/'), finish_token(slash_tok); }

/// Matches '%' operator.
///
///   percent-operator -> '%'
///
/// Returns the corresponding token.
token lexer::percent() { return require('%'), finish_token(percent_tok); }

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
/// \todo We need an identifier attribute that links the identifier to a 
/// position in the symbol table.
token
lexer::word()
{
  consume();
  while (!eof() && ident())
    ;

  auto iter = kw.find(buf);
  if (iter != kw.end())
    return finish_token(iter->second);
  else
    return finish_token<symbol_attr>(id_tok, syms.get(buf));
}

/// Matches an integer value.
///
///   integer-literal -> digit digit*
///
///   digit -> '0' | '1' | ... | '9'
///
/// Returns the corresponding token.
///
/// \todo Handle the entire integer literal suite.
token
lexer::number()
{
  consume();
  while (!eof() && digit())
    ;
  return finish_token<int_attr>(dec_int_tok, std::stoi(buf));
}

/// Throws lexical error exception with the given message.
///
/// \todo Incorporate the source code location.
void
lexer::error(location loc, const std::string& msg)
{
  throw lexical_error(loc, msg.c_str());
}

} // namespace bpl
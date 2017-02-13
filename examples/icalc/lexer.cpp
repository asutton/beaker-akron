// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "lexer.hpp"


namespace icalc {

token_store::~token_store()
{
  for (token* p : *this)
    delete p;
}


/// Returns the next token in the sequence.
token*
lexer::next() 
{
  while (!eof()) {
    switch (lookahead()) {
      case ' ':
      case '\t':
      case '\n':
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

      case '&':
        if (lookahead(1) == '&')
          return amp_amp();
        else
          return amp();
      
      case '|':
        if (lookahead(1) == '|')
          return pipe_pipe();
        else
          return pipe();
      
      case '^':
        return caret();

      case '<':
        if (lookahead(1) == '=')
          return lt_eq();
        else
          return lt();
      
      case '>':
        if (lookahead(1) == '=')
          return gt_eq();
        else
          return gt();

      case '=':
        if (lookahead(1) == '=')
          return eq_eq();
        else
          // FIXME: Throw a lexical error, including source location.
          throw std::runtime_error("invalid character");

      case '!':
        if (lookahead(1) == '=')
          return bang_eq();
        else
          return bang();

      default:
        if (std::isalpha(lookahead()))
          return word();
        if (std::isdigit(lookahead()))
          return number();
        break;
    }
  }
  return nullptr;
}


token*
lexer::word()
{
  // Match the characters in the word.
  const char* iter = first;
  consume();
  while (!eof() && isident(lookahead()))
    consume();

  // Determine if this is a keyword.
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
  while (!eof() && std::isdigit(lookahead()))
    consume();

  std::string val(iter, first);
  return make<int_token>(int_tok, std::stoi(val));
}

} // namespace icalc
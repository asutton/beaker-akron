// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BPL_PARSER_HPP
#define BPL_PARSER_HPP

#include "lang.hpp"
#include "lexer.hpp"
#include "semantics.hpp"


namespace bpl {

/// Represents a syntax error.
///
/// \todo Make a translation error that acts as the base class for all errors
/// that can be mapped to source code.
struct syntax_error : std::runtime_error
{
  syntax_error(location, const char*);

  location get_location() const;

  location loc;
};

inline 
syntax_error::syntax_error(location loc, const char* msg)
  : std::runtime_error(msg), loc(loc)
{ }

inline location syntax_error::get_location() const { return loc; }


/// Parses a sequence of tokens to produce a tree.
///
/// \todo Factor semantic actions out of the parser? It's not strictly 
/// necessary, but it does help keep the implementation clean. Also, we could
/// pass the default builder through that separation (i.e., use a translation
/// context that binds a default builder, module, and language object.)
struct parser
{
  using stream_type = beaker::input_stream<token>;

  parser(stream_type& s, builder& b) : ts(s), act(b) { }

  // Stream control
  bool eof() const;
  const token& current() const;
  int lookahead() const;
  int lookahead(int) const;
  token consume();
  void ignore();
  void ignore(int);
  token accept(int);
  token expect(int);
  token require(int);

  bool next_token_is(int) const;
  bool next_token_is_not(int) const;

  location get_location() const;

  // Grammar productions
  name& declaration_name();
  name& identifier();

  // Types
  type& type_id();
  type& simple_type();

  // Expressions
  expr& expression();
  expr& conditional_expression();
  expr& logical_or_expression();
  expr& logical_and_expression();
  expr& bitwise_or_expression();
  expr& bitwise_xor_expression();
  expr& bitwise_and_expression();
  expr& equality_expression();
  expr& relational_expression();
  expr& additive_expression();
  expr& multiplicative_expression();
  expr& unary_expression();
  expr& primary_expression();
  expr& boolean_literal();
  expr& integer_literal();

  // Declarations
  decl& module();
  decl_seq toplevel_declaration_seq();
  decl& module_declaration();
  decl& import_declaration();
  decl& function_declaration();
  decl& parameter_declaration();
  decl& variable_declaration();

  // Statements
  stmt& statement();
  stmt_seq statement_seq();
  stmt& block_statement();

  stream_type& ts;
  semantics act;
};

/// Returns true if the at the end of input.
inline bool parser::eof() const { return ts.eof(); }

/// Returns the current token.
inline const token& parser::current() const { return ts.peek(); }

/// Returns the current lookahead token.
inline int parser::lookahead() const { return ts.peek().get_kind(); }

/// Returns the current lookahead token.
inline int parser::lookahead(int n) const { return ts.peek(n).get_kind(); }

/// Consumes the current lookahead token.
inline token parser::consume() { return ts.get(); }

/// Ignores a single token.
inline void parser::ignore() { return ts.ignore(); }

/// Ignores n tokens.
inline void parser::ignore(int n) { return ts.ignore(n); }

/// Returns true if the next token has kind `k`.
inline bool
parser::next_token_is(int k) const
{
  if (eof())
    return false;
  else
    return lookahead() == k;
}

/// Return true if the next token is not `k`.
inline bool
parser::next_token_is_not(int k) const
{
  return !next_token_is(k);
}

inline location
parser::get_location() const
{
  return ts.peek().get_location();
}

} // namespace bpl

#endif


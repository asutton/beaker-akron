// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "lang.hpp"
#include "lexer.hpp"
#include "parser.hpp"

#include <beaker/base/printing/print.hpp>
#include <beaker/base/evaluation/evaluate.hpp>

#include <iostream>


/// Reads an expression from the given input. Returns the parsed expression
/// if the input is valid. Returns nullptr if no expression was found (e.g.,
/// the line was empty). Throws when a non-empty line contains a syntactic
/// or semantic error.
///
/// FIXME: Read from a stream inste
beaker::expr*
read(icalc::module& mod, std::istream& is)
{
  // Read the input line.
  std::string line;
  std::getline(is, line);
  if (!is)
    return nullptr;

  // TODO: Handle line breaking: if the last character is \, read another
  // line.

  // Handle comments.
  std::size_t loc = line.find('#');
  if (loc != line.npos)
    line = line.substr(0, loc);

  // Ignore empty lines.
  if (line.empty())
    return nullptr;

  // Lex all of the tokens into a buffer. Note that lexical errors result
  // in exceptions that can be trapped and diagnosed.
  auto ss = beaker::make_stream(line);
  icalc::lexer lex(ss);
  beaker::token_seq toks;
  while (!lex.eof()) {
    try {
      icalc::token tok = lex();
      toks.push_back(tok);
    }
    catch (icalc::lexical_error& err) {
      // FIXME: Trap errors in a diagnostics engine that captures these
      // diagnostics for subsequent rendering.
      std::cerr << err.get_location() << ": error [lex]: " << err.what() << '\n';
    }
    catch (...) {
      throw;
    }
  }

  // If we didn't parse any tokens (non-empty line, possibly with errors),
  // don't proceed to parsing.
  if (toks.empty())
    return nullptr;

  // Try to parse the tokens stream.
  auto ts = beaker::make_stream(toks);
  icalc::builder build(mod);
  icalc::parser parse(ts, build);
  beaker::expr* e = &parse.expression();

  // If there were tokens after the end of the expressions...
  //
  // FIXME: Should this be a parse error? Should it throw?
  if (!ts.eof()) {
    icalc::token tok = ts.peek();
    std::cerr << tok.get_location() << ": error [parse]: " << "expected end-of-line\n";
    return nullptr;
  }

  return e;

  // FIXME: The generator stream doesn't actually work.
#if 0
    icalc::builder build(mod);
    auto ss = beaker::make_stream(line);
    icalc::lexer lex(ss);
    auto ts = beaker::make_generator(lex);
    icalc::parser parse(ts, build);
    beaker::expr& e = parse.expression();
#endif
}


/// Returns the value of an expression. Throws if the evaluation of `e`
/// results in undefined behavior.
beaker::value
eval(icalc::language& lang, beaker::expr& e)
{
  beaker::evaluator eval(lang);
  return evaluate(eval, e); 
}

void
print(icalc::language& lang, beaker::expr& e, const beaker::value& val)
{
 #if 0
  if (beaker::sys_bool::is_boolean_expression(e))
    std::cout << (val.get_int() ? "true\n" : "false\n");
  else
    std::cout << val.get_int() << '\n';
#endif
  
  beaker::pretty_printer pp(lang, std::cout);
  print(pp, e);
  std::cout << " ~~> " << val << '\n';
}


int
main()
{
  icalc::language lang;
  icalc::module mod(lang);

  std::string line;
  while (std::cin) {
    // Read
    beaker::expr* e;
    try {
      e = read(mod, std::cin);
      if (!std::cin) break;
      if (!e) continue;
    } catch (icalc::syntax_error& err) {
      std::cerr << err.get_location() << ": error [parse]: " << err.what() << '\n';
      continue;
    } catch (...) {
      throw;
    }

    // Evaluate
    beaker::value val;
    try {
      val = eval(lang, *e);
    } catch (beaker::evaluation_error& err) {
      std::cerr << "error: " << err.what() << '\n';
      continue;
    }

    // Print
    print(lang, *e, val);
  }
}

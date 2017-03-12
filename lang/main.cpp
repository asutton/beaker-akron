// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "lang.hpp"
#include "lexer.hpp"
#include "parser.hpp"

#include <beaker/base/printing/print.hpp>

#include <fstream>
#include <iterator>
#include <iostream>

int
main(int argc, char* argv[])
{
  // Defines the source language.
  bpl::language lang;

  // FIXME: Actually parse arguments in a meaningful way.
  if (argc < 2) {
    std::cerr << "error: no input files given\n";
    return 1;
  }
  std::string infile = argv[1];

  // Load the file into a stream.
  std::ifstream ifs(infile);
  std::string input {std::istreambuf_iterator<char>(ifs),
                     std::istreambuf_iterator<char>()};

  // Defines the module being parsed.
  bpl::module mod(lang);

  // Lex the input.
  //
  // FIXME: This is currently appending an explicit EOF token. That may not
  // be a good thing.
  auto ss = beaker::make_stream(input);
  bpl::lexer lex(lang.get_symbols(), ss);
  bpl::token_seq toks;
  while (!lex.eof()) {
    try {
      bpl::token tok = lex();
      toks.push_back(tok);
    }
    catch (bpl::lexical_error& err) {
      // FIXME: Trap errors in a diagnostics engine that captures these
      // diagnostics for subsequent rendering.
      std::cerr << err.get_location() << ": error [lex]: " << err.what() << '\n';
    }
    catch (...) {
      throw;
    }
  }

  try {
    auto ts = beaker::make_stream(toks);
    bpl::parser parse(ts, mod);
    bpl::decl& d = parse.module();
    print(lang, d);
  }
  catch (bpl::syntax_error& err) {
    std::cerr << err.get_location() << ": error [parse]: " << err.what() << '\n';
  }

}

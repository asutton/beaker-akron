// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "lang.hpp"
#include "lexer.hpp"
#include "parser.hpp"

#include <beaker/base/printing/print.hpp>
#include <beaker/base/evaluation/evaluate.hpp>

#include <iostream>


int
main()
{
  icalc::language lang;
  icalc::module mod(lang);

  std::string line;
  while (std::getline(std::cin, line)) {
    if (!std::cin)
      break;
    if (line.empty())
      continue;
    if (line[0] == '#')
      continue;
    
    // Lex all of the tokens into a buffer.
    auto ss = beaker::make_stream(line);
    icalc::lexer lex(ss);
    beaker::token_seq toks;
    while (beaker::token tok = lex())
      toks.push_back(tok);

    auto ts = beaker::make_stream(toks);
    icalc::builder build(mod);
    icalc::parser parse(ts, build);
    auto& e = parse.expression();

    beaker::evaluator eval(lang);
    beaker::value val = evaluate(eval, e);
    if (beaker::sys_bool::is_boolean_expression(e))
      std::cout << (val.get_int() ? "true\n" : "false\n");
    else
      std::cout << val.get_int() << '\n';

#if 0
    beaker::pretty_printer pp(lang, std::cout);
    print(pp, e);
    std::cout << " ~> ";
    std::cout << evaluate(eval, e) << '\n';
#endif

  }
}

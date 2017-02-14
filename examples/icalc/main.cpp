// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "lang.hpp"
#include "lexer.hpp"
#include "parser.hpp"

#include <beaker/base/printing/print.hpp>

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
    
    std::vector<icalc::token*> toks;
    icalc::lexer lex(line.c_str(), line.c_str() + line.size());
    while (icalc::token* tok = lex.next()) {
      std::cout << tok->get_kind() << '\n';
      toks.push_back(tok);
    }

    // icalc::builder build(mod);
    // icalc::parser parse(build, toks.data(), toks.data() + toks.size());
    // auto& e = parse.expression();
    // print(lang, e);
  }
}

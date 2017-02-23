// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "lang.hpp"
#include "lexer.hpp"
#include "parser.hpp"

#include <beaker/base/printing/print.hpp>
#include <beaker/base/evaluation/evaluate.hpp>

#include <iostream>

int
main(int argc, char* argv[])
{
  // Defines the source language.
  bpl::language lang;

  // Defines the module being parsed.
  bpl::module mod(lang);
}

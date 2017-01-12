// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef TEST_AST_LANG_HPP
#define TEST_AST_LANG_HPP

#include <beaker/base/lang.hpp>
#include <beaker/base/module.hpp>
#include <beaker/util/symbol_table.hpp>

using namespace beaker;

/// Defines the testing language.
struct system_lang : language 
{
  system_lang(symbol_table&);
};


#endif

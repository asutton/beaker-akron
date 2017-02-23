// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef BPL_TOKEN_HPP
#define BPL_TOKEN_HPP

#include <beaker/base/token.hpp>

#include <string>


namespace bpl {

using beaker::location;
using beaker::token;
using beaker::token_seq;

// Kinds of tokens.
enum token_kind 
{
#define def_token(T, S) T ## _tok,
#define def_keyword(T, S) T ## _kw,
#include "token.def"
};

const char* get_token_name(int);
const char* get_token_name(const token&);

const char* get_token_spelling(int);
std::string get_token_spelling(const token&);

} // namespace bpl

#endif


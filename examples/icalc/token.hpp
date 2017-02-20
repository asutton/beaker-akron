// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#ifndef ICALC_TOKEN_HPP
#define ICALC_TOKEN_HPP

#include <beaker/base/token.hpp>

#include <string>


namespace icalc {

using beaker::location;
using beaker::token;

// Kinds of tokens.
enum token_kind 
{
#define def_token(T, S) T ## _tok,
#include "token.def"
};

const char* get_token_name(int);
const char* get_token_name(const token&);

const char* get_token_spelling(int);
std::string get_token_spelling(const token&);

} // namespace

#endif


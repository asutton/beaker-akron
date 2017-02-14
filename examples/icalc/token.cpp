// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "token.hpp"

namespace icalc {

token_store::~token_store()
{
  for (token* p : *this)
    delete p;
}

} // namespace icalc

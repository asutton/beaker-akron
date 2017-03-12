// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

namespace bpl {

/// FIXME: I don't like this.
decl&
semantics::finish_module()
{
  return mod;
}

decl& 
semantics::on_function_declaration(name& id, type& ty, locations<5> locs)
{
  assert(false && "not implemented");
}

decl& 
semantics::on_function_declaration(name& id, type& ty, stmt& body, locations<4> locs)
{
  assert(false && "not implemented");
}

} // namespace bpl

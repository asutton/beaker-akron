// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

namespace bpl {

/// Returns the name 'n'.
name&
semantics::on_identifier(token tok)
{
  const auto& attr = tok.get_attribute<beaker::symbol_attr>();
  return build_name.get_name(attr.get_symbol());
}

} // namespace bpl

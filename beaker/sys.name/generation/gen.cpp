// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "gen.hpp"
#include "../name.hpp"

#include <beaker/util/symbol.hpp>

#include <sstream>


namespace beaker {
namespace sys_name {

/// The symbol for a basic name is that name's spelling.
static inline std::string
generate_basic_name(generator& gen, const basic_name& n)
{
  return n.get_symbol().get_spelling();
}

/// The symbol for an internal name has the form "_B_id_N" where N is the 
/// unique identifier.
static inline std::string
generate_internal_name(generator& gen, const internal_name& n)
{
  std::stringstream ss;
  ss << "_B_id_" << cast<internal_name>(n).get_id();
  return ss.str(); 
}

/// \todo There are really two operations on names: generate a source
/// representation of the name, and generate a mangled name. I might want
/// to separate these concepts instead of using a single generate function.
std::string
gen_algo::operator()(generator& gen, const name& n) const
{
  if (n.get_kind() == basic_name_kind)
    return generate_basic_name(gen, cast<basic_name>(n));
  else if (n.get_kind() == internal_name_kind)
    return generate_internal_name(gen, cast<internal_name>(n));
  else
    assert(false && "not a system name");
}

} // namespace sys_name
} // namespace beaker

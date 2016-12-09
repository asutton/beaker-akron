// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_LOGIC_VISITATION_HPP
#define BEAKER_LOGIC_VISITATION_HPP

#include <beaker/terms/logic/type.hpp>
#include <beaker/terms/logic/expression.hpp>

namespace beaker {
namespace logic {

// Invoke the visitor and return a value.
template<typename T, typename V>
inline void
visit_type(T&& t, V& vis)
{
  assert(is_boolean_type(t));
  vis.invoke(cast<bool_type>(t));
}


} // namespace logic
} // namespace beaker


#endif

// Copyright (c) 2015-2017 Andrew Sutton
// All rights reserved

#include "class_graph.hpp"

#include <iostream>
#include <queue>


namespace beaker {

/// Inserts a new edge connecting the base class b to derived class d.
void 
inheritance_hierarchy::add_derivation(std::type_index b, std::type_index d) 
{
  int nb = get(b);
  int nd = get(d);
  node(nb).add_derived(nd);
  node(nd).add_base(nb);
}

/// Returns the index of the node for the type information. Inserts a new
/// node if one does not exist.
///
/// \bug We should be assigning ids after all nodes have been added, but for
/// some reason, that doesn't seem to be working --- it skips a number. It
/// almost seems like there is a node being added to an inheritor list that
/// is not in the graph.
int 
inheritance_hierarchy::get(std::type_index ti)
{
  auto result = lookup.emplace(ti, -1);
  auto iter = result.first;
  if (result.second) {
    iter->second = types.size();
    types.emplace_back(ti);
    types.back().id = iter->second;
  }
  return iter->second;
}

void 
print(const inheritance_hierarchy& h) 
{
  std::cout << "digraph G {\n";
  for (const class_node& u : h) {
    for (int n : u.inheritors()) {
      const class_node& v = h.node(n);
      std::cout << u.ti.name() << " -> " << v.ti.name() << ";\n";
      // std::cout << u.id << " -> " << v.id << ";\n";
    }
  }
  std::cout << "}\n";
}

} // namespace beaker

// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#include <beaker/common/visitation.hpp>
#include <beaker/common/symbol_table.hpp>
#include <beaker/common/hash.hpp>
#include <beaker/terms/logic.hpp>
#include <beaker/terms/numeric.hpp>

#include <iostream>


using namespace beaker;

int tree_weight(const type&);

int
tree_weight(const type_seq& ts)
{
  int ret = 0;
  for (const type& t : ts)
    ret += tree_weight(t);
  return ret;
}

int 
tree_weight(const type& t) 
{
  // TODO: Add more overloads in order to handle more terms.
  struct fn : generic_fn<int>
  {
    int operator()(const void_type& t) {
      return 1;
    }
    int operator()(const ref_type& t) {
      return 1 + tree_weight(t.get_referent_type());
    }
    int operator()(const fn_type& t) {
      return 1 + 
             tree_weight(t.get_parameter_types()) + 
             tree_weight(t.get_return_type());
    }

    int operator()(const logic::bool_type& t) { 
      return 1; 
    }
  };
  return apply(t, fn{});
}


int 
main(int argc, char* argv[])
{
  sequential_allocator<> alloc;
  symbol_table syms;
  beaker::builder bb(alloc, syms);
  logic::builder lb(alloc);

  type& b = lb.get_bool_type();
  type& r1 = bb.get_ref_type(b);
  type& r2 = bb.get_ref_type(r1); // Not actually valid, but whatever...

  std::cout << tree_weight(r2) << '\n';
}

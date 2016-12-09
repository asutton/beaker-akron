# Copyright (c) 2015-2016 Andrew Sutton
# All rights reserved

add_beaker_module(
  type.cpp
  expr.cpp
  decl.cpp
  comparison/eq.cpp
  comparison/hash.cpp
  construction/builder.cpp
  printing/print.cpp
  generation/gen.cpp
  generation/gen_var.cpp
  generation/gen_fn.cpp
  generation/gen_init.cpp
  generation/gen_call.cpp
  serialization/read_json.cpp
)

# add_beaker_test(test-common-visit test/visit.cpp)
# add_beaker_test(test-common-function test/function.cpp)
# add_beaker_test(test-common-json test/json.cpp)


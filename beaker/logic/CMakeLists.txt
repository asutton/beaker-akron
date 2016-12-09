# Copyright (c) 2015-2016 Andrew Sutton
# All rights reserved

add_beaker_module(
  type.cpp
  expr.cpp
  comparison/eq.cpp
  comparison/hash.cpp
  construction/builder.cpp
  printing/print.cpp
  generation/gen.cpp
  serialization/read_json.cpp

  # visitation.cpp
  # evaluation.cpp
  # generation.cpp
  # serialization.cpp
)

# add_beaker_test(test-logic-eval test/eval.cpp)
# add_beaker_test(test-logic-gen test/gen.cpp)

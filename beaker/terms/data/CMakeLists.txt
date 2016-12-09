# Copyright (c) 2015-2016 Andrew Sutton
# All rights reserved

add_beaker_module(
  type.cpp
  expression.cpp
  construction.cpp
  generation.cpp
)

add_beaker_test(test-data-gen-tuple-obj test/gen_tuple_obj.cpp)
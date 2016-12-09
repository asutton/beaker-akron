# Copyright (c) 2015-2016 Andrew Sutton
# All rights reserved


add_beaker_module(
  expression.cpp
  construction.cpp
  generation.cpp
)

add_beaker_test(test-functional-gen-lambda test/gen_lambda.cpp)
add_beaker_test(test-functional-gen-let test/gen_let.cpp)
add_beaker_test(test-functional-gen-seq test/gen_seq.cpp)

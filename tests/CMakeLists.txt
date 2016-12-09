# Copyright (c) 2015-2016 Andrew Sutton
# All rights reserved


# Add a test program.
macro(add_beaker_test target)
  add_executable(${target} ${ARGN})
  target_link_libraries(${target} beaker)
endmacro()


# Unit tests
add_beaker_test(test-ast-logic-1 ast/logic-1.cpp)
add_beaker_test(test-ast-numeric-1 ast/numeric-1.cpp)
add_beaker_test(test-ast-var-1 ast/var-1.cpp)
add_beaker_test(test-ast-fn-1 ast/fn-1.cpp)
add_beaker_test(test-ast-fn-2 ast/fn-2.cpp)


# Tools
add_beaker_test(test-json json.cpp)


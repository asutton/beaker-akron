# Copyright (c) 2015-2017 Andrew Sutton
# All rights reserved

add_executable(beaker-compiler
  lang.cpp
  token.cpp
  lexer.cpp
  parser.cpp
  semantics.cpp
  main.cpp
)
target_link_libraries(beaker-compiler beaker)

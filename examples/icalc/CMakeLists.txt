# Copyright (c) 2015-2017 Andrew Sutton
# All rights reserved

add_executable(icalc 
  main.cpp 
  lang.cpp 
  token.cpp
  lexer.cpp 
  parser.cpp)
target_link_libraries(icalc beaker)

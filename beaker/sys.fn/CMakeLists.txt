# Copyright (c) 2015-2017 Andrew Sutton
# All rights reserved

add_beaker_module(
  lang.cpp
  type.cpp
  expr.cpp
  decl.cpp
  stmt.cpp
  build.cpp
  ast.cpp
  fwd.cpp
  comparison/eq.cpp
  comparison/hash.cpp
  printing/print.cpp
  generation/gen.cpp
  serialization/write.cpp
)

# Copyright (c) 2015-2017 Andrew Sutton
# All rights reserved

add_beaker_module(
  name.cpp
  type.cpp
  expr.cpp
  decl.cpp
  stmt.cpp
  ast.cpp
  comparison/equal.cpp
  comparison/hash.cpp
  printing/print.cpp
  evaluation/evaluate.cpp
  # generation/gen.cpp
  # serialization/write.cpp
)

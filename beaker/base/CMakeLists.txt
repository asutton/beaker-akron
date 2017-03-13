# Copyright (c) 2015-2016 Andrew Sutton
# All rights reserved

add_beaker_module(
  error.cpp
  value.cpp
  node.cpp
  lang.cpp
  seq.cpp
  name.cpp
  type.cpp
  expr.cpp
  decl.cpp
  stmt.cpp
  module.cpp
  token.cpp
  scope.cpp
  symbol.cpp
  symbol_table.cpp
  comparison/equal.cpp
  comparison/hash.cpp
  printing/print.cpp
  evaluation/evaluate.cpp
  # generation/generation.cpp
  # generation/type.cpp
  # generation/value.cpp
  # generation/note.cpp
  # serialization/writer.cpp
)

# Copyright (c) 2015-2016 Andrew Sutton
# All rights reserved

add_beaker_module(
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
  comparison/eq.cpp
  comparison/hash.cpp
  construction/builder.cpp
  printing/print.cpp
  generation/generation.cpp
  generation/type.cpp
  generation/value.cpp
  generation/note.cpp

  # serialization/json.cpp
  # serialization/json_reader.cpp
  # serialization/json_writer.cpp
)

# Copyright (c) 2015-2016 Andrew Sutton
# All rights reserved

add_beaker_module(
  type.cpp
  expr.cpp
  comparison/eq.cpp
  comparison/hash.cpp
  construction/builder.cpp
  printing/print.cpp
  serialization/read_json.cpp
  generation/gen.cpp
  generation/gen_math.cpp
  generation/gen_rel.cpp
)

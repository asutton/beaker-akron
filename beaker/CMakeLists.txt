# Copyright (c) 2015-2016 Andrew Sutton
# All rights reserved

set(beaker_top ${CMAKE_CURRENT_SOURCE_DIR})
set(beaker_src)

# Add a subset of a language to the build.
macro(add_beaker_module)
  get_filename_component(input ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)
  file(RELATIVE_PATH path ${CMAKE_CURRENT_SOURCE_DIR} ${input})
  set(files)
  foreach(i ${ARGV})
    list(APPEND files ${path}/${i})
  endforeach()
  set(beaker_src ${beaker_src} ${files})
endmacro()


# Directly include the module definition sources.
include(util/CMakeLists.txt)
include(base/CMakeLists.txt)
include(core/CMakeLists.txt)
include(logic/CMakeLists.txt)
include(numeric/CMakeLists.txt)
include(procedural/CMakeLists.txt)
include(data/CMakeLists.txt)
include(variadic/CMakeLists.txt)
include(sample/CMakeLists.txt)


# Define the core library.
add_library(beaker STATIC ${beaker_src})
target_link_libraries(beaker ${LLVM_LIBRARIES})

# Installation
install(TARGETS beaker DESTINATION lib)

# Add install targets.
install(
  DIRECTORY ${PROJECT_SOURCE_DIR}/beaker
  DESTINATION ${CMAKE_INSTALL_PREFIX}/include
  FILES_MATCHING
  PATTERN "*.hpp"
  PATTERN "CMakeFiles" EXCLUDE
)

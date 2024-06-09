# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/search_engine_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/search_engine_autogen.dir/ParseCache.txt"
  "search_engine_autogen"
  )
endif()

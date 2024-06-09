# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/oop-a10-maraielciu1_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/oop-a10-maraielciu1_autogen.dir/ParseCache.txt"
  "oop-a10-maraielciu1_autogen"
  )
endif()

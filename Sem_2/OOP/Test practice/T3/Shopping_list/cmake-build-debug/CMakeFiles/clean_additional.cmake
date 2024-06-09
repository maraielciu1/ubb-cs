# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/shopping_list_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/shopping_list_autogen.dir/ParseCache.txt"
  "shopping_list_autogen"
  )
endif()

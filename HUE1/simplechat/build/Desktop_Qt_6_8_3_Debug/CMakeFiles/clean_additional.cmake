# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/simplechatserver_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/simplechatserver_autogen.dir/ParseCache.txt"
  "simplechatserver_autogen"
  )
endif()

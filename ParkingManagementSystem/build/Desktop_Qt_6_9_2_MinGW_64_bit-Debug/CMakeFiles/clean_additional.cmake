# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\ParkingManagementSystem_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\ParkingManagementSystem_autogen.dir\\ParseCache.txt"
  "ParkingManagementSystem_autogen"
  )
endif()

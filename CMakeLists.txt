# CMakeList.txt : CMake project for AOC2024, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("AOC2024" CXX)

# Add source to this project's executable.
add_executable (AOC2024 "AOC2024.cpp" "AOC2024.h" "utils/timer.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET AOC2024 PROPERTY CXX_STANDARD 20)
endif()

# Utils subdirectory
add_subdirectory(utils)
add_subdirectory(test)

# Base subdirectory
add_subdirectory(base)

# Day subdirectories
add_subdirectory(day1)
add_subdirectory(day2)
add_subdirectory(day3)
add_subdirectory(day4)



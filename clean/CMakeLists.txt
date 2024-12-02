cmake_minimum_required(VERSION 3.15)

project(fmt_ctre_example)

# Use C++17 standard (or higher, depending on your needs)
set(CMAKE_CXX_STANDARD 17)

# Find fmt library (ensure it is installed or add path if necessary)
find_package(fmt REQUIRED)

# Fetch ctre library
include(FetchContent)
FetchContent_Declare(
    ctre
    GIT_REPOSITORY https://github.com/hanickadot/compile-time-regular-expressions.git
    GIT_TAG main  # You can specify a particular commit or branch if needed
)
FetchContent_MakeAvailable(ctre)

# Add the executable
add_executable(fmt_ctre_example main.cpp)

# Link fmt library
target_link_libraries(fmt_ctre_example PRIVATE fmt::fmt ctre::ctre)

cmake_minimum_required(VERSION 3.8)
project("AOC2024" LANGUAGES CXX)

# Include and fetch ctre library
include(FetchContent)
FetchContent_Declare(
    ctre
    GIT_REPOSITORY https://github.com/hanickadot/compile-time-regular-expressions.git
    GIT_TAG origin/main
)
FetchContent_MakeAvailable(ctre)

# Find necessary packages
find_package(Boost REQUIRED COMPONENTS iostreams multiprecision)
find_package(GTest CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

# Create an INTERFACE library for header-only code
add_library(utils INTERFACE)

# Link the required libraries
target_link_libraries(utils 
    INTERFACE
        fmt::fmt
        ctre::ctre 
        Boost::iostreams
        Boost::multiprecision
        GTest::gtest 
        GTest::gtest_main
)

# Set the include directories for the header-only library
target_include_directories(utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

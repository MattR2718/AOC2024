cmake_minimum_required (VERSION 3.8)

add_executable(base base.cpp)
target_link_libraries(base PUBLIC utils)
target_include_directories(base PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_command(
    TARGET base PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/base/input.txt
    ${CMAKE_CURRENT_BINARY_DIR}/input.txt
)

add_custom_command(
    TARGET base PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/base/test.txt
    ${CMAKE_CURRENT_BINARY_DIR}/test.txt
)
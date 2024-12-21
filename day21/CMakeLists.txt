cmake_minimum_required (VERSION 3.8)

add_executable(day21 day21.cpp)
target_link_libraries(day21 PUBLIC utils)

add_custom_command(
    TARGET day21 PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/input.txt
    ${CMAKE_CURRENT_BINARY_DIR}/input.txt
)

add_custom_command(
    TARGET day21 PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/test.txt
    ${CMAKE_CURRENT_BINARY_DIR}/test.txt
)

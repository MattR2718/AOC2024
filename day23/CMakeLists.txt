cmake_minimum_required (VERSION 3.8)

find_package(Boost REQUIRED COMPONENTS graph)

add_executable(day23 day23.cpp)
target_link_libraries(day23 PUBLIC utils Boost::graph)


add_custom_command(
    TARGET day23 PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/input.txt
    ${CMAKE_CURRENT_BINARY_DIR}/input.txt
)

add_custom_command(
    TARGET day23 PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/test.txt
    ${CMAKE_CURRENT_BINARY_DIR}/test.txt
)

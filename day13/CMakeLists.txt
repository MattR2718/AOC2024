cmake_minimum_required (VERSION 3.8)


include_directories("C:\\Users\\matth\\vcpkg\\installed\\x64-windows\\include")
link_directories("C:\\Users\\matth\\vcpkg\\installed\\x64-windows\\lib")


add_executable(day13 day13.cpp)
target_link_libraries(day13 PUBLIC utils Cbc Cgl Clp CoinUtils Osi OsiCbc OsiClp glpk)


add_custom_command(
    TARGET day13 PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/input.txt
    ${CMAKE_CURRENT_BINARY_DIR}/input.txt
)

add_custom_command(
    TARGET day13 PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/test.txt
    ${CMAKE_CURRENT_BINARY_DIR}/test.txt
)

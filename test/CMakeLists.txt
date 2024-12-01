cmake_minimum_required(VERSION 3.8)


add_executable(test_utils test_utils.cpp)
target_link_libraries(test_utils PUBLIC utils)
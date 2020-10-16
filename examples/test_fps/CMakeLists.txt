cmake_minimum_required(VERSION 3.14)

project(TestFPS VERSION 20200820.1)

add_executable(TestFPS src/main.cpp)

include_directories(../..)
target_link_libraries(TestFPS libswe)

set_target_properties(TestFPS PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..)

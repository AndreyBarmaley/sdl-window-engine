cmake_minimum_required(VERSION 3.14)

project(TestWinFPS VERSION 20210301.1)

add_executable(TestWinFPS src/main.cpp)

include_directories(../..)
target_link_libraries(TestWinFPS libswe)

set_target_properties(TestWinFPS PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..)

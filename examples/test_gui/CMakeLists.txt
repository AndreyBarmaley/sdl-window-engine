cmake_minimum_required(VERSION 3.14)

project(TestGUI VERSION 20200820.1)

add_executable(TestGUI src/main.cpp src/listitems.cpp)

include_directories(../..)
target_link_libraries(TestGUI libswe)

set_target_properties(TestGUI PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..)

cmake_minimum_required(VERSION 3.14)

project(TestGUI VERSION 20200820.1)
add_executable(TestTerm src/mainscreen.cpp src/terminal.cpp)

include_directories(../..)
target_link_libraries(TestTerm swe)

set_target_properties(TestTerm PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..)

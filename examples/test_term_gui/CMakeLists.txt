cmake_minimum_required(VERSION 3.14)

project(TestTermGUI VERSION 20200820.1)
add_executable(TestTermGUI src/mainscreen.cpp src/terminal.cpp)

include_directories(../..)
target_link_libraries(TestTermGUI swe)

set_target_properties(TestTermGUI PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..)

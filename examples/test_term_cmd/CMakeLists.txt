cmake_minimum_required(VERSION 3.14)

project(TestTermCmd VERSION 20200820.1)
add_executable(TestTermCmd src/mainscreen.cpp src/terminal.cpp src/settings.cpp)

include_directories(../..)
target_link_libraries(TestTermCmd swe)

set_target_properties(TestTermCmd PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..)

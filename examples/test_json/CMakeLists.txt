cmake_minimum_required(VERSION 3.14)

project(TestJSON VERSION 20200820.1)

add_executable(TestJSON src/main.cpp)

include_directories(../..)
target_link_libraries(TestJSON libswe)

set_target_properties(TestJSON PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..)

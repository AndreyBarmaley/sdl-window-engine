cmake_minimum_required(VERSION 3.14)

project(SWE VERSION 20200825.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 ")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -ggdb3 -O0 -Wall -Werror -Wno-sign-compare -fsigned-char")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Wall -Wno-sign-compare -fsigned-char")

include(libswe.cmake)
option(SWE_EXAMPLES "build examples" OFF)

add_library(swe STATIC)
add_subdirectory(swe)
set_target_properties(swe PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

if(SWE_EXAMPLES)
    add_subdirectory(examples)
endif()

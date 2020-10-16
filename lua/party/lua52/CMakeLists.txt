cmake_minimum_required(VERSION 3.14)

project(LUA52 VERSION 5.2.4)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DLUA_COMPAT_ALL")

if(UNIX)
    if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DLUA_USE_LINUX")
        link_libraries(dl)
    else()
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DLUA_USE_POSIX -DLUA_USE_DLOPEN")
        link_libraries(dl)
    endif()
endif()

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -ggdb3 -O0 -Wall")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -Wall")

add_library(liblua52 STATIC)

include_directories(src)
add_subdirectory(src)

set_target_properties(liblua52 PROPERTIES PREFIX "")
set_target_properties(liblua52 PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

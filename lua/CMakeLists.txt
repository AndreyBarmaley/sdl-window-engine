cmake_minimum_required(VERSION 3.14)

project(SWE_LUA_BINDINGS VERSION 20200910.1)

add_library(SWE SHARED)
add_subdirectory(src)

if(SWE_WITH_LUA52)
    add_subdirectory(party/lua52)
    target_link_libraries(SWE liblua52)
endif()

target_link_libraries(SWE libswe)
set_target_properties(SWE PROPERTIES PREFIX "")
set_target_properties(SWE PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

project(LuaSWE VERSION 20200920.1)

add_executable(LuaSWE main/SWE_main.cpp 
    src/SWE_audio.cpp
    src/SWE_binarybuf.cpp
    src/SWE_bindings.cpp
    src/SWE_color.cpp
    src/SWE_fontrender.cpp
    src/SWE_keys.cpp
    src/SWE_randomhit.cpp
    src/SWE_rect.cpp
    src/SWE_signal.cpp
    src/SWE_streambuf.cpp
    src/SWE_streamfile.cpp
    src/SWE_streamnet.cpp
    src/SWE_terminal.cpp
    src/SWE_texture.cpp
    src/SWE_tools.cpp
    src/SWE_translation.cpp
    src/SWE_unicodestring.cpp
    src/SWE_videocam.cpp
    src/SWE_videocam_ffmpeg.cpp
    src/SWE_window.cpp)

if(SWE_WITH_LUA52)
    target_link_libraries(LuaSWE liblua52)
endif()

target_link_libraries(LuaSWE libswe)
set_target_properties(LuaSWE PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

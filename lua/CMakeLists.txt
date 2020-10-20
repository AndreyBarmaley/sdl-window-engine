cmake_minimum_required(VERSION 3.14)

set(LUA52_SRC
        party/lua52/src/lapi.c
        party/lua52/src/lcode.c
        party/lua52/src/lctype.c
        party/lua52/src/ldebug.c
        party/lua52/src/ldo.c
        party/lua52/src/ldump.c
        party/lua52/src/lfunc.c
        party/lua52/src/lgc.c
        party/lua52/src/llex.c
        party/lua52/src/lmem.c
        party/lua52/src/lobject.c
        party/lua52/src/lopcodes.c
        party/lua52/src/lparser.c
        party/lua52/src/lstate.c
        party/lua52/src/lstring.c
        party/lua52/src/ltable.c
        party/lua52/src/ltm.c
        party/lua52/src/lundump.c
        party/lua52/src/lvm.c
        party/lua52/src/lzio.c
        party/lua52/src/lauxlib.c
        party/lua52/src/lbaselib.c
        party/lua52/src/lbitlib.c
        party/lua52/src/lcorolib.c
        party/lua52/src/ldblib.c
        party/lua52/src/liolib.c
        party/lua52/src/lmathlib.c
        party/lua52/src/loslib.c
        party/lua52/src/lstrlib.c
        party/lua52/src/ltablib.c
        party/lua52/src/loadlib.c
        party/lua52/src/linit.c)

set(LUA54_SRC
        party/lua54/src/lapi.c
        party/lua54/src/lcode.c
        party/lua54/src/lctype.c
        party/lua54/src/ldebug.c
        party/lua54/src/ldo.c
        party/lua54/src/ldump.c
        party/lua54/src/lfunc.c
        party/lua54/src/lgc.c
        party/lua54/src/llex.c
        party/lua54/src/lmem.c
        party/lua54/src/lobject.c
        party/lua54/src/lopcodes.c
        party/lua54/src/lparser.c
        party/lua54/src/lstate.c
        party/lua54/src/lstring.c
        party/lua54/src/ltable.c
        party/lua54/src/ltm.c
        party/lua54/src/lundump.c
        party/lua54/src/lvm.c
        party/lua54/src/lzio.c
        party/lua54/src/lauxlib.c
        party/lua54/src/lbaselib.c
        party/lua54/src/lcorolib.c
        party/lua54/src/ldblib.c
        party/lua54/src/liolib.c
        party/lua54/src/lmathlib.c
        party/lua54/src/loslib.c
        party/lua54/src/lstrlib.c
        party/lua54/src/ltablib.c
        party/lua54/src/loadlib.c
        party/lua54/src/lutf8lib.c
        party/lua54/src/linit.c)

#poject1
project(SWE_LUA_BINDINGS VERSION 20200910.1)

add_library(SWE SHARED)
add_subdirectory(src)

if(SWE_WITH_LUA52 STREQUAL ON)
    target_sources(SWE PUBLIC ${LUA52_SRC})
    # lua flags
    if(UNIX)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DLUA_USE_POSIX -DLUA_USE_DLOPEN")
        target_link_libraries(SWE dl)
    endif()
else()
    target_link_libraries(SWE lua)
endif()

target_link_libraries(SWE libswe)
set_target_properties(SWE PROPERTIES PREFIX "")
set_target_properties(SWE PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# project2
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

if(SWE_WITH_LUA52 STREQUAL ON)
    target_sources(LuaSWE PUBLIC ${LUA52_SRC})
    # lua flags
    if(UNIX)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DLUA_USE_POSIX -DLUA_USE_DLOPEN")
        target_link_libraries(LuaSWE dl)
    endif()
else()
    target_link_libraries(LuaSWE lua)
endif()

# auto: support ffmpeg
pkg_check_modules(FFMPEG QUIET libavdevice libavformat libavcodec libswscale libavutil)
if(${FFMPEG_FOUND})
    target_compile_options(LuaSWE PUBLIC -DWITH_VIDEOCAM_FFMPEG ${FFMPEG_CFLAGS})
    target_link_libraries(LuaSWE avdevice avformat avcodec swscale avutil)
else()
    message("skipping ffmpeg support, not found: libavdevice, libavformat, libavcodec, libswscale, or libavutil")
endif()

# auto: support fireware
pkg_check_modules(FIREWIRE QUIET libraw1394 libavc1394 libiec61883 libdv libmpeg2 libmpeg2convert)
if(${FIREWIRE_FOUND})
    target_compile_options(LuaSWE PUBLIC -DWITH_VIDEOCAM_FIREWIRE ${FIREWIRE_CFLAGS})
    target_link_libraries(LuaSWE raw1394 avc1394 iec61883 dv mpeg2 mpeg2convert)
else()
    message("skipping fireware support, not found: libraw1394, libavc1394, libiec61883, libdv, libmpeg2, or libmpeg2convert")
endif()

target_link_libraries(LuaSWE libswe)
set_target_properties(LuaSWE PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

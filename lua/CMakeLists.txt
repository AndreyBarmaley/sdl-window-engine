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

# project1 SWE bindings static
project(SWE_bindings VERSION 20210301.1)

add_library(SWE_bindings STATIC)
add_subdirectory(src)

if(SWE_WITH_LUA_PARTY)
    target_sources(SWE_bindings PUBLIC ${LUA52_SRC})
    # lua flags
    if(UNIX)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DLUA_USE_POSIX -DLUA_USE_DLOPEN")
        if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
            target_link_libraries(SWE_bindings dl)
        endif()
    endif()
else()
    target_link_libraries(SWE_bindings lua)
endif()

target_link_libraries(SWE_bindings libswe)

# project2 SWE bindings shared for lua plugins
if(SWE_FOR_LUA_PLUGIN)
    project(SWE_plugin VERSION 20210301.1)

    add_library(SWE_plugin SHARED)
    target_link_libraries(SWE_plugin SWE_bindings)

    set_target_properties(SWE_plugin PROPERTIES PREFIX "")
    set_target_properties(SWE_plugin PROPERTIES OUTPUT_NAME SWE)
    set_target_properties(SWE_plugin PROPERTIES POSITION_INDEPENDENT_CODE 1)
    set_target_properties(SWE_plugin PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# project3 LuaSWE starter
project(LuaSWE VERSION 20210301.1)
add_executable(LuaSWE main/SWE_main.cpp)

# auto: support ffmpeg
pkg_check_modules(FFMPEG QUIET libavdevice libavformat libavcodec libswscale libavutil)
if(${FFMPEG_FOUND})
    if(NOT SWE_SDL12)
        target_compile_options(LuaSWE PUBLIC -DWITH_VIDEOCAM_FFMPEG ${FFMPEG_CFLAGS})
        target_link_libraries(LuaSWE avdevice avformat avcodec swscale avutil)
    else()
        message("ffmpeg need SDL2 skipped..")
    endif()
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

target_link_libraries(LuaSWE SWE_bindings)
target_link_libraries(LuaSWE libswe)
set_target_properties(LuaSWE PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

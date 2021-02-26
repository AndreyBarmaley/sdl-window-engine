include(FindPkgConfig)

set(CMAKE_FIND_FRAMEWORK LAST)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_STATIC_RUNTIME OFF)

FIND_PACKAGE(Boost 1.62 OPTIONAL_COMPONENTS stacktrace_addr2line)

if(Boost_STACKTRACE_ADDR2LINE_FOUND)
    option(SWE_STACKTRACE "boost stacktrace on exception" ON)
endif()

if(SWE_STACKTRACE)
    add_compile_options(${Boost_STACKTRACE_ADDR2LINE_CFLAGS})
    link_libraries(${Boost_STACKTRACE_ADDR2LINE_LIBRARIES})
endif()

if(SWE_DEBUG)
    set(SWE_DEBUG_SCENE ON CACHE BOOL "enable scene debug hotkeys" FORCE)
    set(SWE_DEBUG_MESSAGES ON CACHE BOOL "enable debug messages" FORCE)
endif()

if(SWE_EXAMPLES)
    set(SWE_DISABLE_TERMGUI OFF CACHE BOOL "enable build termwin_gui" FORCE)
    set(SWE_WITH_JSON ON CACHE BOOL "enable builtin json" FORCE)
endif()

if(SWE_SDL12)
    pkg_search_module(SDL REQUIRED SDL>=1.2)
    pkg_search_module(SDLGFX REQUIRED SDL_gfx>=2.0)

    add_compile_options(-DSWE_SDL12)

    if(SWE_DISABLE_AUDIO)
        add_compile_options(-DSWE_DISABLE_AUDIO)
    else()
        pkg_search_module(SDLMIXER REQUIRED SDL_mixer>=1.2)
    endif()

    if(SWE_DISABLE_IMAGE)
        add_compile_options(-DSWE_DISABLE_IMAGE)
    else()
        pkg_search_module(SDLIMAGE REQUIRED SDL_image>=1.2)
    endif()

    if(SWE_DISABLE_TTF)
        add_compile_options(-DSWE_DISABLE_TTF)
    else()
        pkg_search_module(SDLTTF REQUIRED SDL_ttf>=2.0)
    endif()

    if(SWE_DISABLE_NETWORK)
        add_compile_options(-DSWE_DISABLE_NETWORK)
    else()
        pkg_search_module(SDLNET REQUIRED SDL_net>=1.2)
    endif()
    # link X11, see SWE::Display::isMaximizedWindow
    link_libraries(X11)

    add_compile_options(${SDL_CFLAGS})
    add_link_options(${SDL_LDFLAGS})
    link_libraries(${SDL_LIBRARIES})

    add_compile_options(${SDLGFX_CFLAGS})
    add_link_options(${SDLGFX_LDFLAGS})
    link_libraries(${SDLGFX_LIBRARIES})
else()
    pkg_search_module(SDL2 REQUIRED sdl2>=2.0.1)

    if(SWE_DISABLE_AUDIO)
        add_compile_options(-DSWE_DISABLE_AUDIO)
    else()
        pkg_search_module(SDLMIXER REQUIRED SDL2_mixer>=2.0.1)
    endif()

    if(SWE_DISABLE_IMAGE)
        add_compile_options(-DSWE_DISABLE_IMAGE)
    else()
        pkg_search_module(SDLIMAGE REQUIRED SDL2_image>=2.0.1)
    endif()

    if(SWE_DISABLE_TTF)
        add_compile_options(-DSWE_DISABLE_TTF)
    else()
        pkg_search_module(SDLTTF REQUIRED SDL2_ttf>=2.0.1)
    endif()

    if(SWE_DISABLE_NETWORK)
        add_compile_options(-DSWE_DISABLE_NETWORK)
    else()
        pkg_search_module(SDLNET REQUIRED SDL2_net>=2.0.1)
    endif()

    add_compile_options(${SDL2_CFLAGS})
    add_link_options(${SDL2_LDFLAGS})
    link_libraries(${SDL2_LIBRARIES})
endif()


if(NOT SWE_DISABLE_AUDIO)
    add_compile_options(${SDLMIXER_CFLAGS})
    add_link_options(${SDLMIXER_LDFLAGS})
    link_libraries(${SDLMIXER_LIBRARIES})
endif()

if(NOT SWE_DISABLE_IMAGE)
    add_compile_options(${SDLIMAGE_CFLAGS})
    add_link_options(${SDLIMAGE_LDFLAGS})
    link_libraries(${SDLIMAGE_LIBRARIES})
endif()

if(NOT SWE_DISABLE_TTF)
    add_compile_options(${SDLTTF_CFLAGS})
    add_link_options(${SDLTTF_LDFLAGS})
    link_libraries(${SDLTTF_LIBRARIES})
endif()

if(NOT SWE_DISABLE_NETWORK)
    add_compile_options(${SDLNET_CFLAGS})
    add_link_options(${SDLNET_LDFLAGS})
    link_libraries(${SDLNET_LIBRARIES})
endif()

if(SWE_DISABLE_TERMGUI)
    add_compile_options(-DSWE_DISABLE_TERMGUI)
endif()

if(SWE_DEBUG_MESSAGES)
    add_compile_options(-DSWE_DEBUG_MESSAGES)
endif()

if(SWE_DEBUG_SCENE)
    add_compile_options(-DSWE_DEBUG_SCENE)
endif()

if(SWE_WITH_JSON)
    add_compile_options(-DSWE_WITH_JSON)
endif()

if(SWE_SDL12)
    pkg_search_module(PNG REQUIRED libpng)

    add_compile_options(${PNG_CFLAGS})
    add_link_options(${PNG_LDFLAGS})
    link_libraries(${PNG_LIBRARIES})
endif()

pkg_search_module(LZ REQUIRED zlib)
add_compile_options(${LZ_CFLAGS})
add_link_options(${LZ_LDFLAGS})
link_libraries(${LZ_LIBRARIES})

if(SWE_DLOPEN)
    add_compile_options(-DSWE_DLOPEN)
    if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	link_libraries(dl)
    endif()
endif()

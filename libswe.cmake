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

if(SWE_SDL12 STREQUAL ON)
    pkg_search_module(SDL REQUIRED SDL>=1.2)
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
        pkg_search_module(SDLTTF REQUIRED SDL_ttf>=1.2)
    endif()

    if(SWE_DISABLE_NETWORK)
        add_compile_options(-DSWE_DISABLE_NETWORK)
    else()
        pkg_search_module(SDLNET REQUIRED SDL_net>=1.2)
    endif()
else()
    pkg_search_module(SDL REQUIRED sdl2>=2.0.1)

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
endif()

add_compile_options(${SDL_CFLAGS})
link_libraries(${SDL_LIBRARIES})

if(NOT SWE_DISABLE_AUDIO)
    add_compile_options(${SDLMIXER_CFLAGS})
    link_libraries(${SDLMIXER_LIBRARIES})
endif()

if(NOT SWE_DISABLE_IMAGE)
    add_compile_options(${SDLIMAGE_CFLAGS})
    link_libraries(${SDLIMAGE_LIBRARIES})
endif()

if(NOT SWE_DISABLE_TTF)
    add_compile_options(${SDLTTF_CFLAGS})
    link_libraries(${SDLTTF_LIBRARIES})
endif()

if(NOT SWE_DISABLE_NETWORK)
    add_compile_options(${SDLNET_CFLAGS})
    link_libraries(${SDLNET_LIBRARIES})
endif()

link_libraries(z)

if(SWE_DLOPEN STREQUAL ON)
    link_libraries(dl)
endif()

if(SWE_DISABLE_TERMGUI STREQUAL ON)
    add_compile_options(-DSWE_DISABLE_TERMGUI)
endif()

if(SWE_DEBUG_MESSAGES STREQUAL ON)
    add_compile_options(-DSWE_DEBUG_MESSAGES)
endif()

if(SWE_DEBUG_SCENE STREQUAL ON)
    add_compile_options(-DSWE_DEBUG_SCENE)
endif()

if(SWE_WITH_JSON STREQUAL ON)
    add_compile_options(-DSWE_WITH_JSON)
endif()

if(SWE_SDL12 STREQUAL ON)
    pkg_search_module(PNG REQUIRED libpng)
    link_libraries(png)
endif()

if(SWE_WITH_XML STREQUAL ON)
    add_compile_options(-DSWE_WITH_XML)
endif()
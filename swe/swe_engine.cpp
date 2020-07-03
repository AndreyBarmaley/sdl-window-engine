/***************************************************************************
 *   Copyright (C) 2017 by SWE team <sdl.window.engine@gmail.com>          *
 *                                                                         *
 *   Part of the SWE: SDL Window Engine:                                   *
 *   https://github.com/AndreyBarmaley/sdl-window-engine                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <ctime>

#include "swe_engine.h"
#include "swe_events.h"
#include "swe_music.h"
#include "swe_display.h"

namespace SWE
{

    namespace Engine
    {
	bool		engineDebug = false;

        void		quit(void);
    }

    namespace Display
    {
	void		closeWindow(void);
    }
}

#ifdef BUILD_STACKTRACE
#include "boost/stacktrace.hpp"
#endif

void SWE::Engine::except(const char* func, const char* message)
{
    if(func && message) COUT(String::time() << ": [EXCEPTION]\t" << func << ":  " << message);

#ifdef BUILD_STACKTRACE

    if(!message || strcmp(message, "SDL_QUIT"))
        LogWrapper() << boost::stacktrace::stacktrace();

#endif
#ifdef ANDROID
    exit(0);
#else
    throw exception();
#endif
}

bool SWE::Engine::init(bool debug)
{
    engineDebug = debug;
    int init_flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER;
    std::srand((u32) std::time(0));
    DEBUG("SDL Window Engine: " << "version: " << version());

    if(SDL_Init(init_flags) != 0)
    {
        ERROR("SDL_Init" << ": " << SDL_GetError());
        return false;
    }
    else
    {
#ifdef OLDENGINE
        const SDL_version* sdlver = SDL_Linked_Version();
        DEBUG("usage " << "SDL" << ", " << "version: " << static_cast<int>(sdlver->major) << "." << static_cast<int>(sdlver->minor) << "." << static_cast<int>(sdlver->patch));
#else
        SDL_version sdlver;
        SDL_GetVersion(&sdlver);
        DEBUG("usage " << "SDL" << ", " << "version: " << static_cast<int>(sdlver.major) << "." << static_cast<int>(sdlver.minor) << "." << static_cast<int>(sdlver.patch) <<
              ", revision: " << SDL_GetRevision());
#endif
    }

#ifndef DISABLE_NETWORK

    if(SDLNet_Init() < 0)
    {
        ERROR("SDLNet_Init" << ": " << SDL_GetError());
        return false;
    }
    else
    {
        const SDL_version* sdlver = SDLNet_Linked_Version();

        if(sdlver)
            DEBUG("usage " << "SDL_net" << ", " << "version: " << static_cast<int>(sdlver->major) << "." << static_cast<int>(sdlver->minor) << "." << static_cast<int>(sdlver->patch));
    }

#endif
#ifndef DISABLE_IMAGE

    if(IMG_Init(IMG_INIT_PNG) == 0)
    {
        ERROR("IMG_Init" << ": " << SDL_GetError());
        return false;
    }
    else
    {
        const SDL_version* sdlver = IMG_Linked_Version();

        if(sdlver)
            DEBUG("usage " << "SDL_image" << ", " << "version: " << static_cast<int>(sdlver->major) << "." << static_cast<int>(sdlver->minor) << "." << static_cast<int>(sdlver->patch));
    }

#endif
#ifndef DISABLE_TTF

    if(TTF_Init() != 0)
    {
        ERROR("TTF_Init" << ": " << SDL_GetError());
        return false;
    }
    else
    {
        const SDL_version* sdlver = TTF_Linked_Version();

        if(sdlver)
            DEBUG("usage " << "SDL_ttf" << ", " << "version: " << static_cast<int>(sdlver->major) << "." << static_cast<int>(sdlver->minor) << "." << static_cast<int>(sdlver->patch));
    }

#endif
#ifndef DISABLE_AUDIO
#if (SDL_VERSIONNUM(SDL_MIXER_MAJOR_VERSION, SDL_MIXER_MINOR_VERSION, SDL_MIXER_PATCHLEVEL) > SDL_VERSIONNUM(1,2,8))

    if((Mix_Init(MIX_INIT_OGG) & MIX_INIT_OGG) != MIX_INIT_OGG)
    {
        ERROR("MIX_Init" << ": " << Mix_GetError());
        return false;
    }
    else
#endif
    {
        if(0 > Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024))
            ERROR("MIX_OpenAudio" << ": " << Mix_GetError());

        const SDL_version* sdlver = Mix_Linked_Version();

        if(sdlver)
            DEBUG("usage " << "SDL_mixer" << ", " << "version: " << static_cast<int>(sdlver->major) << "." << static_cast<int>(sdlver->minor) << "." << static_cast<int>(sdlver->patch));
    }

#endif
#ifdef __MINGW32__
    Display::fingerEventEmulation = true;
#endif

#ifdef OLDENGINE
    SDL_EnableKeyRepeat(400, 75);
#endif

    return true;
}

int SWE::Engine::version(void)
{
    return SWE_VERSION;
}

void SWE::Engine::setDebugMode(bool f)
{
    engineDebug = f;
}

bool SWE::Engine::debugMode(void)
{
    return engineDebug;
}

void SWE::Engine::quit(void)
{
    clearAllSignals();
    Display::closeWindow();
#ifndef DISABLE_AUDIO
    Music::reset();
    Mix_CloseAudio();
#if (SDL_VERSIONNUM(SDL_MIXER_MAJOR_VERSION, SDL_MIXER_MINOR_VERSION, SDL_MIXER_PATCHLEVEL) > SDL_VERSIONNUM(1,2,8))
    Mix_Quit();
#endif
#endif
#ifndef DISABLE_IMAGE
    IMG_Quit();
#endif
#ifndef DISABLE_NETWORK
    SDLNet_Quit();
#endif
#ifndef DISABLE_TTF
    TTF_Quit();
#endif
    SDL_Quit();
}


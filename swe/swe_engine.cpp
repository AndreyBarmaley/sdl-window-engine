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

#ifdef BOOST_STACKTRACE_USE_ADDR2LINE
#include "boost/stacktrace.hpp"
#endif

void SWE::Engine::except(const char* func, const char* message)
{
    if(func && message) COUT(String::time() << ": [EXCEPTION]\t" << func << ":  " << message);

#ifdef BOOST_STACKTRACE_USE_ADDR2LINE

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
    const SDL_version* sdlver1 = nullptr;

#ifdef SWE_SDL12
    sdlver1 = SDL_Linked_Version();
    DEBUG("usage " << "SDL" << ", " << "version: " << static_cast<int>(sdlver1->major) << "." << static_cast<int>(sdlver1->minor) << "." << static_cast<int>(sdlver1->patch));
#else
    SDL_version sdlver2;
    SDL_GetVersion(&sdlver2);
    DEBUG("usage " << "SDL" << ", " << "version: " << static_cast<int>(sdlver2.major) << "." << static_cast<int>(sdlver2.minor) << "." << static_cast<int>(sdlver2.patch) <<
              ", revision: " << SDL_GetRevision());
#endif

    if(SDL_Init(init_flags) != 0)
    {
        ERROR("SDL_Init" << ": " << SDL_GetError());
        return false;
    }

#ifndef SWE_DISABLE_NETWORK
    sdlver1 = SDLNet_Linked_Version();

    if(sdlver1)
        DEBUG("usage " << "SDL_net" << ", " << "version: " << static_cast<int>(sdlver1->major) << "." << static_cast<int>(sdlver1->minor) << "." << static_cast<int>(sdlver1->patch));

    if(SDLNet_Init() < 0)
    {
        ERROR("SDLNet_Init" << ": " << SDL_GetError());
        return false;
    }
#endif // SWE_DISABLE_NETWORK

#ifndef SWE_DISABLE_IMAGE
    sdlver1 = IMG_Linked_Version();

    if(sdlver1)
        DEBUG("usage " << "SDL_image" << ", " << "version: " << static_cast<int>(sdlver1->major) << "." << static_cast<int>(sdlver1->minor) << "." << static_cast<int>(sdlver1->patch));

    if(IMG_Init(IMG_INIT_PNG) == 0)
    {
        ERROR("IMG_Init" << ": " << SDL_GetError());
        return false;
    }
#endif // SWE_DISABLE_IMAGE

#ifndef SWE_DISABLE_TTF
    sdlver1 = TTF_Linked_Version();

    if(sdlver1)
        DEBUG("usage " << "SDL_ttf" << ", " << "version: " << static_cast<int>(sdlver1->major) << "." << static_cast<int>(sdlver1->minor) << "." << static_cast<int>(sdlver1->patch));

    if(TTF_Init() != 0)
    {
        ERROR("TTF_Init" << ": " << SDL_GetError());
	return false;
    }
#endif // SWE_DISABLE_TTF

#ifndef SWE_DISABLE_AUDIO
#if (SDL_VERSIONNUM(SDL_MIXER_MAJOR_VERSION, SDL_MIXER_MINOR_VERSION, SDL_MIXER_PATCHLEVEL) > SDL_VERSIONNUM(1,2,8))
    sdlver1 = Mix_Linked_Version();

    if(sdlver1)
        DEBUG("usage " << "SDL_mixer" << ", " << "version: " << static_cast<int>(sdlver1->major) << "." << static_cast<int>(sdlver1->minor) << "." << static_cast<int>(sdlver1->patch));

    int formats = MIX_INIT_MP3|MIX_INIT_OGG|MIX_INIT_MOD;
    int res = Mix_Init(formats);

    if(res != formats)
        ERROR("MIX_Init" << ": " << Mix_GetError());
#endif // SDL_VERSION

    if(0 > Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024))
        ERROR("MIX_OpenAudio" << ": " << Mix_GetError());

#endif // SWE_DISABLE_AUDIO

#ifdef __MINGW32__
    Display::fingerEventEmulation = true;
#endif

#ifdef SWE_SDL12
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
#ifndef SWE_DISABLE_AUDIO
    Music::reset();
    Mix_CloseAudio();
#if (SDL_VERSIONNUM(SDL_MIXER_MAJOR_VERSION, SDL_MIXER_MINOR_VERSION, SDL_MIXER_PATCHLEVEL) > SDL_VERSIONNUM(1,2,8))
    Mix_Quit();
#endif
#endif
#ifndef SWE_DISABLE_IMAGE
    IMG_Quit();
#endif
#ifndef SWE_DISABLE_NETWORK
    SDLNet_Quit();
#endif
#ifndef SWE_DISABLE_TTF
    TTF_Quit();
#endif
    SDL_Quit();
}


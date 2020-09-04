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

#ifndef SWE_DISABLE_AUDIO

#include "swe_systems.h"
#include "swe_tools.h"
#include "swe_cstring.h"
#include "swe_music.h"

namespace SWE
{

    namespace Music
    {
        Mix_Music*			mixer = nullptr;
        u32				currentId = 0;
    }

    /* Music */
    void Music::setHookFinished(void (*ptr)())
    {
        Mix_HookMusicFinished(ptr);
    }

    bool Music::play(const std::string & name)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(mixer);
        mixer = Mix_LoadMUS(name.c_str());

        if(mixer)
        {
            if(0 <= Mix_PlayMusic(mixer, 0))
            {
                currentId = Tools::crc32b(name.c_str());
                return true;
            }
        }

        ERROR(Mix_GetError());
        return false;
    }

    bool Music::play(const BinaryBuf & raw)
    {
        SDL_RWops* rw = SDL_RWFromConstMem(raw.data(), raw.size());

        if(rw)
        {
            Mix_HaltMusic();
            Mix_FreeMusic(mixer);
#ifdef SWE_SDL12
            mixer = Mix_LoadMUS_RW(rw);
            SDL_FreeRW(rw);
#else
            mixer = Mix_LoadMUS_RW(rw, 1);
#endif

            if(mixer)
            {
                if(0 <= Mix_PlayMusic(mixer, 0))
                {
                    currentId = raw.crc32b();
                    return true;
                }
            }
        }

        ERROR(Mix_GetError());
        return false;
    }

    int Music::volume(int volume)
    {
        return Mix_VolumeMusic(0 < volume ? volume % MIX_MAX_VOLUME : volume);
    }

    void Music::pause(void)
    {
        Mix_PauseMusic();
    }

    void Music::resume(void)
    {
        Mix_ResumeMusic();
    }

    void Music::reset(void)
    {
        if(mixer)
        {
            Mix_HookMusicFinished(nullptr);
            Mix_HaltMusic();
            Mix_FreeMusic(mixer);
            mixer = nullptr;
        }
    }

    bool Music::isPlaying(void)
    {
        return mixer && Mix_PlayingMusic();
    }

    bool Music::isPlaying(u32 crc32b)
    {
        return isPlaying() && currentId == crc32b;
    }

    bool Music::isPlaying(const std::string & name)
    {
        return isPlaying() && currentId == Tools::crc32b(name.c_str());
    }

    /* Sound*/
    void FreeChannel(int channel)
    {
        Mix_Chunk* sample = Mix_GetChunk(channel);

        if(sample) Mix_FreeChunk(sample);
    }

    bool Sound::play(const BinaryBuf & raw)
    {
	return 0 <= playChannel(raw, -1, false);
    }

    int Sound::playChannel(const BinaryBuf & raw, int channel, bool loop)
    {
        SDL_RWops* rw = SDL_RWFromConstMem(raw.data(), raw.size());

        if(rw)
        {
            Mix_Chunk* sample = Mix_LoadWAV_RW(rw, 1);

            if(sample)
            {
                Mix_ChannelFinished(FreeChannel);
                int ch = Mix_PlayChannel(channel, sample, loop ? -1 : 0);
                if(0 <= ch) return ch;
            }
        }

        ERROR(Mix_GetError());
        return -1;
    }

    bool Sound::play(const std::string & name)
    {
	return 0 <= playChannel(name, -1, false);
    }

    int Sound::playChannel(const std::string & name, int channel, bool loop)
    {
        Mix_Chunk* sample = Mix_LoadWAV(name.c_str());

        if(sample)
        {
            Mix_ChannelFinished(FreeChannel);

            int ch = Mix_PlayChannel(channel, sample, loop ? -1 : 0);
            if(0 <= ch) return ch;
        }

        ERROR(Mix_GetError());
        return -1;
    }

    bool Sound::isPlaying(int channel)
    {
        return 0 < Mix_Playing(channel);
    }

    int Sound::volume(int channel, int vol)
    {
	return Mix_Volume(channel, vol > MIX_MAX_VOLUME ? MIX_MAX_VOLUME : vol);
    }

    void Sound::pause(int channel)
    {
	Mix_Pause(channel);
    }

    void Sound::resume(int channel)
    {
	Mix_Resume(channel);
    }

    void Sound::stop(int channel)
    {
	Mix_HaltChannel(channel);
    }
}

#endif

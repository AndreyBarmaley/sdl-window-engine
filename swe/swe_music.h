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

#ifndef _SWE_MUSIC_
#define _SWE_MUSIC_

#include "swe_types.h"

/// @brief пространство SWE
namespace SWE
{

    class BinaryBuf;

    namespace Music
    {
        bool	play(const std::string &);
        bool	play(const BinaryBuf &);
        void	setHookFinished(void (*ptr)());
        int	volume(int volume = -1);
        void	pause(void);
        void	resume(void);
        void	reset(void);
        bool	isPlaying(void);
        bool	isPlaying(u32);
        bool	isPlaying(const std::string &);
    }

    namespace Sound
    {
        bool	play(const std::string &);
        bool	play(const BinaryBuf &);
        int	playChannel(const std::string &, int channel = -1, bool loop = false);
        int	playChannel(const BinaryBuf &, int channel = -1, bool loop = false);
        bool	isPlaying(int channel = -1);
	int	volume(int channel, int volume);
        void	pause(int channel);
        void	resume(int channel);
        void	stop(int channel);
    }

} // SWE
#endif
#endif

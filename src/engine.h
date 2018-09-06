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

#ifndef _SWE_ENGINE_
#define _SWE_ENGINE_

#include <cstdlib>

#include "types.h"
#include "binarybuf.h"
#include "tools.h"
#include "rect.h"
#include "display.h"
#include "events.h"
#include "surface.h"
#include "cstring.h"
#include "cunicode.h"
#include "cunicode_color.h"
#include "systems.h"
#include "serialize.h"
#include "translations.h"
#include "window.h"
#include "termwin.h"
#include "termwin_gui.h"
#include "fontset.h"
#include "inputs_keys.h"
#include "music.h"
#include "window_gui.h"
#include "json_wrapper.h"
#include "lua_wrapper.h"
#include "tinyxml2.h"
#include "sharedlist.h"
#include "sharedvector.h"
#include "sharedmap.h"

namespace Engine
{
    bool		init(void);
    void		quit(void);

    class		exception {};
    void		except(const char* func, const char* message);
}

#endif

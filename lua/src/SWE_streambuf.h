/***************************************************************************
 *   Copyright (C) 2019 by SWE team <sdl.window.engine@gmail.com>          *
 *                                                                         *
 *   Part of the SWE Lua:                                                  *
 *   https://github.com/AndreyBarmaley/SWE_lua                             *
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

#ifndef _SWE_LUA_STREAMBUF_
#define _SWE_LUA_STREAMBUF_

#include "SWE_global.h"

struct lua_State;

enum class IntType { Int8, IntBE16, IntBE32, IntBE64, IntLE16, IntLE32, IntLE64 };

class SWE_StreamBuf : public StreamBufRW
{
public:
    SWE_StreamBuf() {}
    SWE_StreamBuf(const BinaryBuf & buf) : StreamBufRW(buf) {}

    static SWE_StreamBuf* get(LuaState &, int tableIndex, const char* funcName);
    static void registers(LuaState &);
};

#endif

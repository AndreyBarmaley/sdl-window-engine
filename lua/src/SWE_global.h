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

#ifndef _SWE_LUA_GLOBAL_
#define _SWE_LUA_GLOBAL_

#include "libswe.h"
using namespace SWE;

#define SWE_LUA_VERSION 20210620
#define SWE_LUA_LICENSE "GPL3"

#define LuaUnused(val) ((void) val)

#ifdef LUA_STACK_DEBUG
 #define LuaStateDefine(ls,L,num) LuaStateDebug ls(L,__PRETTY_FUNCTION__,num)
 #define LuaStateValidator(ls,num) LuaStateDebug ls2(ls.L(),__PRETTY_FUNCTION__,num);LuaUnused(ls2)
#else
 #define LuaStateDefine(ls,L,num) LuaState ls(L)
 #define LuaStateValidator(ls,num) ;
#endif

#endif

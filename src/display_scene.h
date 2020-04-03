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

#ifndef _SWE_DISPLAY_SCENE_
#define _SWE_DISPLAY_SCENE_

#include <list>
#include "events.h"

class Window;
class Texture;
struct KeySym;

class DisplayScene
{
public:
    static Window*             rootWindow(void);
    static bool                keyPressHandle(const KeySym &);
    static bool                keyReleaseHandle(const KeySym &);
    static bool                textInputHandle(const std::string &);
    static bool                mousePressHandle(const ButtonEvent &);
    static bool                mouseReleaseHandle(const ButtonEvent &);
    static bool                mouseClickHandle(const ButtonsEvent &);
    static void                mouseMotionHandle(const Point &, u32 buttons);
#ifdef OLDENGINE
    static bool                userHandle(const UserEvent &);
#else
    static bool                userHandle(const SDL_UserEvent &);
#endif
    static bool                scrollUpHandle(const Point &);
    static bool                scrollDownHandle(const Point &);
    static void                tickHandle(u32);
    static void                renderPresentHandle(u32);
    static void                textureInvalidHandle(void);
    static void                displayResizeHandle(const Size &, bool);
    static void                displayFocusHandle(bool);

public:
    static std::list<Window*>	findChilds(const Window &);
    static void			destroyChilds(const Window &);

    static void			addItem(Window &);
    static void			removeItem(Window &);
    static void			dumpScene(void);
    static void			pushEvent(const Window* dst, int code, void* data);

    static void			sceneRedraw(void);
    static void			sceneDestroy(void);
    static void			setCursor(const Texture &, const Point &);

    static bool			isDirty(void);
    static void			setDirty(bool);
    static void			waitDelay(int);
};

#endif

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
#include "swe_events.h"

#ifdef SWE_WITH_JSON
#include "swe_json_ext.h"
#endif

namespace SWE
{
    class Window;
    class Texture;
    struct KeySym;
    class BaseObject;

    class DisplayScene
    {
    public:
        static Window*             rootWindow(void);
	static Window*             focusedWindow(void);
	static Window*             windowsFocusHandle(void);
        static bool                keyHandle(const KeySym &, bool);
        static bool                textInputHandle(const std::string &);
        static bool                mouseButtonHandle(const ButtonEvent &, bool);
        static bool                mouseClickHandle(const ButtonsEvent &);
        static bool                mouseMotionHandle(const Point &, u32 buttons);
#ifdef SWE_SDL12
        static bool                userHandle(const UserEvent &);
#else
        static bool                userHandle(const SDL_UserEvent &);
#endif
        static bool                scrollHandle(bool);
        static void                tickHandle(u32);
        static void                renderPresentHandle(u32);
        static void                textureInvalidHandle(void);
        static void                displayResizeHandle(const Size &, bool);
        static void                displayFocusHandle(bool);

	static const std::list<Window*> & items(void);
        static std::list<Window*>  findChilds(const Window &);
        static std::list<Window*>  findParents(const Window &);

	static void		addObject(BaseObject &);
	static void		removeObject(const BaseObject &);

        static void		addItem(Window &);
        static void		removeItem(const Window &);
        static void		pushEvent(const ObjectEvent* dst, int code, void* data);
	static void		moveTopLayer(const Window &);

        static void		sceneRedraw(void);
        static void		sceneDestroy(void);
        static void		handleWhileVisible(const Window &);
        static void		setCursor(const Texture &, const Point &);
        static void		resetCursor(void);

        static bool		isDirty(void);
        static void		setDirty(bool);
        static void		handleEvents(u32 interval = 0);

#ifdef SWE_WITH_JSON
	static JsonObject	toJson(void);
#endif
        static void		dumpScene(void);
    };

} // SWE
#endif

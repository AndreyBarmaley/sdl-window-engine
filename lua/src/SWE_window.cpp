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

#include <sstream>
#include <algorithm>

#include "SWE_rect.h"
#include "SWE_tools.h"
#include "SWE_texture.h"
#include "SWE_fontrender.h"
#include "SWE_window.h"

int SWE_window_create(lua_State*);
int SWE_window_destroy(lua_State*);

int SWE_polygon_create(lua_State*);
int SWE_polygon_destroy(lua_State*);

void SWE_window_render(LuaState & ll, Window & win)
{
    LuaStateValidator(ll, 0);
    bool extrender = false;

    if(SWE_Scene::window_pushtop(ll, win))
    {
	if(ll.getFieldTableIndex("RenderWindow", -1, false).isTopFunction())
	{
	    extrender = ll.callFunction(0, 1).getTopBoolean();
	    ll.stackPop();
	}
	else
	{
	    ll.stackPop();
	}

	ll.stackPop();
    }

    if(! extrender && Engine::debugMode())
    {
	win.renderClear(Color::LightGreen);
	for(int yy = 0; yy < win.height(); yy += 16)
	{
	    for(int xx = 0; xx < win.width(); xx += 32)
	    {
		win.renderColor(Color::Gray, Rect(0 == (yy % 32) ? xx + 16 : xx, yy, 16, 16));
	    }
	}
	win.renderRect(Color::RoyalBlue, win.rect());
    }
}

bool SWE_mouse_press_event(LuaState & ll, Window & win, const ButtonEvent & be)
{
    LuaStateValidator(ll, 0);

    if(SWE_Scene::window_pushtop(ll, win))
    {
	if(ll.getFieldTableIndex("MousePressEvent", -1, false).isTopFunction())
	{
	    ll.pushInteger(be.position().x).pushInteger(be.position().y).pushInteger(be.button());
	    bool res = ll.callFunction(3, 1).getTopBoolean();
	    // remove boolean, table
	    ll.stackPop(2);
	    return res;
	}
	else
	{
	    ll.stackPop();
	}

	ll.stackPop();
    }

    return false;
}

bool SWE_mouse_release_event(LuaState & ll, Window & win, const ButtonEvent & be)
{
    LuaStateValidator(ll, 0);

    if(SWE_Scene::window_pushtop(ll, win))
    {
	if(ll.getFieldTableIndex("MouseReleaseEvent", -1, false).isTopFunction())
	{
	    ll.pushInteger(be.position().x).pushInteger(be.position().y).pushInteger(be.button());
	    bool res = ll.callFunction(3, 1).getTopBoolean();
	    // remove boolean, table
	    ll.stackPop(2);
	    return res;
	}
	else
	{
	    ll.stackPop();
	}

	ll.stackPop();
    }

    return false;
}

bool SWE_mouse_motion_event(LuaState & ll, Window & win, const Point & pos, u32 buttons)
{
    LuaStateValidator(ll, 0);

    if(SWE_Scene::window_pushtop(ll, win))
    {
	if(ll.getFieldTableIndex("MouseMotionEvent", -1, false).isTopFunction())
	{
	    ll.pushInteger(pos.x).pushInteger(pos.y).pushInteger(buttons);
	    bool res = ll.callFunction(3, 1).getTopBoolean();
	    // remove boolean, table
	    ll.stackPop(2);
	    return res;
	}
	else
	{
	    ll.stackPop();
	}

	ll.stackPop();
    }

    return false;
}

void SWE_mouse_tracking_event(LuaState & ll, Window & win, const Point & pos, u32 buttons)
{
    LuaStateValidator(ll, 0);

    if(SWE_Scene::window_pushtop(ll, win))
    {
	if(ll.getFieldTableIndex("MouseTrackingEvent", -1, false).isTopFunction())
	{
	    ll.pushInteger(pos.x).pushInteger(pos.y).pushInteger(buttons);
	    ll.callFunction(3, 0);
	}
	else
	{
	    ll.stackPop();
	}

	ll.stackPop();
    }
}

bool SWE_mouse_click_event(LuaState & ll, Window & win, const ButtonsEvent & be)
{
    LuaStateValidator(ll, 0);

    if(SWE_Scene::window_pushtop(ll, win))
    {
	if(ll.getFieldTableIndex("MouseClickEvent", -1, false).isTopFunction())
	{
	    ll.pushInteger(be.press().position().x).pushInteger(be.press().position().y).pushInteger(be.press().button());
	    ll.pushInteger(be.release().position().x).pushInteger(be.release().position().y).pushInteger(be.release().button());
	    bool res = ll.callFunction(6, 1).getTopBoolean();
	    // remove boolean, table
	    ll.stackPop(2);
	    return res;
	}
	else
	{
	    ll.stackPop();
	}

	ll.stackPop();
    }

    return false;
}

void SWE_mouse_focus_event(LuaState & ll, Window & win)
{
    LuaStateValidator(ll, 0);

    if(SWE_Scene::window_pushtop(ll, win))
    {
	if(ll.getFieldTableIndex("MouseFocusEvent", -1, false).isTopFunction())
	{
	    ll.pushBoolean(true).callFunction(1, 0);
	}
	else
	{
	    ll.stackPop();
	}

	ll.stackPop();
    }
}

void SWE_mouse_leave_event(LuaState & ll, Window & win)
{
    LuaStateValidator(ll, 0);

    if(SWE_Scene::window_pushtop(ll, win))
    {
	if(ll.getFieldTableIndex("MouseFocusEvent", -1, false).isTopFunction())
	{
	    ll.pushBoolean(false).callFunction(1, 0);
	}
	else
	{
	    ll.stackPop();
	}

	ll.stackPop();
    }
}

void SWE_window_create_event(LuaState & ll, Window & win)
{
    LuaStateValidator(ll, 0);

    if(SWE_Scene::window_pushtop(ll, win))
    {
	if(ll.getFieldTableIndex("WindowCreateEvent", -1, false).isTopFunction())
	{
	    ll.callFunction(0, 0);
	}
	else
	{
	    ll.stackPop();
	}

	ll.stackPop();
    }
}

bool SWE_text_input_event(LuaState & ll, Window & win, const std::string & str)
{
    LuaStateValidator(ll, 0);

    if(SWE_Scene::window_pushtop(ll, win))
    {
	if(ll.getFieldTableIndex("TextInputEvent", -1, false).isTopFunction())
	{
	    ll.pushString(str);
	    bool res = ll.callFunction(1, 1).getTopBoolean();
	    // remove boolean, table
	    ll.stackPop(2);
	    return res;
	}
	else
	{
	    ll.stackPop();
	}

	ll.stackPop();
    }

    return false;
}

void SWE_display_focus_event(LuaState & ll, Window & win, bool f)
{
    LuaStateValidator(ll, 0);

    if(SWE_Scene::window_pushtop(ll, win))
    {
	if(ll.getFieldTableIndex("DisplayFocusEvent", -1, false).isTopFunction())
	{
	    ll.pushBoolean(f);
	    ll.callFunction(1, 0);
	}
	else
	{
	    ll.stackPop();
	}

	ll.stackPop();
    }
}

void SWE_window_visible_event(LuaState & ll, Window & win, bool f)
{
    LuaStateValidator(ll, 0);

    if(SWE_Scene::window_pushtop(ll, win))
    {
	if(ll.getFieldTableIndex("WindowVisibleEvent", -1, false).isTopFunction())
	{
	    ll.pushBoolean(f);
	    ll.callFunction(1, 0);
	}
	else
	{
	    ll.stackPop();
	}

	ll.stackPop();
    }
}

void SWE_window_resize_event(LuaState & ll, Window & win, const Size & sz)
{
    LuaStateValidator(ll, 0);

    if(SWE_Scene::window_pushtop(ll, win))
    {
	if(ll.getFieldTableIndex("WindowResizeEvent", -1, false).isTopFunction())
	{
	    ll.pushInteger(sz.w).pushInteger(sz.h);
	    ll.callFunction(2, 0);
	}
	else
	{
	    ll.stackPop();
	}

	ll.stackPop();
    }
}

void SWE_window_move_event(LuaState & ll, Window & win, const Point & pos)
{
    LuaStateValidator(ll, 0);

    if(SWE_Scene::window_pushtop(ll, win))
    {
	if(ll.getFieldTableIndex("WindowMoveEvent", -1, false).isTopFunction())
	{
	    ll.pushInteger(pos.x).pushInteger(pos.y);
	    ll.callFunction(2, 0);
	}
	else
	{
	    ll.stackPop();
	}

	ll.stackPop();
    }
}

void SWE_texture_invalid_event(LuaState & ll, Window & win)
{
    LuaStateValidator(ll, 0);

    if(SWE_Scene::window_pushtop(ll, win))
    {
	if(ll.getFieldTableIndex("TextureInvalidEvent", -1, false).isTopFunction())
	{
	    ll.callFunction(0, 0);
	}
	else
	{
	    ll.stackPop();
	}

	ll.stackPop();
    }
}

void SWE_display_resize_event(LuaState & ll, Window & win, const Size & winsz)
{
    LuaStateValidator(ll, 0);

    if(SWE_Scene::window_pushtop(ll, win))
    {
        // fix size fields
        ll.pushInteger(win.width()).setFieldTableIndex("width", -2);
        ll.pushInteger(win.height()).setFieldTableIndex("height", -2);

	if(ll.getFieldTableIndex("DisplayResizeEvent", -1, false).isTopFunction())
	{
	    ll.pushInteger(winsz.w).pushInteger(winsz.h).callFunction(2, 0);
	}
	else
	{
	    ll.stackPop();
	}

	ll.stackPop();
    }
}

bool SWE_key_press_event(LuaState & ll, Window & win, const KeySym & key)
{
    LuaStateValidator(ll, 0);

    if(SWE_Scene::window_pushtop(ll, win))
    {
	if(ll.getFieldTableIndex("KeyPressEvent", -1, false).isTopFunction())
	{
	    ll.pushInteger(key.keycode()).pushInteger(key.keymod().mod).pushInteger(key.scancode());
	    bool res = ll.callFunction(3, 1).getTopBoolean();
	    // remove boolean, table
	    ll.stackPop(2);
	    return res;
	}
	else
	{
	    ll.stackPop();
	}

	ll.stackPop();
    }

    return false;
}

bool SWE_key_release_event(LuaState & ll, Window & win, const KeySym & key)
{
    LuaStateValidator(ll, 0);

    if(SWE_Scene::window_pushtop(ll, win))
    {
	if(ll.getFieldTableIndex("KeyReleaseEvent", -1, false).isTopFunction())
	{
	    ll.pushInteger(key.keycode()).pushInteger(key.keymod().mod).pushInteger(key.scancode());
	    bool res = ll.callFunction(3, 1).getTopBoolean();
	    // remove boolean, table
	    ll.stackPop(2);
	    return res;
	}
	else
	{
	    ll.stackPop();
	}

	ll.stackPop();
    }

    return false;
}

bool SWE_scroll_up_event(LuaState & ll, Window & win)
{
    LuaStateValidator(ll, 0);

    if(SWE_Scene::window_pushtop(ll, win))
    {
	if(ll.getFieldTableIndex("ScrollUpEvent", -1, false).isTopFunction())
	{
	    const Point & pos = Display::mouseCursorPosition();
	    ll.pushInteger(pos.x).pushInteger(pos.y);
	    bool res = ll.callFunction(2, 1).getTopBoolean();
	    // remove boolean, table
	    ll.stackPop(2);
	    return res;
	}
	else
	{
	    ll.stackPop();
	}

	ll.stackPop();
    }

    return false;
}

bool SWE_scroll_down_event(LuaState & ll, Window & win)
{
    LuaStateValidator(ll, 0);

    if(SWE_Scene::window_pushtop(ll, win))
    {
	if(ll.getFieldTableIndex("ScrollDownEvent", -1, false).isTopFunction())
	{
	    const Point & pos = Display::mouseCursorPosition();
	    ll.pushInteger(pos.x).pushInteger(pos.y);
	    bool res = ll.callFunction(2, 1).getTopBoolean();
	    // remove boolean, table
	    ll.stackPop(2);
	    return res;
	}
	else
	{
	    ll.stackPop();
	}

	ll.stackPop();
    }

    return false;
}

bool SWE_system_user_event(LuaState & ll, Window & win, int code, void* data)
{
    LuaStateValidator(ll, 0);

    if(SWE_Scene::window_pushtop(ll, win))
    {
	if(ll.getFieldTableIndex("SystemUserEvent", -1, false).isTopFunction())
	{
	    // unref data
	    if(code == Signal::LuaUnrefAction)
	    {
		if(data)
		{
		    int objRef = reinterpret_cast<intptr_t>(data);
		    luaL_unref(ll.L(), LUA_REGISTRYINDEX, objRef);
		    DEBUGN("object unref: " << String::hex(objRef), 2);
		}

		// remove function, table
		ll.stackPop(2);
		return true;
	    }

	    ll.pushInteger(code);
	    if(data)
	    {
		int objRef = reinterpret_cast<intptr_t>(data);
		lua_rawgeti(ll.L(), LUA_REGISTRYINDEX, objRef);
	    }
	    else
	    {
		ll.pushNil();
	    }

	    bool res = ll.callFunction(2, 1).getTopBoolean();

	    // remove boolean, table
	    ll.stackPop(2);
	    return res;
	}
	else
	{
	    ll.stackPop();
	}

	ll.stackPop();
    }

    return false;
}

void SWE_system_tick_event(LuaState & ll, Window & win, u32 ms)
{
    LuaStateValidator(ll, 0);

    if(SWE_Scene::window_pushtop(ll, win))
    {
	if(ll.getFieldTableIndex("SystemTickEvent", -1, false).isTopFunction())
	{
	    ll.pushInteger(ms);
	    ll.callFunction(1, 0);
	}
	else
	{
	    ll.stackPop();
	}

	ll.stackPop();
    }
}

void SWE_system_render_event(LuaState & ll, Window & win, u32 ms)
{
    LuaStateValidator(ll, 0);

    if(SWE_Scene::window_pushtop(ll, win))
    {
	if(ll.getFieldTableIndex("SystemRenderEvent", -1, false).isTopFunction())
	{
	    ll.pushInteger(ms);
	    ll.callFunction(1, 0);
	}
	else
	{
	    ll.stackPop();
	}

	ll.stackPop();
    }
}

SWE_Window::SWE_Window(lua_State* L, Window* parent)
    : WindowToolTipArea(parent), ll(L)
{
    setState(FlagModality, ! parent);
}

SWE_Window::SWE_Window(lua_State* L, const Point & pos, const Size & winsz, Window* parent)
    : WindowToolTipArea(pos, winsz, parent), ll(L)
{
    setState(FlagModality, ! parent);
    setVisible(true);
}

SWE_Window* SWE_Window::get(LuaState & ll, int tableIndex, const char* funcName)
{
    LuaStateValidator(ll, 0);

    if(ll.isTableIndex(tableIndex))
    {
	const std::string type = ll.popFieldTableIndex("__type", tableIndex);

	if(0 != type.compare("swe.window") &&
	    0 != type.compare("swe.terminal") && 0 != type.compare("swe.polygon"))
	{
    	    ERROR(funcName << ": " << "table not found, index: " << tableIndex << ", type: " << type);
    	    return NULL;
	}
    }
    else
    {
    	ERROR(funcName << ": " << "table not found, index: " << tableIndex);
    	return NULL;
    }

    if(! ll.getFieldTableIndex("userdata", tableIndex).isTopUserData())
    {
	ERROR(funcName << ": " << "not userdata, index: " << tableIndex << ", " << ll.getTopTypeName());
	ll.stackPop();
	return NULL;
    }

    auto ptr = static_cast<SWE_Window**>(ll.getTopUserData());
    ll.stackPop();

    return ptr ? *ptr : NULL;
}

SWE_Polygon* SWE_Polygon::get(LuaState & ll, int tableIndex, const char* funcName)
{
    LuaStateValidator(ll, 0);

    if(ll.isTableIndex(tableIndex))
    {
	const std::string type = ll.popFieldTableIndex("__type", tableIndex);

	if(0 != type.compare("swe.polygon"))
	{
    	    ERROR(funcName << ": " << "table not found, index: " << tableIndex << ", type: " << type);
    	    return NULL;
	}
    }
    else
    {
    	ERROR(funcName << ": " << "table not found, index: " << tableIndex);
    	return NULL;
    }

    if(! ll.getFieldTableIndex("userdata", tableIndex).isTopUserData())
    {
	ERROR(funcName << ": " << "not userdata, index: " << tableIndex << ", " << ll.getTopTypeName());
	ll.stackPop();
	return NULL;
    }

    auto ptr = static_cast<SWE_Polygon**>(ll.getTopUserData());
    ll.stackPop();

    return ptr ? *ptr : NULL;
}

/////////////////////////////////////// 

int SWE_window_set_position(lua_State* L)
{
    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    // params: swe_window, posx, posy

    if(auto win = SWE_Window::get(ll, 1, __FUNCTION__))
    {
	int posx = ll.toIntegerIndex(2);
	int posy = ll.toIntegerIndex(3);

	win->setPosition(Point(posx, posy));

	// userdata, posy, posx, swe_window...
	const auto & pos = win->position();
	ll.pushInteger(pos.x).setFieldTableIndex("posx", 1);
	ll.pushInteger(pos.y).setFieldTableIndex("posy", 1);
    }
    else
    {
	ERROR("userdata empty");
    }

    return rescount;
}

int SWE_window_set_size(lua_State* L)
{
    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    // params: swe_window, width, height

    if(auto win = SWE_Window::get(ll, 1, __FUNCTION__))
    {
	int width = ll.toIntegerIndex(2);
	int height = ll.toIntegerIndex(3);

	win->setSize(Size(width, height));

	// userdata, posy, posx, swe_window...
	ll.pushInteger(width).setFieldTableIndex("width", 1);
	ll.pushInteger(height).setFieldTableIndex("height", 1);
    }
    else
    {
	ERROR("userdata empty");
    }

    return rescount;
}

int SWE_window_set_result(lua_State* L)
{
    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    // params: swe_window, int

    if(auto win = SWE_Window::get(ll, 1, __FUNCTION__))
    {
	int result = ll.toIntegerIndex(2);
	win->setResultCode(result);

	// userdata, bool, swe_window...
	ll.pushInteger(result).setFieldTableIndex("result", 1);
    }
    else
    {
	ERROR("userdata empty");
    }

    return rescount;
}

int SWE_window_set_destroyed(lua_State* L)
{
    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    // params: swe_window, bool

    if(auto win = SWE_Window::get(ll, 1, __FUNCTION__))
    {
	win->setState(FlagBroken, true);
    }
    else
    {
	ERROR("userdata empty");
    }

    return rescount;
}

int SWE_window_set_visible(lua_State* L)
{
    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    // params: swe_window, bool

    if(auto win = SWE_Window::get(ll, 1, __FUNCTION__))
    {
	bool flag = ll.toBooleanIndex(2);
	win->setVisible(flag);
	ll.pushBoolean(flag).setFieldTableIndex("visible", 1);
    }
    else
    {
	ERROR("userdata empty");
    }

    return rescount;
}

int SWE_window_set_modality(lua_State* L)
{
    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    // params: swe_window, bool

    if(auto win = SWE_Window::get(ll, 1, __FUNCTION__))
    {
	bool flag = ll.toBooleanIndex(2);
	win->setModality(flag);
	ll.pushBoolean(flag).setFieldTableIndex("modality", 1);
    }
    else
    {
	ERROR("userdata empty");
    }

    return rescount;
}

int SWE_window_set_keyhandle(lua_State* L)
{
    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    // params: swe_window, bool

    if(auto win = SWE_Window::get(ll, 1, __FUNCTION__))
    {
	bool flag = ll.toBooleanIndex(2);
	win->setKeyHandle(flag);
	ll.pushBoolean(flag).setFieldTableIndex("keyhandle", 1);
    }
    else
    {
	ERROR("userdata empty");
    }

    return rescount;
}

int SWE_window_set_mousetrack(lua_State* L)
{
    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    // params: swe_window, bool

    if(auto win = SWE_Window::get(ll, 1, __FUNCTION__))
    {
	bool flag = ll.toBooleanIndex(2);
	win->setMouseTrack(flag);
    }
    else
    {
	ERROR("userdata empty");
    }

    return rescount;
}

int SWE_window_render_clear(lua_State* L)
{
    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    // params: swe_window, color

    if(auto win = SWE_Window::get(ll, 1, __FUNCTION__))
    {
	ARGB argb = ll.toIntegerIndex(2);
	win->renderClear(Color(argb));
    }
    else
    {
	ERROR("userdata empty");
    }

    return rescount;
}

int SWE_window_render_rect(lua_State* L)
{
    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    // params: swe_window, color, posx, posy, width, height, bool
    // params: swe_window, color, rect, bool

    if(auto win = SWE_Window::get(ll, 1, __FUNCTION__))
    {
        ARGB argb = ll.toIntegerIndex(2);
	int filled = false;
	Rect rt;

	if(ll.isTableIndex(3))
	{
	    rt = SWE_Rect::get(ll, 3, __FUNCTION__);

	    if(ll.isBooleanIndex(4))
		filled = ll.toBooleanIndex(4);
	}
	else
	{
	    rt.x = ll.toIntegerIndex(3);
    	    rt.y = ll.toIntegerIndex(4);
	    rt.w = ll.toIntegerIndex(5);
    	    rt.h = ll.toIntegerIndex(6);

	    if(ll.isBooleanIndex(7))
		filled = ll.toBooleanIndex(7);
	}

	if(filled)
	    win->renderColor(Color(argb), rt);
	else
	    win->renderRect(Color(argb), rt);
    }
    else
    {
	ERROR("userdata empty");
    }

    return rescount;
}

int SWE_window_render_point(lua_State* L)
{
    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    // params: swe_window, color, posx. posy
    // params: swe_window, color, point

    if(auto win = SWE_Window::get(ll, 1, __FUNCTION__))
    {
	ARGB argb = ll.toIntegerIndex(2);
	Point pt;

	if(ll.isTableIndex(3))
	    pt = SWE_Point::get(ll, 3, __FUNCTION__);
	else
	{
    	    pt.x = ll.toIntegerIndex(3);
	    pt.y = ll.toIntegerIndex(4);
	}

	win->renderPoint(Color(argb), pt);
    }
    else
    {
	ERROR("userdata empty");
    }

    return rescount;
}

int SWE_window_render_line(lua_State* L)
{
    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    // params: swe_window, color, pt1x, pt1y, pt2x, pt2y
    // params: swe_window, color, point1, point2

    if(auto win = SWE_Window::get(ll, 1, __FUNCTION__))
    {
	ARGB argb = ll.toIntegerIndex(2);
	Point pt1, pt2;

	if(ll.isTableIndex(3))
	{
	    pt1 = SWE_Point::get(ll, 3, __FUNCTION__);

	    if(ll.isTableIndex(4))
		pt2 = SWE_Point::get(ll, 4, __FUNCTION__);
	    else
	    {
		pt2.x = ll.toIntegerIndex(4);
    		pt2.y = ll.toIntegerIndex(5);
	    }
	}
	else
	{
    	    pt1.x = ll.toIntegerIndex(3);
    	    pt1.y = ll.toIntegerIndex(4);
	    pt2.x = ll.toIntegerIndex(5);
    	    pt2.y = ll.toIntegerIndex(6);
	}

	win->renderLine(Color(argb), pt1, pt2);
    }
    else
    {
	ERROR("userdata empty");
    }

    return rescount;
}

int SWE_window_render_cyrcle(lua_State* L)
{
    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    // params: swe_window, color, px, py, int radius, bool filled
    // params: swe_window, color, point center, int radius, bool filled

    if(auto win = SWE_Window::get(ll, 1, __FUNCTION__))
    {
        ARGB argb = ll.toIntegerIndex(2);
	Point center;
        int radius = 0;
        bool filled = false;
        int params = ll.stackSize();

	if(ll.isTableIndex(3))
	{
	    center = SWE_Point::get(ll, 3, __FUNCTION__);
    	    radius = ll.toIntegerIndex(4);
    	    if(4 < params) filled = ll.toBooleanIndex(5);
	}
	else
	{
    	    center.x = ll.toIntegerIndex(3);
    	    center.y = ll.toIntegerIndex(4);
    	    radius = ll.toIntegerIndex(5);
    	    if(5 < params) filled = ll.toBooleanIndex(6);
	}

	Points points = Tools::renderCircle(center, radius, filled);

	for(auto it = points.begin(); it != points.end(); ++it)
	    win->renderPoint(Color(argb), *it);
    }
    else
    {
	ERROR("userdata empty");
    }

    return rescount;
}

int SWE_window_render_texture(lua_State* L)
{
    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);


    SWE_Window* win = SWE_Window::get(ll, 1, __FUNCTION__);
    SWE_Texture* ptr = SWE_Texture::get(ll, 2, __FUNCTION__);
    int flip = SWE::FlipNone;

    if(win && ptr)
    {
	Rect src;
	Point dst;
        int params = ll.stackSize();

	// params: swe_window, swe_texture, [rect src], [point dst], flip
	// params: swe_window, swe_texture, [point dst], flip
	if(ll.isTableIndex(3))
	{
	    if(ll.isTableIndex(4))
	    {
		src = SWE_Rect::get(ll, 3, __FUNCTION__);
		dst = SWE_Point::get(ll, 4, __FUNCTION__);
    		flip = 4 < params ? ll.toIntegerIndex(5) : SWE::FlipNone;
	    }
	    else
	    {
		src = ptr->rect();
		dst = SWE_Point::get(ll, 3, __FUNCTION__);
    		flip = 3 < params ? ll.toIntegerIndex(4) : SWE::FlipNone;
	    }
	}
	// params: swe_window, swe_texture, srcx, srcy, srcw, srch, dstx, dsty, flip
        // params: swe_window, swe_texture, dstx, dsty, flip
	else
	{
	    if(5 < params)
    	    {
		src.x = ll.toIntegerIndex(3);
    		src.y = ll.toIntegerIndex(4);
		src.w = ll.toIntegerIndex(5);
    		src.h = ll.toIntegerIndex(6);
    		dst.x = ll.toIntegerIndex(7);
    		dst.y = ll.toIntegerIndex(8);
    		flip = 8 < params ? ll.toIntegerIndex(9) : SWE::FlipNone;
	    }
	    else
	    {
    		dst.x = ll.toIntegerIndex(3);
    		dst.y = ll.toIntegerIndex(4);
    		flip = 4 < params ? ll.toIntegerIndex(5) : SWE::FlipNone;
		src = ptr->rect();
	    }
	}

	win->renderTexture(*ptr, src, dst, flip);
    }
    else
    {
	ERROR("userdata empty");
    }

    return rescount;
}

int SWE_window_render_text(lua_State* L)
{
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    // params: swe_window, swe_fontrender, string, color, dstx, dsty, int halign, int valign, boolean horizontal, int render, int style, int hinting
    // params: swe_window, swe_fontrender, string, color, point dst, int halign, int valign, boolean horizontal, int render, int style, int hinting

    SWE_Window* win = SWE_Window::get(ll, 1, __FUNCTION__);
    SWE_FontRender* frs = SWE_FontRender::get(ll, 2, __FUNCTION__);

    if(win && frs)
    {
	int params = ll.stackSize();
	std::string text = SWE_Tools::convertEncoding(ll, ll.toStringIndex(3));
	ARGB argb = ll.toIntegerIndex(4);
	Point dst;
	auto halign = AlignLeft;
	auto valign = AlignTop;
	bool horizontal = true;
        auto render = RenderDefault;
        int  style = StyleDefault;
        auto hinting = HintingDefault;

	if(ll.isTableIndex(5))
	{
	    dst = SWE_Point::get(ll, 5, __FUNCTION__);

	    if(5 < params)
		halign = static_cast<AlignType>(ll.toIntegerIndex(6));
	    if(6 < params)
		valign = static_cast<AlignType>(ll.toIntegerIndex(7));
	    if(7 < params)
		horizontal = ll.toBooleanIndex(8);

            if(8 < params)
                render = static_cast<CharRender>(ll.toIntegerIndex(9));
            if(9 < params)
                style = ll.toIntegerIndex(10);
            if(10 < params)
                hinting = static_cast<CharHinting>(ll.toBooleanIndex(11));
	}
	else
	{
	    dst.x = ll.toIntegerIndex(5);
	    dst.y = ll.toIntegerIndex(6);

	    if(6 < params)
		halign = static_cast<AlignType>(ll.toIntegerIndex(7));
	    if(7 < params)
		valign = static_cast<AlignType>(ll.toIntegerIndex(8));
	    if(8 < params)
		horizontal = ll.toBooleanIndex(9);

            if(9 < params)
                render = static_cast<CharRender>(ll.toIntegerIndex(10));
            if(10 < params)
                style = ll.toIntegerIndex(11);
            if(11 < params)
                hinting = static_cast<CharHinting>(ll.toBooleanIndex(12));
	}

	Rect area = win->renderText(*frs, text, Color(argb), dst, halign, valign, horizontal, render, style, hinting);
	SWE_Stack::rect_create(ll, area.x, area.y, area.w, area.h);
    }
    else
    {
	ERROR("userdata empty");
	ll.pushNil();
    }

    return rescount;
}

int SWE_window_to_json(lua_State* L)
{
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    // params: swe_window

    if(SWE_Window::get(ll, 1, __FUNCTION__))
    {
        bool visible = ll.getFieldTableIndex("visible", 1).getTopBoolean();
        bool modality = ll.getFieldTableIndex("modality", 1).getTopBoolean();
        bool keyhandle = ll.getFieldTableIndex("keyhandle", 1).getTopBoolean();
        int posx = ll.getFieldTableIndex("posx", 1).getTopInteger();
        int posy = ll.getFieldTableIndex("posy", 1).getTopInteger();
        int width = ll.getFieldTableIndex("width", 1).getTopInteger();
        int height = ll.getFieldTableIndex("height", 1).getTopInteger();
        ll.stackPop(8);

        std::string str = StringFormat("{\"type\":\"swe.window\",\"posx\":%1,\"posy\":%2,\"width\":%3,\"height\":%4,\"visible\":%5,\"modality\":%6,\"keyhandle\":%7}").
            arg(posx).arg(posy).arg(width).arg(height).arg(visible).arg(modality).arg(keyhandle);

        ll.pushString(str);
    }
    else
    {
	ERROR("userdata empty");
	ll.pushNil();
    }

    return rescount;
}

int SWE_window_is_visible(lua_State* L)
{
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    // params: swe_window

    if(auto win = SWE_Window::get(ll, 1, __FUNCTION__))
    {
	bool res = win->isVisible();
        ll.pushBoolean(res);
    }
    else
    {
	ERROR("userdata empty");
        ll.pushBoolean(false);
    }

    return rescount;
}

int SWE_window_point_inarea(lua_State* L)
{
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    // params: swe_window, ptx, pty
    // params: swe_window, point

    if(auto win = SWE_Window::get(ll, 1, __FUNCTION__))
    {
	Point pt;

	// params: swe_window, point
	if(ll.isTableIndex(2))
	    pt = SWE_Point::get(ll, 2, __FUNCTION__);
	else
	{
    	    pt.x = ll.toIntegerIndex(2);
    	    pt.y = ll.toIntegerIndex(3);
	}

	bool res = win->isAreaPoint(pt);
        ll.pushBoolean(res);
    }
    else
    {
	ERROR("userdata empty");
        ll.pushBoolean(false);
    }

    return rescount;
}

int SWE_window_set_tooltip(lua_State* L)
{
    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    // params: swe_window, string, fontrender, color, color, color

    if(! ll.isStringIndex(2))
    {
        ERROR("string not found");
	return rescount;
    }

    auto win = static_cast<SWE_Window*>(SWE_Window::get(ll, 1, __FUNCTION__));

    if(! win)
    {
	ERROR("userdata empty");
	return rescount;
    }

    std::string text = ll.toStringIndex(2);

    if(ll.isTableIndex(3))
    {
	SWE_FontRender* frs = SWE_FontRender::get(ll, 3, __FUNCTION__);
	if(frs)
	{
	    ARGB colorFn = ll.toIntegerIndex(4);
	    ARGB colorBg = ll.toIntegerIndex(5);
	    ARGB colorRt = ll.toIntegerIndex(6);
	    win->renderToolTip(text, *frs, Color(colorFn), Color(colorBg), Color(colorRt));
	}
	else
	{
	    ERROR("userdata empty");
	}
    }
    else
    {
	win->setToolTip(text);
    }

    return rescount;
}

int SWE_window_childrens(lua_State* L)
{
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    // params: swe_window
    auto win = SWE_Window::get(ll, 1, __FUNCTION__);

    // add result: table
    ll.pushTable();

    if(! win)
    {
	ERROR("userdata empty");
	return rescount;
    }

    if(! ll.pushTable("SWE.Scene.Windows").isTopTable())
    {
	ERROR("table not found" << ": " << "swe.scene");
	return rescount;
    }

    // iterate SWE.Scene.Windows
    ll.pushNil();
    int index = 1;

    while(ll.nextTableIndex(-2))
    {
	// stack: swe_window, table result, SWE.Scene.Windows, key, value(swe_window)
	auto child = SWE_Window::get(ll, -1, __FUNCTION__);
	if(child && child->parent() == win)
	{
	    // add value to table result
	    ll.pushInteger(index++).pushValueIndex(-2).setTableIndex(-6);
	}

	// pop value
	ll.stackPop();
    }

    // stack: swe_window, table result, SWE.Scene.Windows
    ll.stackPop();

    // table result
    return rescount;
}

/// move swe_window top layer
int SWE_scene_move_toplayer(lua_State* L)
{
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    // stack: ..., swe_window

    if(! ll.isTopTable())
    {
	ERROR("table not found" << ": " << "swe.window");
	ll.pushBoolean(false);
	return rescount;
    }

    if(auto win = SWE_Window::get(ll, -1, __FUNCTION__))
    {
        SWE::DisplayScene::moveTopLayer(*win);
	ll.pushBoolean(true);
    }
    else
    {
	ERROR("userdata empty");
	ll.pushBoolean(false);
    }

    return rescount;
}

/// remove swe_window from stack
int SWE_scene_removetop(lua_State* L)
{
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    // stack: ..., swe_window

    if(! ll.isTopTable())
    {
	ERROR("table not found" << ": " << "swe.window");
	ll.pushBoolean(false);
	return rescount;
    }

    auto win = SWE_Window::get(ll, -1, __FUNCTION__);
    if(! win)
    {
	ERROR("userdata empty");
	ll.pushBoolean(false);
	return rescount;
    }

    if(! ll.pushTable("SWE.Scene.Windows").isTopTable())
    {
	ERROR("table not found" << ": " << "swe.scene.windows");
	ll.stackPop();
	ll.pushBoolean(false);
	return rescount;
    }

    win->setVisible(false);
    std::string hexid = String::hex(win->id());

    ll.pushNil().setFieldTableIndex(hexid, -2);
    ll.stackPop();

    // SWE.Scene.Windows mode: weak values
    DisplayScene::removeItem(*win);
    ll.garbageCollect();

    ll.pushBoolean(true);
    return rescount;
}

/* SWE_Scene */
/// find swe_window and push top stack
bool SWE_Scene::window_pushtop(LuaState & ll, const Window & win)
{
    // LuaStateValidator(ll, dinamic results);

    // stack: ..., swe_window
    if(! ll.pushTable("SWE.Scene.Windows").isTopTable())
    {
	ERROR("table not found" << ": " << "swe.scene.windows");
	ll.stackPop();
	return false;
    }

    std::string label = String::hex(win.id());
    ll.getFieldTableIndex(label, -1, false);

    if(ll.isTopNil())
    {
	//ERROR("window not found" << ": " << label);
	ll.stackPop(2);
	return false;
    }

    // stack: ..., SWE.Scene.Windows, swe_window
    ll.stackRemoveIndex(-2);
    return true;
}

/// push top stack swe_window to scene
bool SWE_Scene::window_add(LuaState & ll, const std::string & label, bool isroot)
{
    LuaStateValidator(ll, 0);

    // check: swe_window
    if(! ll.pushTable("SWE.Scene.Windows").isTopTable())
    {
	ERROR("table not found" << ": " << "swe.scene.windows");
	ll.stackPop();
	return false;
    }

    if(isroot)
    {
	// save ref: to SWE.Scene.display
	if(ll.pushTable("SWE.Scene").isTopTable())
	{
	    // stack: swe_window, SWE.Scene.Windows, SWE.Scene
	    if(ll.getFieldTableIndex("display", -1, false).isTopTable())
	    {
		// remove old display from SWE.Scene.Window
		std::string hexid = ll.popFieldTableIndex("hexid", -1);
		ll.pushNil().setFieldTableIndex(hexid, -4);
		DEBUGN("display ref: " << "remove", 2);
	    }
	    ll.stackPop();

	    // set swe_window to SWE.Scene.display
	    ll.pushValueIndex(-3).setFieldTableIndex("display", -2);
	    DEBUGN("display ref: " << "set" << ": " << label, 2);

	    // clean old display references
	    ll.garbageCollect();
	}

	// remove SWE.Scene
	ll.stackPop();
    }

    // add swe_window to SWE.Scene.Windows[label]
    ll.pushValueIndex(-2).setFieldTableIndex(label, -2);

    // remove SWE.Scene.Windows
    ll.stackPop();

    DEBUGN("add window" << ": " << label << (isroot ? " (display)" : ""), 2);
    return true;
}

void SWE_Scene::clean(LuaState & ll, bool saveDisplay)
{
    LuaStateValidator(ll, 0);

    DisplayScene::sceneDestroy();

    if(ll.pushTable("SWE.Scene.Windows").isTopTable())
    {
	// iterate SWE.Scene.Windows: remove all items
	ll.pushNil();
	while(ll.nextTableIndex(-2))
	{
	    std::string hexid = ll.toStringIndex(-2);

	    // pop value
	    ll.stackPop();
	    DEBUGN("window id: " << hexid, 2);

	    // stack: ..., SWE.Scene.Window, key,
	    ll.pushNil().setFieldTableIndex(hexid, -3);
	}

	if(ll.pushTable("SWE.Scene").isTopTable())
	{
	    // remove SWE.Scene.display
	    if(! saveDisplay)
	    {
		DEBUGN("display ref: " << "remove", 2);
		// remove old display from SWE.Scene
		ll.pushNil().setFieldTableIndex("display", -2);
	    }
	    else
	    {
		// add SWE.Scene.display to SWE.Scene.Windows
		if(ll.getFieldTableIndex("display", -1).isTopTable())
		{
		    auto win = SWE_Window::get(ll, -1, __FUNCTION__);
		    if(win)
		    {
			DisplayScene::addItem(*win);
			// add SWE.Scene.Window hexid
			std::string hexid = String::hex(win->id());
			ll.setFieldTableIndex(hexid, -3);
			DEBUGN("display ref: " << "set" << ": " << hexid, 2);
		    }
		    else
		    {
			ll.stackPop();
			ERROR("userdata empty");
		    }
		}
		else
	        {
		    ll.stackPop();
		}
	    }
	}
	else
	{
	    ERROR("table not found" << ": " << "swe.scene");
	}

	// SWE.Scene
	ll.stackPop();

	// SWE.Scene.Windows mode: weak values
	ll.garbageCollect();
    }
    else
    {
	ERROR("table not found" << ": " << "swe.scene.windows");
    }

    // SWE.Scene.Windows
    ll.stackPop();
}

const struct luaL_Reg SWE_window_functions[] = {
    { "MarkDestroyed",    SWE_window_set_destroyed },  // [void], table window
    { "SetVisible",     SWE_window_set_visible },      // [void], table window, bool flag
    { "SetResultCode",  SWE_window_set_result },       // [void], table window, int code
    { "SetModality",    SWE_window_set_modality },     // [void], table window, int code
    { "SetKeyHandle",   SWE_window_set_keyhandle },    // [void], table window, int code
    { "SetMouseTracking", SWE_window_set_mousetrack }, // [void], table window, int code
    { "SetPosition",    SWE_window_set_position },     // [void], table window. point pos
    { "SetSize",        SWE_window_set_size },         // [void], table window. size win
    { "SetToolTip",	SWE_window_set_tooltip },      // [void], table window, string, fontrender, color, color, color
    { "RenderClear",    SWE_window_render_clear },     // [void], table window, enum: color
    { "RenderRect",     SWE_window_render_rect },      // [void], table window, enum: color, rect, bool
    { "RenderLine",     SWE_window_render_line },      // [void], table window, enum: color, point, point
    { "RenderCyrcle",   SWE_window_render_cyrcle },    // [void], table window, enum: color, point, int, bool
    { "RenderPoint",    SWE_window_render_point },     // [void], table window, enum: color, point
    { "RenderTexture",  SWE_window_render_texture },   // [void]. table window, table texture, rect, point
    { "RenderText",     SWE_window_render_text },      // [rect coords], table window, table fontrender, string, color, point, orientation, render, style, hinting
    { "PointInArea",	SWE_window_point_inarea },     // [bool], table window, int, int
    { "IsVisible",	SWE_window_is_visible },       // [bool], table window
    { "GetChildrens",	SWE_window_childrens },        // [table], table window
    { "ToJson",		SWE_window_to_json },          // [string], table window
    // virtual
    { "TextureInvalidEvent",NULL },
    { "WindowCreateEvent", NULL },
    { "WindowCloseEvent",  NULL },
    { "DisplayResizeEvent",NULL },
    { "MousePressEvent",   NULL },
    { "MouseReleaseEvent", NULL },
    { "MouseClickEvent",   NULL },
    { "MouseFocusEvent",   NULL },
    { "MouseMotionEvent",  NULL },
    { "KeyPressEvent",     NULL },
    { "KeyReleaseEvent",   NULL },
    { "ScrollUpEvent",     NULL },
    { "ScrollDownEvent",   NULL },
    { "SystemUserEvent",   NULL },
    { "SystemTickEvent",   NULL },
    { "RenderWindow",      NULL },
    { NULL, NULL }
};

void SWE_Stack::window_create(LuaState & ll, const Point & pos, const Size & wsz, Window* parent)
{
    LuaStateValidator(ll, 1);

    ll.pushTable();

    // userdata
    ll.pushString("userdata");
    auto ptr = static_cast<SWE_Window**>(ll.pushUserData(sizeof(SWE_Window*)));

    ll.setTableIndex(-3);
    *ptr = new SWE_Window(ll.L(), pos, wsz, parent);

    // set metatable: __gc to table
    ll.pushTable(0, 1);
    ll.pushFunction(SWE_window_destroy).setFieldTableIndex("__gc", -2);
    ll.setMetaTableIndex(-2);

    const std::string hexid = String::hex((*ptr)->id());
    const std::string parid = parent ? String::hex(parent->id()) : "0";

    // add values
    ll.pushString("__type").pushString("swe.window").setTableIndex(-3);
    ll.pushString("posx").pushInteger((*ptr)->position().x).setTableIndex(-3);
    ll.pushString("posy").pushInteger((*ptr)->position().y).setTableIndex(-3);
    ll.pushString("width").pushInteger((*ptr)->width()).setTableIndex(-3);
    ll.pushString("height").pushInteger((*ptr)->height()).setTableIndex(-3);
    ll.pushString("visible").pushBoolean((*ptr)->isVisible()).setTableIndex(-3);
    ll.pushString("modality").pushBoolean(false).setTableIndex(-3);
    ll.pushString("keyhandle").pushBoolean(false).setTableIndex(-3);
    ll.pushString("hexid").pushString(hexid).setTableIndex(-3);

    if(parent)
    {
	ll.pushString("parent");
	SWE_Scene::window_pushtop(ll, *parent);
	ll.setTableIndex(-3);
    }

    // set functions
    ll.setFunctionsTableIndex(SWE_window_functions, -1);

    DEBUGN(String::pointer(ptr) << ": [" << (*ptr)->toString() << "]", 2);

    SWE_Scene::window_add(ll, hexid, ! parent);
}

int SWE_window_create(lua_State* L)
{
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    int params = ll.stackSize();
    Point winpos;
    Size winsz;
    Window* parent = NULL;

    // self, x, y, w, h, parent
    if(5 <= params)
    {
	winpos.x = ll.toIntegerIndex(2);
	winpos.y = ll.toIntegerIndex(3);
	winsz.w = ll.toIntegerIndex(4);
	winsz.h = ll.toIntegerIndex(5);
    }
    else
    // self, {x, y}, {w, h}, parent
    // if(3 <= params)
    {
	winpos = SWE_Point::get(ll, 2, __FUNCTION__);
	winsz = SWE_Size::get(ll, 3, __FUNCTION__);
    }

    // get parent
    if(ll.isTopTable())
	parent = SWE_Window::get(ll, -1, __FUNCTION__);
    else
    // set parent DisplayWindow
	parent = DisplayScene::rootWindow();

    SWE_Stack::window_create(ll, winpos, winsz, parent);
    return rescount;
}

int SWE_window_destroy(lua_State* L)
{
    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    if(ll.isTopTable())
    {
	SWE_Window** ptr = NULL;

	if(ll.getFieldTableIndex("userdata", -1).isTopUserData())
	    ptr = static_cast<SWE_Window**>(ll.getTopUserData());

	// stack: swe_window, userdata
	ll.stackPop();

	if(ptr && *ptr)
	{
	    // stack: swe_window
	    if(ll.getFieldTableIndex("WindowCloseEvent", -1, false).isTopFunction())
	    {
		ll.callFunction(0, 0);
	    }
	    else
	    {
		ll.stackPop();
	    }

	    DEBUGN(String::pointer(ptr) << ": [" << String::pointer(*ptr) << "]", 2);
	    // SWE.Scene.Windows mode: weak value
	    // auto remove SWE_Scene::window_remove(ll, *ptr);

	    delete *ptr;
	    *ptr = NULL;
	}
	else
	{
	    ERROR("userdata empty");
	}
    }
    else
    {
	ERROR("not userdata");
    }

    return rescount;
}

// SWE_Polygon
int SWE_polygon_to_json(lua_State* L)
{
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    // params: swe_polygon

    if(auto poly = static_cast<SWE_Polygon*>(SWE_Polygon::get(ll, 1, __FUNCTION__)))
    {
	SWE_window_to_json(ll.L());

	if(ll.isTopString())
	{
	    std::string window = ll.getTopString();
	    ll.stackPop(1);

	    std::ostringstream os;
    	    os << "{\"type\":\"swe.polygon\",\"window\":" << window << ",\"points\":" <<
		JsonPack::points(poly->getAreaPoints()).toString() << "}";

	    ll.pushString(os.str());
	}
    }
    else
    {
	ERROR("userdata empty");
	ll.pushNil();
    }

    return rescount;
}

int SWE_polygon_points1(lua_State* L)
{
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(auto poly = static_cast<SWE_Polygon*>(SWE_Polygon::get(ll, 1, __FUNCTION__)))
    {
	auto points = poly->getBoundaryPoints();
	ll.pushTable(points.size(), 0);

	for(size_t seqIndex = 0; seqIndex < points.size(); ++seqIndex)
	{
	    Point pt = points[seqIndex] - poly->position();
	    SWE_Stack::point_create(ll, pt.x, pt.y);
	    ll.setIndexTableIndex(seqIndex + 1, -2);
	}
    }
    else
    {
        ERROR("userdata empty");
        ll.pushNil();
    }
    
    return rescount;
}

int SWE_polygon_points2(lua_State* L)
{
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(auto poly = static_cast<SWE_Polygon*>(SWE_Polygon::get(ll, 1, __FUNCTION__)))
    {
	auto points = poly->getAreaPoints();
	ll.pushTable(points.size(), 0);

	for(size_t seqIndex = 0; seqIndex < points.size(); ++seqIndex)
	{
	    Point pt = points[seqIndex] - poly->position();
	    SWE_Stack::point_create(ll, pt.x, pt.y);
	    ll.setIndexTableIndex(seqIndex + 1, -2);
	}
    }
    else
    {
        ERROR("userdata empty");
        ll.pushNil();
    }
    
    return rescount;
}

const struct luaL_Reg SWE_polygon_functions[] = {
    // window func
    { "MarkDestroyed",   SWE_window_set_destroyed },   // [void], table window
    { "SetVisible",     SWE_window_set_visible },      // [void], table window, bool flag
    { "SetResultCode",  SWE_window_set_result },       // [void], table window, int code
    { "SetModality",    SWE_window_set_modality },     // [void], table window, int code
    { "SetKeyHandle",   SWE_window_set_keyhandle },    // [void], table window, int code
    { "SetMouseTracking", SWE_window_set_mousetrack }, // [void], table window, int code
    { "SetPosition",    SWE_window_set_position },     // [void], table window. point pos
    { "SetSize",        SWE_window_set_size },         // [void], table window. size win
    { "SetToolTip",	SWE_window_set_tooltip },      // [void], table window, string, fontrender, color, color, color
    { "RenderClear",    SWE_window_render_clear },     // [void], table window, enum: color
    { "RenderRect",     SWE_window_render_rect },      // [void], table window, enum: color, rect, bool
    { "RenderLine",     SWE_window_render_line },      // [void], table window, enum: color, point, point
    { "RenderCyrcle",   SWE_window_render_cyrcle },    // [void], table window, enum: color, point, int, bool
    { "RenderPoint",    SWE_window_render_point },     // [void], table window, enum: color, point
    { "RenderTexture",  SWE_window_render_texture },   // [void]. table window, table texture, rect, rect
    { "RenderText",     SWE_window_render_text },      // [rect coords], table window, table fontrender, string, color, point
    { "PointInArea",	SWE_window_point_inarea },     // [bool], table window, int, int
    { "ToJson",		SWE_polygon_to_json },          // [string], table polygon
    // window virtual
    { "TextureInvalidEvent",NULL },
    { "WindowCreateEvent", NULL },
    { "WindowCloseEvent",  NULL },
    { "DisplayResizeEvent",NULL },
    { "MousePressEvent",   NULL },
    { "MouseReleaseEvent", NULL },
    { "MouseClickEvent",   NULL },
    { "MouseFocusEvent",   NULL },
    { "MouseMotionEvent",  NULL },
    { "KeyPressEvent",     NULL },
    { "KeyReleaseEvent",   NULL },
    { "ScrollUpEvent",     NULL },
    { "ScrollDownEvent",   NULL },
    { "SystemUserEvent",   NULL },
    { "SystemTickEvent",   NULL },
    { "RenderWindow",      NULL },
    // polygon func
    { "GetBoundaryPoints", SWE_polygon_points1 },	// [table points], table polygon
    { "GetAreaPoints", SWE_polygon_points2 },		// [table points], table polygon
    { NULL, NULL }
};

SWE_Polygon::SWE_Polygon(lua_State* L, const Points & pts, Window* parent)
    : SWE_Window(L, parent)
{
    Polygon poly(pts);
    Rect area = poly.around();

    setSize(area);
    setPosition(area);

    fillPoints(poly);
    setVisible(true);
}

enum { StatusNew = 0, StatusOpen = 1, StatusClosed = 2 };

/// \cond StatusPoint
struct StatusPoint
{
    int val;

    StatusPoint(int v = StatusNew) : val(v) {}

    bool checkOpen(void) const { return val == StatusOpen; }
    void setOpen(void) { val = StatusOpen; }
    void setClosed(void) { val = StatusClosed; }
};

void SWE_Polygon::fillPoints(const Polygon & poly)
{
    const Rect & area = Window::area();
    Point pt = Point(area.x + area.w / 2, area.y + area.h / 2);

    swe_unordered_map<Point, StatusPoint, PointHasher> status;

    for(auto & pt : poly)
	status[pt].setClosed();

    if(poly & pt)
    {
	Points arounds;
	status[pt].setOpen();
	bool loop = true;

	while(loop)
	{
	    for(auto it = status.begin(); it != status.end(); ++it)
		if((*it).second.checkOpen())
	    {
		const Point & cpt = (*it).first;
		(*it).second = StatusPoint(StatusClosed);

		Point npt;
		int direction = 0;

		// top
		npt = cpt + Point(0, -1);
		if(status.end() == status.find(npt)) { direction |= 0x01; arounds.push_back(npt); }
		// left
		npt = cpt + Point(1, 0);
		if(status.end() == status.find(npt)) { direction |= 0x02; arounds.push_back(npt); }
		// bottom
		npt = cpt + Point(0, 1);
		if(status.end() == status.find(npt)) { direction |= 0x04; arounds.push_back(npt); }
		// right
		npt = cpt + Point(-1, 0);
		if(status.end() == status.find(npt)) { direction |= 0x08; arounds.push_back(npt); }

		// top left
		npt = cpt + Point(1, -1);
		if(0x03 == (direction & 0x03) && status.end() == status.find(npt))
		    arounds.push_back(npt);

		// top right
		npt = cpt + Point(-1, -1);
		if(0x09 == (direction & 0x09) && status.end() == status.find(npt))
		    arounds.push_back(npt);
		
		// bottom left
		npt = cpt + Point(1, 1);
		if(0x06 == (direction & 0x06) && status.end() == status.find(npt))
		    arounds.push_back(npt);

		// top right
		npt = cpt + Point(-1, 1);
		if(0x0c == (direction & 0x0c) && status.end() == status.find(npt))
		    arounds.push_back(npt);
	    }

	    loop = false;
	    for(auto at = arounds.begin(); at != arounds.end(); ++at)
	    {
		status[*at].setOpen();
		loop = true;
	    }
    
	    arounds.clear();
	}
    }
    else
    {
	FIXME("start point not found...");
    }

    for(auto it = status.begin(); it != status.end(); ++it)
	if(! (*it).second.checkOpen()) areaPoints.insert((*it).first - position());
}

void SWE_Polygon::renderClear(const Color & col)
{
    for(auto & pt : areaPoints)
	renderPoint(col, pt);
}

void SWE_Polygon::renderWindow(void)
{
    bool extrender = false;

    if(SWE_Scene::window_pushtop(ll, *this))
    {
        if(ll.getFieldTableIndex("RenderWindow", -1).isTopFunction())
        {
            extrender = ll.callFunction(0, 1).getTopBoolean();
            ll.stackPop();
        }
        else
        {
            ll.stackPop();
        }
 
        ll.stackPop();
    }

    if(! extrender && Engine::debugMode())
    {
        for(auto & pt : areaPoints)
	    renderPoint(Color::Red, pt);
    }
}

bool SWE_Polygon::isAreaPoint(const Point & pos) const
{
    if(Window::isAreaPoint(pos))
	return areaPoints.end() != areaPoints.find(pos - position());

    return false;
}

Points SWE_Polygon::getBoundaryPoints(void) const
{
    const Rect & rect = Window::area();
    Points res;

    for(auto & pt : areaPoints)
    {
        if(0 == pt.x || 0 == pt.y ||
            rect.w - 1 == pt.x || rect.h - 1 == pt.y)
        {
	    res.push_back(pt + rect.toPoint());
        }
        else
        {
	    auto lf = pt - SWE::Point(1, 0);
	    auto rt = pt + SWE::Point(1, 0);
	    auto tp = pt - SWE::Point(0, 1);
	    auto bt = pt + SWE::Point(0, 1);

	    if(areaPoints.end() == areaPoints.find(lf))
		res.push_back(lf + rect.toPoint());

	    if(areaPoints.end() == areaPoints.find(tp))
		res.push_back(tp + rect.toPoint());

	    if(rect.w > rt.x)
	    {
		if(areaPoints.end() == areaPoints.find(rt))
		    res.push_back(rt + rect.toPoint());
	    }

	    if(rect.h > bt.y)
	    {
		if(areaPoints.end() == areaPoints.find(bt))
		    res.push_back(bt + rect.toPoint());
	    }
	}
    }

    return res;
}

Points SWE_Polygon::getAreaPoints(void) const
{
    Points res;

    for(auto & pt : areaPoints)
	res.push_back(pt + position());

    return res;
}

void SWE_Polygon::includeRegion(const Points & pts)
{
    Rect area1 = pts.around();
    Point offset = area1.toPoint() - position();

    for(auto & pt : pts)
	areaPoints.insert(pt + offset);

    Rect area2 = getAreaPoints().around();

    setSize(area2);
    setPosition(area2);
}

void SWE_Polygon::excludeRegion(const Points & pts)
{
    Rect area1 = pts.around();
    Point offset = area1.toPoint() - position();

    for(auto & pt : pts)
	areaPoints.erase(pt + offset);

    Rect area2 = getAreaPoints().around();

    setSize(area2);
    setPosition(area2);
}

int SWE_polygon_create(lua_State* L)
{
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    // SWE.Polygon(self, x1, y1, x2, y2, ... [xN, yN], table parent)
    
    int params = ll.stackSize();
    Window* parent = NULL;

    // get parent
    if(ll.isTopTable())
	parent = SWE_Window::get(ll, -1, __FUNCTION__);
    else
    // set parent DisplayWindow
	parent = DisplayScene::rootWindow();

    Points points;
    for(int it = 2; it < params; it += 2)
    {
	int px = ll.toIntegerIndex(it);
	if(it < params)
	{
	    int py = ll.toIntegerIndex(it + 1);
	    points.emplace_back(px, py);
	}
	else
	{
	    ERROR("incorrect points size");
	}
    }

    ll.pushTable();

    // userdata
    ll.pushString("userdata");
    auto ptr = static_cast<SWE_Polygon**>(ll.pushUserData(sizeof(SWE_Polygon*)));
    *ptr = new SWE_Polygon(L, points, parent);

    ll.setTableIndex(-3);
    // set metatable: __gc to table
    ll.pushTable(0, 1);
    ll.pushFunction(SWE_polygon_destroy).setFieldTableIndex("__gc", -2);
    ll.setMetaTableIndex(-2);

    const std::string hexid = String::hex((*ptr)->id());
    const std::string parid = parent ? String::hex(parent->id()) : "0";

    ll.pushString("__type").pushString("swe.polygon").setTableIndex(-3);
    ll.pushString("posx").pushInteger((*ptr)->position().x).setTableIndex(-3);
    ll.pushString("posy").pushInteger((*ptr)->position().y).setTableIndex(-3);
    ll.pushString("width").pushInteger((*ptr)->width()).setTableIndex(-3);
    ll.pushString("height").pushInteger((*ptr)->height()).setTableIndex(-3);
    ll.pushString("visible").pushBoolean((*ptr)->isVisible()).setTableIndex(-3);
    ll.pushString("modality").pushBoolean(false).setTableIndex(-3);
    ll.pushString("keyhandle").pushBoolean(false).setTableIndex(-3);
    ll.pushString("hexid").pushString(hexid).setTableIndex(-3);

    if(parent)
    {
	ll.pushString("parent");
	SWE_Scene::window_pushtop(ll, *parent);
	ll.setTableIndex(-3);
    }

    // set functions
    ll.setFunctionsTableIndex(SWE_polygon_functions, -1);

    DEBUGN(String::pointer(ptr) << ": [" << String::pointer(*ptr) << "]", 2);

    SWE_Scene::window_add(ll,  hexid, !parent);

    return rescount;
}

int SWE_polygon_destroy(lua_State* L)
{
    return SWE_window_destroy(L);
}

void SWE_Scene::registers(LuaState & ll)
{
    LuaStateValidator(ll, 0);

    // SWE.Scene
    ll.pushTable("SWE.Scene");
    ll.pushFunction(SWE_scene_removetop).setFieldTableIndex("Remove", -2);
    ll.pushFunction(SWE_scene_move_toplayer).setFieldTableIndex("MoveTop", -2);

    // SWE.Scene.Windows
    ll.pushTable("SWE.Scene.Windows");
    // set metatable: weak mode
    ll.pushTable(0, 1).pushString("v").setFieldTableIndex("__mode", -2);
    ll.setMetaTableIndex(-2).stackPop();

    ll.stackPop();
}

void SWE_Window::registers(LuaState & ll)
{
    LuaStateValidator(ll, 0);

    // SWE.Window
    ll.pushTable("SWE.Window");
    // set metatable: __call
    ll.pushTable(0, 1).pushFunction(SWE_window_create).setFieldTableIndex("__call", -2);
    ll.setMetaTableIndex(-2).stackPop();

    // SWE.Button constants
    ll.pushTable("SWE.MouseButton");
    ll.pushInteger(SDL_BUTTON(SDL_BUTTON_LEFT)).setFieldTableIndex("Left", -2);
    ll.pushInteger(SDL_BUTTON(SDL_BUTTON_MIDDLE)).setFieldTableIndex("Middle", -2);
    ll.pushInteger(SDL_BUTTON(SDL_BUTTON_RIGHT)).setFieldTableIndex("Right", -2);
    ll.pushInteger(SDL_BUTTON(SDL_BUTTON_X1)).setFieldTableIndex("X1", -2);
    ll.pushInteger(SDL_BUTTON(SDL_BUTTON_X2)).setFieldTableIndex("X2", -2);
    ll.stackPop();
}

void SWE_Polygon::registers(LuaState & ll)
{
    LuaStateValidator(ll, 0);

    // SWE.Polygon
    ll.pushTable("SWE.Polygon");
    // set metatable: __call
    ll.pushTable(0, 1).pushFunction(SWE_polygon_create).setFieldTableIndex("__call", -2);
    ll.setMetaTableIndex(-2).stackPop();
}

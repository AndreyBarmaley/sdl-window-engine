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

#ifndef _SWE_LUA_WINDOW_
#define _SWE_LUA_WINDOW_

#include <set>
#include "SWE_global.h"

struct lua_State;

void SWE_window_create_event(LuaState &, Window &);
void SWE_window_move_event(LuaState &, Window &, const Point &);
void SWE_window_resize_event(LuaState &, Window &, const Size &);
void SWE_window_visible_event(LuaState &, Window &, bool f);
void SWE_display_focus_event(LuaState &, Window &, bool f);
void SWE_display_resize_event(LuaState &, Window &, const Size &);
void SWE_texture_invalid_event(LuaState &, Window &);
bool SWE_mouse_press_event(LuaState &, Window &, const ButtonEvent &);
bool SWE_mouse_release_event(LuaState &, Window &, const ButtonEvent &);
bool SWE_mouse_motion_event(LuaState &, Window &, const Point &, u32);
void SWE_mouse_tracking_event(LuaState &, Window &, const Point &, u32);
bool SWE_mouse_click_event(LuaState &, Window &, const ButtonsEvent &);
void SWE_mouse_focus_event(LuaState &, Window &);
void SWE_mouse_leave_event(LuaState &, Window &);
bool SWE_key_press_event(LuaState &, Window &, const KeySym &);
bool SWE_key_release_event(LuaState &, Window &, const KeySym &);
bool SWE_scroll_up_event(LuaState &, Window &);
bool SWE_scroll_down_event(LuaState &, Window &);
bool SWE_text_input_event(LuaState &, Window &, const std::string &);
bool SWE_system_user_event(LuaState &, Window &, int, void*);
void SWE_system_tick_event(LuaState &, Window &, u32);
void SWE_system_render_event(LuaState &, Window &, u32);
void SWE_window_render(LuaState &, Window &);

int SWE_window_set_destroyed(lua_State* L);
int SWE_window_set_visible(lua_State* L);
int SWE_window_set_result(lua_State* L);
int SWE_window_set_modality(lua_State* L);
int SWE_window_set_position(lua_State* L);
int SWE_window_set_keyhandle(lua_State* L);
int SWE_window_point_inarea(lua_State* L);
int SWE_window_render_texture(lua_State* L);

class SWE_Window : public WindowToolTipArea
{
protected:
    LuaState	ll;

    void	windowCreateEvent(void) override { SWE_window_create_event(ll, *this); }
    void	windowMoveEvent(const Point & pt) override { SWE_window_move_event(ll, *this, pt); }
    void	windowResizeEvent(const Size & sz) override { SWE_window_resize_event(ll, *this, sz); }
    void	windowVisibleEvent(bool f) override { SWE_window_visible_event(ll, *this, f); }
    void	textureInvalidEvent(void) override { SWE_texture_invalid_event(ll, *this); }
    void	displayResizeEvent(const Size & sz) override { SWE_display_resize_event(ll, *this, sz); }
    void	displayFocusEvent(bool f) override { SWE_display_focus_event(ll, *this, f); }
    bool	mousePressEvent(const ButtonEvent & be) override { return SWE_mouse_press_event(ll, *this, be); }
    bool	mouseReleaseEvent(const ButtonEvent & be) override { return SWE_mouse_release_event(ll, *this, be); }
    bool	mouseMotionEvent(const Point & pos, u32 buttons) override { return SWE_mouse_motion_event(ll, *this, pos, buttons); }
    void	mouseTrackingEvent(const Point & pos, u32 buttons) override { SWE_mouse_tracking_event(ll, *this, pos, buttons); }
    bool	mouseClickEvent(const ButtonsEvent & be) override { return SWE_mouse_click_event(ll, *this, be); }
    void        mouseFocusEvent(void) override { SWE_mouse_focus_event(ll, *this); }
    void        mouseLeaveEvent(void) override { SWE_mouse_leave_event(ll, *this); }
    bool	keyPressEvent(const KeySym & ks) override { return SWE_key_press_event(ll, *this, ks); }
    bool	keyReleaseEvent(const KeySym & ks) override { return SWE_key_release_event(ll, *this, ks); }
    bool        scrollUpEvent(void) override { return SWE_scroll_up_event(ll, *this); }
    bool        scrollDownEvent(void) override { return SWE_scroll_down_event(ll, *this); }
    bool        userEvent(int code, void* data) override { return SWE_system_user_event(ll, *this, code, data); }
    void        tickEvent(u32 ms) override { SWE_system_tick_event(ll, *this, ms); }
    bool	textInputEvent(const std::string & text) override { return SWE_text_input_event(ll, *this, text); }
    void        renderPresentEvent(u32 ms) override { SWE_system_render_event(ll, *this, ms); }

public:
    SWE_Window(lua_State*, Window* parent);
    SWE_Window(lua_State*, const Point &, const Size &, Window* parent);

    void        renderWindow(void) override { SWE_window_render(ll, *this); }

    static SWE_Window* get(LuaState &, int tableIndex, const char* funcName);
    static void registers(LuaState &);
};

/// \cond PointComp
struct PointComp
{
    bool operator() (const Point & lhs, const Point & rhs) const
    {
	return lhs.y < rhs.y || (lhs.y == rhs.y && (lhs.x < rhs.x));
    }
};

/// \cond PointHasher
struct PointHasher
{
    size_t operator() (const Point & pt) const
    {
        return std::hash<u32>()((pt.y << 16) | pt.x);
    }
};

class SWE_Polygon : public SWE_Window
{
protected:
    swe_unordered_set<Point, PointHasher> areaPoints;

    void	fillPoints(const Polygon &);
    Rect	aroundArea(void) const;

public:
    SWE_Polygon(lua_State*, const Points &, Window* parent);

    void        renderWindow(void) override;
    void        renderClear(const Color &) override;
    bool        isAreaPoint(const Point &) const override;

    Points	getAreaPoints(void) const;
    Points	getBoundaryPoints(void) const;

    void	includeRegion(const Points &);
    void	excludeRegion(const Points &);

    static SWE_Polygon* get(LuaState &, int tableIndex, const char* funcName);
    static void registers(LuaState &);
};

namespace SWE_Stack
{
    void	window_create(LuaState &, const Point &, const Size &, Window* parent);
}

namespace SWE_Scene
{
    bool	window_add(LuaState &, const std::string &, bool isroot);
    bool	window_pushtop(LuaState &, const Window &);

    void	clean(LuaState &, bool saveDisplay);
    void	registers(LuaState &);
}

#endif

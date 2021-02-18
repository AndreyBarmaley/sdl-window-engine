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

#include "SWE_rect.h"

int SWE_point_create(lua_State*);
int SWE_size_create(lua_State*);
int SWE_rect_create(lua_State*);

SDL_Rect SDLRect(int x, int y, int w, int h)
{
    SDL_Rect rt;
    rt.x = x; rt.y = y; rt.w = w; rt.h = h;
    return rt;
}

Point SWE_Point::get(LuaState & ll, int tableIndex, const char* funcName)
{
    LuaStateValidator(ll, 0);
    Point res;

    if(! ll.isTableIndex(tableIndex))
    {
        ERROR(funcName << ": " << "table not found, index: " << tableIndex);
        return res;
    }

    res.x = ll.getFieldTableIndex("posx", tableIndex).getTopInteger();
    res.y = ll.getFieldTableIndex("posy", tableIndex).getTopInteger();
 
    ll.stackPop(2);

    return res;
}

Size SWE_Size::get(LuaState & ll, int tableIndex, const char* funcName)
{
    LuaStateValidator(ll, 0);
    Size res;

    if(! ll.isTableIndex(tableIndex))
    {
        ERROR(funcName << ": " << "table not found, index: " << tableIndex);
        return res;
    }

    res.w = ll.getFieldTableIndex("width", tableIndex).getTopInteger();
    res.h = ll.getFieldTableIndex("height", tableIndex).getTopInteger();
 
    ll.stackPop(2);

    return res;
}

Rect SWE_Rect::get(LuaState & ll, int tableIndex, const char* funcName)
{
    LuaStateValidator(ll, 0);
    Rect res;

    if(! ll.isTableIndex(tableIndex))
    {
        ERROR(funcName << ": " << "table not found, index: " << tableIndex);
        return res;
    }

    res.x = ll.getFieldTableIndex("posx", tableIndex).getTopInteger();
    res.y = ll.getFieldTableIndex("posy", tableIndex).getTopInteger();
    res.w = ll.getFieldTableIndex("width", tableIndex).getTopInteger();
    res.h = ll.getFieldTableIndex("height", tableIndex).getTopInteger();

    ll.stackPop(4);

    return res;
}

/////////////////////////////////////////////////////////////////////
int SWE_rect_point_inrect(lua_State* L)
{
    // params: swe_rect, swe.point
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(ll.isTableIndex(1) && 0 == ll.popFieldTableIndex("__type", 1).compare("swe.rect") &&
	ll.isTableIndex(2) && 0 == ll.popFieldTableIndex("__type", 2).compare("swe.point"))
    {
	Rect rt; Point pt;

	rt.x = ll.getFieldTableIndex("posx", 1).getTopInteger();
	rt.y = ll.getFieldTableIndex("posy", 1).getTopInteger();
	rt.w = ll.getFieldTableIndex("width", 1).getTopInteger();
	rt.h = ll.getFieldTableIndex("height", 1).getTopInteger();
	ll.stackPop(4);

	pt.x = ll.getFieldTableIndex("posx", 2).getTopInteger();
	pt.y = ll.getFieldTableIndex("posy", 2).getTopInteger();
	ll.stackPop(2);

	bool res = rt & pt;
	ll.pushBoolean(res);
    }
    else
    {
        ERROR("userdata empty");
	ll.pushBoolean(false);
    }

    return rescount;
}

int SWE_rect_equals(lua_State* L)
{
    // params: swe_rect, swe.rect
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(ll.isTableIndex(1) && 0 == ll.popFieldTableIndex("__type", 1).compare("swe.rect") &&
	ll.isTableIndex(2) && 0 == ll.popFieldTableIndex("__type", 2).compare("swe.rect"))
    {
	Rect rt1, rt2;

	rt1.x = ll.getFieldTableIndex("posx", 1).getTopInteger();
	rt1.y = ll.getFieldTableIndex("posy", 1).getTopInteger();
	rt1.w = ll.getFieldTableIndex("width", 1).getTopInteger();
	rt1.h = ll.getFieldTableIndex("height", 1).getTopInteger();
	ll.stackPop(4);

	rt2.x = ll.getFieldTableIndex("posx", 2).getTopInteger();
	rt2.y = ll.getFieldTableIndex("posy", 2).getTopInteger();
	rt2.w = ll.getFieldTableIndex("width", 2).getTopInteger();
	rt2.h = ll.getFieldTableIndex("height", 2).getTopInteger();
	ll.stackPop(4);

	bool res = rt1 == rt2;
	ll.pushBoolean(res);
    }
    else
    {
        ERROR("userdata empty");
        ll.pushBoolean(false);
    }

    return rescount;
}

int SWE_rect_has_intersection(lua_State* L)
{
    // params: swe_rect, swe.rect
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(ll.isTableIndex(1) && 0 == ll.popFieldTableIndex("__type", 1).compare("swe.rect") &&
	ll.isTableIndex(2) && 0 == ll.popFieldTableIndex("__type", 2).compare("swe.rect"))
    {
	Rect rt1, rt2;

	rt1.x = ll.getFieldTableIndex("posx", 1).getTopInteger();
	rt1.y = ll.getFieldTableIndex("posy", 1).getTopInteger();
	rt1.w = ll.getFieldTableIndex("width", 1).getTopInteger();
	rt1.h = ll.getFieldTableIndex("height", 1).getTopInteger();
	ll.stackPop(4);

	rt2.x = ll.getFieldTableIndex("posx", 2).getTopInteger();
	rt2.y = ll.getFieldTableIndex("posy", 2).getTopInteger();
	rt2.w = ll.getFieldTableIndex("width", 2).getTopInteger();
	rt2.h = ll.getFieldTableIndex("height", 2).getTopInteger();
	ll.stackPop(4);

	bool res = rt1 & rt2;
	ll.pushBoolean(res);
    }
    else
    {
        ERROR("userdata empty");
        ll.pushBoolean(false);
    }

    return rescount;
}

int SWE_rect_intersect(lua_State* L)
{
    // params: swe_rect, swe.rect
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(ll.isTableIndex(1) && 0 == ll.popFieldTableIndex("__type", 1).compare("swe.rect") &&
	ll.isTableIndex(2) && 0 == ll.popFieldTableIndex("__type", 2).compare("swe.rect"))
    {
	Rect rt1, rt2, res;

	rt1.x = ll.getFieldTableIndex("posx", 1).getTopInteger();
	rt1.y = ll.getFieldTableIndex("posy", 1).getTopInteger();
	rt1.w = ll.getFieldTableIndex("width", 1).getTopInteger();
	rt1.h = ll.getFieldTableIndex("height", 1).getTopInteger();
	ll.stackPop(4);

	rt2.x = ll.getFieldTableIndex("posx", 2).getTopInteger();
	rt2.y = ll.getFieldTableIndex("posy", 2).getTopInteger();
	rt2.w = ll.getFieldTableIndex("width", 2).getTopInteger();
	rt2.h = ll.getFieldTableIndex("height", 2).getTopInteger();
	ll.stackPop(4);

	if(Rect::intersection(rt1, rt2, & res))
	    SWE_Stack::rect_create(ll, res.x, res.y, res.w, res.h);
        else
            ll.pushNil();
    }
    else
    {
        ERROR("userdata empty");
        ll.pushNil();
    }

    return rescount;
}

int SWE_rect_to_json(lua_State* L)
{
    // params: swe_rect
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(ll.isTableIndex(1) && 0 == ll.popFieldTableIndex("__type", 1).compare("swe.rect"))
    {
	int rx = ll.getFieldTableIndex("posx", 1).getTopInteger();
	int ry = ll.getFieldTableIndex("posy", 1).getTopInteger();
	int rw = ll.getFieldTableIndex("width", 1).getTopInteger();
	int rh = ll.getFieldTableIndex("height", 1).getTopInteger();
	ll.stackPop(4);

	std::string str = StringFormat("{\"type\":\"swe.rect\",\"posx\":%1,\"posy\":%2,\"width\":%3,\"height\":%4}").arg(rx).arg(ry).arg(rw).arg(rh);
	ll.pushString(str);
    }
    else
    {
        ERROR("userdata empty");
        ll.pushNil();
    }

    return rescount;
}

int SWE_rect_tostring(lua_State* L)
{
    // params: swe_rect
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(ll.isTableIndex(1) && 0 == ll.popFieldTableIndex("__type", 1).compare("swe.rect"))
    {
	int rx = ll.getFieldTableIndex("posx", 1).getTopInteger();
	int ry = ll.getFieldTableIndex("posy", 1).getTopInteger();
	int rw = ll.getFieldTableIndex("width", 1).getTopInteger();
	int rh = ll.getFieldTableIndex("height", 1).getTopInteger();

	ll.stackPop(4);
	ll.pushString(StringFormat("%1[%2,%3,%4,%5]").arg("swe.rect").arg(rx).arg(ry).arg(rw).arg(rh));
    }
    else
    {
        ERROR("userdata empty");
        ll.pushNil();
    }

    return rescount;
}

int SWE_rect_unpack(lua_State* L)
{
    // params: swe_rect
    const int rescount = 4;
    LuaStateDefine(ll, L, rescount);

    if(ll.isTableIndex(1) && 0 == ll.popFieldTableIndex("__type", 1).compare("swe.rect"))
    {
	int rx = ll.getFieldTableIndex("posx", 1).getTopInteger();
	int ry = ll.getFieldTableIndex("posy", 1).getTopInteger();
	int rw = ll.getFieldTableIndex("width", 1).getTopInteger();
	int rh = ll.getFieldTableIndex("height", 1).getTopInteger();
	ll.stackPop(4);

	ll.pushInteger(rx).pushInteger(ry).pushInteger(rw).pushInteger(rh);
    }
    else
    {
        ERROR("userdata empty");
	ll.pushNil().pushNil().pushNil().pushNil();
    }

    return rescount;
}

const struct luaL_Reg SWE_rect_functions[] = {
    { "PointInRect", SWE_rect_point_inrect },		// [bool], swe_rect, swe_point
    { "HasIntersection", SWE_rect_has_intersection },	// [bool], swe_rect, swe_rect
    { "RectEquals", SWE_rect_equals },			// [bool], swe_rect, swe_rect
    { "GetIntersectRect", SWE_rect_intersect },		// [swe_rect], swe_rect, swe_rect
    { "ToJson", SWE_rect_to_json }, 			// [string], swe_rect
    { "Unpack", SWE_rect_unpack }, 			// [list int], swe_rect
    { NULL, NULL }
};

void SWE_Stack::rect_create(LuaState & ll, int rx, int ry, int rw, int rh)
{
    LuaStateValidator(ll, 1);

    ll.pushTable();
    ll.pushString("swe.rect").setFieldTableIndex("__type", -2);
    ll.pushInteger(rx).setFieldTableIndex("posx", -2);
    ll.pushInteger(ry).setFieldTableIndex("posy", -2);
    ll.pushInteger(rw).setFieldTableIndex("width", -2);
    ll.pushInteger(rh).setFieldTableIndex("height", -2);

    // set: tostring
    ll.pushTable(0, 1);
    ll.pushFunction(SWE_rect_tostring).setFieldTableIndex("__tostring", -2);
    ll.setMetaTableIndex(-2);

    // set functions
    ll.setFunctionsTableIndex(SWE_rect_functions, -1);
}

int SWE_rect_create(lua_State* L)
{
    // empty params
    LuaState ll(L);

    int rx = ll.toIntegerIndex(2);
    int ry = ll.toIntegerIndex(3);
    int rw = ll.toIntegerIndex(4);
    int rh = ll.toIntegerIndex(5);

    SWE_Stack::rect_create(ll, rx, ry, rw, rh);
    return 1;
}

void SWE_Rect::registers(LuaState & ll)
{
    LuaStateValidator(ll, 0);

    // SWE.Rect
    ll.pushTable("SWE.Rect");
    // set metatable: __call
    ll.pushTable(0, 1).pushFunction(SWE_rect_create).setFieldTableIndex("__call", -2);
    ll.setMetaTableIndex(-2).stackPop();
}

/////////////////////////////////////////////////////////////////////
int SWE_point_to_json(lua_State* L)
{
    // params: swe_point

    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(ll.isTableIndex(1) && 0 == ll.popFieldTableIndex("__type", 1).compare("swe.point"))
    {
	int px = ll.getFieldTableIndex("posx", 1).getTopInteger();
	int py = ll.getFieldTableIndex("posy", 1).getTopInteger();
	ll.stackPop(2);

	std::string str = StringFormat("{\"type\":\"swe.point\",\"posx\":%1,\"posy\":%2}").arg(px).arg(py);
	ll.pushString(str);
    }
    else
    {
        ERROR("userdata empty");
        ll.pushNil();
    }

    return rescount;
}

int SWE_point_tostring(lua_State* L)
{
    // params: swe_point

    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(ll.isTableIndex(1) && 0 == ll.popFieldTableIndex("__type", 1).compare("swe.point"))
    {
	int px = ll.getFieldTableIndex("posx", 1).getTopInteger();
	int py = ll.getFieldTableIndex("posy", 1).getTopInteger();

	ll.stackPop(2);
	ll.pushString(StringFormat("%1[%2,%3]").arg("swe.point").arg(px).arg(py));
    }
    else
    {
        ERROR("userdata empty");
        ll.pushNil();
    }

    return rescount;
}

int SWE_point_unpack(lua_State* L)
{
    // params: swe_point

    const int rescount = 2;
    LuaStateDefine(ll, L, rescount);

    if(ll.isTableIndex(1) && 0 == ll.popFieldTableIndex("__type", 1).compare("swe.point"))
    {
	int px = ll.getFieldTableIndex("posx", 1).getTopInteger();
	int py = ll.getFieldTableIndex("posy", 1).getTopInteger();
	ll.stackPop(2);

	ll.pushInteger(px).pushInteger(py);
    }
    else
    {
        ERROR("userdata empty");
        ll.pushNil().pushNil();
    }

    return rescount;
}

const struct luaL_Reg SWE_point_functions[] = {
    { "ToJson", SWE_point_to_json }, 	// [string], swe_point
    { "Unpack", SWE_point_unpack }, 	// [list int], swe_point
    { NULL, NULL }
};

void SWE_Stack::point_create(LuaState & ll, int px, int py)
{
    LuaStateValidator(ll, 1);

    ll.pushTable();
    ll.pushString("swe.point").setFieldTableIndex("__type", -2);
    ll.pushInteger(px).setFieldTableIndex("posx", -2);
    ll.pushInteger(py).setFieldTableIndex("posy", -2);

    // set: tostring
    ll.pushTable(0, 1);
    ll.pushFunction(SWE_point_tostring).setFieldTableIndex("__tostring", -2);
    ll.setMetaTableIndex(-2);

    // set functions
    ll.setFunctionsTableIndex(SWE_point_functions, -1);
}

int SWE_point_create(lua_State* L)
{
    // empty params

    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    int px = ll.toIntegerIndex(2);
    int py = ll.toIntegerIndex(3);

    SWE_Stack::point_create(ll, px, py);
    return rescount;
}

void SWE_Point::registers(LuaState & ll)
{
    LuaStateValidator(ll, 0);

    // SWE.Point
    ll.pushTable("SWE.Point");
    // set metatable: __call
    ll.pushTable(0, 1).pushFunction(SWE_point_create).setFieldTableIndex("__call", -2);
    ll.setMetaTableIndex(-2).stackPop();
}

/////////////////////////////////////////////////////////////////////
int SWE_size_to_json(lua_State* L)
{
    // params: swe_size

    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(ll.isTableIndex(1) && 0 == ll.popFieldTableIndex("__type", 1).compare("swe.size"))
    {
	int sw = ll.getFieldTableIndex("width", 1).getTopInteger();
	int sh = ll.getFieldTableIndex("height", 1).getTopInteger();
	ll.stackPop(2);

	std::string str = StringFormat("{\"type\":\"swe.size\",\"width\":%1,\"height\":%2}").arg(sw).arg(sh);
	ll.pushString(str);
    }
    else
    {
        ERROR("userdata empty");
        ll.pushNil();
    }

    return rescount;
}

int SWE_size_tostring(lua_State* L)
{
    // params: swe_size

    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(ll.isTableIndex(1) && 0 == ll.popFieldTableIndex("__type", 1).compare("swe.size"))
    {
	int sw = ll.getFieldTableIndex("width", 1).getTopInteger();
	int sh = ll.getFieldTableIndex("height", 1).getTopInteger();

	ll.stackPop(2);
	ll.pushString(StringFormat("%1[%2,%3]").arg("swe.size").arg(sw).arg(sh));
    }
    else
    {
        ERROR("userdata empty");
        ll.pushNil();
    }

    return rescount;
}

int SWE_size_unpack(lua_State* L)
{
    // params: swe_size

    const int rescount = 2;
    LuaStateDefine(ll, L, rescount);

    if(ll.isTableIndex(1) && 0 == ll.popFieldTableIndex("__type", 1).compare("swe.size"))
    {
	int sw = ll.getFieldTableIndex("width", 1).getTopInteger();
	int sh = ll.getFieldTableIndex("height", 1).getTopInteger();
	ll.stackPop(2);

	ll.pushInteger(sw).pushInteger(sh);
    }
    else
    {
        ERROR("userdata empty");
        ll.pushNil().pushNil();
    }

    return rescount;
}

const struct luaL_Reg SWE_size_functions[] = {
    { "ToJson", SWE_size_to_json }, 		// [string], swe_size
    { "Unpack", SWE_size_unpack }, 		// [list int], swe_size
    { NULL, NULL }
};

void SWE_Stack::size_create(LuaState & ll, int sw, int sh)
{
    LuaStateValidator(ll, 1);

    ll.pushTable();
    ll.pushString("swe.size").setFieldTableIndex("__type", -2);
    ll.pushInteger(sw).setFieldTableIndex("width", -2);
    ll.pushInteger(sh).setFieldTableIndex("height", -2);

    // set: tostring
    ll.pushTable(0, 1);
    ll.pushFunction(SWE_point_tostring).setFieldTableIndex("__tostring", -2);
    ll.setMetaTableIndex(-2);

    // set functions
    ll.setFunctionsTableIndex(SWE_point_functions, -1);
}

int SWE_size_create(lua_State* L)
{
    // empty params

    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    int sw = ll.toIntegerIndex(2);
    int sh = ll.toIntegerIndex(3);

    SWE_Stack::size_create(ll, sw, sh);
    return rescount;
}

void SWE_Size::registers(LuaState & ll)
{
    LuaStateValidator(ll, 0);

    // SWE.Rect
    ll.pushTable("SWE.Size");
    // set metatable: __call
    ll.pushTable(0, 1).pushFunction(SWE_size_create).setFieldTableIndex("__call", -2);
    ll.setMetaTableIndex(-2).stackPop();
}

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

    tableIndex = ll.absTableIndex(tableIndex);

    if(! ll.isTableIndex(tableIndex))
    {
        ERROR(funcName << ": " << "table not found, index: " << tableIndex);
        return res;
    }

    ll.getFieldTableIndex("posx", tableIndex, false);
    if(ll.isTopNil())
        ll.stackPop().getIndexTableIndex(1, tableIndex);

    ll.getFieldTableIndex("posy", tableIndex, false);
    if(ll.isTopNil())
        ll.stackPop().getIndexTableIndex(2, tableIndex);

    res.y = ll.toIntegerIndex(-1);
    res.x = ll.toIntegerIndex(-2);

    ll.stackPop(2);
    return res;
}

Size SWE_Size::get(LuaState & ll, int tableIndex, const char* funcName)
{
    LuaStateValidator(ll, 0);
    Size res;

    tableIndex = ll.absTableIndex(tableIndex);

    if(! ll.isTableIndex(tableIndex))
    {
        ERROR(funcName << ": " << "table not found, index: " << tableIndex);
        return res;
    }

    ll.getFieldTableIndex("width", tableIndex, false);
    if(ll.isTopNil())
        ll.stackPop().getIndexTableIndex(1, tableIndex);

    ll.getFieldTableIndex("height", tableIndex, false);
    if(ll.isTopNil())
        ll.stackPop().getIndexTableIndex(2, tableIndex);

    res.h = ll.toIntegerIndex(-1);
    res.w = ll.toIntegerIndex(-2);

    ll.stackPop(2);
    return res;
}

Rect SWE_Rect::get(LuaState & ll, int tableIndex, const char* funcName)
{
    LuaStateValidator(ll, 0);
    Rect res;

    tableIndex = ll.absTableIndex(tableIndex);

    if(! ll.isTableIndex(tableIndex))
    {
        ERROR(funcName << ": " << "table not found, index: " << tableIndex);
        return res;
    }

    ll.getFieldTableIndex("posx", tableIndex, false);
    if(ll.isTopNil())
        ll.stackPop().getIndexTableIndex(1, tableIndex);

    ll.getFieldTableIndex("posy", tableIndex, false);
    if(ll.isTopNil())
        ll.stackPop().getIndexTableIndex(2, tableIndex);

    ll.getFieldTableIndex("width", tableIndex, false);
    if(ll.isTopNil())
        ll.stackPop().getIndexTableIndex(3, tableIndex);

    ll.getFieldTableIndex("height", tableIndex, false);
    if(ll.isTopNil())
        ll.stackPop().getIndexTableIndex(4, tableIndex);

    res.h = ll.toIntegerIndex(-1);
    res.w = ll.toIntegerIndex(-2);
    res.y = ll.toIntegerIndex(-3);
    res.x = ll.toIntegerIndex(-4);

    ll.stackPop(4);
    return res;
}

/////////////////////////////////////////////////////////////////////
int SWE_rect_point_inrect(lua_State* L)
{
    // params: swe_rect, swe.point
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(ll.isTableIndex(1))
    {
        auto rt = SWE_Rect::get(ll, 1, __FUNCTION__);

        if(ll.isNumberIndex(2) && ll.isNumberIndex(3))
        {
            Point pt;

            pt.x = ll.toIntegerIndex(2);
            pt.y = ll.toIntegerIndex(3);

	    bool res = rt & pt;
	    ll.pushBoolean(res);
        }
        else
        if(ll.isTableIndex(2))
        {
            auto pt = SWE_Point::get(ll, 2, __FUNCTION__);
	    bool res = rt & pt;
	    ll.pushBoolean(res);
        }
        else
        {
            ERROR("unknown params");
	    ll.pushBoolean(false);
        }
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

    if(ll.isTableIndex(1) && ll.isTableIndex(2))
    {
        auto rt1 = SWE_Rect::get(ll, 1, __FUNCTION__);
        auto rt2 = SWE_Rect::get(ll, 2, __FUNCTION__);

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

    if(ll.isTableIndex(1) && ll.isTableIndex(2))
    {
        auto rt1 = SWE_Rect::get(ll, 1, __FUNCTION__);
        auto rt2 = SWE_Rect::get(ll, 2, __FUNCTION__);

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

    if(ll.isTableIndex(1) && ll.isTableIndex(2))
    {
        auto rt1 = SWE_Rect::get(ll, 1, __FUNCTION__);
        auto rt2 = SWE_Rect::get(ll, 2, __FUNCTION__);
	Rect res;

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

    if(ll.isTableIndex(1))
    {
        auto rt = SWE_Rect::get(ll, 1, __FUNCTION__);

	std::string str = StringFormat("{\"type\":\"swe.rect\",\"posx\":%1,\"posy\":%2,\"width\":%3,\"height\":%4}").arg(rt.x).arg(rt.y).arg(rt.w).arg(rt.h);
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

    if(ll.isTableIndex(1))
    {
        auto rt = SWE_Rect::get(ll, 1, __FUNCTION__);
	std::string str = StringFormat("%1[%2,%3,%4,%5]").arg("swe.rect").arg(rt.x).arg(rt.y).arg(rt.w).arg(rt.h);
        ll.pushString(str);
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

    if(ll.isTableIndex(1))
    {
        auto rt = SWE_Rect::get(ll, 1, __FUNCTION__);
	ll.pushInteger(rt.x).pushInteger(rt.y).pushInteger(rt.w).pushInteger(rt.h);
    }
    else
    {
        ERROR("userdata empty");
	ll.pushNil().pushNil().pushNil().pushNil();
    }

    return rescount;
}

int SWE_rect_index(lua_State* L)
{
    // params: table, key
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);
    
    if(ll.isTopNumber())
    {
	switch(ll.toIntegerIndex(-1))
	{
	    case 1:
		ll.getFieldTableIndex("posx", -2, false);
		return rescount;

	    case 2:
		ll.getFieldTableIndex("posy", -2, false);
		return rescount;

	    case 3:
		ll.getFieldTableIndex("width", -2, false);
		return rescount;

	    case 4:
		ll.getFieldTableIndex("height", -2, false);
		return rescount;

	    default: break;
	}
    }

    ERROR("unknown params");
    ll.pushNil();

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
    ll.pushFunction(SWE_rect_equals).setFieldTableIndex("__eq", -2);
    ll.pushFunction(SWE_rect_index).setFieldTableIndex("__index", -2);
    ll.setMetaTableIndex(-2);

    // set functions
    ll.setFunctionsTableIndex(SWE_rect_functions, -1);
}

int SWE_rect_create(lua_State* L)
{
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    int rx,ry,rw,rh;

    if(ll.isTableIndex(2))
    {
        auto rt = SWE_Rect::get(ll, 2, __FUNCTION__);
        rx = rt.x;
        ry = rt.y;
        rw = rt.w;
        rh = rt.h;
    }
    else
    if(1 == ll.stackSize() && ll.isStringIndex(2))
    {
        auto str = ll.toStringIndex(2);
	StringList list = String::split(str, [](int ch){ return ! ::isdigit(ch); });
	if(3 < list.size())
	{
            // swe.rect[xxx,xxx]
            if(0 == str.compare(0, 9, "swe.rect[") && str.back() == ']')
	    {
    	        rx = String::toInt(*std::prev(list.end(), 4));
    	        ry = String::toInt(*std::prev(list.end(), 3));
    	        rw = String::toInt(*std::prev(list.end(), 2));
    	        rh = String::toInt(*std::prev(list.end(), 1));
            }
            // any: xxx,xxx
            else
            {
    	        rx = String::toInt(list.front());
    	        ry = String::toInt(*std::next(list.begin(), 1));
    	        rw = String::toInt(*std::next(list.begin(), 2));
    	        rh = String::toInt(list.back());
            }
	}
	else
	{
	    ERROR("parse error: `" << str << "'");
    	    rx = -1;
    	    ry = -1;
    	    rw = 0;
    	    rh = 0;
	}
    }
    else
    {
        rx = ll.toIntegerIndex(2);
        ry = ll.toIntegerIndex(3);
        rw = ll.toIntegerIndex(4);
        rh = ll.toIntegerIndex(5);
    }

    SWE_Stack::rect_create(ll, rx, ry, rw, rh);
    return rescount;
}

const struct luaL_Reg SWE_rect_functions2[] = {
    { "PointInRect", SWE_rect_point_inrect },		// [bool], swe_rect, swe_point
    { "HasIntersection", SWE_rect_has_intersection },	// [bool], swe_rect, swe_rect
    { "RectEquals", SWE_rect_equals },			// [bool], swe_rect, swe_rect
    { "GetIntersectRect", SWE_rect_intersect },		// [swe_rect], swe_rect, swe_rect
    { NULL, NULL }
};


void SWE_Rect::registers(LuaState & ll)
{
    LuaStateValidator(ll, 0);

    // SWE.Rect
    ll.pushTable("SWE.Rect");
    // set metatable: __call
    ll.pushTable(0, 1).pushFunction(SWE_rect_create).setFieldTableIndex("__call", -2);
    ll.setMetaTableIndex(-2);

    ll.setFunctionsTableIndex(SWE_rect_functions2, -1);
    ll.stackPop();
}

/////////////////////////////////////////////////////////////////////
int SWE_point_to_json(lua_State* L)
{
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    // params: swe_point

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

int SWE_point_equals(lua_State* L)
{
    // params: swe_point, swe_point
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(ll.isTableIndex(1) && ll.isTableIndex(2))
    {
        auto pt1 = SWE_Point::get(ll, 1, __FUNCTION__);
        auto pt2 = SWE_Point::get(ll, 2, __FUNCTION__);

	bool res = pt1 == pt2;
	ll.pushBoolean(res);
    }
    else
    {
        ERROR("userdata empty");
        ll.pushBoolean(false);
    }

    return rescount;
}

int SWE_point_index(lua_State* L)
{
    // params: table, key
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);
    
    if(ll.isTopNumber())
    {
	switch(ll.toIntegerIndex(-1))
	{
	    case 1:
		ll.getFieldTableIndex("posx", -2, false);
		return rescount;

	    case 2:
		ll.getFieldTableIndex("posy", -2, false);
		return rescount;

	    default: break;
	}
    }

    ERROR("unknown params");
    ll.pushNil();

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

    // set: metatable
    ll.pushTable(0, 1);
    ll.pushFunction(SWE_point_tostring).setFieldTableIndex("__tostring", -2);
    ll.pushFunction(SWE_point_equals).setFieldTableIndex("__eq", -2);
    ll.pushFunction(SWE_point_index).setFieldTableIndex("__index", -2);
    ll.setMetaTableIndex(-2);

    // set functions
    ll.setFunctionsTableIndex(SWE_point_functions, -1);
}

int SWE_point_create(lua_State* L)
{
    // empty params

    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    int px, py;

    if(ll.isTableIndex(2))
    {
        auto pt = SWE_Point::get(ll, 2, __FUNCTION__);
        px = pt.x;
        py = pt.y;
    }
    else
    if(1 == ll.stackSize() && ll.isStringIndex(2))
    {
        auto str = ll.toStringIndex(2);

	StringList list = String::split(str, [](int ch){ return ! ::isdigit(ch); });
	if(1 < list.size())
	{
            // swe.point[xxx,xxx]
            if(0 == str.compare(0, 10, "swe.point[") && str.back() == ']')
	    {
                auto it1 = std::prev(list.end(), 2);
                auto it2 = std::prev(list.end(), 1);
    	        px = String::toInt(*it1);
    	        py = String::toInt(*it2);
            }
            // any: xxx,xxx
            else
            {
    	        px = String::toInt(list.front());
    	        py = String::toInt(list.back());
	    }
	}
        else
	{
	    ERROR("parse error: `" << str << "'");
    	    px = -1;
    	    py = -1;
        }
    }
    else
    {
        px = ll.toIntegerIndex(2);
        py = ll.toIntegerIndex(3);
    }

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

int SWE_size_equals(lua_State* L)
{
    // params: swe_size, swe_size
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(ll.isTableIndex(1) && ll.isTableIndex(2))
    {
        auto sz1 = SWE_Size::get(ll, 1, __FUNCTION__);
        auto sz2 = SWE_Size::get(ll, 2, __FUNCTION__);

	bool res = sz1 == sz2;
	ll.pushBoolean(res);
    }
    else
    {
        ERROR("userdata empty");
        ll.pushBoolean(false);
    }

    return rescount;
}

int SWE_size_index(lua_State* L)
{
    // params: table, key
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);
    
    if(ll.isTopNumber())
    {
	switch(ll.toIntegerIndex(-1))
	{
	    case 1:
		ll.getFieldTableIndex("width", -2, false);
		return rescount;

	    case 2:
		ll.getFieldTableIndex("height", -2, false);
		return rescount;

	    default: break;
	}
    }

    ERROR("unknown params");
    ll.pushNil();

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

    // set: metatable
    ll.pushTable(0, 1);
    ll.pushFunction(SWE_size_tostring).setFieldTableIndex("__tostring", -2);
    ll.pushFunction(SWE_size_equals).setFieldTableIndex("__eq", -2);
    ll.pushFunction(SWE_size_index).setFieldTableIndex("__index", -2);
    ll.setMetaTableIndex(-2);

    // set functions
    ll.setFunctionsTableIndex(SWE_size_functions, -1);
}

int SWE_size_create(lua_State* L)
{
    // empty params

    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    int sw, sh;

    if(ll.isTableIndex(2))
    {
        auto sz = SWE_Size::get(ll, 2, __FUNCTION__);
        sw = sz.w;
        sh = sz.h;
    }
    else
    if(1 == ll.stackSize() && ll.isStringIndex(2))
    {
        auto str = ll.toStringIndex(2);
	StringList list = String::split(str, [](int ch){ return ! ::isdigit(ch); });
	if(1 < list.size())
	{
            // swe.size[xxx,xxx]
            if(0 == str.compare(0, 9, "swe.size[") && str.back() == ']')
	    {
                auto it1 = std::prev(list.end(), 2);
                auto it2 = std::prev(list.end(), 1);
    	        sw = String::toInt(*it1);
    	        sh = String::toInt(*it2);
            }
            // any: xxx,xxx
            else
            {
    	        sw = String::toInt(list.front());
    	        sh = String::toInt(list.back());
            }
	}
	else
	{
	    ERROR("parse error: `" << str << "'");
    	    sw = 0;
    	    sh = 0;
	}
    }
    else
    {
        sw = ll.toIntegerIndex(2);
        sh = ll.toIntegerIndex(3);
    }

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

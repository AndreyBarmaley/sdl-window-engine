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

#include "SWE_keys.h"

int SWE_key_index(lua_State* L)
{
    // params: table, string name
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    std::string upper = String::toUpper(ll.getTopString());
    ll.getFieldTableIndex(upper, 1);

    if(ll.isTopNil())
        ERROR("unknown key: " << upper);

    return rescount;
}

int SWE_key_tochar(lua_State* L)
{
    // params: int key
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);
    
    if(ll.isTopInteger())
    {
	int sym = ll.toIntegerIndex(1);
	int ch = KeySym(sym).keychar();

	ll.pushString(std::string(1, ch));
    }
    else
    {
        ERROR("integer not found");
	ll.pushString(NULL);
    }

    return rescount;
}

int SWE_key_tostring(lua_State* L)
{
    // params: int key
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);
    
    if(ll.isTopInteger())
    {
	std::string res = Key::toName(ll.getTopInteger());
	ll.pushString(res);
    }
    else
    {
        ERROR("integer not found");
	ll.pushString(NULL);
    }

    return rescount;
}

const struct luaL_Reg SWE_keys_functions[] = {
    { "ToChar", SWE_key_tochar },	// [int char], int key
    { "ToString", SWE_key_tostring },	// [string name], int key
    { NULL, NULL }
};

void SWE_Key::registers(LuaState & ll)
{
    LuaStateValidator(ll, 0);

    // SWE.Key
    ll.pushTable("SWE.Key");
    ll.setFunctionsTableIndex(SWE_keys_functions, -1);

    // SWE.Key: insert values
    for(auto & kv : Key::allKeys())
        ll.pushInteger(kv.key).setFieldTableIndex(kv.name, -2);

    ll.pushInteger(0x4000010e).setFieldTableIndex("ANDROID_BACK", -2);
    ll.pushInteger(312).setFieldTableIndex("WINCE_EXIT", -2);

    // SWE.Key: set metatable: __index
    ll.pushTable(0, 1).pushFunction(SWE_key_index).setFieldTableIndex("__index", -2);
    ll.setMetaTableIndex(-2).stackPop();
}

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
        int params = ll.stackSize();
	int sym = ll.toIntegerIndex(1);
	int mod = 1 < params ? ll.toIntegerIndex(2) : 0;

	ll.pushInteger(KeySym(sym, mod).keychar());
    }
    else
    {
        ERROR("integer not found");
	ll.pushNil();
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

int SWE_key_ismod(lua_State* L, int val)
{
    // params: int key
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);
    
    if(ll.isTopInteger())
    {
	int mod = ll.getTopInteger();
	ll.pushBoolean(mod & val);
    }
    else
    {
        ERROR("integer not found");
	ll.pushBoolean(false);
    }

    return rescount;
}

int SWE_mod_lshift(lua_State* L)
{
    return SWE_key_ismod(L, KMOD_LSHIFT);
}

int SWE_mod_rshift(lua_State* L)
{
    return SWE_key_ismod(L, KMOD_RSHIFT);
}

int SWE_mod_shift(lua_State* L)
{
    return SWE_key_ismod(L, KMOD_SHIFT);
}

int SWE_mod_lctrl(lua_State* L)
{
    return SWE_key_ismod(L, KMOD_LCTRL);
}

int SWE_mod_rctrl(lua_State* L)
{
    return SWE_key_ismod(L, KMOD_RCTRL);
}

int SWE_mod_ctrl(lua_State* L)
{
    return SWE_key_ismod(L, KMOD_CTRL);
}

int SWE_mod_lalt(lua_State* L)
{
    return SWE_key_ismod(L, KMOD_LALT);
}

int SWE_mod_ralt(lua_State* L)
{
    return SWE_key_ismod(L, KMOD_RALT);
}

int SWE_mod_alt(lua_State* L)
{
    return SWE_key_ismod(L, KMOD_ALT);
}

const struct luaL_Reg SWE_keys_functions[] = {
    { "ToKeyChar", SWE_key_tochar },	  // [int char], int key, int mod
    { "ToString", SWE_key_tostring },	  // [string name], int key
    { "IsLeftShift", SWE_mod_lshift },    // [bool], int mod
    { "IsRightShift", SWE_mod_rshift },   // [bool], int mod
    { "IsShift", SWE_mod_shift },         // [bool], int mod
    { "IsLeftCtrl", SWE_mod_lctrl },      // [bool], int mod
    { "IsRightCtrl", SWE_mod_rctrl },     // [bool], int mod
    { "IsCtrl", SWE_mod_ctrl },           // [bool], int mod
    { "IsLeftAlt", SWE_mod_lalt },        // [bool], int mod
    { "IsRightAlt", SWE_mod_ralt },       // [bool], int mod
    { "IsAlt", SWE_mod_alt },             // [bool], int mod
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

/***************************************************************************
 *   Copyright (C) 2021 by SWE team <sdl.window.engine@gmail.com>          *
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

#include <codecvt>

#include "SWE_unicodestring.h"
#include "SWE_unicoderegex.h"

int SWE_unicoderegex_create(lua_State*);
int SWE_unicoderegex_destroy(lua_State*);

SWE_UnicodeRegex* SWE_UnicodeRegex::get(LuaState & ll, int tableIndex, const char* funcName)
{
    LuaStateValidator(ll, 0);

    if(! ll.isTableIndex(tableIndex) ||
	0 != ll.popFieldTableIndex("__type", tableIndex).compare("swe.unicoderegex"))
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
    
    auto ptr = static_cast<SWE_UnicodeRegex**>(ll.getTopUserData());
    ll.stackPop();

    return ptr ? *ptr : NULL;
}

///////////////////////////////////////////////////////////
int SWE_unicoderegex_find_all(lua_State* L)
{
    // params: table unicoderegex, table unicodestring, int init

    // returned variable size
    LuaState ll(L);

    auto ureg_find_all = [&](const srell::u16regex & reg, const std::u16string & str)
    {
        auto resbeg = srell::u16sregex_iterator(str.begin(), str.end(), reg);
        auto resend = srell::u16sregex_iterator();

        if(resbeg != resend)
        {
            ll.pushTable(std::distance(resbeg, resend), 0);
            size_t seqindex = 1;

            for(auto it = resbeg; it != resend; ++it)
            {
                auto & match = *it;

                ll.pushInteger(match.position()).setIndexTableIndex(seqindex++, -2);
                ll.pushInteger(match.length()).setIndexTableIndex(seqindex++, -2);
            }

            return 1;
        }

	ll.pushNil();
        return 1;
    };

    // regex, string, index
    if(auto ureg = SWE_UnicodeRegex::get(ll, 1, __FUNCTION__))
    {
        if(ll.isTableIndex(2))
        {
            if(auto ustr = SWE_UnicodeString::get(ll, 2, __FUNCTION__))
                return ureg_find_all(*ureg, *ustr);
            else
            {
                ERROR("table not found" << ": " << "swe.unicodestring");
	        ll.pushNil();
            }
        }
        else
        if(ll.isStringIndex(2))
            return ureg_find_all(*ureg, SWE::UnicodeString(ll.toStringIndex(2)));
        else
        {
	    ERROR("unknown params");
	    ll.pushNil();
        }
    }
    else
    {
	ERROR("userdata empty");
	ll.pushNil();
    }

    return 1;
}

int SWE_unicoderegex_find_aux(lua_State* L, bool match)
{
    // params: table unicoderegex, table unicodestring, int init

    // returned variable size
    LuaState ll(L);

    auto ureg_find = [&](const srell::u16regex & reg, const std::u16string & str)
    {
        int init = ll.isNumberIndex(3) ? ll.toIntegerIndex(3) : 0;
        if((init < 0 && std::abs(init) > str.size()) ||
           (init > 0 && init >= str.size()))
            init = 0;

        // to abs index
        if(init < 0)
            init = str.size() + init;

        srell::u16smatch m;
        bool res = false;

        if(match)
        {
            if(0 < init)
                res = srell::regex_match(std::next(str.begin(), init), str.end(), m, reg);
            else
                res = srell::regex_match(str, m, reg);
        }
        else
        {
            if(0 < init)
                res = srell::regex_search(std::next(str.begin(), init), str.end(), m, reg);
            else
                res = srell::regex_search(str, m, reg);
        }

        if(res)
        {
            ll.pushInteger(init + m.position());
            ll.pushInteger(m.length());
            int ret = 2;

            if(1 < m.size())
            {
                std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;

                for(size_t it = 1; it < m.size(); ++it)
                    ll.pushString(conv.to_bytes(m[it]));

                ret = m.size() + 1;
            }

            return ret;
        }

	ll.pushNil();
        return 1;
    };

    // regex, string, index
    if(auto ureg = SWE_UnicodeRegex::get(ll, 1, __FUNCTION__))
    {
        if(ll.isTableIndex(2))
        {
            if(auto ustr = SWE_UnicodeString::get(ll, 2, __FUNCTION__))
                return ureg_find(*ureg, *ustr);
            else
            {
                ERROR("table not found" << ": " << "swe.unicodestring");
	        ll.pushNil();
            }
        }
        else
        if(ll.isStringIndex(2))
            return ureg_find(*ureg, SWE::UnicodeString(ll.toStringIndex(2)));
        else
        {
	    ERROR("unknown params");
	    ll.pushNil();
        }
    }
    else
    {
	ERROR("userdata empty");
	ll.pushNil();
    }

    return 1;
}

int SWE_unicoderegex_search(lua_State* L)
{
    return SWE_unicoderegex_find_aux(L, false);
}

int SWE_unicoderegex_match(lua_State* L)
{
    return SWE_unicoderegex_find_aux(L, true);
}

int SWE_unicoderegex_assign(lua_State* L)
{
    // params: table unicoderegex, string
    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    if(SWE_UnicodeRegex* ureg = SWE_UnicodeRegex::get(ll, 1, __FUNCTION__))
    {
        auto str = ll.toStringIndex(2);

        try
        {
            ureg->assign(SWE::UnicodeString(str));
        }
        catch(srell::regex_error & err)
        {
            ERROR("not regex: `" << str << "'");
        }
    }
    else
    {
        ERROR("userdata empty");
    }
    
    return rescount;
}

const struct luaL_Reg SWE_unicoderegex_functions[] = {
    { "Search", SWE_unicoderegex_search },		// [list], table unicoderegex, table unicodestring, int init
    { "Match", SWE_unicoderegex_match },		// [list], table unicoderegex, table unicodestring, int init
    { "FindAll", SWE_unicoderegex_find_all },		// [list], table unicoderegex, table unicodestring
    { "Assign", SWE_unicoderegex_assign },		// [void], table unicoderegex, string
    { NULL, NULL }
};

void SWE_UnicodeRegex::registers(LuaState & ll)
{
    LuaStateValidator(ll, 0);

    // SWE.UnicodeRegex
    ll.pushTable("SWE.UnicodeRegex");
    ll.setFunctionsTableIndex(SWE_unicoderegex_functions, -1);

    // set metatable: __call
    ll.pushTable(0, 1);
    ll.pushFunction(SWE_unicoderegex_create).setFieldTableIndex("__call", -2);
    ll.pushFunction(SWE_unicoderegex_search).setFieldTableIndex("Search", -2);
    ll.pushFunction(SWE_unicoderegex_match).setFieldTableIndex("Match", -2);
    ll.pushFunction(SWE_unicoderegex_find_all).setFieldTableIndex("FindAll", -2);
    ll.setMetaTableIndex(-2).stackPop();
}

SWE_UnicodeRegex* SWE_Stack::u16regex_create(LuaState & ll)
{
    LuaStateValidator(ll, 1);

    // result
    ll.pushTable();

    // set: meta
    // ll.pushTable(0, 1);
    // ll.pushFunction(SWE_unicoderegex_tostring).setFieldTableIndex("__tostring", -2);
    // ll.setMetaTableIndex(-2);

    // set userdata
    ll.pushString("userdata");
    auto ptr = static_cast<SWE_UnicodeRegex**>(ll.pushUserData(sizeof(SWE_UnicodeRegex*)));
    *ptr = new SWE_UnicodeRegex();

    // set metatable: __gc to userdata
    ll.pushTable(0, 1);
    ll.pushFunction(SWE_unicoderegex_destroy).setFieldTableIndex("__gc", -2);
    ll.setMetaTableIndex(-2).setTableIndex(-3);

    // set functions
    ll.setFunctionsTableIndex(SWE_unicoderegex_functions, -1);

    DEBUG(String::pointer(ptr) << ": [" << String::pointer(*ptr) << "]");

    ll.pushString("__type").pushString("swe.unicoderegex").setTableIndex(-3);

    return *ptr;
}

int SWE_unicoderegex_create(lua_State* L)
{
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    SWE_UnicodeRegex* ureg = SWE_Stack::u16regex_create(ll);

    // SWE_UnicodeRegex: string
    if(ll.isStringIndex(2))
    {
        auto str = ll.toStringIndex(2);
    
        try
        {
	    ureg->assign(SWE::UnicodeString(str));
        }
        catch(srell::regex_error & err)
        {
            ERROR("not regex: `" << str << "'");
        }
    }
    else
    // SWE_UnicodeRegex: UnicodeString
    if(SWE_UnicodeString* ustr = SWE_UnicodeString::get(ll, 2, __FUNCTION__))
    {
        try
        {
            ureg->assign(*ustr);
        }
        catch(srell::regex_error & err)
        {
            ERROR("not regex: `" << ustr->toString() << "'");
        }
    }

    return rescount;
}

int SWE_unicoderegex_destroy(lua_State* L)
{
    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    if(ll.isTopUserData())
    {
        auto ptr = static_cast<SWE_UnicodeRegex**>(ll.getTopUserData());
        if(ptr && *ptr)
        {
            DEBUG(String::pointer(ptr) << ": [" << String::pointer(*ptr) << "]");

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

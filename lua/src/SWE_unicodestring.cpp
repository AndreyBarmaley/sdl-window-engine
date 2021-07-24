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

#include "SWE_unicodestring.h"

int SWE_unicodestring_create(lua_State*);
int SWE_unicodestring_destroy(lua_State*);

SWE_UnicodeString* SWE_UnicodeString::get(LuaState & ll, int tableIndex, const char* funcName)
{
    LuaStateValidator(ll, 0);

    if(! ll.isTableIndex(tableIndex) ||
	0 != ll.popFieldTableIndex("__type", tableIndex).compare("swe.unicodestring"))
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
    
    auto ptr = static_cast<SWE_UnicodeString**>(ll.getTopUserData());
    ll.stackPop();

    return ptr ? *ptr : NULL;
}

///////////////////////////////////////////////////////////
int SWE_unicodestring_pushback(lua_State* L)
{
    // params: table unicodestring, int char,int char .. int char

    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_UnicodeString* ustr = SWE_UnicodeString::get(ll, 1, __FUNCTION__))
    {
	int params = ll.stackSize();

	for(int ii = 2; ii <= params; ++ii)
	{
	    int ch = ll.toIntegerIndex(ii);
	    ustr->push_back(ch);
	}

	ll.pushInteger(ustr->size()).setFieldTableIndex("size", 1);
	ll.pushBoolean(true);
    }
    else
    {
	ERROR("userdata empty");
	ll.pushNil();
    }

    return rescount;
}

int SWE_unicodestring_setchar(lua_State* L)
{
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    // params: table unicodestring, int pos, int char, int char ... int char
    int params = ll.stackSize();

    if(SWE_UnicodeString* ustr = SWE_UnicodeString::get(ll, 1, __FUNCTION__))
    {
        int pos = ll.toIntegerIndex(2);

        if(0 <= pos && pos <= ustr->size())
        {
            for(int ii = 3; ii <= params; ++ii)
            {
                int ch = ll.toIntegerIndex(ii);
                if(pos < ustr->size())
                    ustr->operator[](pos) = ch;
                else
                    ustr->push_back(ch);
                pos += 1;
            }

            ll.pushInteger(ustr->size()).setFieldTableIndex("size", 1);
            ll.pushBoolean(true);
        }
        else
        {
            ERROR("out of range" << ", " << ustr->size() << ", " << pos);
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

int SWE_unicodestring_find(lua_State* L)
{
    // params: table unicodestring, string val, int pos

    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_UnicodeString* ustr = SWE_UnicodeString::get(ll, 1, __FUNCTION__))
    {
        int res = -1;
        int params = ll.stackSize();

        if(ll.isTableIndex(2))
        {
            if(SWE_UnicodeString* ustr2 = SWE_UnicodeString::get(ll, 2, __FUNCTION__))
            {
	        int pos = 2 < params ? ll.toIntegerIndex(3) : 0;
                auto it = ustr->find(*ustr2, pos);
                if(it != std::u16string::npos) res = it;
            }
            else
            {
		ERROR("table not found" << ": " << "swe.unicodestring");
            }
        }
        else
        {
	    std::string val = ll.toStringIndex(2);

	    int pos = 2 < params ? ll.toIntegerIndex(3) : 0;
            if(pos < 0 || pos >= ustr->size()) pos = 0;

            if(! val.empty())
            {
                auto it1 = std::next(ustr->begin(), pos);
                while(it1 < ustr->end())
                {
                    auto it2 = val.begin();
                    // find
                    while(*it1 != *it2){ it1++; }
                    // compare
                    while(it2 != val.end() && *it1 == *it2){ it1++; it2++; }

                    if(it2 == val.end())
                    {
                        res = std::distance(ustr->begin(), std::prev(it1, val.size()));
                        break;
                    }
                }
            }
        }
        ll.pushInteger(res);
    }
    else
    {
	ERROR("userdata empty");
	ll.pushInteger(-1);
    }

    return rescount;
}

int SWE_unicodestring_getchar(lua_State* L)
{
    // params: table unicodestring, int pos

    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_UnicodeString* ustr = SWE_UnicodeString::get(ll, 1, __FUNCTION__))
    {
	int pos = ll.toIntegerIndex(2);

	if(0 <= pos && pos < ustr->size())
	{
	    ll.pushInteger(ustr->operator[](pos));
	}
	else
	{
    	    ERROR("out of range" << ", " << ustr->size() << ", " << pos);
	    ll.pushNil();
	}
    }
    else
    {
	ERROR("userdata empty");
	ll.pushNil();
    }

    return rescount;
}

int SWE_unicodestring_insert(lua_State* L)
{
    // params: table unicodestring, int pos, (unicodestring, subpos, sublen), (int count, int char)

    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_UnicodeString* ustr = SWE_UnicodeString::get(ll, 1, __FUNCTION__))
    {
	int pos = ll.toIntegerIndex(2);

        if(0 > pos || pos > ustr->size())
        {
            ERROR("out of range" << ", " << ustr->size() << ", " << pos);
            ll.pushBoolean(false);
        }
        else
	// params: int count, int char
	if(ll.isNumberIndex(3))
	{
	    int count = ll.toIntegerIndex(3);
	    int ch = ll.toIntegerIndex(4);

	    ustr->insert(pos, count, ch);
	    ll.pushInteger(ustr->size()).setFieldTableIndex("size", 1);
	    ll.pushBoolean(true);
	}
	else
	// params: table unicodestring, int subpos, int sublen
	if(ll.isTableIndex(3))
	{
	    SWE_UnicodeString* ustr2 = SWE_UnicodeString::get(ll, 3, __FUNCTION__);
	    if(ustr2)
	    {
		int params = ll.stackSize();
		if(3 < params)
		{
		    int subpos = ll.toIntegerIndex(4);
		    int sublen = ll.toIntegerIndex(5);

		    if(0 <= subpos && subpos < ustr2->size())
		    {
			if(sublen <= 0 || subpos + sublen > ustr2->size())
			    sublen = ustr2->size() - subpos;

			ustr->insert(pos, *ustr2, subpos, sublen);
			ll.pushInteger(ustr->size()).setFieldTableIndex("size", 1);
			ll.pushBoolean(true);
		    }
		    else
		    {
    		        ERROR("out of range" << ", " << ustr2->size() << ", " << subpos);
			ll.pushBoolean(false);
		    }
		}
		else
		{
		    ustr->insert(pos, *ustr2);
		    ll.pushInteger(ustr->size()).setFieldTableIndex("size", 1);
		    ll.pushBoolean(true);
		}
	    }
	    else
	    {
		ERROR("table not found" << ": " << "swe.unicodestring");
		ll.pushBoolean(false);
	    }
	}
    }
    else
    {
	ERROR("userdata empty");
	ll.pushBoolean(false);
    }

    return rescount;
}

int SWE_unicodestring_substring(lua_State* L)
{
    // params: table unicodestring, int pos, int len
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_UnicodeString* ustr = SWE_UnicodeString::get(ll, 1, __FUNCTION__))
    {
	int pos = ll.toIntegerIndex(2);
	int len = ll.toIntegerIndex(3);

	if(0 <= pos && pos < ustr->size())
	{
	    if(len <= 0 || pos + len > ustr->size())
		len = ustr->size() - pos;

	    SWE_UnicodeString* res = SWE_Stack::u16string_create(ll);
	    res->assign(ustr->begin() + pos, ustr->begin() + pos + len);
	    ll.pushString("size").pushInteger(res->size()).setTableIndex(-3);
	}
	else
	{
    	    ERROR("out of range" << ", " << ustr->size() << ", " << pos);
	    ll.pushNil();
	}
    }
    else
    {
	ERROR("userdata empty");
	ll.pushNil();
    }

    return rescount;
}

int SWE_unicodestring_erase(lua_State* L)
{
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    // params: table unicodestring, int size, int val
    int params = ll.stackSize();

    if(SWE_UnicodeString* ustr = SWE_UnicodeString::get(ll, 1, __FUNCTION__))
    {
	int pos = ll.toIntegerIndex(2);
	int count = 3 > params ? 1 : ll.toIntegerIndex(3);

	if(0 <= pos && pos < ustr->size() && 0 < count && pos + count <= ustr->size())
	{
	    if(1 == count)
		ustr->erase(ustr->begin() + pos);
	    else
		ustr->erase(ustr->begin() + pos, ustr->begin() + pos + count);

	    ll.pushString("size").pushInteger(ustr->size()).setTableIndex(1);
	    ll.pushBoolean(true);
	}
	else
	{
    	    ERROR("out of range" << ", " << ", " << ustr->size() << ", " << pos);
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

int SWE_unicodestring_resize(lua_State* L)
{
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    // params: table unicodestring, int size, int val

    if(SWE_UnicodeString* ustr = SWE_UnicodeString::get(ll, 1, __FUNCTION__))
    {
	int size = ll.toIntegerIndex(2);

	if(0 < size)
	{
	    int val = ll.toIntegerIndex(3);
	    ustr->resize(size, val);
	    ll.pushString("size").pushInteger(ustr->size()).setTableIndex(1);
	    ll.pushBoolean(true);
	}
	else
	if(0 == size)
	{
	    ustr->clear();
	    ll.pushString("size").pushInteger(ustr->size()).setTableIndex(1);
	    ll.pushBoolean(true);
	}
	else
	{
    	    ERROR("out of range" << ", " << size);
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

int SWE_unicodestring_to_cstring(lua_State* L)
{
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    // params: table unicodestring

    if(SWE_UnicodeString* ustr = SWE_UnicodeString::get(ll, 1, __FUNCTION__))
    {
        ll.pushString(ustr->toString());
    }
    else
    {
	ll.pushNil();
	ERROR("userdata empty");
    }

    return rescount;
}

int SWE_unicodestring_clear(lua_State* L)
{
    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    // params: table unicodestring, int pos

    if(SWE_UnicodeString* ustr = SWE_UnicodeString::get(ll, 1, __FUNCTION__))
    {
	ustr->clear();
	ll.pushInteger(0).setFieldTableIndex("size", -2);
    }
    else
    {
	ERROR("userdata empty");
    }

    return rescount;
}

int SWE_unicodestring_to_json(lua_State* L)
{
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    // params: swe_unicodestring

    if(SWE_UnicodeString* ustr = SWE_UnicodeString::get(ll, 1, __FUNCTION__))
    {
        std::string json = StringFormat("[%1]").arg(ustr->toHexString(",", true));
        ll.pushString(json);
    }
    else
    {    
	ERROR("userdata empty");
	ll.pushNil();
    }

    return rescount;
}           

int SWE_unicodestring_tostring(lua_State* L)
{
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    // params: swe_unicodestring

    if(SWE_UnicodeString* ustr = SWE_UnicodeString::get(ll, 1, __FUNCTION__))
    {
        std::string str = StringFormat("%1[%2]").arg("swe.unicodestring").arg(ustr->toHexString(",", true));
        ll.pushString(str);
    }
    else
    {    
	ERROR("userdata empty");
	ll.pushNil();
    }
    
    return rescount;
}

int SWE_unicodestring_equals(lua_State* L)
{
    // params: table unicodestring1, table unicodestring2
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    SWE_UnicodeString* ustr1 = SWE_UnicodeString::get(ll, 1, __FUNCTION__);
    SWE_UnicodeString* ustr2 = SWE_UnicodeString::get(ll, 2, __FUNCTION__);

    if(ustr1 && ustr2)
    {
        bool res = ustr1->size() == ustr2->size() &&
            std::equal(ustr1->begin(), ustr1->end(), ustr2->begin());

	ll.pushBoolean(res);
    }
    else
    {
	ERROR("userdata empty");
	ll.pushNil();
    }

    return rescount;
}

int SWE_unicodestring_concat(lua_State* L)
{
    // params: table unicodestring1, table unicodestring2
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    SWE_UnicodeString* ustr1 = SWE_UnicodeString::get(ll, 1, __FUNCTION__);
    SWE_UnicodeString* ustr2 = SWE_UnicodeString::get(ll, 2, __FUNCTION__);

    if(ustr1 && ustr2)
    {
	SWE_UnicodeString* res = SWE_Stack::u16string_create(ll);
	res->resize(ustr1->size() + ustr2->size(), 0);

	std::copy(ustr1->begin(), ustr1->end(), res->begin());
	std::copy(ustr2->begin(), ustr2->end(), res->begin() + ustr1->size());

	ll.pushString("size").pushInteger(res->size()).setTableIndex(-3);
    }
    else
    {
	ERROR("userdata empty");
	ll.pushNil();
    }

    return rescount;
}

int SWE_unicodestring_getsize(lua_State* L)
{
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    // params: swe_unicodestring

    if(SWE_UnicodeString* ustr = SWE_UnicodeString::get(ll, 1, __FUNCTION__))
    {
        ll.pushNumber(ustr->size());
    }
    else
    {    
	ERROR("userdata empty");
	ll.pushNumber(0);
    }
    
    return rescount;
}

int SWE_unicodestring_index(lua_State* L)
{
    // params: table unicodestring, int offset (lua style: start 1)
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_UnicodeString* ustr = SWE_UnicodeString::get(ll, 1, __FUNCTION__))
    {
        int offset = ll.toIntegerIndex(2);
        if(0 < offset && offset <= ustr->size())
        {
            ll.pushInteger(ustr->operator[](offset - 1));
        }
        else
        {
            ERROR("out of range" << ", " << ustr->size() << ", " << offset);
            ll.pushNil();
        }
    }
    else
    {
        ERROR("userdata empty");
        ll.pushNil();
    }

    return rescount;
}

const struct luaL_Reg SWE_unicodestring_functions[] = {
    { "PushBack", SWE_unicodestring_pushback },		// [bool], table unicodestring, int char, int char .. int char
    { "SetChar", SWE_unicodestring_setchar },		// [bool], table unicodestring, int pos, int char, int char .. int char
    { "GetChar", SWE_unicodestring_getchar },		// [int char], table unicodestring, int pos
    { "Find", SWE_unicodestring_find },                 // [int], table unicodestring, string, int pos
    { "Insert", SWE_unicodestring_insert },		// [bool], table unicodestring, int pos, (unicodestring), (int count, int char)
    { "SubString", SWE_unicodestring_substring },	// [table unicodestring], table unicodestring, int pos, int len
    { "ToUtf8String", SWE_unicodestring_to_cstring },   // [string], table unicodestring
    { "ToJson", SWE_unicodestring_to_json },		// [string], table unicodestring
    { "Clear", SWE_unicodestring_clear },		// [void], table unicodestring
    { "Resize", SWE_unicodestring_resize },		// [bool], table unicodestring, int size, int val
    { "Erase", SWE_unicodestring_erase },		// [bool], table unicodestring, int pos, int count
    { NULL, NULL }
};

void SWE_UnicodeString::registers(LuaState & ll)
{
    LuaStateValidator(ll, 0);

    // SWE.UnicodeString
    ll.pushTable("SWE.UnicodeString");
    ll.setFunctionsTableIndex(SWE_unicodestring_functions, -1);

    // set metatable: __call
    ll.pushTable(0, 1);
    ll.pushFunction(SWE_unicodestring_create).setFieldTableIndex("__call", -2);
    ll.setMetaTableIndex(-2).stackPop();
}

SWE_UnicodeString* SWE_Stack::u16string_create(LuaState & ll)
{
    LuaStateValidator(ll, 1);

    // result
    ll.pushTable();

    // set: meta
    ll.pushTable(0, 1);
    //ll.pushFunction(SWE_unicodestring_tostring).setFieldTableIndex("__tostring", -2);
    ll.pushFunction(SWE_unicodestring_index).setFieldTableIndex("__index", -2);
    ll.pushFunction(SWE_unicodestring_concat).setFieldTableIndex("__concat", -2);
    ll.pushFunction(SWE_unicodestring_getsize).setFieldTableIndex("__len", -2);
    ll.pushFunction(SWE_unicodestring_equals).setFieldTableIndex("__eq", -2);
    ll.setMetaTableIndex(-2);

    // set userdata
    ll.pushString("userdata");
    auto ptr = static_cast<SWE_UnicodeString**>(ll.pushUserData(sizeof(SWE_UnicodeString*)));
    *ptr = new SWE_UnicodeString();

    // set metatable: __gc to userdata
    ll.pushTable(0, 1);
    ll.pushFunction(SWE_unicodestring_destroy).setFieldTableIndex("__gc", -2);
    ll.setMetaTableIndex(-2).setTableIndex(-3);

    // set functions
    ll.setFunctionsTableIndex(SWE_unicodestring_functions, -1);

    DEBUG(String::pointer(ptr) << ": [" << String::pointer(*ptr) << "]");

    ll.pushString("__type").pushString("swe.unicodestring").setTableIndex(-3);
    ll.pushString("size").pushInteger((*ptr)->size()).setTableIndex(-3);

    return *ptr;
}

int SWE_unicodestring_create(lua_State* L)
{
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    // SWE.UnicodeString(self, string)

    SWE_UnicodeString* ustr = SWE_Stack::u16string_create(ll);

    // SWE_UnicodeString: length, fill value
    if(ll.isNumberIndex(2) && ll.isNumberIndex(3))
    {
	int bsz = ll.toIntegerIndex(2);
	int bvl = ll.toIntegerIndex(3);

	ustr->assign(bsz, bvl);
	ll.pushString("size").pushInteger(ustr->size()).setTableIndex(-3);
    }
    else
    // SWE_UnicodeString: string
    if(ll.isStringIndex(2))
    {
        std::string str = ll.toStringIndex(2);

        // swe.unicodestring[0x0001,0x0002,.. 0x0099]
        if(0 == str.compare(0, 18, "swe.unicodestring[") && str.back() == ']')
        {
            auto list = String::split(str.substr(17, str.size() - 18), [](int ch){ return ! ::isxdigit(ch); });
            ustr->reserve(list.size());

            for(auto & hv : list)
                ustr->append(String::toInt(hv));
        }
        else
        {
            ustr->assign(str);
        }
        ll.pushString("size").pushInteger(ustr->size()).setTableIndex(-3);
    }

    return rescount;
}

int SWE_unicodestring_destroy(lua_State* L)
{
    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    if(ll.isTopUserData())
    {
        auto ptr = static_cast<SWE_UnicodeString**>(ll.getTopUserData());
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

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

/* http://www.lua.org/manual/5.1/manual.html */
#ifdef WITH_LUA
#include <sstream>

#include "engine.h"
#include "lua_wrapper.h"

#if LUA_VERSION_NUM >= 502
 #define LUA_GLOBALSINDEX LUA_REGISTRYINDEX
 #define lua_setfenv lua_setuservalue
 #define lua_getfenv lua_getuservalue
 #define luaL_getn(L,i) ((int)lua_rawlen(L, i))
#endif

#if LUA_VERSION_NUM <= 502
 #define lua_isinteger(L, i) ((int)lua_isnumber(L, i))
#endif

/* LuaState */
LuaState LuaState::newState(void)
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    return LuaState(L);
}

void LuaState::closeState(LuaState & ll)
{
    if(ll.ptr) lua_close(ll.ptr);
}

LuaState::LuaState(lua_State* L) : ptr(L)
{
    if(! ptr)
    {
	Engine::except(__FUNCTION__, "null pointer");
    }
}

int LuaState::version(void)
{
    const lua_Number* num = lua_version(ptr);
    return num ? *num : 0;
}

int LuaState::doFile(const std::string & file)
{
    return luaL_dofile(ptr, file.c_str());
}

int LuaState::doString(const std::string & str)
{
    return luaL_dostring(ptr, str.c_str());
}

bool LuaState::registerDirectory(const std::string & dir)
{
    if(getGlobalName("package").isTopTable())
    {
	if(getFieldTableIndex("path", -1).isTopString())
	{
	    std::string newPath = StringFormat("%1;%2").arg(toStringIndex(-1)).arg(Systems::concatePath(dir, "?.lua"));
	    stackPop();

	    if(Systems::isDirectory(dir))
	    {
		pushString(newPath);
		setFieldTableIndex("path", -2);
		return true;
	    }
	}
	else
	{
	    ERROR("field not found: " << "path");
	}
    }

    stackPop();
    return false;
}

bool LuaState::registerLibrary(const char* name, const luaL_Reg funcs[])
{
    lua_newtable(ptr);
    lua_setglobal(ptr, name);
    lua_getglobal(ptr, name);

    for(int it = 0; funcs[it].name; ++it)
    {
	lua_pushcfunction(ptr, funcs[it].func);
	lua_setfield(ptr, -2, funcs[it].name);
    }

    return true;
}

bool LuaState::isNilIndex(int index) const
{
    return lua_isnil(ptr, index);
}

bool LuaState::isBooleanIndex(int index) const
{
    return lua_isboolean(ptr, index);
}

bool LuaState::isNumberIndex(int index) const
{
    return lua_isnumber(ptr, index);
}

bool LuaState::isIntegerIndex(int index) const
{
    return lua_isinteger(ptr, index) || lua_isnumber(ptr, index);
}

bool LuaState::isStringIndex(int index) const
{
    return lua_isstring(ptr, index);
}

bool LuaState::isTableIndex(int index) const
{
    return lua_istable(ptr, index);
}

bool LuaState::isFunctionIndex(int index) const
{
    return lua_isfunction(ptr, index);
}

bool LuaState::isUserDataIndex(int index) const
{
    return lua_isuserdata(ptr, index);
}

bool LuaState::isLightUserDataIndex(int index) const
{
    return lua_islightuserdata(ptr, index);
}

bool LuaState::isThreadIndex(int index) const
{
    return lua_isthread(ptr, index);
}

bool LuaState::toBooleanIndex(int index) const
{
    return lua_toboolean(ptr, index);
}

double LuaState::toNumberIndex(int index) const
{
    return lua_tonumber(ptr, index);
}

int LuaState::toIntegerIndex(int index) const
{
    return lua_isinteger(ptr, index) ?
	lua_tointeger(ptr, index) :  lua_tonumber(ptr, index);
}

std::string LuaState::toStringIndex(int index) const
{
    return lua_tostring(ptr, index);
}

BinaryBuf LuaState::toBinaryIndex(int index) const
{
    const char* data = lua_tostring(ptr, index);

    lua_len(ptr, index);
    int size = lua_tointeger(ptr, -1);
    lua_pop(ptr, 1);

    return BinaryBuf(reinterpret_cast<const u8*>(data), size);
}

void* LuaState::toUserDataIndex(int index) const
{
    return lua_touserdata(ptr, index);
}

const void* LuaState::toPointerIndex(int index) const
{
    return lua_topointer(ptr, index);
}

lua_CFunction LuaState::toFunctionIndex(int index) const
{
    return lua_tocfunction(ptr, index);
}

lua_State* LuaState::toThreadIndex(int index) const
{
    return lua_tothread(ptr, index);
}

LuaState & LuaState::pushNil(void)
{
    lua_pushnil(ptr);
    return *this;
}

LuaState & LuaState::pushInteger(const int & value)
{
    lua_pushinteger(ptr, value);
    return *this;
}

LuaState & LuaState::pushNumber(const double & value)
{
    lua_pushnumber(ptr, value);
    return *this;
}

LuaState & LuaState::pushBoolean(const bool & val)
{
    lua_pushboolean(ptr, val);
    return *this;
}

LuaState & LuaState::pushString(const char* str)
{
    lua_pushstring(ptr, str);
    return *this;
}

LuaState & LuaState::pushFunction(lua_CFunction f)
{
    lua_pushcfunction(ptr, f);
    return *this;
}

LuaState & LuaState::pushString(const std::string & str)
{
    lua_pushstring(ptr, str.c_str());
    return *this;
}

LuaState & LuaState::pushLightUserData(void* data)
{
    lua_pushlightuserdata(ptr, data);
    return *this;
}

LuaState & LuaState::pushValueIndex(int index)
{
    lua_pushvalue(ptr, index);
    return *this;
}

void* LuaState::pushUserData(size_t sz)
{
    return lua_newuserdata(ptr, sz);
}

LuaState & LuaState::stackDump(void)
{
    dump();
    return *this;
}

LuaState & LuaState::stackDump(const std::string & label)
{
    dump(label.c_str());
    return *this;
}

int LuaState::dumpTable(int index)
{
    int counts = 0;

    if(isTableIndex(index))
    {
        std::string name = "table";

        if(getFieldTableIndex("__type", index, false).isTopString())
	    name = getTopString();

        // iterate
        pushNil();

        // fixed index
        if(0 > index) index -= 2;

        while(nextTableIndex(index))
        {
            VERBOSE(name << "[" << dumpValue(-2) << "] = " << dumpValue(-1));
            stackPop();
            counts++;
        }

        // remove name
        stackPop();
    }
    
    return counts;
}

std::string LuaState::dumpValue(int index)
{
    int type = getTypeIndex(index);
    const char* typeName = getTypeName(type);

    switch(type)
    {
        case LUA_TNIL:
            return typeName;

        case LUA_TSTRING:
            return StringFormat("%1(`%2')").arg(typeName).arg(toStringIndex(index));

        case LUA_TBOOLEAN:
            return StringFormat("%1(%2)").arg(typeName).arg(toBooleanIndex(index) ? "true" : "false");

        case LUA_TNUMBER:
            if(isIntegerIndex(index))
		return StringFormat("%1(%2)").arg(typeName).arg(toIntegerIndex(index), 6);
            else
		return StringFormat("%1(%2)").arg(typeName).arg(String::number(toNumberIndex(index), 6));

        case LUA_TTABLE:
        {
	    std::string name = "[]";

	    if(getFieldTableIndex("__type", index, false).isTopString())
        	name = getTopString();

    	    std::string str = StringFormat("%1(%2,%3)").arg(typeName).arg(name).arg(luaL_getn(L(), index));
            stackPop();
            return str;
        }

        case LUA_TTHREAD:
        case LUA_TFUNCTION:
        case LUA_TUSERDATA:
        case LUA_TLIGHTUSERDATA:
            return StringFormat("%1(%2)").arg(typeName).
			arg(String::hex64(reinterpret_cast<u64>(toPointerIndex(index))));

        default: break;
    }

    return StringFormat("unknown type: %1(%2)").arg(typeName).arg(type);
}

void LuaState::dump(const char* label)
{
    if(label) VERBOSE(label << ":");
    VERBOSE("+---------------------->");

    int index = stackSize();
    while(index)
    {
        VERBOSE("| " << index << ": " << dumpValue(index));
        if(isTableIndex(index)) dumpTable(index);
        index--;
    }

    VERBOSE("+----------------------<");
}

LuaState & LuaState::callFunction(int nargs, int nresults)
{
    lua_call(ptr, nargs, nresults);
    return *this;
}

LuaState & LuaState::getGlobalName(const std::string & name)
{
    lua_getglobal(ptr, name.c_str());
    if(lua_isnil(ptr, -1))
    {
	ERROR("global is nil: " << name);
    }
    return *this;
}

LuaState & LuaState::setGlobalName(const std::string & name)
{
    lua_setglobal(ptr, name.c_str());
    return *this;
}

LuaState & LuaState::pushTable(const std::string & path)
{
    StringList tables = String::split(path, '.');

    if(1 < tables.size())
    {
	lua_getglobal(ptr, tables.front().c_str());
	if(lua_isnil(ptr, -1))
	{
	    lua_pop(ptr, 1);

	    lua_newtable(ptr);
	    //lua_pushstring(ptr, tables.front().c_str());
	    //lua_setfield(ptr, -2, "__type");

	    lua_setglobal(ptr, tables.front().c_str());
	    lua_getglobal(ptr, tables.front().c_str());
	}
	else
	if(! lua_istable(ptr, -1))
	{
	    ERROR("global present, `" << lua_typename(ptr, lua_type(ptr, -1)) << "', name: " << tables.front());
	    lua_pop(ptr, 1);
	    lua_pushnil(ptr);
	    return *this;
	}

	StringList objects;
	objects.push_back(tables.front());
	tables.pop_front();

	while(tables.size())
	{
	    lua_getfield(ptr, -1, tables.front().c_str());
	    if(lua_istable(ptr, -1))
	    {
		objects.push_back(tables.front());
		tables.pop_front();
		lua_remove(ptr, -2);
	    }
	    else
	    if(lua_isnil(ptr, -1))
	    {
		lua_pop(ptr, 1);
		lua_pushstring(ptr, tables.front().c_str());
		lua_newtable(ptr);
		//std::string name = objects.join(".").append(".").append(tables.front());
		//lua_pushstring(ptr, name.c_str());
		//lua_setfield(ptr, -2, "__type");
		lua_settable(ptr, -3);
	    }
	    else
	    {
		ERROR("field present, `" << lua_typename(ptr, lua_type(ptr, -1)) << "', name: " << tables.front());
		lua_pop(ptr, 1);
		lua_pushnil(ptr);
		return *this;
	    }
	}

	return *this;
    }

    // checksingle object
    lua_getglobal(ptr, path.c_str());
    if(lua_isnil(ptr, -1))
    {
	lua_pop(ptr, 1);

	lua_newtable(ptr);
	//lua_pushstring(ptr, path.c_str());
	//lua_setfield(ptr, -2, "__type");

	lua_setglobal(ptr, path.c_str());
	lua_getglobal(ptr, path.c_str());
    }

    return *this;
}

LuaState & LuaState::pushTable(int narr, int nrec)
{
    lua_createtable(ptr, narr, nrec);
    return *this;
}

LuaState & LuaState::pushTable(void)
{
    lua_newtable(ptr);
    return *this;
}

LuaState & LuaState::setMetaTableIndex(int index)
{
    if(lua_istable(ptr, -1))
    {
	lua_setmetatable(ptr, index);
    }
    else
    {
	ERROR("metatable not found");
    }

    return *this;
}

LuaState & LuaState::setTableIndex(int index)
{
    if(lua_istable(ptr, index))
    {
	lua_settable(ptr, index);
    }
    else
    {
	ERROR("table not found, index: " << index);
    }

    return *this;
}

LuaState & LuaState::setFunctionsTableIndex(const luaL_Reg funcs[], int index)
{
    for(int it = 0; funcs[it].name; ++it)
	pushFunction(funcs[it].func).setFieldTableIndex(funcs[it].name, 0 < index ? index : index - 1);

    return *this;
}

LuaState & LuaState::getIndexTableIndex(int index, int tableIndex)
{
    lua_rawgeti(ptr, tableIndex, index);
    return *this;
}

LuaState & LuaState::getFieldTableIndex(const char* field, int index, bool verboseNil)
{
    if(lua_istable(ptr, index))
    {
	lua_getfield(ptr, index, field);

	if(lua_isnil(ptr, -1) && verboseNil)
	{
	    ERROR("field is nil: " << field);
	}
    }
    else
    {
	ERROR("table not found, index: " << index);
	lua_pushnil(ptr);
    }

    return *this;
}

LuaState & LuaState::getFieldTableIndex(const std::string & field, int index, bool verboseNil)
{
    return getFieldTableIndex(field.c_str(), index, verboseNil);
}

std::string LuaState::popFieldTableIndex(const std::string & field, int index)
{
    return popFieldTableIndex(field.c_str(), index);
}

std::string LuaState::popFieldTableIndex(const char* field, int index)
{
    std::string res;

    if(lua_istable(ptr, index))
    {
	lua_getfield(ptr, index, field);

	if(lua_isnil(ptr, -1))
	{
	    ERROR("field is nil: " << field);
	}
	else
	{
	    res = lua_tostring(ptr, -1);
	}

	lua_pop(ptr, 1);
    }
    else
    {
	ERROR("table not found, index: " << index);
    }

    return res;
}

LuaState & LuaState::setFieldTableIndex(const char* field, int index)
{
    if(lua_istable(ptr, index))
    {
	lua_setfield(ptr, index, field);
    }
    else
    {
	ERROR("table not found, index: " << index);
    }
    return *this;
}

LuaState & LuaState::setFieldTableIndex(const std::string & field, int index)
{
    return setFieldTableIndex(field.c_str(), index);
}

int LuaState::getTypeIndex(int index) const
{
    return lua_type(ptr, index);
}

const char* LuaState::getTypeName(int type) const
{
    return lua_typename(ptr, type);
}

int LuaState::nextTableIndex(int index)
{
    return lua_next(ptr, index);
}

int LuaState::countFieldsTableIndex(int index)
{
    int res = 0;

    lua_pushnil(ptr);
    while(lua_next(ptr, index - 1))
    {
	lua_pop(ptr, 1);
        res++;
    }

    return res;
}

int LuaState::stackTopIndex(void) const
{
    return lua_gettop(ptr);
}

LuaState & LuaState::stackReplaceIndex(int index)
{
    lua_replace(ptr, index);
    return *this;
}

LuaState & LuaState::stackRemoveIndex(int index)
{
    lua_remove(ptr, index);
    return *this;
}

LuaState & LuaState::stackTopIndex(int index)
{
    lua_settop(ptr, index);
    return *this;
}

LuaState & LuaState::stackInsertIndex(int index)
{
    lua_insert(ptr, index);
    return *this;
}

size_t LuaState::stackSize(void) const
{
    return lua_gettop(ptr);
}

void LuaState::stackClear(void)
{
    lua_settop(ptr, 0);
}

LuaState & LuaState::stackPop(size_t num)
{
    if(0 < num) lua_pop(ptr, num);
    return *this;
}

#endif

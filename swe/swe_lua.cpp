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

#ifdef SWE_WITH_LUA
#include <cmath>
#include <sstream>
#include <algorithm>

#include "swe_engine.h"
#include "swe_systems.h"
#include "swe_lua.h"

#if LUA_VERSION_NUM >= 502
 #define LUA_GLOBALSINDEX LUA_REGISTRYINDEX
 #define lua_setfenv lua_setuservalue
 #define lua_getfenv lua_getuservalue
 #define luaL_getn(L,i) ((int)lua_rawlen(L, i))
#else
#include "lua-5.1/c-api/compat-5.3.h"
#endif

#if LUA_VERSION_NUM < 503
int lua_isinteger(lua_State* L, int index)
{
    if(lua_isnumber(L, index))
    {
        double number = lua_tonumber(L, index);
        double intpart = 0;
        return 0.0 != std::modf(number, &intpart) ? 0 : 1;
    }

    return 0;
}
#endif

namespace SWE
{
    std::string stacktrace;

    /* LuaStateDebug */
    LuaStateDebug::LuaStateDebug(lua_State* L, const char* fname, int num) : LuaState(L), funcname(fname), top(-1), rescount(num)
    {
        if(L) top = lua_gettop(L);
    }

    LuaStateDebug::~LuaStateDebug()
    {
        if(LuaState::ptr && 0 <= top)
        {
            int size = lua_gettop(LuaState::ptr);
            if(top + rescount != size)
                COUT(SWE::String::time() << ": [" << "FIXME" << "]\t" << SWE::shortPrettyName(funcname) << ": " << "stack invalid, " << size - (top + rescount));
        }
    }

    /* LuaState */
    LuaState LuaState::newState(void)
    {
        lua_State* L = luaL_newstate();
        if(L) luaL_openlibs(L);

        return LuaState(L);
    }

    void LuaState::closeState(LuaState & ll)
    {
        if(ll.ptr) lua_close(ll.ptr);
    }

    LuaState::LuaState(lua_State* L) : ptr(L)
    {
        if(! ptr)
            Engine::except(__FUNCTION__, "null pointer");
    }

    const std::string & LuaState::stackTrace(void)
    {
        return stacktrace;
    }

    int LuaState::version(void)
    {
#if LUA_VERSION_NUM < 502
	return LUA_VERSION_NUM;
#elif LUA_VERSION_NUM < 504
        const lua_Number* num = lua_version(ptr);
        return num ? *num : 0;
#else
        return lua_version(ptr);
#endif
    }


    const char* luaStringError(int err)
    {
        switch(err)
        {
            case LUA_OK:        return "LUA_OK";
            case LUA_ERRRUN:    return "LUA_ERRRUN";
            case LUA_ERRMEM:    return "LUA_ERRMEM";
            case LUA_ERRERR:    return "LUA_ERRERR";
            case LUA_ERRGCMM:    return "LUA_ERRGCMM";
            default: break;
        }

        return "LUA_UNKNOWN";
    }

    int errorHandler(lua_State* L)
    {
	// param: origin error

	lua_getglobal(L, "debug");
	if(LUA_TTABLE != lua_type(L, -1))
	{
	    lua_pop(L, 1);
	    // return origin error
	    return 1;
	}

	lua_getfield(L, -1, "traceback");
	if(LUA_TFUNCTION != lua_type(L, -1))
	{
	    lua_pop(L, 2);
	    // return origin error
	    return 1;
	}

	// remove debug
	lua_remove(L, 2);

	int err = lua_pcall(L, 0, 1, 0);

	// stack: traceback, error
	if(err == LUA_OK)
        {
            stacktrace = lua_tostring(L, -1);
	    lua_pop(L, 1);
        }
        else
        {
            stacktrace = "pcall return: ";
            stacktrace.append(luaStringError(err));
        }

	return 1;
    }

    bool LuaState::doFile(const std::string & file)
    {
	int err = luaL_loadfile(ptr, file.c_str());
	if(LUA_OK != err)
	{
            DEBUG("loadfile return: " << luaStringError(err));
	    return false;
	}

	lua_pushcfunction(ptr, errorHandler);
        int findex = lua_absindex(ptr, -2);
	lua_insert(ptr, findex);

	bool res = LUA_OK == lua_pcall(ptr, 0, LUA_MULTRET, findex);
   
	// remove func
	lua_remove(ptr, findex);

	return res;
    }

    bool LuaState::doString(const std::string & str)
    {
	int err = luaL_loadstring(ptr, str.c_str());
	if(LUA_OK != err)
	{
            DEBUG("loadstring return: " << luaStringError(err));
	    return false;
	}

	lua_pushcfunction(ptr, errorHandler);
        int findex = lua_absindex(ptr, -2);
	lua_insert(ptr, findex);

	bool res = LUA_OK == lua_pcall(ptr, 0, LUA_MULTRET, findex);

	// remove func
	lua_remove(ptr, findex);

	return res;
    }

    bool LuaState::registerDirectory(const std::string & dir)
    {
	if(! Systems::isDirectory(dir))
	{
	    ERROR("dir not found: " << dir);
	    return false;
	}

        if(getGlobalName("package").isTopTable())
        {
            if(getFieldTableIndex("path", -1).isTopString())
            {
                std::string newPath = StringFormat("%1;%2").arg(toStringIndex(-1)).arg(Systems::concatePath(dir, "?.lua"));
                stackPop(); // field
                pushString(newPath).setFieldTableIndex("path", -2);
                stackPop(); // table
                return true;
            }
            else
	    {
                ERROR("field not found: " << "path");
    		stackPop(2);
	    }
        }
	else
	{
	    ERROR("table not found: " << "package");
    	    stackPop();
	}

        return false;
    }

    bool LuaState::registerLibrary(const std::string & libname, const luaL_Reg funcs[])
    {
        pushTable().setGlobalName(libname);

        if(getGlobalName(libname).isTopTable())
        {
            for(int it = 0; funcs[it].name; ++it)
                pushFunction(funcs[it].func).setFieldTableIndex(funcs[it].name, -2);

            return true;
        }
        else
            ERROR("name not found: " << libname);

        return false;
    }

    bool LuaState::isNoneIndex(int index) const
    {
        return lua_isnone(ptr, index);
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
        return lua_isinteger(ptr, index);
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

    bool LuaState::isTopNil(void) const
    {
        return isNilIndex(-1);
    }

    bool LuaState::isTopBoolean(void) const
    {
        return isBooleanIndex(-1);
    }

    bool LuaState::isTopNumber(void) const
    {
        return isNumberIndex(-1);
    }

    bool LuaState::isTopInteger(void) const
    {
        return isIntegerIndex(-1);
    }

    bool LuaState::isTopString(void) const
    {
        return isStringIndex(-1);
    }

    bool LuaState::isTopTable(void) const
    {
        return isTableIndex(-1);
    }

    bool LuaState::isTopUserData(void) const
    {
        return isUserDataIndex(-1);
    }

    bool LuaState::isTopLightUserData(void) const
    {
        return isLightUserDataIndex(-1);
    }

    bool LuaState::isTopFunction(void) const
    {
        return isFunctionIndex(-1);
    }

    bool LuaState::isTopThread(void) const
    {
        return isThreadIndex(-1);
    }

    bool LuaState::getTopBoolean(void)
    {
        return toBooleanIndex(-1);
    }

    double LuaState::getTopNumber(void)
    {
        return toNumberIndex(-1);
    }

    int LuaState::getTopInteger(void)
    {
        return toIntegerIndex(-1);
    }

    std::string LuaState::getTopString(void)
    {
        return toStringIndex(-1);
    }

    void* LuaState::getTopUserData(void)
    {
        return toUserDataIndex(-1);
    }

    const void* LuaState::getTopPointer(void)
    {
        return toPointerIndex(-1);
    }

    lua_CFunction LuaState::getTopFunction(void)
    {
        return toFunctionIndex(-1);
    }

    lua_State* LuaState::getTopThread(void)
    {
        return toThreadIndex(-1);
    }

    int LuaState::getTopType(void) const
    {
        return getTypeIndex(-1);
    }

    const char* LuaState::getTopTypeName(void) const
    {
        return getTypeName(getTopType());
    }

    bool LuaState::toBooleanIndex(int index) const
    {
        if(lua_isstring(ptr, index) &&
           0 == String::toLower(lua_tostring(ptr, index)).compare("false"))
            return false;

        if(lua_isinteger(ptr, index))
            return lua_tointeger(ptr, index);

        return lua_toboolean(ptr, index);
    }

    double LuaState::toNumberIndex(int index) const
    {
        if(lua_isboolean(ptr, index))
            return lua_toboolean(ptr, index) ? 1 : 0;

        return lua_tonumber(ptr, index);
    }

    int LuaState::toIntegerIndex(int index) const
    {
        if(lua_isboolean(ptr, index))
            return lua_toboolean(ptr, index) ? 1 : 0;

        return lua_isinteger(ptr, index) ?
               lua_tointeger(ptr, index) :  lua_tonumber(ptr, index);
    }

    std::string LuaState::toStringIndex(int index) const
    {
        int type = lua_type(ptr, index);

        switch(type)
        {
            case LUA_TNIL:
                return "nil";

            case LUA_TSTRING:
                return lua_tostring(ptr, index);

            case LUA_TBOOLEAN:
                return String::Bool(lua_toboolean(ptr, index));

            case LUA_TTABLE:
            case LUA_TTHREAD:
            case LUA_TFUNCTION:
            case LUA_TUSERDATA:
            case LUA_TLIGHTUSERDATA:
                return StringFormat("%1(%2)").arg(lua_typename(ptr, type)).arg(String::pointer(lua_topointer(ptr, index)));

            case LUA_TNUMBER:
            {
                double number = lua_tonumber(ptr, index);
                double intpart = 0;

                // return fixed double
                if(0.0 != std::modf(number, &intpart))
                    return String::number(number, 8);
                else
                    // return integer
                    return String::number(static_cast<int>(intpart));
            }

            default:
                ERROR("unknown value" << ", " << "type: " << lua_typename(ptr, type));
                break;
        }

        return "unknown value";
    }

    BinaryBuf LuaState::toBinaryIndex(int index) const
    {
        if(lua_isnumber(ptr, index))
            FIXME("lua_tolstring changes the actual value in the stack to a string" << ", " << "index: " << index);

        size_t len = 0;
        const char* data = lua_tolstring(ptr, index, & len);
        return BinaryBuf(reinterpret_cast<const u8*>(data), len);
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

    LuaState & LuaState::stackDump(bool expand)
    {
        VERBOSE("+----------------------> " << "stack");
        int index = stackSize();

        while(index)
        {
            VERBOSE("| " << index << ": " << dumpValue(index));
            index--;
        }

        VERBOSE("+----------------------< " << "stack");
        index = stackSize();

        if(expand)
        {
            while(index)
            {
                if(isTableIndex(index))
                    dumpTable(index);
                index--;
            }
        }

        return *this;
    }

    void LuaState::dumpTable(int index, int tabs)
    {
        static std::list<const void*> checkLoop;

        if(isTableIndex(index))
        {
            const std::string spaces(tabs * 2, 0x20);
            // fixed index
            index = toAbsIndex(index);
            const void* ptrTable = toPointerIndex(index);
            checkLoop.push_back(ptrTable);
            VERBOSE(spaces << "+----------------------> " << String::pointer(ptrTable));
            // iterate
            pushNil();

            while(nextTableIndex(index))
            {
                // key index: -2, value index: -1
                if(isTableIndex(-1))
                {
                    if(checkLoop.end() == std::find(checkLoop.begin(), checkLoop.end(), toPointerIndex(-1)))
                    {
                        VERBOSE(spaces << "[`" << toStringIndex(-2) << "'] = " << dumpValue(-1));
                        dumpTable(-1, tabs + 1);
                    }
                    else
                        VERBOSE(spaces << "[`" << toStringIndex(-2) << "'] = " << dumpValue(-1) << "->isref");
                }
                else
                    VERBOSE(spaces << "[`" << toStringIndex(-2) << "'] = " << dumpValue(-1));

                stackPop();
            }

            VERBOSE(spaces << "+----------------------< " << String::pointer(ptrTable));
            checkLoop.remove(ptrTable);
        }
        else
            ERROR("not table");
    }

    std::string LuaState::dumpValue(int index)
    {
        int type = getTypeIndex(index);

        switch(type)
        {
            case LUA_TSTRING:
                return StringFormat("%1(`%2')").arg(getTypeName(type)).arg(toStringIndex(index));

            case LUA_TBOOLEAN:
                return StringFormat("%1(%2)").arg(getTypeName(type)).arg(toStringIndex(index));

            case LUA_TNUMBER:
                if(isIntegerIndex(index))
                {
                    int val = toIntegerIndex(index);
                    return StringFormat("%1(%2,%3)").arg("integer").arg(val).arg(String::hex(val));
                }
                else
                    return StringFormat("%1(%2)").arg(getTypeName(type)).arg(toStringIndex(index));

            default:
                break;
        }

        return toStringIndex(index);
    }

    int LuaState::toAbsIndex(int index)
    {
        return lua_absindex(ptr, index);
    }

#ifdef SWE_WITH_JSON
    std::string LuaState::toJsonStringTableIndex(int index)
    {
        if(isSequenceTableIndex(index))
            return toJsonArrayTableIndex(-1).toString();

        return toJsonObjectTableIndex(-1).toString();
    }

    JsonArray LuaState::toJsonArrayTableIndex(int index)
    {
        JsonArray res;
        static std::list<const void*> checkLoop;

        if(isTableIndex(index))
        {
            // fixed index
            index = toAbsIndex(index);
            const void* ptrTable = toPointerIndex(index);
            checkLoop.push_back(ptrTable);
            // iterate
            pushNil();

            while(nextTableIndex(index))
            {
                // stack add: key, value
                int valueType = getTypeIndex(-1);
                const char* typeName = getTypeName(valueType);

                switch(valueType)
                {
                    case LUA_TNIL:
                        break;

                    case LUA_TSTRING:
                        res.addString(toStringIndex(-1));
                        break;

                    case LUA_TNUMBER:
                        if(isIntegerIndex(-1))
                            res.addInteger(toIntegerIndex(-1));
                        else
                            res.addDouble(toNumberIndex(-1));

                        break;

                    case LUA_TBOOLEAN:
                        res.addBoolean(toBooleanIndex(-1));
                        break;

                    case LUA_TTHREAD:
                    case LUA_TFUNCTION:
                    case LUA_TUSERDATA:
                    case LUA_TLIGHTUSERDATA:
                        res.addString(StringFormat("%1(%2)").arg(typeName).arg(String::pointer(toPointerIndex(-1))));
                        break;

                    case LUA_TTABLE:
                        if(checkLoop.end() == std::find(checkLoop.begin(), checkLoop.end(), toPointerIndex(-1)))
                        {
                            if(isSequenceTableIndex(-1))
                                res.addArray(toJsonArrayTableIndex(-1));
                            else
                                res.addObject(toJsonObjectTableIndex(-1));
                        }

                        break;

                    default:
                        ERROR("unknown type: " << typeName);
                        break;
                }

                // remove value
                stackPop();
            }

            checkLoop.remove(ptrTable);
        }
        else
            ERROR("table not found, index: " << index);

        return res;
    }

    JsonObject LuaState::toJsonObjectTableIndex(int index)
    {
        JsonObject res;
        static std::list<const void*> checkLoop;

        if(isTableIndex(index))
        {
            // fixed index
            index = toAbsIndex(index);
            const void* ptrTable = toPointerIndex(index);
            checkLoop.push_back(ptrTable);
            // iterate
            pushNil();

            while(nextTableIndex(index))
            {
                // stack add: key, value
                std::string key = toStringIndex(-2);
                int valueType = getTypeIndex(-1);
                const char* typeName = getTypeName(valueType);

                switch(valueType)
                {
                    case LUA_TNIL:
                        res.addNull(key);
                        break;

                    case LUA_TSTRING:
                        res.addString(key, toStringIndex(-1));
                        break;

                    case LUA_TNUMBER:
                        if(isIntegerIndex(-1))
                            res.addInteger(key, toIntegerIndex(-1));
                        else
                            res.addDouble(key, toNumberIndex(-1));

                        break;

                    case LUA_TBOOLEAN:
                        res.addBoolean(key, toBooleanIndex(-1));
                        break;

                    case LUA_TTHREAD:
                    case LUA_TFUNCTION:
                    case LUA_TUSERDATA:
                    case LUA_TLIGHTUSERDATA:
                        res.addString(key, StringFormat("%1(%2)").arg(typeName).arg(String::pointer(toPointerIndex(-1))));
                        break;

                    case LUA_TTABLE:
                        if(checkLoop.end() == std::find(checkLoop.begin(), checkLoop.end(), toPointerIndex(-1)))
                        {
                            if(isSequenceTableIndex(-1))
                                res.addArray(key, toJsonArrayTableIndex(-1));
                            else
                                res.addObject(key, toJsonObjectTableIndex(-1));
                        }
                        else
                            res.addString(key, "reference to table");

                        break;

                    default:
                        ERROR("unknown type: " << typeName);
                        break;
                }

                // remove value
                stackPop();
            }

            checkLoop.remove(ptrTable);
        }
        else
            ERROR("table not found, index: " << index);

        return res;
    }
#endif

    LuaState & LuaState::callFunction(int nargs, int nresults)
    {
        lua_call(ptr, nargs, nresults);
        return *this;
    }

    LuaState & LuaState::getGlobalName(const std::string & name)
    {
        lua_getglobal(ptr, name.c_str());

        if(lua_isnil(ptr, -1))
            ERROR("global is nil: " << name);

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
                lua_setglobal(ptr, tables.front().c_str());
                lua_getglobal(ptr, tables.front().c_str());
            }
            else if(! lua_istable(ptr, -1))
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
                // change getfield, disable meta
                lua_pushstring(ptr, tables.front().c_str());
                lua_rawget(ptr, -2);

                if(lua_istable(ptr, -1))
                {
                    objects.push_back(tables.front());
                    tables.pop_front();
                    lua_remove(ptr, -2);
                }
                else if(lua_isnil(ptr, -1))
                {
                    lua_pop(ptr, 1);
                    lua_pushstring(ptr, tables.front().c_str());
                    lua_newtable(ptr);
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
            lua_setmetatable(ptr, index);
        else
            ERROR("metatable not found");

        return *this;
    }

    LuaState & LuaState::getTableIndex(int index)
    {
        if(lua_istable(ptr, index))
            lua_gettable(ptr, index);
        else
            ERROR("table not found, index: " << index);

        return *this;
    }

    LuaState & LuaState::setTableIndex(int index)
    {
        if(lua_istable(ptr, index))
            lua_settable(ptr, index);
        else
            ERROR("table not found, index: " << index);

        return *this;
    }

    LuaState & LuaState::setFunctionsTableIndex(const luaL_Reg funcs[], int index)
    {
        if(index < 0)
            index = lua_absindex(ptr, index);

        for(int it = 0; funcs[it].name; ++it)
        {
            if(funcs[it].func)
                pushFunction(funcs[it].func).setFieldTableIndex(funcs[it].name, index);
            else
                pushNil().setFieldTableIndex(funcs[it].name, index);
        }

        return *this;
    }

    bool LuaState::isFieldTableIndex(const char* field, int index)
    {
        if(index < 0)
            index = lua_absindex(ptr, index);

        if(lua_istable(ptr, index))
        {
            lua_pushstring(ptr, field);
            lua_rawget(ptr, index);

            bool res = ! lua_isnil(ptr, -1);
            lua_pop(ptr, 1);
            return res;
        }
        else
        {
            ERROR("table not found, index: " << index);
            lua_pushnil(ptr);
        }

        return false;
    }

    bool LuaState::isFieldTableIndex(const std::string & field, int index)
    {
        return isFieldTableIndex(field.c_str(), index);
    }

    LuaState & LuaState::getFieldTableIndex(const char* field, int index, bool verboseNil)
    {
        if(index < 0)
            index = lua_absindex(ptr, index);

        if(lua_istable(ptr, index))
        {
            lua_pushstring(ptr, field);
            lua_rawget(ptr, index);

            if(lua_isnil(ptr, -1) && verboseNil)
                ERROR("field is nil: " << field);
        }
        else
        {
            ERROR("table not found, index: " << index);
            lua_pushnil(ptr);
        }

        return *this;
    }

    int LuaState::absTableIndex(int index)
    {
        return lua_absindex(ptr, index);
    }

    LuaState & LuaState::getFieldTableIndex(const std::string & field, int index, bool verboseNil)
    {
        return getFieldTableIndex(field.c_str(), index, verboseNil);
    }

    LuaState & LuaState::getIndexTableIndex(unsigned int seqindex, int index, bool verboseNil)
    {
        if(index < 0)
            index = lua_absindex(ptr, index);

        if(lua_istable(ptr, index))
        {
	    lua_rawgeti(ptr, index, seqindex);

            if(lua_isnil(ptr, -1) && verboseNil)
                ERROR("seq index is nil: " << seqindex);
        }
        else
        {
            ERROR("table not found, index: " << index);
            lua_pushnil(ptr);
        }

        return *this;
    }

    std::string LuaState::popFieldTableIndex(const std::string & field, int index)
    {
        return popFieldTableIndex(field.c_str(), index);
    }

    std::string LuaState::popFieldTableIndex(const char* field, int index)
    {
        std::string res;

        if(index < 0)
            index = lua_absindex(ptr, index);

        if(lua_istable(ptr, index))
        {
            lua_pushstring(ptr, field);
            lua_rawget(ptr, index);

            if(lua_isnil(ptr, -1))
	    {
                ERROR("field is nil: " << field);
            }
	    else
                res = lua_tostring(ptr, -1);

            lua_pop(ptr, 1);
        }
        else
            ERROR("table not found, index: " << index);

        return res;
    }

    LuaState & LuaState::setFieldTableIndex(const char* field, int index)
    {
        if(index < 0)
            index = lua_absindex(ptr, index);

        if(lua_istable(ptr, index))
            lua_setfield(ptr, index, field);
        else
            ERROR("table not found, index: " << index);

        return *this;
    }

    LuaState & LuaState::setFieldTableIndex(const std::string & field, int index)
    {
        return setFieldTableIndex(field.c_str(), index);
    }

    LuaState & LuaState::setIndexTableIndex(unsigned int seqindex, int index)
    {
        if(index < 0)
            index = lua_absindex(ptr, index);

        if(lua_istable(ptr, index))
            lua_rawseti(ptr, index, seqindex);
        else
            ERROR("table not found, index: " << index);

        return *this;
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
        if(index < 0)
            index = lua_absindex(ptr, index);

        int res = 0;
        lua_pushnil(ptr);

        while(lua_next(ptr, index))
        {
            lua_pop(ptr, 1);
            res++;
        }

        return res;
    }

    bool LuaState::isSequenceTableIndex(int index)
    {
        if(index < 0)
            index = lua_absindex(ptr, index);

        // fast check
        lua_len(ptr, index);
        int len = lua_tointeger(ptr, -1);
        lua_pop(ptr, 1);

        if(0 == len)
            return false;

        int prev = 0;
        lua_pushnil(ptr);

        while(lua_next(ptr, index))
        {
            // value
            lua_pop(ptr, 1);

            // key
            if(! lua_isinteger(ptr, -1))
            {
                lua_pop(ptr, 1);
                return false;
            }

            // next key is sequence
            int next = lua_tointeger(ptr, -1);
            if(next - prev != 1)
            {
                lua_pop(ptr, 1);
                return false;
            }

            prev = next;
        }

        return true;
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
        if(index < 0)
            index = lua_absindex(ptr, index);

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
        if(index < 0)
            index = lua_absindex(ptr, index);

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

    LuaState & LuaState::garbageCollect(int mode)
    {
        lua_gc(ptr, mode, 0);
        return *this;
    }
}
#endif

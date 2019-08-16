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

#ifndef _SWE_LUAWRAPPER_
#define _SWE_LUAWRAPPER_

#include <string>
#include <unordered_map>

#include <lua.hpp>

class LuaState
{
    lua_State*		ptr;

    void		dump(const char* label = NULL);
    int			dumpTable(int index);
    std::string		dumpValue(int index);

public:
    LuaState(lua_State*);

    static LuaState	newState(void);
    static void		closeState(LuaState &);

    int			doFile(const std::string &);
    int			doString(const std::string &);

    lua_State*		L(void) { return ptr; }
    LuaState & 		callFunction(int nargs, int nresults);

    // lib
    bool		registerLibrary(const char*, const luaL_Reg[]);
    bool		registerDirectory(const std::string &);

    // check type
    bool		isNilIndex(int) const;
    bool		isBooleanIndex(int) const;
    bool		isNumberIndex(int) const;
    bool		isIntegerIndex(int) const;
    bool		isStringIndex(int) const;
    bool		isTableIndex(int) const;
    bool		isUserDataIndex(int) const;
    bool		isLightUserDataIndex(int) const;
    bool		isFunctionIndex(int) const;
    bool		isThreadIndex(int) const;

    bool		isTopNil(void) const { return isNilIndex(-1); }
    bool		isTopBoolean(void) const { return isBooleanIndex(-1); }
    bool		isTopNumber(void) const { return isNumberIndex(-1); }
    bool		isTopInteger(void) const { return isIntegerIndex(-1); }
    bool		isTopString(void) const { return isStringIndex(-1); }
    bool		isTopTable(void) const { return isTableIndex(-1); }
    bool		isTopUserData(void) const { return isUserDataIndex(-1); }
    bool		isTopLightUserData(void) const { return isLightUserDataIndex(-1); }
    bool		isTopFunction(void) const { return isFunctionIndex(-1); }
    bool		isTopThread(void) const { return isThreadIndex(-1); }

    // convert type
    bool		toBooleanIndex(int) const;
    double		toNumberIndex(int) const;
    int			toIntegerIndex(int) const;
    std::string		toStringIndex(int) const;
    void*		toUserDataIndex(int) const;
    const void* 	toPointerIndex(int) const;
    lua_CFunction	toFunctionIndex(int) const;
    lua_State*		toThreadIndex(int) const;

    bool		getTopBoolean(void) { return toBooleanIndex(-1); }
    double		getTopNumber(void) { return toNumberIndex(-1); }
    int			getTopInteger(void) { return toIntegerIndex(-1); }
    std::string		getTopString(void) { return toStringIndex(-1); }
    void*		getTopUserData(void) { return toUserDataIndex(-1); }
    const void* 	getTopPointer(void) { return toPointerIndex(-1); }
    lua_CFunction	getTopFunction(void) { return toFunctionIndex(-1); }
    lua_State*		getTopThread(void) { return toThreadIndex(-1); }

    // push value: push().push().push()
    LuaState &		pushNil(void);
    LuaState &		pushBoolean(const bool &);
    LuaState &		pushInteger(const int &);
    LuaState &		pushNumber(const double &);
    LuaState &		pushString(const std::string &);
    LuaState &		pushString(const char*);
    LuaState &		pushFunction(lua_CFunction);
    LuaState &		pushLightUserData(void*);
    LuaState &		pushValueIndex(int);

    void*		pushUserData(size_t);

    // global
    LuaState &		getGlobalName(const std::string &);
    LuaState &		setGlobalName(const std::string &);

    // return type
    int			getTypeIndex(int) const;
    const char* 	getTypeName(int type) const;

    int			getTopType(void) const { return getTypeIndex(-1); }
    const char*		getTopTypeName(void) const { return getTypeName(getTopType()); }

    // table manip
    int			nextTableIndex(int);
    int			countFieldsTableIndex(int);

    LuaState &		getIndexTableIndex(int, int);
    LuaState &		getFieldTableIndex(const std::string &, int, bool verboseNil = true);
    LuaState &		setFieldTableIndex(const std::string &, int);

    LuaState &		getFieldTableIndex(const char*, int, bool verboseNil = true);
    LuaState &		setFieldTableIndex(const char*, int);

    std::string		popFieldTableIndex(const std::string &, int);
    std::string		popFieldTableIndex(const char*, int);

    LuaState &		setTableIndex(int);
    LuaState &		setMetaTableIndex(int);
    LuaState &		setFunctionsTableIndex(const luaL_Reg[], int);

    LuaState & 		pushTable(void);
    LuaState & 		pushTable(int narr, int nrec);
    LuaState & 		pushTable(const std::string & path);

    // stack manipulation
    size_t		stackSize(void) const;
    int			stackTopIndex(void) const;
    void		stackClear(void);
    LuaState &		stackReplaceIndex(int);
    LuaState &		stackInsertIndex(int);
    LuaState &		stackRemoveIndex(int);
    LuaState &		stackTopIndex(int);
    LuaState &		stackDump(const std::string &);
    LuaState &		stackDump(void);
    LuaState &		stackPop(size_t num = 1);
};

#endif

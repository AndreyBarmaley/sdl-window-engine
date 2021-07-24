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

#ifdef SWE_WITH_LUA
#include <string>
#include <unordered_map>

#ifdef SWE_WITH_JSON
#include "swe_json_ext.h"
#endif

#include "lua.hpp"

/// @brief пространство SWE
namespace SWE
{

    class LuaState
    {
    protected:
        lua_State*	ptr;

        void		dumpTable(int index, int tabs = 0);
        std::string	dumpValue(int index);

    public:
        LuaState(lua_State*);
        virtual ~LuaState(){}

        static LuaState	newState(void);
        static void	closeState(LuaState &);

        int		version(void);
        bool		doFile(const std::string &);
        bool		doString(const std::string &);

        lua_State*	L(void) { return ptr; }
        LuaState  &	callFunction(int nargs, int nresults);

        // lib
        bool		registerLibrary(const std::string &, const luaL_Reg[]);
        bool		registerDirectory(const std::string &);

        // check type
        bool		isNoneIndex(int) const;
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

        bool		isTopNil(void) const;
        bool		isTopBoolean(void) const;
        bool		isTopNumber(void) const;
        bool		isTopInteger(void) const;
        bool		isTopString(void) const;
        bool		isTopTable(void) const;
        bool		isTopUserData(void) const;
        bool		isTopLightUserData(void) const;
        bool		isTopFunction(void) const;
        bool		isTopThread(void) const;

        // convert type
        bool		toBooleanIndex(int) const;
        double		toNumberIndex(int) const;
        int		toIntegerIndex(int) const;
        std::string	toStringIndex(int) const;
        void*		toUserDataIndex(int) const;
        const void* 	toPointerIndex(int) const;
        lua_CFunction	toFunctionIndex(int) const;
        lua_State*	toThreadIndex(int) const;
        BinaryBuf	toBinaryIndex(int) const;

        bool		getTopBoolean(void);
        double		getTopNumber(void);
        int		getTopInteger(void);
        std::string	getTopString(void);
        void*		getTopUserData(void);
        const void* 	getTopPointer(void);
        lua_CFunction	getTopFunction(void);
        lua_State*	getTopThread(void);

        // push value: push().push().push()
        LuaState &	pushNil(void);
        LuaState &	pushBoolean(const bool &);
        LuaState &	pushInteger(const int &);
        LuaState &	pushNumber(const double &);
        LuaState &	pushString(const std::string &);
        LuaState &	pushString(const char*);
        LuaState &	pushFunction(lua_CFunction);
        LuaState &	pushLightUserData(void*);
        LuaState &	pushValueIndex(int);

        void*		pushUserData(size_t);

        // global
        LuaState &	getGlobalName(const std::string &);
        LuaState &	setGlobalName(const std::string &);
        int		toAbsIndex(int);

        // return type
        int		getTypeIndex(int) const;
        const char* 	getTypeName(int type) const;

        int		getTopType(void) const;
        const char*	getTopTypeName(void) const;

        // table manip
        int		nextTableIndex(int);
        int		countFieldsTableIndex(int);
        bool		isSequenceTableIndex(int);
        bool            isFieldTableIndex(const char*, int);
        bool            isFieldTableIndex(const std::string &, int);

#ifdef SWE_WITH_JSON
        std::string	toJsonStringTableIndex(int);
        JsonArray	toJsonArrayTableIndex(int);
        JsonObject	toJsonObjectTableIndex(int);
#endif

        int             absTableIndex(int);

        LuaState &	getIndexTableIndex(unsigned int seqindex, int tableindex, bool verboseNil = true);
        LuaState &	setIndexTableIndex(unsigned int seqindex, int tableindex);

        LuaState &	getFieldTableIndex(const std::string &, int, bool verboseNil = true);
        LuaState &	setFieldTableIndex(const std::string &, int);

        LuaState &	getFieldTableIndex(const char*, int, bool verboseNil = true);
        LuaState &	setFieldTableIndex(const char*, int);

        std::string	popFieldTableIndex(const std::string &, int);
        std::string	popFieldTableIndex(const char*, int);

        LuaState &	getTableIndex(int);
        LuaState &	setTableIndex(int);

        LuaState &	setMetaTableIndex(int);
        LuaState &	setFunctionsTableIndex(const luaL_Reg[], int);

        LuaState  &	pushTable(void);
        LuaState  &	pushTable(int narr, int nrec);
        LuaState  &	pushTable(const std::string & path);

        // stack manipulation
        size_t		stackSize(void) const;
        int		stackTopIndex(void) const;
        void		stackClear(void);
        LuaState &	stackReplaceIndex(int);		// replace top to stack index
        LuaState &	stackInsertIndex(int);		// insert top to stack index
        LuaState &	stackRemoveIndex(int);		// remove stack index
        LuaState &	stackTopIndex(int);		// resize stack, set new top index
        LuaState &	stackDump(bool expand = true);
        LuaState &	stackPop(size_t num = 1);

        static const std::string &
                        stackTrace(void);

        LuaState &	garbageCollect(int mode = LUA_GCCOLLECT);
    };

    class LuaStateDebug : public LuaState
    {
        const char*     funcname;
        int             top;
        int             rescount;

    public:
        LuaStateDebug(lua_State*, const char* funcname, int num = 0);
        ~LuaStateDebug();
    };

} // SWE

#endif
#endif

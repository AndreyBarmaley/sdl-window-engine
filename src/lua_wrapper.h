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

#ifdef WITH_LUA
#include <string>
#include <utility>
#include <memory>
#include <unordered_map>

#include <lua.hpp>

namespace Lua
{
    enum Type
    {
	TypeNil			= LUA_TNIL,
	TypeNumber		= LUA_TNUMBER,
	TypeBool 		= LUA_TBOOLEAN,
	TypeString		= LUA_TSTRING,
	TypeTable		= LUA_TTABLE,
	TypeFunction		= LUA_TFUNCTION,
	TypeUserData		= LUA_TUSERDATA,
	TypeThread		= LUA_TTHREAD,
	TypeLightUserData	= LUA_TLIGHTUSERDATA
    };

    const char* typeString(int);

    struct UserDataWrapper
    {
	void*	data;
	UserDataWrapper(void* v = NULL) : data(NULL) {}
    };

    class BaseValue
    {
    protected:
	int	luaType;
	void*	luaValue;

    public:
	BaseValue() : luaType(Lua::TypeNil), luaValue(NULL) {}
	BaseValue(int t, void* v = NULL) : luaType(t), luaValue(v) {}
	virtual ~BaseValue() {}

	bool			isNil(void) const { return type() == Lua::TypeNil; }
	bool			isValid(void) const { return ! isNil(); }
	bool			isNumber(void) const { return type() == Lua::TypeNumber; }
	bool			isBool(void) const { return type() == Lua::TypeBool; }
	bool			isString(void) const { return type() == Lua::TypeString; }
	bool			isFunction(void) const { return type() == Lua::TypeFunction; }
	bool			isTable(void) const { return type() == Lua::TypeTable; }
	bool			isThread(void) const { return type() == Lua::TypeThread; }
	bool			isUserData(void) const { return type() == Lua::TypeUserData; }
	bool			isLightUserData(void) const { return type() == Lua::TypeLightUserData; }

	int			type(void) const { return luaType; }
	void			setType(int val) { luaType = val; }

	virtual void		destroy(void) {}
        virtual std::string	getString(void) const;
	virtual s32		getInt(void) const { return 0; }
	virtual s64		getInt64(void) const { return 0; }
	virtual bool		getBool(void) const { return false; }
	virtual double		getDouble(void) const { return 0; }
	virtual lua_CFunction	getFunction(void) const { return NULL; }
	virtual void*		getUserData(void) const { return NULL; }
    };

    struct StringValue : public BaseValue
    {
	StringValue(const std::string &);
	~StringValue() { destroy(); }

	void			setString(const std::string &);

	void			destroy(void);
	std::string		getString(void) const;
	s32			getInt(void) const;
	s64			getInt64(void) const;
	bool			getBool(void) const;
	double			getDouble(void) const;
    };

    struct FunctionValue : public BaseValue
    {
	FunctionValue(lua_CFunction);

	void                    setFunction(lua_CFunction);
	lua_CFunction		getFunction(void) const;
    };

    struct UserDataValue : public BaseValue
    {
	UserDataValue(void* val) : BaseValue(TypeUserData) { setUserData(val); }

	void                    setUserData(void* val) { luaValue = val; }
	void*			getUserData(void) const { return luaValue; }
    };

    struct NumberValue : public BaseValue
    {
	NumberValue(double);
	~NumberValue() { destroy(); }

	void			setNumber(double);

	void			destroy(void);
	std::string		getString(void) const;
	s32			getInt(void) const;
	s64			getInt64(void) const;
	bool			getBool(void) const;
	double			getDouble(void) const;
    };

    struct BoolValue : public BaseValue
    {
	BoolValue(bool);
	~BoolValue() { destroy(); }

	void			setBool(bool);

	std::string		getString(void) const;
	s32			getInt(void) const;
	s64			getInt64(void) const;
	bool			getBool(void) const;
	double			getDouble(void) const;
    };

    const BaseValue & operator>> (const BaseValue &, std::string &);
    const BaseValue & operator>> (const BaseValue &, s32 &);
    const BaseValue & operator>> (const BaseValue &, s64 &);
    const BaseValue & operator>> (const BaseValue &, bool &);
    const BaseValue & operator>> (const BaseValue &, double &);

    class TableValue : protected std::unordered_map<std::string, BaseValue*>
    {
    public:
	TableValue() {}
	~TableValue()
	{
	    for(auto it = begin(); it != end(); ++it)
		delete (*it).second;
	}

	void push(const std::string & key, BaseValue* val);

	const BaseValue* value(const std::string & key) const
	{
    	    auto it = find(key);
    	    return it != end() ? (*it).second : NULL;
	}

	template<typename T>
	T get(const std::string & key) const
	{
    	    T val;
	    const BaseValue* baseVal = value(key);
            if(baseVal) *baseVal >> val;
	    return val;
	}
    };

    void StackDump(lua_State*, const char* label = NULL);

    class State
    {
    protected:
	std::shared_ptr<lua_State>
                    		ptr;
	bool			isconst;
	int			next(int);

    public:
        State();

	int			doFile(const std::string &);
	int			doString(const std::string &);
	void			loadDirectory(const std::string &);
	void			call(int, int);


	BaseValue*		getValue(int index) const;
	void			tableDump(const std::string &) const;
	StringList		tableKeys(const std::string &) const;
	//std::string		tableValue(const std::string &, const std::string &) const;
	TableValue		findTable(const std::string &);

	template<typename T>
	T getField(const std::string & global, const std::string & field)
	{
	    T res;
	    pushGlobal(global);
	    getField(field);
	    BaseValue* bval = NULL;
	    switch(typeIndex(-1))
	    {
		case TypeString:	bval = new StringValue(toStringIndex(-1)); break;
		case TypeNumber:	bval = new NumberValue(toNumberIndex(-1)); break;
		case TypeBool:		bval = new BoolValue(toBoolIndex(-1)); break;
		case TypeFunction:	bval = new FunctionValue(toFunctionIndex(-1)); break;
		case TypeUserData:	bval = new UserDataValue(toUserDataIndex(-1)); break;
		case TypeTable:		bval = new BaseValue(Type::TypeTable); break;
		case TypeThread:	bval = new BaseValue(Type::TypeThread); break;
		case TypeLightUserData:	bval = new BaseValue(Type::TypeLightUserData); break;
	        case TypeNil:		bval = new BaseValue(); break;
		default: break;
	    }
	    if(bval)
	    {
		*bval >> res;
		delete bval;
	    }
	    stackPopElements(2);
	    return res;
	}

	/* stack manipulation */
	int			stackTopIndex(void) const;
	void			stackTopIndex(int);
        size_t			stackSize(void) const;
	bool			stackCheckFreeSlots(int) const;
	void			stackDump(const std::string &) const;
	void			stackDump(void) const;
	void			stackClear(void);
	void			stackRemoveIndex(int);
	void			stackTopMoveIndex(int);
	void			stackTopReplaceIndex(int);
	void			stackPopElement(void);
	void			stackPopElements(int count);

	void			getField(const std::string &, int index = -1) const;
	void			setField(const std::string &, int index = -1);

	void			pushValueIndex(int);
	void			pushGlobal(const std::string &);
	void			pushNil(void);
	void			pushBool(bool);
	void			pushInteger(int);
	void			pushNumber(double);
	void			pushString(const std::string &);

        bool			isNil(int) const;
        bool			isBool(int) const;
        bool			isNumber(int) const;
        bool			isString(int) const;
        bool			isTable(int) const;
        bool			isFunction(int) const;
        bool			isCFunction(int) const;
        bool			isUserData(int) const;
	bool			isLightUserData(int) const;

	int			typeIndex(int) const;
	std::string 		typeName(int) const;

	bool			isEqual(int, int) const;
        bool			isRawEqual(int, int) const;
	bool			isLessThan(int, int) const;

	s64			toIntegerIndex(int) const;
	bool			toBoolIndex(int) const;
	double			toNumberIndex(int) const;
	std::string		toStringIndex(int) const;
	void*			toUserDataIndex(int) const;
	const void* 		toPointerIndex(int) const;
	lua_CFunction		toFunctionIndex(int) const;
	lua_State*		toThreadIndex(int) const;
	lua_State*		toLuaState(void);
    };
}

#endif
#endif

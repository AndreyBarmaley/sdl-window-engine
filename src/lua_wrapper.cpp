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
#include "systems.h"
#include "lua_wrapper.h"

namespace Lua
{
    const BaseValue & operator>> (const BaseValue & ltv, std::string & val) { val = ltv.getString(); return ltv; }
    const BaseValue & operator>> (const BaseValue & ltv, s32 & val) { val = ltv.getInt(); return ltv; }
    const BaseValue & operator>> (const BaseValue & ltv, s64 & val) { val = ltv.getInt64(); return ltv; }
    const BaseValue & operator>> (const BaseValue & ltv, bool & val) { val = ltv.getBool(); return ltv; }
    const BaseValue & operator>> (const BaseValue & ltv, double & val) { val = ltv.getDouble(); return ltv; }
    const BaseValue & operator>> (const BaseValue & ltv, lua_CFunction & val) { val = ltv.getFunction(); return ltv; }
    const BaseValue & operator>> (const BaseValue & ltv, UserDataWrapper & val) { val.data = ltv.getUserData(); return ltv; }

    const char* typeString(int type)
    {
	switch(type)
	{
    	    case TypeNumber:		return "number";
    	    case TypeBool:		return "bool";
    	    case TypeString:		return "string";
    	    case TypeFunction:		return "function";
    	    case TypeTable:		return "table";
    	    case TypeThread:		return "thread";
    	    case TypeUserData:		return "userdata";
    	    case TypeLightUserData: 	return "lightuserdata";
    	    case TypeNil:		return "nil";
    	    default: break;
	}

	return "unknown";
    }
}

std::string Lua::BaseValue::getString(void) const
{
    return typeString(type());
}

/* Lua::StringValue */
Lua::StringValue::StringValue(const std::string & str) : Lua::BaseValue(Lua::TypeString)
{
    setString(str);
}

void Lua::StringValue::destroy(void)
{
    delete static_cast<std::string*>(luaValue);
    luaValue = NULL;
}

void Lua::StringValue::setString(const std::string & str)
{
    destroy();
    luaValue = new std::string(str);
}

std::string Lua::StringValue::getString(void) const
{
    std::string* str = static_cast<std::string*>(luaValue);
    return *str;
}

s64 Lua::StringValue::getInt64(void) const
{
    return static_cast<s64>(getDouble());
}

s32 Lua::StringValue::getInt(void) const
{
    bool isok = false;
    int res = String::toInt(getString(), &isok);
    if(! isok)
    {
        ERROR("convert error");
        return 0;
    }
    return res;
}

bool Lua::StringValue::getBool(void) const
{
    const std::string & str = getString();
    return str.empty() || String::toLower(str) == "false" || str == "0" ? false : true;
}

double Lua::StringValue::getDouble(void) const
{
    bool isok = false;
    double res = String::toDouble(getString(), &isok);
    if(! isok)
    {
        ERROR("convert error");
        return 0;
    }
    return res;
}

/* Lua::NumberValue */
Lua::NumberValue::NumberValue(double val) : Lua::BaseValue(Lua::TypeNumber)
{
    setNumber(val);
}

void Lua::NumberValue::destroy(void)
{
    delete static_cast<double*>(luaValue);
    luaValue = NULL;
}

void Lua::NumberValue::setNumber(double val)
{
    destroy();
    luaValue = new double(val);
}

std::string Lua::NumberValue::getString(void) const
{
    return String::number(getDouble(), 6);
}

s64 Lua::NumberValue::getInt64(void) const
{
    double val = getDouble();
    if(val < static_cast<double>(LLONG_MIN))
    {
        ERROR("int overflow");
        return LLONG_MIN;
    }
    if(val > static_cast<double>(LLONG_MAX))
    {
        ERROR("int overflow");
        return LLONG_MAX;
    }
    return static_cast<s64>(val);
}

s32 Lua::NumberValue::getInt(void) const
{
    double val = getDouble();
    if(val < static_cast<double>(INT_MIN))
    {
        ERROR("int overflow");
        return INT_MIN;
    }
    if(val > static_cast<double>(INT_MAX))
    {
        ERROR("int overflow");
        return INT_MAX;
    }
    return static_cast<s32>(val);
}

bool Lua::NumberValue::getBool(void) const
{
    return getDouble() != 0;
}

double Lua::NumberValue::getDouble(void) const
{
    double* val = static_cast<double*>(luaValue);
    return *val;
}


/* Lua::BoolValue */
Lua::BoolValue::BoolValue(bool val) : Lua::BaseValue(Lua::TypeBool)
{
    setBool(val);
}

void Lua::BoolValue::setBool(bool val)
{
    luaValue = val ? this : NULL;
}

std::string Lua::BoolValue::getString(void) const
{
    return getBool() ? "true" : "false";
}

s64 Lua::BoolValue::getInt64(void) const
{
    return getBool();
}

s32 Lua::BoolValue::getInt(void) const
{
    return getBool();
}

bool Lua::BoolValue::getBool(void) const
{
    return luaValue;
}

double Lua::BoolValue::getDouble(void) const
{
    return getInt();
}

Lua::FunctionValue::FunctionValue(lua_CFunction ptr) : BaseValue(TypeFunction)
{
    setFunction(ptr);
}

void Lua::FunctionValue::setFunction(lua_CFunction ptr)
{
    luaValue = reinterpret_cast<void*>(ptr);
}

lua_CFunction Lua::FunctionValue::getFunction(void) const
{
    return reinterpret_cast<lua_CFunction>(luaValue);
}

void Lua::TableValue::push(const std::string & key, BaseValue* val)
{
    if(val) insert(std::make_pair(key, val));
}


/* Lua::State */
Lua::State::State() : ptr(std::shared_ptr<lua_State>(luaL_newstate(), lua_close)), isconst(false)
{
    if(ptr.get()) luaL_openlibs(ptr.get());
}

int Lua::State::doFile(const std::string & file)
{
    return luaL_dofile(ptr.get(), file.c_str());
}

void Lua::State::loadDirectory(const std::string & dir)
{
    pushGlobal("package");
    getField("path");

    std::string curPath = toStringIndex(-1);
    curPath.append(";");
    curPath.append(dir);
    curPath.append("?.lua");

    stackPopElement();
    pushString(curPath);
    setField("path", -2);
    stackPopElement();
}

void Lua::State::call(int nargs, int nresults)
{
    lua_call(ptr.get(), nargs, nresults);
}

int Lua::State::next(int index)
{
    return lua_next(ptr.get(), index);
}

int Lua::State::doString(const std::string & str)
{
    return luaL_dostring(ptr.get(), str.c_str());
}

void Lua::StackDump(lua_State* L, const char* label)
{
    lua_pushstring(L, label);
    int it = lua_gettop(L);

    VERBOSE("+---------------------->");
    while(it)
    {
	switch(lua_type(L, it))
	{
    	    case LUA_TSTRING:
                VERBOSE("| " << it << ": " << "`" << lua_tostring(L, it) << "'");
		break;

    	    case LUA_TBOOLEAN:
                VERBOSE("| " << it << ": " << (lua_toboolean(L, it) ? "true" : "false"));
		break;

    	    case LUA_TNUMBER:
                VERBOSE("| " << it << ": " << String::number(lua_tointeger(L, it), 6));
		break;

    	    case LUA_TTABLE:
    	    case LUA_TFUNCTION:
    	    case LUA_TUSERDATA:
    	    case LUA_TTHREAD:
    	    case LUA_TLIGHTUSERDATA:
                VERBOSE("| " << it << ": " << lua_typename(L, it));
		//const_cast<State &>(*this).dumpTable(it);
		break;

	    default:
		break;
	}

	it--;
    }
    VERBOSE("+----------------------<");
    lua_pop(L, 1);
}

void Lua::State::stackDump(void) const
{
    StackDump(ptr.get());
}

void Lua::State::stackDump(const std::string & label) const
{
    State & state = const_cast<State &>(*this);
    StackDump(state.toLuaState(), label.c_str());
}

Lua::BaseValue* Lua::State::getValue(int index) const
{
    Lua::State & state = const_cast<Lua::State &>(*this);
    int type = state.typeIndex(index);

    switch(type)
    {
        case Lua::TypeNumber:	return new NumberValue(toNumberIndex(index));
        case Lua::TypeBool:	return new BoolValue(toBoolIndex(index));
        case Lua::TypeString:	return new StringValue(toStringIndex(index));
        case Lua::TypeFunction: return new FunctionValue(toFunctionIndex(index));
        case Lua::TypeUserData:	return new UserDataValue(toUserDataIndex(index));

        case Lua::TypeTable:	return new BaseValue(Lua::TypeTable);
        case Lua::TypeThread:	return new BaseValue(Lua::TypeThread);
	case Lua::TypeLightUserData: return new BaseValue(Lua::TypeLightUserData);

        default: break;
    }

    return new BaseValue();
}

void Lua::State::tableDump(const std::string & name) const
{
    Lua::State & state = const_cast<Lua::State &>(*this);
    state.pushGlobal(name.c_str());

    if(state.isTable(1))
    {
    	state.pushNil();

    	while(state.next(1) != 0)
    	{
	    state.stackDump();
	    BaseValue* value = getValue(-1);
	    const std::string & key = toStringIndex(-2);

	    VERBOSE(key << ": " << typeString(value->type()) << "(" << value->getString() <<")");
	    if(value) delete value;

    	    state.stackPopElement();
    	}
    }
    else
    {
	ERROR("table not found: " << name);
    }

    state.stackPopElement();
}

bool Lua::State::stackCheckFreeSlots(int extra) const
{
    return 0 != lua_checkstack(ptr.get(), extra);
}

int Lua::State::stackTopIndex(void) const
{
    return lua_gettop(ptr.get());
}

size_t Lua::State::stackSize(void) const
{
    return lua_gettop(ptr.get());
}

void Lua::State::stackClear(void)
{
    lua_settop(ptr.get(), 0);
}

void Lua::State::stackTopIndex(int index)
{
    lua_settop(ptr.get(), index);
}

void Lua::State::stackRemoveIndex(int index)
{
    lua_remove(ptr.get(), index);
}

void Lua::State::stackTopMoveIndex(int index)
{
    lua_insert(ptr.get(), index);
}

void Lua::State::stackTopReplaceIndex(int index)
{
    lua_replace(ptr.get(), index);
}

void Lua::State::stackPopElement(void)
{
    lua_pop(ptr.get(), 1);
}

void Lua::State::stackPopElements(int num)
{
    if(0 < num) lua_pop(ptr.get(), num);
}

void Lua::State::pushGlobal(const std::string & name)
{
    lua_getglobal(ptr.get(), name.c_str());
}

void Lua::State::setField(const std::string & name, int index)
{
    lua_setfield(ptr.get(), index, name.c_str());
}

void Lua::State::getField(const std::string & name, int index) const
{
    lua_getfield(ptr.get(), index, name.c_str());
}

void Lua::State::pushValueIndex(int index)
{
    lua_pushvalue(ptr.get(), index);
}

void Lua::State::pushNil(void)
{
    lua_pushnil(ptr.get());
}

void Lua::State::pushInteger(int value)
{
    lua_pushinteger(ptr.get(), value);
}

void Lua::State::pushNumber(double value)
{
    lua_pushnumber(ptr.get(), value);
}

void Lua::State::pushBool(bool val)
{
    lua_pushboolean(ptr.get(), val);
}

void Lua::State::pushString(const std::string & str)
{
    lua_pushstring(ptr.get(), str.c_str());
}

bool Lua::State::isNil(int index) const
{
    return lua_isnil(ptr.get(), index);
}

bool Lua::State::isBool(int index) const
{
    return lua_isboolean(ptr.get(), index);
}

bool Lua::State::isNumber(int index) const
{
    return lua_isnumber(ptr.get(), index);
}

bool Lua::State::isString(int index) const
{
    return lua_isstring(ptr.get(), index);
}

bool Lua::State::isTable(int index) const
{
    return lua_istable(ptr.get(), index);
}

bool Lua::State::isFunction(int index) const
{
    return lua_isfunction(ptr.get(), index);
}

bool Lua::State::isCFunction(int index) const
{
    return lua_iscfunction(ptr.get(), index);
}

bool Lua::State::isUserData(int index) const
{
    return lua_isuserdata(ptr.get(), index);
}

bool Lua::State::isLightUserData(int index) const
{
    return lua_islightuserdata(ptr.get(), index);
}

int Lua::State::typeIndex(int index) const
{
    switch(lua_type(ptr.get(), index))
    {
	case LUA_TNUMBER:		return TypeNumber;
        case LUA_TBOOLEAN:		return TypeBool;
        case LUA_TSTRING:		return TypeString;
        case LUA_TTABLE:		return TypeTable;
        case LUA_TFUNCTION:		return TypeFunction;
        case LUA_TUSERDATA:		return TypeUserData;
        case LUA_TTHREAD:		return TypeThread;
        case LUA_TLIGHTUSERDATA:	return TypeLightUserData;
	default: break;
    }

    return TypeNil;
}

std::string Lua::State::typeName(int index) const
{
    return std::string(lua_typename(ptr.get(), index));
}

bool Lua::State::isEqual(int index1, int index2) const
{
    return lua_equal(ptr.get(), index1, index2);
}

bool Lua::State::isRawEqual(int index1, int index2) const
{
    return lua_rawequal(ptr.get(), index1, index2);
}

bool Lua::State::isLessThan(int index1, int index2) const
{
    return lua_lessthan(ptr.get(), index1, index2);
}

bool Lua::State::toBoolIndex(int index) const
{
    return lua_toboolean(ptr.get(), index);
}

s64 Lua::State::toIntegerIndex(int index) const
{
    return lua_tointeger(ptr.get(), index);
}

double Lua::State::toNumberIndex(int index) const
{
    return lua_tonumber(ptr.get(), index);
}

std::string Lua::State::toStringIndex(int index) const
{
    return std::string(lua_tostring(ptr.get(), index));
}

void* Lua::State::toUserDataIndex(int index) const
{
    return lua_touserdata(ptr.get(), index);
}

const void* Lua::State::toPointerIndex(int index) const
{
    return lua_topointer(ptr.get(), index);
}

lua_CFunction Lua::State::toFunctionIndex(int index) const
{
    return lua_tocfunction(ptr.get(), index);
}

lua_State* Lua::State::toThreadIndex(int index) const
{
    return lua_tothread(ptr.get(), index);
}

lua_State* Lua::State::toLuaState(void)
{
    return ptr.get();
}

StringList Lua::State::tableKeys(const std::string & name) const
{
    StringList res;
    Lua::State & state = const_cast<Lua::State &>(*this);

    state.pushGlobal(name.c_str());

    if(isTable(1))
    {
        state.pushNil();

        while(state.next(1) != 0)
        {
	    res.push_back(toStringIndex(-2));
    	    state.stackPopElement();
	}
    }
    else
    {
        ERROR("table not found: " << name);
    }

    state.stackPopElement();
    return res;
}

Lua::TableValue Lua::State::findTable(const std::string & name)
{
    TableValue res;
    pushGlobal(name.c_str());

    if(isTable(1))
    {
        pushNil();

        while(next(1) != 0)
        {
    	    const std::string & key = toStringIndex(-2);
            int type = typeIndex(-1);

            switch(type)
            {
                case Lua::TypeNumber:
            	    res.push(key, new NumberValue(toNumberIndex(-1)));
            	    break;

                case Lua::TypeBool:
            	    res.push(key, new BoolValue(toBoolIndex(-1)));
            	    break;

                case Lua::TypeString:
            	    res.push(key, new StringValue(toStringIndex(-1)));
            	    break;

                case Lua::TypeFunction:
            	    res.push(key, new FunctionValue(toFunctionIndex(-1)));
            	    break;

                case Lua::TypeTable:
            	    res.push(key, new BaseValue(Lua::TypeTable));
            	    break;

                case Lua::TypeThread:
            	    res.push(key, new BaseValue(Lua::TypeThread));
            	    break;

                case Lua::TypeUserData:
            	    res.push(key, new UserDataValue(toUserDataIndex(-1)));
            	    break;

                case Lua::TypeLightUserData:
            	    res.push(key, new BaseValue(Lua::TypeLightUserData));
            	    break;

                case Lua::TypeNil:
            	    res.push(key, new BaseValue());
            	    break;

                default: break;
            }

    	    stackPopElement();
        }
    }
    else
    {
        ERROR("table not found: " << name);
    }

    stackPopElement();
    return res;
}
#endif

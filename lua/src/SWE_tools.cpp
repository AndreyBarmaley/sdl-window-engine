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

#include "SWE_tools.h"

std::string SWE_Tools::convertEncoding(LuaState & ll, const std::string & str)
{
    LuaStateValidator(ll, 0);

    if(! ll.pushTable("SWE").isTopTable())
    {
        ERROR("table not found" << ": " << "swe");
	ll.stackPop();
	return str;
    }

    if(! ll.getFieldTableIndex("encoding", -1, false).isTopString())
    {
	ll.stackPop(2);
	return str;
    }

    std::string charset = ll.getTopString();
    std::string res = Tools::stringEncode(str, charset.c_str());

    ll.stackPop(2);
    return res;
}

bool SWE_Tools::addResourceDir(LuaState & ll, const std::string & dir)
{
    LuaStateValidator(ll, 0);

    if(ll.pushTable("SWE.ShareDirs").isTopTable())
    {
        int nindex = 1;
        bool present = false;

        // iterate SWE.Scene.Resources
        ll.pushNil();
        while(ll.nextTableIndex(-2))
        {
            if(! present)
            {
                std::string val = ll.toStringIndex(-1);
                present = val == dir;
            }
            // pop value
            ll.stackPop();
            nindex++;
        }

        if(present)
        {
            ERROR("resource exist" << ": " << dir << ", skipping...");
            return false;
        }

        // add value
        ll.pushNumber(nindex).pushString(dir).setTableIndex(-3);
        // remove table
        ll.stackPop();

        return true;
    }

    ERROR("table not found" << ": " << "swe.sharedirs");
    return false;
}

std::string SWE_Tools::findResource(LuaState & ll, const std::string & res)
{
    LuaStateValidator(ll, 0);

    if(ll.pushTable("SWE.ShareDirs").isTopTable())
    {
        // iterate SWE.Scene.Resources
        ll.pushNil();
        while(ll.nextTableIndex(-2))
        {
            std::string dir = ll.toStringIndex(-1);
            std::string file = Systems::concatePath(dir, res);

            // pop value
            ll.stackPop();

            if(Systems::isFile(file))
            {
                ll.stackPop(2);
                return file;
            }
        }

        // remove table
	ll.stackPop();

        std::string file = SWE_Tools::toCurrentPath(ll, res);
        if(Systems::isFile(file))
            return file;

        file = SWE_Tools::toRunningPath(ll, res);
        if(Systems::isFile(file))
            return file;

        ERROR("resource not found" << ": " << res);
    }
    else
    {
        ERROR("table not found" << ": " << "swe.sharedirs");
	ll.stackPop();
    }

    return res;
}

std::string SWE_Tools::toRunningPath(LuaState & ll, const std::string & file)
{
    LuaStateValidator(ll, 0);

#ifdef ANDROID
    // check assets
    StreamFile sf(file, "rb");
    if(sf.isValid())
    {
	sf.close();
	return file;
    }
#endif
    if(! ll.pushTable("SWE").isTopTable())
    {
        ERROR("table not found" << ": " << "swe");
	ll.stackPop();
	return file;
    }

    if(! ll.getFieldTableIndex("runfile", -1, false).isTopString())
    {
	ll.stackPop(2);
	return file;
    }

    std::string runfile = ll.getTopString();
    std::string res = Systems::concatePath(Systems::dirname(runfile), file);

    ll.stackPop(2);
    return res;
}

std::string SWE_Tools::toCurrentPath(LuaState & ll, const std::string & file)
{
#ifdef ANDROID
    // check assets
    StreamFile sf(file, "rb");
    if(sf.isValid())
    {
	sf.close();
	return file;
    }
#endif

    LuaStateValidator(ll, 0);

    if(! ll.pushTable("SWE").isTopTable())
    {
        ERROR("table not found" << ": " << "swe");
	ll.stackPop();
	return file;
    }

    if(! ll.getFieldTableIndex("getcwd", -1, false).isTopString())
    {
	ll.stackPop(2);
	return file;
    }

    std::string getcwd = ll.getTopString();
    std::string res = Systems::concatePath(getcwd, file);

    ll.stackPop(2);
    return res;
}

int SWE_Tools::pushJsonValue(LuaState & ll, const JsonValue* jv)
{
    if(! jv)
	return 0;

    LuaStateValidator(ll, 1);

    switch(jv->getType())
    {
	case JsonType::Null:	ll.pushNil(); break;
	case JsonType::Integer:	ll.pushInteger(jv->getInteger()); break;
	case JsonType::Double:	ll.pushNumber(jv->getDouble()); break;
	case JsonType::String:	ll.pushString(jv->getString()); break;
	case JsonType::Boolean:	ll.pushBoolean(jv->getBoolean()); break;

	case JsonType::Array:	return pushJsonArray(ll, static_cast<const JsonArray*>(jv));
	case JsonType::Object:	return pushJsonObject(ll, static_cast<const JsonObject*>(jv));

	default: ERROR("unknown type: " << static_cast<int>(jv->getType())); ll.pushNil(); break;
    }

    return 1;
}

int SWE_Tools::pushJsonArray(LuaState & ll, const JsonArray* ja)
{
    if(! ja)
	return 0;

    LuaStateValidator(ll, 1);

    ll.pushTable();
    for(int ii = 0; ii < ja->count(); ++ii)
    {
	ll.pushInteger(ii + 1);
	int offset = pushJsonValue(ll, ja->getValue(ii));
	ll.setTableIndex(-2 - offset);
    }

    return 1;
}

int SWE_Tools::pushJsonObject(LuaState & ll, const JsonObject* jo)
{
    if(! jo)
	return 0;

    LuaStateValidator(ll, 1);

    ll.pushTable();
    for(auto & key : jo->keys())
    {
	int offset = pushJsonValue(ll, jo->getValue(key));
	ll.setFieldTableIndex(key, -1 - offset);
    }

    return 1;
}

std::string SWE_Tools::toJsonString(LuaState & ll, int index)
{
    LuaStateValidator(ll, 0);
    std::string res;

    if(ll.isTableIndex(index))
    {
        if(ll.getFieldTableIndex("ToJson", index, false).isTopFunction())
	{
	    ll.stackPop(1);
    	    // clone table
    	    ll.pushValueIndex(index);
	    // set function
    	    ll.getFieldTableIndex("ToJson", -1);
    	    // set params: self
    	    ll.pushValueIndex(index);
	    // run as obj:ToJson(obj)
    	    res = ll.callFunction(1, 1).getTopString();
    	    // remove string, table
    	    ll.stackPop(2);
	}
	else
	{
	    ll.stackPop(1);
	    // dump table to json
	    res = ll.toJsonStringTableIndex(index);
	}
    }
    else
    {
        ERROR("table not found, index: " << index);
    }

    return res;
}

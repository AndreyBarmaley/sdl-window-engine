/***************************************************************************
 *   Copyright (C) 2020 by SWE team <sdl.window.engine@gmail.com>          *
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

#include "libswe.h"
using namespace SWE;

int main(int argc, char** argv)
{
    const std::string file = "test.json";

    if(! Systems::isFile(file))
    {
	ERROR("file not found: " << file);
	return EXIT_FAILURE;
    }

    JsonContentFile jf(file);
    if(! jf.isValid())
    {
	ERROR("json invalid");
	return EXIT_FAILURE;
    }

    JsonObject jo = jf.toObject();

    VERBOSE("== parse primitive, variant 1");
    for(auto & key : jo.keys())
    {
	auto type = jo.getType(key);

	switch(type)
	{
	    case JsonType::Null:	VERBOSE(key << ": " << "null"); break;
	    case JsonType::Integer:	VERBOSE(key << ": " << jo.getInteger(key)); break;
	    case JsonType::Double:	VERBOSE(key << ": " << jo.getDouble(key)); break;
	    case JsonType::String:	VERBOSE(key << ": " << jo.getString(key)); break;
	    case JsonType::Boolean:	VERBOSE(key << ": " << String::Bool(jo.getBoolean(key))); break;
	    default: break;
	}
    }

    VERBOSE("== parse primitive, variant 2");
    for(auto & key : jo.keys())
    {
	const JsonValue* jv = jo.getValue(key);
	switch(jv->getType())
	{
	    case JsonType::Null:	VERBOSE(key << ": " << "null"); break;
	    case JsonType::Integer:	VERBOSE(key << ": " << jv->getInteger()); break;
	    case JsonType::Double:	VERBOSE(key << ": " << jv->getDouble()); break;
	    case JsonType::String:	VERBOSE(key << ": " << jv->getString()); break;
	    case JsonType::Boolean:	VERBOSE(key << ": " << String::Bool(jv->getBoolean())); break;
	    default: break;
	}
    }

    VERBOSE("== parse arrays");
    for(auto & key : jo.keys())
    {
	const JsonArray* ja = jo.getArray(key);
	if(ja)
	{
	    VERBOSE(key << ": " << ja->toString());
	}
    }

    const JsonArray* ja = nullptr;

    VERBOSE("== parse array ints");
    ja = jo.getArray("array int");
    if(ja)
    {
	for(auto & val : ja->toStdVector<int>())
	    VERBOSE("int: " << val);
    }

    VERBOSE("== parse array strings");
    ja = jo.getArray("array str");
    if(ja)
    {
	for(auto & val : ja->toStdList<std::string>())
	    VERBOSE("string: " << val);
    }

    VERBOSE("== parse array points");
    ja = jo.getArray("array point");
    if(ja)
    {
	for(auto & pt : JsonUnpack::points(*ja))
	    VERBOSE("point: " << pt.toString());
    }

    return EXIT_SUCCESS;
}

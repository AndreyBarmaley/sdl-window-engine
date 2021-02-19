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

#include "SWE_binarybuf.h"
#include "SWE_streambuf.h"

int SWE_streambuf_create(lua_State*);
int SWE_streambuf_destroy(lua_State*);

SWE_StreamBuf* SWE_StreamBuf::get(LuaState & ll, int tableIndex, const char* funcName)
{
    LuaStateValidator(ll, 0);

    if(! ll.isTableIndex(tableIndex) ||
	0 != ll.popFieldTableIndex("__type", tableIndex).compare("swe.streambuf"))
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

    auto ptr = static_cast<SWE_StreamBuf**>(ll.getTopUserData());
    ll.stackPop();

    return ptr ? *ptr : NULL;
}

/////////////////////////////////////////////////////////////////////
int SWE_streambuf_get_value_type(lua_State* L, IntType type)
{
    // params: swe_streambuf

    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_StreamBuf* buf = SWE_StreamBuf::get(ll, 1, __FUNCTION__))
    {
        int val = 0;
        switch(type)
        {
            case IntType::Int8:    val = buf->get8(); break;
            case IntType::IntBE16: val = buf->getBE16(); break;
            case IntType::IntLE16: val = buf->getLE16(); break;
            case IntType::IntBE32: val = buf->getBE32(); break;
            case IntType::IntLE32: val = buf->getLE32(); break;
            case IntType::IntBE64: val = buf->getBE64(); break;
            case IntType::IntLE64: val = buf->getLE64(); break;
        }

	ll.pushInteger(val);
    }
    else
    {
        ERROR("userdata empty");
	ll.pushNil();
    }

    return rescount;
}

int SWE_streambuf_get_byte(lua_State* L)
{
    return SWE_streambuf_get_value_type(L, IntType::Int8);
}

int SWE_streambuf_get_be16(lua_State* L)
{
    return SWE_streambuf_get_value_type(L, IntType::IntBE16);
}

int SWE_streambuf_get_be32(lua_State* L)
{
    return SWE_streambuf_get_value_type(L, IntType::IntBE32);
}

int SWE_streambuf_get_be64(lua_State* L)
{
    return SWE_streambuf_get_value_type(L, IntType::IntBE64);
}

int SWE_streambuf_get_le16(lua_State* L)
{
    return SWE_streambuf_get_value_type(L, IntType::IntLE16);
}

int SWE_streambuf_get_le32(lua_State* L)
{
    return SWE_streambuf_get_value_type(L, IntType::IntLE32);
}

int SWE_streambuf_get_le64(lua_State* L)
{
    return SWE_streambuf_get_value_type(L, IntType::IntLE64);
}

int SWE_streambuf_get_bytes(lua_State* L)
{
    // params: swe_streambuf, size
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_StreamBuf* buf = SWE_StreamBuf::get(ll, 1, __FUNCTION__))
    {
	int size = ll.toIntegerIndex(2);
        auto data = static_cast<StreamBufRW*>(buf)->get(size);

	SWE_BinaryBuf* res = SWE_Stack::binarybuf_create(ll);
        res->swap(data);
        ll.pushString("size").pushInteger(res->size()).setTableIndex(-3);
    }
    else
    {
	ERROR("userdata empty");
	ll.pushNil();
    }

    return rescount;
}

int SWE_streambuf_put_bytes(lua_State* L)
{
    // params: swe_streambuf, swe_binarybuf

    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    SWE_StreamBuf* stream = SWE_StreamBuf::get(ll, 1, __FUNCTION__);
    SWE_BinaryBuf* buf = SWE_BinaryBuf::get(ll, 2, __FUNCTION__);

    if(stream && buf)
    {
        stream->put(reinterpret_cast<const char*>(buf->data()), buf->size());
	ll.pushString("size").pushInteger(stream->size()).setTableIndex(1);
    }
    else
    {
        ERROR("userdata empty");
    }

    return rescount;
}

int SWE_streambuf_put_value_type(lua_State* L, IntType type)
{
    // params: swe_streambuf

    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    if(SWE_StreamBuf* buf = SWE_StreamBuf::get(ll, 1, __FUNCTION__))
    {
	int val = ll.toIntegerIndex(2);
        switch(type)
        {
            case IntType::Int8:    buf->put8(val); break;
            case IntType::IntBE16: buf->putBE16(val); break;
            case IntType::IntLE16: buf->putLE16(val); break;
            case IntType::IntBE32: buf->putBE32(val); break;
            case IntType::IntLE32: buf->putLE32(val); break;
            case IntType::IntBE64: buf->putBE64(val); break;
            case IntType::IntLE64: buf->putLE64(val); break;
        }

	ll.pushString("size").pushInteger(buf->size()).setTableIndex(1);
    }
    else
    {
        ERROR("userdata empty");
    }

    return rescount;
}

int SWE_streambuf_put_byte(lua_State* L)
{
    return SWE_streambuf_put_value_type(L, IntType::Int8);
}

int SWE_streambuf_put_be16(lua_State* L)
{
    return SWE_streambuf_put_value_type(L, IntType::IntBE16);
}

int SWE_streambuf_put_be32(lua_State* L)
{
    return SWE_streambuf_put_value_type(L, IntType::IntBE32);
}

int SWE_streambuf_put_be64(lua_State* L)
{
    return SWE_streambuf_put_value_type(L, IntType::IntBE64);
}

int SWE_streambuf_put_le16(lua_State* L)
{
    return SWE_streambuf_put_value_type(L, IntType::IntLE16);
}

int SWE_streambuf_put_le32(lua_State* L)
{
    return SWE_streambuf_put_value_type(L, IntType::IntLE32);
}

int SWE_streambuf_put_le64(lua_State* L)
{
    return SWE_streambuf_put_value_type(L, IntType::IntLE64);
}

int SWE_streambuf_to_json(lua_State* L)
{   
    // params: swe_streambuf
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(StreamBufRW* stream = SWE_StreamBuf::get(ll, 1, __FUNCTION__))
    {
        size_t tellg = stream->tellg();
	stream->seekg(0);
	std::string data = stream->get().toHexString(",", true);
	stream->seekg(tellg);

        std::string str = StringFormat("{\"type\":\"%1\",\"tellg\":%2,\"tellp\":%3,\"binary\":[%4]}").
            arg("swe.streambuf").arg(stream->tellg()).arg(stream->tellp()).arg(data);

        ll.pushString(str);
    }
    else
    {
        ERROR("userdata empty");
        ll.pushNil();
    }

    return rescount;
}

const struct luaL_Reg SWE_streambuf_functions[] = {
    { "GetByte", SWE_streambuf_get_byte },		// [int], swe_streambuf
    { "GetBE16", SWE_streambuf_get_be16 },		// [int], swe_streambuf
    { "GetBE32", SWE_streambuf_get_be32 },		// [int], swe_streambuf
    { "GetBE64", SWE_streambuf_get_be64 },		// [number], swe_streambuf
    { "GetLE16", SWE_streambuf_get_le16 },		// [int], swe_streambuf
    { "GetLE32", SWE_streambuf_get_le32 },		// [int], swe_streambuf
    { "GetLE64", SWE_streambuf_get_le64 },		// [number], swe_streambuf
    { "GetBytes", SWE_streambuf_get_bytes }, 		// [swe_binarybuf], swe_streambuf, number size
    { "PutByte", SWE_streambuf_put_byte }, 		// [void], swe_streambuf, int
    { "PutBE16", SWE_streambuf_put_be16 },		// [void], swe_streambuf, int
    { "PutBE32", SWE_streambuf_put_be32 },		// [void], swe_streambuf, int
    { "PutBE64", SWE_streambuf_put_be64 },		// [void], swe_streambuf, number
    { "PutLE16", SWE_streambuf_put_le16 },		// [void], swe_streambuf, int
    { "PutLE32", SWE_streambuf_put_le32 },		// [void], swe_streambuf, int
    { "PutLE64", SWE_streambuf_put_le64 },		// [void], swe_streambuf, number
    { "PutBytes", SWE_streambuf_put_bytes }, 		// [void], swe_streambuf, swe_binarybuf
//    { "Seek", SWE_streambuf_seek }, 			// [bool], swe_streambuf, int, int
//    { "Tell", SWE_streambuf_tell },			// [int], swe_streambuf
//    { "Open", SWE_streambuf_open }, 			// [bool], swe_streambuf, string, string
//    { "Close", SWE_streambuf_close }, 		// [swe_streambuf], swe_streambuf
    { "ToJson", SWE_streambuf_to_json },		// [string], swe_streambuf
    { NULL, NULL }
};

int SWE_streambuf_create(lua_State* L)
{
    // empty params

    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    // result
    ll.pushTable();

    // userdata
    ll.pushString("userdata");
    auto ptr = static_cast<SWE_StreamBuf**>(ll.pushUserData(sizeof(SWE_StreamBuf*)));

    // set metatable: __gc
    ll.pushTable(0, 1);
    ll.pushFunction(SWE_streambuf_destroy).setFieldTableIndex("__gc", -2);
    ll.setMetaTableIndex(-2).setTableIndex(-3);

    if(ll.isTableIndex(2))
    {
	SWE_BinaryBuf* buf = SWE_BinaryBuf::get(ll, 2, __FUNCTION__);

	if(buf)
	{
	    // SWE_StreamBuf: BinaryBuf
	    SWE_StreamBuf* stream = new SWE_StreamBuf(*buf);
	    *ptr = stream;

	    // add values
	    ll.pushString("__type").pushString("swe.streambuf").setTableIndex(-3);
	}
	else
	{
	    *ptr = new SWE_StreamBuf();

	    // add values
	    ll.pushString("__type").pushString("swe.streambuf").setTableIndex(-3);

    	    ERROR("userdata empty");
	}
    }
    else
    {
	// SWE_StreamBuf()
	*ptr = new SWE_StreamBuf();

	// add values
	ll.pushString("__type").pushString("swe.streambuf").setTableIndex(-3);
    }

    // set functions
    ll.setFunctionsTableIndex(SWE_streambuf_functions, -1);

    DEBUG(String::pointer(ptr) << ": [" << String::pointer(*ptr) << "]");

    return rescount;
}

int SWE_streambuf_destroy(lua_State* L)
{
    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    if(ll.isTopUserData())
    {
        auto ptr = static_cast<SWE_StreamBuf**>(ll.getTopUserData());
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

void SWE_StreamBuf::registers(LuaState & ll)
{
    LuaStateValidator(ll, 0);

    // SWE.StreamBuf
    ll.pushTable("SWE.StreamBuf");

    // set metatable: __call
    ll.pushTable(0, 1).pushFunction(SWE_streambuf_create).setFieldTableIndex("__call", -2);
    ll.setMetaTableIndex(-2).stackPop();
}

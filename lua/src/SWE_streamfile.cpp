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
#include "SWE_streamfile.h"

int SWE_streamfile_create(lua_State*);
int SWE_streamfile_destroy(lua_State*);

SWE_StreamFile* SWE_StreamFile::get(LuaState & ll, int tableIndex, const char* funcName)
{
    LuaStateValidator(ll, 0);

    if(! ll.isTableIndex(tableIndex) ||
	0 != ll.popFieldTableIndex("__type", tableIndex).compare("swe.streamfile"))
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

    auto ptr = static_cast<SWE_StreamFile**>(ll.getTopUserData());
    ll.stackPop();

    return ptr ? *ptr : NULL;
}

/////////////////////////////////////////////////////////////////////
int SWE_streamfile_read_value_type(lua_State* L, IntType type)
{
    // params: swe_streamfile

    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_StreamFile* stream = SWE_StreamFile::get(ll, 1, __FUNCTION__))
    {
        int val = 0;
        switch(type)
        {
            case IntType::Int8:    val = stream->get8(); break;
            case IntType::IntBE16: val = stream->getBE16(); break;
            case IntType::IntLE16: val = stream->getLE16(); break;
            case IntType::IntBE32: val = stream->getBE32(); break;
            case IntType::IntLE32: val = stream->getLE32(); break;
            case IntType::IntBE64: val = stream->getBE64(); break;
            case IntType::IntLE64: val = stream->getLE64(); break;
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

int SWE_streamfile_read_byte(lua_State* L)
{
    return SWE_streamfile_read_value_type(L, IntType::Int8);
}

int SWE_streamfile_read_be16(lua_State* L)
{
    return SWE_streamfile_read_value_type(L, IntType::IntBE16);
}

int SWE_streamfile_read_be32(lua_State* L)
{
    return SWE_streamfile_read_value_type(L, IntType::IntBE32);
}

int SWE_streamfile_read_be64(lua_State* L)
{
    return SWE_streamfile_read_value_type(L, IntType::IntBE64);
}

int SWE_streamfile_read_le16(lua_State* L)
{
    return SWE_streamfile_read_value_type(L, IntType::IntLE16);
}

int SWE_streamfile_read_le32(lua_State* L)
{
    return SWE_streamfile_read_value_type(L, IntType::IntLE32);
}

int SWE_streamfile_read_le64(lua_State* L)
{
    return SWE_streamfile_read_value_type(L, IntType::IntLE64);
}

int SWE_streamfile_read_string(lua_State* L)
{
    // params: swe_streamfile, size
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_StreamFile* stream = SWE_StreamFile::get(ll, 1, __FUNCTION__))
    {
	int size = ll.toIntegerIndex(2);
        auto data = static_cast<StreamFile*>(stream)->get(size);

        ll.pushString(data.toString());
    }
    else
    {
	ERROR("userdata empty");
	ll.pushNil();
    }

    return rescount;
}

int SWE_streamfile_read_bytes(lua_State* L)
{
    // params: swe_streamfile, size
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_StreamFile* stream = SWE_StreamFile::get(ll, 1, __FUNCTION__))
    {
	int size = ll.toIntegerIndex(2);
        auto data = static_cast<StreamFile*>(stream)->get(size);

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

int SWE_streamfile_write_bytes(lua_State* L)
{
    // params: swe_streamfile, swe_binarybuf

    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    SWE_StreamFile* stream = SWE_StreamFile::get(ll, 1, __FUNCTION__);
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

int SWE_streamfile_write_value_type(lua_State* L, IntType type)
{
    // params: swe_streamfile

    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_StreamFile* stream = SWE_StreamFile::get(ll, 1, __FUNCTION__))
    {
	int val = ll.toIntegerIndex(2);
        switch(type)
        {
            case IntType::Int8:    stream->put8(val); break;
            case IntType::IntBE16: stream->putBE16(val); break;
            case IntType::IntLE16: stream->putLE16(val); break;
            case IntType::IntBE32: stream->putBE32(val); break;
            case IntType::IntLE32: stream->putLE32(val); break;
            case IntType::IntBE64: stream->putBE64(val); break;
            case IntType::IntLE64: stream->putLE64(val); break;
        }

	ll.pushString("size").pushInteger(stream->size()).setTableIndex(1);
    }
    else
    {
        ERROR("userdata empty");
        ll.pushNil();
    }

    return rescount;
}

int SWE_streamfile_write_byte(lua_State* L)
{
    return SWE_streamfile_write_value_type(L, IntType::Int8);
}

int SWE_streamfile_write_be16(lua_State* L)
{
    return SWE_streamfile_write_value_type(L, IntType::IntBE16);
}

int SWE_streamfile_write_be32(lua_State* L)
{
    return SWE_streamfile_write_value_type(L, IntType::IntBE32);
}

int SWE_streamfile_write_be64(lua_State* L)
{
    return SWE_streamfile_write_value_type(L, IntType::IntBE64);
}

int SWE_streamfile_write_le16(lua_State* L)
{
    return SWE_streamfile_write_value_type(L, IntType::IntLE16);
}

int SWE_streamfile_write_le32(lua_State* L)
{
    return SWE_streamfile_write_value_type(L, IntType::IntLE32);
}

int SWE_streamfile_write_le64(lua_State* L)
{
    return SWE_streamfile_write_value_type(L, IntType::IntLE64);
}

int SWE_streamfile_seek(lua_State* L)
{
    // params: swe_streamfile, number

    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);
    SWE_StreamFile* stream = SWE_StreamFile::get(ll, 1, __FUNCTION__);

    if(stream)
    {
	int offset = ll.toIntegerIndex(2);
	int whence = ll.isNumberIndex(3) ? ll.toIntegerIndex(3) : RW_SEEK_SET;
	bool res = stream->seek(offset, whence);
	ll.pushBoolean(res);
    }
    else
    {
        ERROR("userdata empty");
	ll.pushBoolean(false);
    }

    return rescount;
}

int SWE_streamfile_tell(lua_State* L)
{
    // params: swe_streamfile

    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    SWE_StreamFile* stream = SWE_StreamFile::get(ll, 1, __FUNCTION__);

    if(stream)
    {
        int res = stream->tell();
	ll.pushInteger(res);
    }
    else
    {
        ERROR("userdata empty");
	ll.pushNil();
    }

    return rescount;
}

int SWE_streamfile_open(lua_State* L)
{
    // params: swe_streamfile, filename, mode

    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);
    SWE_StreamFile* stream = SWE_StreamFile::get(ll, 1, __FUNCTION__);

    if(stream)
    {
	std::string name = ll.toStringIndex(2);
	std::string mode = ll.toStringIndex(3);

        bool res = stream->open(name, mode.c_str());

	ll.pushString("filename").pushString(res ? name : "").setTableIndex(1);
	ll.pushString("openmode").pushString(res ? mode : "").setTableIndex(1);
	ll.pushString("valid").pushBoolean(stream->isValid()).setTableIndex(1);
	ll.pushString("size").pushInteger(stream->size()).setTableIndex(1);

        ll.pushBoolean(res);
    }
    else
    {
        ll.pushBoolean(false);
	ERROR("userdata empty");
    }

    return rescount;
}

int SWE_streamfile_close(lua_State* L)
{
    // params: swe_streamfile

    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);
    SWE_StreamFile* stream = SWE_StreamFile::get(ll, 1, __FUNCTION__);

    if(stream)
    {
	stream->close();

	ll.pushString("filename").pushString("").setTableIndex(1);
	ll.pushString("openmode").pushString("").setTableIndex(1);
	ll.pushString("valid").pushBoolean(false).setTableIndex(1);
	ll.pushString("size").pushInteger(0).setTableIndex(1);
    }
    else
    {
        ERROR("userdata empty");
    }

    return rescount;
}

int SWE_streamfile_to_json(lua_State* L)
{   
    // params: swe_streamfile

    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);
    SWE_StreamFile* stream = SWE_StreamFile::get(ll, 1, __FUNCTION__);

    if(stream)
    {
        std::string name = ll.getFieldTableIndex("filename", 1).getTopString();
        std::string mode = ll.getFieldTableIndex("openmode", 1).getTopString();
	int pos = stream->tell();

        ll.stackPop(2);
        std::string str = StringFormat("{\"type\":\"%1\",\"filename\":\"%2\",\"mode\":\"%3\",\"tell\":%4}").
            arg("swe.streamfile").arg(name).arg(mode).arg(pos);

        ll.pushString(str);
    }
    else
    {
	ERROR("userdata empty");
        ll.pushNil();
    }

    return rescount;
}

const struct luaL_Reg SWE_streamfile_functions[] = {
    { "ReadByte", SWE_streamfile_read_byte },		// [int], swe_streamfile
    { "ReadBE16", SWE_streamfile_read_be16 },		// [int], swe_streamfile
    { "ReadBE32", SWE_streamfile_read_be32 },		// [int], swe_streamfile
    { "ReadBE64", SWE_streamfile_read_be64 },		// [number], swe_streamfile
    { "ReadLE16", SWE_streamfile_read_le16 },		// [int], swe_streamfile
    { "ReadLE32", SWE_streamfile_read_le32 },		// [int], swe_streamfile
    { "ReadLE64", SWE_streamfile_read_le64 },		// [number], swe_streamfile
    { "ReadBytes", SWE_streamfile_read_bytes }, 	// [swe_binarybuf], swe_streamfile, number size
    { "ReadString",SWE_streamfile_read_string }, 	// [string], swe_streamfile, number size
    { "WriteByte", SWE_streamfile_write_byte }, 	// [void], swe_streamfile, int
    { "WriteBE16", SWE_streamfile_write_be16 },		// [void], swe_streamfile, int
    { "WriteBE32", SWE_streamfile_write_be32 },		// [void], swe_streamfile, int
    { "WriteBE64", SWE_streamfile_write_be64 },		// [void], swe_streamfile, number
    { "WriteLE16", SWE_streamfile_write_le16 },		// [void], swe_streamfile, int
    { "WriteLE32", SWE_streamfile_write_le32 },		// [void], swe_streamfile, int
    { "WriteLE64", SWE_streamfile_write_le64 },		// [void], swe_streamfile, number
    { "WriteBytes", SWE_streamfile_write_bytes }, 	// [void], swe_streamfile, swe_binarybuf
    { "Seek", SWE_streamfile_seek }, 			// [bool], swe_streamfile, int, int
    { "Tell", SWE_streamfile_tell },			// [int], swe_streamfile
    { "Open", SWE_streamfile_open }, 			// [bool], swe_streamfile, string, string
    { "Close", SWE_streamfile_close }, 			// [swe_streamfile], swe_streamfile
    { "ToJson", SWE_streamfile_to_json },		// [string], swe_streamfile
    { NULL, NULL }
};

int SWE_streamfile_create(lua_State* L)
{
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    ll.pushTable();

    // userdata
    ll.pushString("userdata");
    auto ptr = static_cast<SWE_StreamFile**>(ll.pushUserData(sizeof(SWE_StreamFile*)));

    // set metatable: __gc
    ll.pushTable(0, 1);
    ll.pushFunction(SWE_streamfile_destroy).setFieldTableIndex("__gc", -2);
    ll.setMetaTableIndex(-2).setTableIndex(-3);

    if(ll.isStringIndex(2) && ll.isStringIndex(3))
    {
	std::string name = ll.toStringIndex(2);
	std::string mode = ll.toStringIndex(3);

	// SWE_StreamFile: file, mode
	SWE_StreamFile* stream = new SWE_StreamFile(name, mode);
	*ptr = stream;

	// add values
	ll.pushString("__type").pushString("swe.streamfile").setTableIndex(-3);
	ll.pushString("filename").pushString(stream->isValid() ? name : "").setTableIndex(-3);
	ll.pushString("openmode").pushString(stream->isValid() ? mode : "").setTableIndex(-3);
	ll.pushString("valid").pushBoolean(stream->isValid()).setTableIndex(1);
	ll.pushString("size").pushInteger(stream->size()).setTableIndex(1);
    }
    else
    {
	// SWE_StreamFile()
	*ptr = new SWE_StreamFile();

	// add values
	ll.pushString("__type").pushString("swe.streamfile").setTableIndex(-3);
	ll.pushString("valid").pushBoolean(false).setTableIndex(1);
    }

    // set functions
    ll.setFunctionsTableIndex(SWE_streamfile_functions, -1);

    DEBUGN(String::pointer(ptr) << ": [" << String::pointer(*ptr) << "]", 2);

    return rescount;
}

int SWE_streamfile_destroy(lua_State* L)
{
    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    if(ll.isTopUserData())
    {
        auto ptr = static_cast<SWE_StreamFile**>(ll.getTopUserData());
        if(ptr && *ptr)
        {
    	    DEBUGN(String::pointer(ptr) << ": [" << String::pointer(*ptr) << "]", 2);

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

void SWE_StreamFile::registers(LuaState & ll)
{
    LuaStateValidator(ll, 0);

    // SWE.StreamFile
    ll.pushTable("SWE.StreamFile");

    // set metatable: __call
    ll.pushTable(0, 1).pushFunction(SWE_streamfile_create).setFieldTableIndex("__call", -2);
    ll.setMetaTableIndex(-2).stackPop();

    // SWE.RW constants
    ll.pushTable("SWE.RW");
    ll.pushInteger(RW_SEEK_SET).setFieldTableIndex("SeekSet", -2);
    ll.pushInteger(RW_SEEK_CUR).setFieldTableIndex("SeekCur", -2);
    ll.pushInteger(RW_SEEK_END).setFieldTableIndex("SeekEnd", -2);
    ll.stackPop();
}

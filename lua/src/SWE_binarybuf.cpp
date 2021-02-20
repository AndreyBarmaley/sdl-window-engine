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
#include "SWE_binarybuf.h"

int SWE_binarybuf_create(lua_State*);
int SWE_binarybuf_destroy(lua_State*);

SWE_BinaryBuf* SWE_BinaryBuf::get(LuaState & ll, int tableIndex, const char* funcName)
{
    LuaStateValidator(ll, 0);

    if(! ll.isTableIndex(tableIndex) ||
	0 != ll.popFieldTableIndex("__type", tableIndex).compare("swe.binarybuf"))
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
    
    auto ptr = static_cast<SWE_BinaryBuf**>(ll.getTopUserData());
    ll.stackPop();

    return ptr ? *ptr : NULL;
}

///////////////////////////////////////////////////////////
int SWE_binarybuf_pushback(lua_State* L)
{
    // params: table unicodestring, int byte, int byte .. int byte
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_BinaryBuf* buf = SWE_BinaryBuf::get(ll, 1, __FUNCTION__))
    {
        int params = ll.stackSize();
        for(int ii = 2; ii <= params; ++ii)
        {
            int byte = ll.toIntegerIndex(ii);
            buf->push_back(byte);
        }

        ll.pushInteger(buf->size()).setFieldTableIndex("size", 1);
        ll.pushBoolean(true);
    }
    else
    {
	ERROR("userdata empty");
        ll.pushBoolean(false);
    }

    return rescount;
}

int SWE_binarybuf_zlib_compress(lua_State* L)
{
    // params: table binarybuf
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_BinaryBuf* buf = SWE_BinaryBuf::get(ll, 1, __FUNCTION__))
    {
	auto data = Tools::zlibCompress(buf->data(), buf->size());
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

int SWE_binarybuf_zlib_decompress(lua_State* L)
{
    // params: table binarybuf
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_BinaryBuf* buf = SWE_BinaryBuf::get(ll, 1, __FUNCTION__))
    {
	auto data = Tools::zlibUncompress(buf->data(), buf->size());
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

int SWE_binarybuf_base64_encode(lua_State* L)
{
    // params: table binarybuf
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_BinaryBuf* buf = SWE_BinaryBuf::get(ll, 1, __FUNCTION__))
    {
	BinaryBuf res = Tools::base64Encode(buf->data(), buf->size());
	ll.pushString(res.toString());
    }
    else
    { 
        ERROR("userdata empty");
	ll.pushNil();
    }

    return rescount;
}

int SWE_binarybuf_base64_decode1(lua_State* L)
{
    // params: table binarybuf
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_BinaryBuf* buf = SWE_BinaryBuf::get(ll, 1, __FUNCTION__))
    {
	auto data = Tools::base64Decode(buf->data(), buf->size());
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

int SWE_binarybuf_base64_decode2(lua_State* L)
{
    // params: string bas64
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(! ll.isStringIndex(1))
    {
        ERROR("string not found");
	ll.pushNil();
	return rescount;
    }

    std::string base64 = ll.toStringIndex(1);
    if(base64.size())
    {
	auto data = Tools::base64Decode(base64.c_str());
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

int SWE_binarybuf_base64_decode(lua_State* L)
{
    // params: table binarybuf or string bas64
    LuaState ll(L);

    if(ll.isTableIndex(1))
	return SWE_binarybuf_base64_decode1(L);
    else
    if(ll.isStringIndex(1))
	return SWE_binarybuf_base64_decode2(L);

    ERROR("unknown params");
    ll.pushNil();

    return 1;
}

int SWE_binarybuf_readfile1(lua_State* L)
{
    // params: table binarybuf, string, int
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    std::string filename = ll.toStringIndex(2);

    if(! Systems::isFile(filename))
    {
	std::string filename2 = SWE_Tools::findResource(ll, filename);
	if(Systems::isFile(filename2)) std::swap(filename, filename2);
    }

    if(Systems::isFile(filename))
    {
	DEBUG(filename);

	if(SWE_BinaryBuf* buf = SWE_BinaryBuf::get(ll, 1, __FUNCTION__))
	{
	    int offset = ll.isNumberIndex(2) ? ll.toNumberIndex(2) : 0;
	    int size = ll.isNumberIndex(3) ? ll.toNumberIndex(3) : 0;
	    auto data = Systems::readFile(filename, offset, size);

	    buf->swap(data);
	    ll.pushInteger(buf->size()).setFieldTableIndex("size", 1);
	    ll.pushBoolean(buf->size());
	}
	else
	{
	    ERROR("userdata empty");
	    ll.pushBoolean(false);
	}
    }
    else
    {
	ERROR("file not found: " << filename);
	ll.pushBoolean(false);
    }

    return rescount;
}

int SWE_binarybuf_readfile2(lua_State* L)
{
    // params: string
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    std::string filename = ll.toStringIndex(1);

    if(! Systems::isFile(filename))
    {
	std::string filename2 = SWE_Tools::findResource(ll, filename);
	if(Systems::isFile(filename2)) std::swap(filename, filename2);
    }

    if(Systems::isFile(filename))
    {
	DEBUG(filename);

	int offset = ll.isNumberIndex(2) ? ll.toNumberIndex(2) : 0;
	int size = ll.isNumberIndex(3) ? ll.toNumberIndex(3) : 0;
	auto data = Systems::readFile(filename, offset, size);

	SWE_BinaryBuf* res = SWE_Stack::binarybuf_create(ll);
	res->swap(data);
	ll.pushString("size").pushInteger(res->size()).setTableIndex(-3);
    }
    else
    {
	ERROR("file not found: " << filename);
	ll.pushNil();
    }

    return rescount;
}

int SWE_binarybuf_readfile(lua_State* L)
{
    // params: table binarybuf or string
    LuaState ll(L);

    if(ll.isTableIndex(1))
	return SWE_binarybuf_readfile1(L);
    else
    if(ll.isStringIndex(1))
	return SWE_binarybuf_readfile2(L);
        
    ERROR("unknown params");
    ll.pushNil();

    return 1;
}

int SWE_binarybuf_savefile(lua_State* L)
{
    // params: table binarybuf, string
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(! ll.isStringIndex(2))
    {
        ERROR("string not found");
	ll.pushBoolean(false);
	return rescount;
    }

    if(SWE_BinaryBuf* buf = SWE_BinaryBuf::get(ll, 1, __FUNCTION__))
    {
	std::string filename = ll.toStringIndex(2);
	int offset = ll.isNumberIndex(3) ? ll.toNumberIndex(3) : 0;

	bool res = Systems::saveFile(*buf, filename, offset);
	ll.pushBoolean(res);
    }
    else
    { 
        ERROR("userdata empty");
	ll.pushBoolean(false);
    }

    return rescount;
}

int SWE_binarybuf_to_hexstring(lua_State* L)
{
    // params: table binarybuf, string, boolean
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_BinaryBuf* buf = SWE_BinaryBuf::get(ll, 1, __FUNCTION__))
    {
	std::string sep = ll.isStringIndex(2) ? ll.toStringIndex(2) : ",";
	bool prefix = ll.isBooleanIndex(3) ? ll.toBooleanIndex(3) : true;
	ll.pushString(buf->toHexString(sep, prefix));
    }
    else
    { 
        ERROR("userdata empty");
	ll.pushNil();
    }

    return rescount;
}

int SWE_binarybuf_to_cstring(lua_State* L)
{
    // params: table binarybuf
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_BinaryBuf* buf = SWE_BinaryBuf::get(ll, 1, __FUNCTION__))
    {
	ll.pushString(buf->toString());
	return 1;
    }
    else
    {
	ERROR("userdata empty");
	ll.pushNil();
    }

    return rescount;
}

int SWE_binarybuf_setbyte(lua_State* L)
{
    // params: table binarybuf, int offset, int byte, int byte ... int byte
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    int params = ll.stackSize();
    if(SWE_BinaryBuf* buf = SWE_BinaryBuf::get(ll, 1, __FUNCTION__))
    {
	int offset = ll.toIntegerIndex(2);
	if(0 <= offset && offset <= buf->size())
	{
    	    for(int ii = 3; ii <= params; ++ii)
    	    {
    		int byte = ll.toIntegerIndex(ii);
		if(offset < buf->size())
		    buf->operator[](offset) = byte;
		else
		    buf->push_back(byte);
		offset += 1;
    	    }

    	    ll.pushInteger(buf->size()).setFieldTableIndex("size", 1);
    	    ll.pushBoolean(true);
	}
	else
	{
    	    ERROR("out of range");
	    ll.pushBoolean(false);
	}
	return 1;
    }
    else
    {
	ERROR("userdata empty");
	ll.pushBoolean(false);
    }

    return rescount;
}

int SWE_binarybuf_getbyte(lua_State* L)
{
    // params: table binarybuf, int offset
    // returned variable size
    LuaState ll(L);

    if(SWE_BinaryBuf* buf = SWE_BinaryBuf::get(ll, 1, __FUNCTION__))
    {
	int offset1 = ll.toIntegerIndex(2);
	int offset2 = ll.isNumberIndex(3) ? ll.toIntegerIndex(3) : -1;

	if(0 <= offset2)
	{
	    if(offset2 < buf->size() && 0 <= offset1 && offset1 <= offset2)
	    {
		for(int offset = offset1; offset <= offset2; ++offset)
		    ll.pushInteger(buf->operator[](offset));

		return offset2 - offset1 + 1;
	    }
	    else
	    {
    		ERROR("out of range " << buf->size() << ", " << offset1 << ", " << offset2);
		ll.pushNil();

		return 1;
	    }
	}
	else
	{
	    if(0 <= offset1 && offset1 < buf->size())
	    {
		ll.pushInteger(buf->operator[](offset1));
	    }
	    else
	    {
    		ERROR("out of range " << buf->size() << offset1);
		ll.pushNil();
	    }

	    return 1;
	}
    }
    else
    {
	ERROR("userdata empty");
	ll.pushNil();
    }

    return 1;
}

int SWE_binarybuf_getbytes(lua_State* L)
{
    // params: table binarybuf, int offset, int size
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_BinaryBuf* buf = SWE_BinaryBuf::get(ll, 1, __FUNCTION__))
    {
	int pos = ll.toIntegerIndex(2);
	int len = ll.toIntegerIndex(3);

        if(0 <= pos && pos < buf->size())
        {
            if(len <= 0 || pos + len > buf->size())
                len = buf->size() - pos;

	    SWE_BinaryBuf* res = SWE_Stack::binarybuf_create(ll);
	    res->assign(buf->data() + pos, buf->data() + pos + len);
	    ll.pushString("size").pushInteger(res->size()).setTableIndex(-3);
	}
	else
	{
    	    ERROR("out of range");
	    ll.pushNil();
	}
    }
    else
    {
	ERROR("userdata empty");
        ll.pushNil();
    }

    return rescount;
}

int SWE_binarybuf_insert(lua_State* L)
{
    // params: table binarybuf, int offset, (binarybuf, subpos, sublen), (int count, int byte)
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_BinaryBuf* buf1 = SWE_BinaryBuf::get(ll, 1, __FUNCTION__))
    {
        int pos = ll.toIntegerIndex(2);

	if(0 > pos || pos > buf1->size())
        {
            ERROR("out of range");
            ll.pushBoolean(false);
	}
	else
        // params: int count, int byte
        if(ll.isNumberIndex(3))
        {
            int count = ll.toIntegerIndex(3);
            int ch = ll.toIntegerIndex(4);

            buf1->insert(buf1->begin() + pos, count, ch);
            ll.pushInteger(buf1->size()).setFieldTableIndex("size", 1);
            ll.pushBoolean(true);
        }
        else
	// params: table binarybuf, int subpos, int sublen
        if(ll.isTableIndex(3))
        {
            if(SWE_BinaryBuf* buf2 = SWE_BinaryBuf::get(ll, 3, __FUNCTION__))
            {
                int params = ll.stackSize();
                if(3 < params)
                {
                    int subpos = ll.toIntegerIndex(4);
                    int sublen = ll.toIntegerIndex(5);

                    if(0 <= subpos && subpos < buf2->size())
                    {
                        if(sublen <= 0 || subpos + sublen > buf2->size())
                            sublen = buf2->size() - subpos;

                        buf1->insert(buf1->begin() + pos, buf2->begin() + subpos, buf2->begin() + subpos + sublen);
                        ll.pushInteger(buf1->size()).setFieldTableIndex("size", 1);
                        ll.pushBoolean(true);
                    }
                    else
                    {
                        ERROR("out of range");
                        ll.pushBoolean(false);
                    }
                }
                else
                {
                    buf1->insert(buf1->begin() + pos, buf2->begin(), buf2->end());
                    ll.pushInteger(buf1->size()).setFieldTableIndex("size", 1);
                    ll.pushBoolean(true);
                }
            }
            else
            {
                ERROR("table not found" << ": " << "swe.binarybuf");
                ll.pushBoolean(false);
            }
        }
    }       
    else
    {
	ERROR("userdata empty");
        ll.pushBoolean(false);
    }

    return rescount;
}

int SWE_binarybuf_setbytes(lua_State* L)
{
    // params: table binarybuf, int offset, table binarybuf, int offset, int size
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    SWE_BinaryBuf* buf1 = SWE_BinaryBuf::get(ll, 1, __FUNCTION__);
    SWE_BinaryBuf* buf2 = SWE_BinaryBuf::get(ll, 3, __FUNCTION__);

    if(buf1 && buf2)
    {
	int offset1 = ll.toIntegerIndex(2);
	int offset2 = ll.toIntegerIndex(4);
	int size = ll.toIntegerIndex(5);

	if(0 <= offset2 && offset2 + size < buf2->size() + 1 &&
	    0 <= offset1 && offset1 < buf1->size() + 1)
	{
	    for(int pos = 0; pos < size; ++pos)
	    {
		if(offset1 + pos < buf1->size() + 1)
		    buf1->operator[](offset1 + pos) = buf2->operator[](offset2 + pos);
		else
		    buf1->push_back(buf2->operator[](offset2 + pos));
	    }

	    ll.pushInteger(buf1->size()).setFieldTableIndex("size", 1);
	    ll.pushBoolean(true);
	}
	else
	{
    	    ERROR("out of range");
	    ll.pushBoolean(false);
	}
    }
    else
    {
	ERROR("userdata empty");
        ll.pushBoolean(false);
    }

    return rescount;
}

int SWE_binarybuf_getcrc32b(lua_State* L)
{
    // params: table binarybuf, int offset
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_BinaryBuf* buf = SWE_BinaryBuf::get(ll, 1, __FUNCTION__))
    {
	ll.pushInteger(buf->crc32b());
    }
    else
    {
	ERROR("userdata empty");
	ll.pushInteger(0);
    }

    return rescount;
}

int SWE_binarybuf_to_json(lua_State* L)
{
    // params: table binarybuf
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_BinaryBuf* buf = SWE_BinaryBuf::get(ll, 1, __FUNCTION__))
    {
	std::string json = StringFormat("[%1]").arg(buf->toHexString(",", true));
	ll.pushString(json);
    }
    else
    {
	ERROR("userdata empty");
	ll.pushNil();
    }

    return rescount;
}

int SWE_binarybuf_tostring(lua_State* L)
{
    // params: table binarybuf
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_BinaryBuf* buf = SWE_BinaryBuf::get(ll, 1, __FUNCTION__))
    {
	std::string str = StringFormat("%1[%2]").arg("swe.binarybuf").arg(buf->toHexString(",", true));
	ll.pushString(str);
    }
    else
    {
	ERROR("userdata empty");
	ll.pushNil();
    }

    return rescount;
}

int SWE_binarybuf_clear(lua_State* L)
{
    // params: table binarybuf, int offset
    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    if(SWE_BinaryBuf* buf = SWE_BinaryBuf::get(ll, 1, __FUNCTION__))
    {
	buf->clear();
	ll.pushInteger(0).setFieldTableIndex("size", -2);
    }
    else
    {
	ERROR("userdata empty");
    }

    return rescount;
}

int SWE_binarybuf_erase(lua_State* L)
{
    // params: table binarybuf, int size, int val
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    int params = ll.stackSize();
    if(SWE_BinaryBuf* buf = SWE_BinaryBuf::get(ll, 1, __FUNCTION__))
    {
        int pos = ll.toIntegerIndex(2);
        int count = 3 > params ? 1 : ll.toIntegerIndex(3);

        if(0 <= pos && pos < buf->size() && 0 < count && pos + count <= buf->size())
        {
            if(1 == count)
                buf->erase(buf->begin() + pos);
            else
                buf->erase(buf->begin() + pos, buf->begin() + pos + count);

            ll.pushString("size").pushInteger(buf->size()).setTableIndex(1);
            ll.pushBoolean(true);
        }
        else
        {
            ERROR("out of range" << ", " << pos << ", " << buf->size());
            ll.pushBoolean(false);
        }
    }
    else
    {
        ERROR("userdata empty");
        ll.pushBoolean(false);
    }

    return rescount;
}

int SWE_binarybuf_resize(lua_State* L)
{
    // params: table binarybuf, int size, int val
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_BinaryBuf* buf = SWE_BinaryBuf::get(ll, 1, __FUNCTION__))
    {
        int size = ll.toIntegerIndex(2);

        if(0 < size)
        {
            int val = ll.toIntegerIndex(3);
            buf->resize(size, val);
            ll.pushString("size").pushInteger(buf->size()).setTableIndex(1);
            ll.pushBoolean(true);
        }
        else
	if(0 == size)
        {
            buf->clear();
            ll.pushString("size").pushInteger(buf->size()).setTableIndex(1);
            ll.pushBoolean(true);
        }
        else
        {
            ERROR("out of range");
            ll.pushBoolean(false);
        }
    }
    else
    {
        ERROR("userdata empty");
        ll.pushBoolean(false);
    }

    return rescount;
}

const struct luaL_Reg SWE_binarybuf_functions[] = {
    { "PushBack", SWE_binarybuf_pushback },             // [bool], table binarybuf, int byte, int byte ... int byte
    { "ZlibCompress", SWE_binarybuf_zlib_compress },	// [table binarybuf], table binarybuf
    { "ZlibDecompress", SWE_binarybuf_zlib_decompress },// [table binarybuf], table binarybuf
    { "Base64Decode", SWE_binarybuf_base64_decode },	// [table binarybuf], table binarybuf | string base64
    { "Base64Encode", SWE_binarybuf_base64_encode },	// [string base64], table binarybuf
    { "GetCRC32b", SWE_binarybuf_getcrc32b },		// [int crc], table binarybuf
    { "ReadFromFile", SWE_binarybuf_readfile },		// [bool], table binarybuf, string filename, number
    { "SaveToFile", SWE_binarybuf_savefile },		// [bool], table binarybuf, string filename, number
    { "ToString", SWE_binarybuf_to_cstring },		// [string], table binarybuf
    { "ToJson", SWE_binarybuf_to_json },		// [string], table binarybuf
    { "SetByte", SWE_binarybuf_setbyte },		// [bool], table binarybuf, int offset, int byte, int byte ... int byte
    { "GetByte", SWE_binarybuf_getbyte },		// [int byte], table binarybuf, int offset1, int offset2
    { "SetBytes", SWE_binarybuf_setbytes },		// [bool], table binarybuf, int offset, table binarybuf, int offset, int size
    { "GetBytes", SWE_binarybuf_getbytes },		// [table binarybuf], table binarybuf, int offset, int size
    { "Insert", SWE_binarybuf_insert },                 // [bool], table binarybuf, int offset, (binarybuf, subpos, sublen), (int count, int byte)
    { "Clear", SWE_binarybuf_clear },			// [void], table binarybuf
    { "Resize", SWE_binarybuf_resize },                 // [bool], table binarybuf, int size, int val
    { "Erase", SWE_binarybuf_erase },                   // [bool], table binarybuf, int pos, int count
    { NULL, NULL }
};

void SWE_BinaryBuf::registers(LuaState & ll)
{
    LuaStateValidator(ll, 0);

    // SWE.BinaryBuf
    ll.pushTable("SWE.BinaryBuf");
    ll.setFunctionsTableIndex(SWE_binarybuf_functions, -1);

    // set metatable: __call
    ll.pushTable(0, 1);
    ll.pushFunction(SWE_binarybuf_create).setFieldTableIndex("__call", -2);
    ll.setMetaTableIndex(-2).stackPop();
}

SWE_BinaryBuf* SWE_Stack::binarybuf_create(LuaState & ll)
{
    LuaStateValidator(ll, 1);

    // result
    ll.pushTable();

    // set: tostring
    ll.pushTable(0, 1);
    ll.pushFunction(SWE_binarybuf_tostring).setFieldTableIndex("__tostring", -2);
    ll.setMetaTableIndex(-2);

    // userdata
    ll.pushString("userdata");
    auto ptr = static_cast<SWE_BinaryBuf**>(ll.pushUserData(sizeof(SWE_BinaryBuf*)));
    *ptr = new SWE_BinaryBuf();

    // set metatable: __gc to userdata
    ll.pushTable(0, 1);
    ll.pushFunction(SWE_binarybuf_destroy).setFieldTableIndex("__gc", -2);
    ll.setMetaTableIndex(-2).setTableIndex(-3);

    // set functions
    ll.setFunctionsTableIndex(SWE_binarybuf_functions, -1);

    DEBUG(String::pointer(ptr) << ": [" << String::pointer(*ptr) << "]");

    ll.pushString("__type").pushString("swe.binarybuf").setTableIndex(-3);
    ll.pushString("size").pushInteger((*ptr)->size()).setTableIndex(-3);

    return *ptr;
}

int SWE_binarybuf_create(lua_State* L)
{
    // SWE.BinaryBuf(self, string)
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    SWE_BinaryBuf* buf = SWE_Stack::binarybuf_create(ll);

    // SWE_BinaryBuf: length, fill value
    if(ll.isNumberIndex(2) && ll.isNumberIndex(3))
    {
	int bsz = ll.toIntegerIndex(2);
	int bvl = ll.toIntegerIndex(3);
	buf->assign(bsz, bvl);
        ll.pushString("size").pushInteger(buf->size()).setTableIndex(-3);
    }
    else
    // SWE_BinaryBuf: string
    if(ll.isStringIndex(2))
    {
	auto str = ll.toStringIndex(2);
	buf->assign(str.begin(), str.end());
	ll.pushString("size").pushInteger(buf->size()).setTableIndex(-3);
    }
    else
    // SWE_BinaryBuf: pointer, size
    if((ll.isUserDataIndex(2) || ll.isLightUserDataIndex(2)) && ll.isNumberIndex(3))
    {
	auto it1 = static_cast<const char*>(ll.toUserDataIndex(2));
	auto it2 = it1 + ll.toIntegerIndex(3);
	buf->assign(it1, it2);
	ll.pushString("size").pushInteger(buf->size()).setTableIndex(-3);
    }

    return rescount;
}

int SWE_binarybuf_destroy(lua_State* L)
{
    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    if(ll.isTopUserData())
    {
        auto ptr = static_cast<SWE_BinaryBuf**>(ll.getTopUserData());
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

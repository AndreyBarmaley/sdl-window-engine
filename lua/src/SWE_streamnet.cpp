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
#include "SWE_streamnet.h"

int SWE_streamnet_create(lua_State*);
int SWE_streamnet_destroy(lua_State*);

SWE_StreamNet* SWE_StreamNet::get(LuaState & ll, int tableIndex, const char* funcName)
{
    LuaStateValidator(ll, 0);

    if(! ll.isTableIndex(tableIndex) ||
	0 != ll.popFieldTableIndex("__type", tableIndex).compare("swe.streamnet"))
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

    auto ptr = static_cast<SWE_StreamNet**>(ll.getTopUserData());
    ll.stackPop();

    return ptr ? *ptr : NULL;
}

/////////////////////////////////////////////////////////////////////
int SWE_streamnet_recv_value_type(lua_State* L, IntType type)
{
#ifdef SWE_DISABLE_NETWORK
        ERROR("not supported module: " << "swe.streamnet");
	return 0;
#else
    // params: swe_streamnet

    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_StreamNet* stream = SWE_StreamNet::get(ll, 1, __FUNCTION__))
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
#endif
}

int SWE_streamnet_recv_byte(lua_State* L)
{
    return SWE_streamnet_recv_value_type(L, IntType::Int8);
}

int SWE_streamnet_recv_be16(lua_State* L)
{
    return SWE_streamnet_recv_value_type(L, IntType::IntBE16);
}

int SWE_streamnet_recv_be32(lua_State* L)
{
    return SWE_streamnet_recv_value_type(L, IntType::IntBE32);
}

int SWE_streamnet_recv_be64(lua_State* L)
{
    return SWE_streamnet_recv_value_type(L, IntType::IntBE64);
}

int SWE_streamnet_recv_le16(lua_State* L)
{
    return SWE_streamnet_recv_value_type(L, IntType::IntLE16);
}

int SWE_streamnet_recv_le32(lua_State* L)
{
    return SWE_streamnet_recv_value_type(L, IntType::IntLE32);
}

int SWE_streamnet_recv_le64(lua_State* L)
{
    return SWE_streamnet_recv_value_type(L, IntType::IntLE64);
}

int SWE_streamnet_recv_bytes(lua_State* L)
{
#ifdef SWE_DISABLE_NETWORK
        ERROR("not supported module: " << "swe.streamnet");
        return 0;
#else
    // params: swe_streamnet, size
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_StreamNet* stream = SWE_StreamNet::get(ll, 1, __FUNCTION__))
    {
	int size = ll.toIntegerIndex(2);
        auto data = static_cast<StreamNetwork*>(stream)->get(size);

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
#endif
}

int SWE_streamnet_recv_string(lua_State* L)
{
#ifdef SWE_DISABLE_NETWORK
        ERROR("not supported module: " << "swe.streamnet");
        return 0;
#else
    // params: swe_streamnet, int endl, int wait ms

    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    SWE_StreamNet* stream = SWE_StreamNet::get(ll, 1, __FUNCTION__);
    if(stream)
    {
	std::string res;
	int endl = ll.toIntegerIndex(2);
	int wait = ll.isNumberIndex(3) ? ll.toIntegerIndex(3) : 2000; // default: 2000 ms
	bool forcebr = ll.isBooleanIndex(4) ? ll.toBooleanIndex(4) : false;
	u32 start = Tools::ticks();
	int byte = 0;

	while(stream->isEnabled() && !stream->fail())
	{
	    if(stream->ready(0))
	    {
		byte = stream->get8();
		if(byte) res.append(1, byte);
		if(byte == endl) break;
	    }
	    else
	    {
    	        DisplayScene::handleEvents(300);
		// DEBUG("wait string data");
	    }
    
	    if(0 < wait && start + wait < Tools::ticks())
	    {
		// empty data or force break
		if(res.empty() || forcebr)
        	    break;
 	    }
	}

	ll.pushString(res);
    }
    else
    {
        ERROR("userdata empty");
	ll.pushNil();
    }

    return rescount;
#endif
}

int SWE_streamnet_send_bytes(lua_State* L)
{
#ifdef SWE_DISABLE_NETWORK
        ERROR("not supported module: " << "swe.streamnet");
        return 0;
#else
    // params: swe_streamnet, swe_binarybuf

    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    SWE_StreamNet* stream = SWE_StreamNet::get(ll, 1, __FUNCTION__);
    SWE_BinaryBuf* buf = SWE_BinaryBuf::get(ll, 2, __FUNCTION__);

    if(stream && buf)
    {
        stream->put(reinterpret_cast<const char*>(buf->data()), buf->size());
    }
    else
    {
        ERROR("userdata empty");
    }

    return rescount;
#endif
}

int SWE_streamnet_send_string(lua_State* L)
{
#ifdef SWE_DISABLE_NETWORK
        ERROR("not supported module: " << "swe.streamnet");
        return 0;
#else
    // params: swe_streamnet, string

    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    SWE_StreamNet* stream = SWE_StreamNet::get(ll, 1, __FUNCTION__);
    if(stream)
    {
	std::string str = ll.toStringIndex(2);
        stream->put(str.data(), str.size());
    }
    else
    {
        ERROR("userdata empty");
    }

    return rescount;
#endif
}

int SWE_streamnet_send_value_type(lua_State* L, IntType type)
{
    // params: swe_streamnet
#ifdef SWE_DISABLE_NETWORK
        ERROR("not supported module: " << "swe.streamnet");
        return 0;
#else
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_StreamNet* stream = SWE_StreamNet::get(ll, 1, __FUNCTION__))
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
    }
    else
    {
        ERROR("userdata empty");
        ll.pushNil();
    }
        
    return rescount;
#endif
}       

int SWE_streamnet_send_byte(lua_State* L)
{
    return SWE_streamnet_send_value_type(L, IntType::Int8);
}

int SWE_streamnet_send_be16(lua_State* L)
{
    return SWE_streamnet_send_value_type(L, IntType::IntBE16);
}

int SWE_streamnet_send_be32(lua_State* L)
{
    return SWE_streamnet_send_value_type(L, IntType::IntBE32);
}

int SWE_streamnet_send_be64(lua_State* L)
{
    return SWE_streamnet_send_value_type(L, IntType::IntBE64);
}

int SWE_streamnet_send_le16(lua_State* L)
{
    return SWE_streamnet_send_value_type(L, IntType::IntLE16);
}

int SWE_streamnet_send_le32(lua_State* L)
{
    return SWE_streamnet_send_value_type(L, IntType::IntLE32);
}

int SWE_streamnet_send_le64(lua_State* L)
{
    return SWE_streamnet_send_value_type(L, IntType::IntLE64);
}

int SWE_streamnet_wait_string(lua_State* L)
{
#ifdef SWE_DISABLE_NETWORK
        ERROR("not supported module: " << "swe.streamnet");
        return 0;
#else
    // params: swe_streamnet, string

    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    SWE_StreamNet* stream = SWE_StreamNet::get(ll, 1, __FUNCTION__);
    std::string str = ll.toStringIndex(2);

    if(stream && str.size())
    {
	bool res = stream->wait(str);
	ll.pushBoolean(res);
    }
    else
    if(str.empty())
    {
        ERROR("string empty");
	ll.pushNil();
    }
    else
    {
        ERROR("userdata empty");
	ll.pushNil();
    }

    return rescount;
#endif
}

int SWE_streamnet_connect(lua_State* L)
{
#ifdef SWE_DISABLE_NETWORK
        ERROR("not supported module: " << "swe.streamnet");
        return 0;
#else
    // params: swe_streamnet, string, int port

    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    SWE_StreamNet* stream = SWE_StreamNet::get(ll, 1, __FUNCTION__);
    if(stream)
    {
	std::string server = ll.toStringIndex(2);
	int port = ll.toIntegerIndex(3);
	bool res = stream->connect(server, port);

	if(res)
	{
	    ll.pushString("address").pushString(server).setTableIndex(1);
	    ll.pushString("port").pushInteger(port).setTableIndex(1);
	}

	ll.pushBoolean(res);
    }
    else
    {
	ll.pushNil();
	ERROR("userdata empty");
    }

    return rescount;
#endif
}

int SWE_streamnet_data_ready(lua_State* L)
{
#ifdef SWE_DISABLE_NETWORK
        ERROR("not supported module: " << "swe.streamnet");
        return 0;
#else
    // params: swe_streamnet, int port

    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    SWE_StreamNet* stream = SWE_StreamNet::get(ll, 1, __FUNCTION__);
    if(stream)
    {
	bool res = stream->ready();
	ll.pushBoolean(res);
    }
    else
    {
	ERROR("userdata empty");
	ll.pushNil();
    }

    return rescount;
#endif
}

int SWE_streamnet_listen(lua_State* L)
{
#ifdef SWE_DISABLE_NETWORK
        ERROR("not supported module: " << "swe.streamnet");
        return 0;
#else
    // params: swe_streamnet, int port
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    SWE_StreamNet* stream = SWE_StreamNet::get(ll, 1, __FUNCTION__);
    if(stream)
    {
	int port = ll.toIntegerIndex(2);
	bool res = stream->listen(port);

	if(res)
	{
	    ll.pushString("address").pushString("0.0.0.0").setTableIndex(1);
	    ll.pushString("port").pushInteger(port).setTableIndex(1);
	}

	ll.pushBoolean(res);
    }
    else
    {
	ERROR("userdata empty");
	ll.pushNil();
    }

    return rescount;
#endif
}

int SWE_streamnet_wait_accept(lua_State* L)
{
#ifdef SWE_DISABLE_NETWORK
        ERROR("not supported module: " << "swe.streamnet");
        return 0;
#else
    // params: swe_streamnet
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_StreamNet* stream = SWE_StreamNet::get(ll, 1, __FUNCTION__))
    {
	TCPsocket sock = NULL;

	while(stream->isEnabled() && !stream->fail())
	{
	    sock = stream->accept();
	    if(sock) break;

    	    DisplayScene::handleEvents(20);
	}

	if(sock)
	{
	    SWE_StreamNet* stream2 = SWE_Stack::streamnet_create(ll);
	    if(stream2->open(sock))
	    {
		std::pair<std::string, int> info = StreamNetwork::peerAddress(sock);
    		ll.pushString("address").pushString(info.first).setTableIndex(-3);
		ll.pushString("port").pushInteger(info.second).setTableIndex(-3);
		ll.pushString("enabled").pushBoolean(true).setTableIndex(-3);
	    }
	}
	else
	{
    	    ERROR("accept faled");
	    ll.pushNil();
	}
    }
    else
    {
        ERROR("userdata empty");
	ll.pushNil();
    }

    return rescount;
#endif
}

int SWE_streamnet_disable(lua_State* L)
{
#ifdef SWE_DISABLE_NETWORK
        ERROR("not supported module: " << "swe.streamnet");
        return 0;
#else
    // params: swe_streamnet
    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    SWE_StreamNet* stream = SWE_StreamNet::get(ll, 1, __FUNCTION__);
    if(stream)
    {
	bool enabled = ll.toBooleanIndex(2);
	stream->setDisable(enabled);
	ll.pushString("enabled").pushBoolean(enabled).setTableIndex(1);
    }
    else
    {
        ERROR("userdata empty");
    }

    return rescount;
#endif
}

int SWE_streamnet_close(lua_State* L)
{
#ifdef SWE_DISABLE_NETWORK
        ERROR("not supported module: " << "swe.streamnet");
        return 0;
#else
    // params: swe_streamnet

    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    SWE_StreamNet* stream = SWE_StreamNet::get(ll, 1, __FUNCTION__);
    if(stream)
    {
	stream->close();
    }
    else
    {
        ERROR("userdata empty");
    }

    return rescount;
#endif
}

int SWE_streamnet_to_json(lua_State* L)
{   
#ifdef SWE_DISABLE_NETWORK
        ERROR("not supported module: " << "swe.streamnet");
        return 0;
#else
    // params: swe_streamnet

    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    SWE_StreamNet* stream = SWE_StreamNet::get(ll, 1, __FUNCTION__);
    if(stream)
    {
        std::string address = ll.getFieldTableIndex("address", 1).getTopString();
        int port = ll.getFieldTableIndex("port", 1).getTopInteger();

        ll.stackPop(2);
        std::string str = StringFormat("{\"type\":\"%1\",\"address\":\"%2\",\"port\":%3}").
            arg("swe.streamnet").arg(address).arg(port);

        ll.pushString(str);
    }
    else
    {
	ERROR("userdata empty");
	ll.pushNil();
    }

    return rescount;
#endif
}

const struct luaL_Reg SWE_streamnet_functions[] = {
    { "RecvByte", SWE_streamnet_recv_byte },		// [int], swe_streamnet
    { "RecvBE16", SWE_streamnet_recv_be16 },		// [int], swe_streamnet
    { "RecvBE32", SWE_streamnet_recv_be32 },		// [int], swe_streamnet
    { "RecvBE64", SWE_streamnet_recv_be64 },		// [number], swe_streamnet
    { "RecvLE16", SWE_streamnet_recv_le16 },		// [int], swe_streamnet
    { "RecvLE32", SWE_streamnet_recv_le32 },		// [int], swe_streamnet
    { "RecvLE64", SWE_streamnet_recv_le64 },		// [number], swe_streamnet
    { "RecvBytes", SWE_streamnet_recv_bytes }, 		// [swe_binarybuf], swe_streamnet, number size
    { "RecvString", SWE_streamnet_recv_string },	// [string], swe_streamnet, int endl
    { "SendByte", SWE_streamnet_send_byte }, 		// [void], swe_streamnet, int
    { "SendBE16", SWE_streamnet_send_be16 },		// [void], swe_streamnet, int
    { "SendBE32", SWE_streamnet_send_be32 },		// [void], swe_streamnet, int
    { "SendBE64", SWE_streamnet_send_be64 },		// [void], swe_streamnet, number
    { "SendLE16", SWE_streamnet_send_le16 },		// [void], swe_streamnet, int
    { "SendLE32", SWE_streamnet_send_le32 },		// [void], swe_streamnet, int
    { "SendLE64", SWE_streamnet_send_le64 },		// [void], swe_streamnet, number
    { "SendBytes", SWE_streamnet_send_bytes }, 		// [void], swe_streamnet, swe_binarybuf
    { "SendString", SWE_streamnet_send_string },	// [void], swe_streamnet, string
    { "WaitString", SWE_streamnet_wait_string },	// [bool], swe_streamnet, string
    { "Connect", SWE_streamnet_connect },		// [bool], swe_streamnet, string, int port
    { "Listen", SWE_streamnet_listen },			// [bool], swe_streamnet, int port
    { "SetDisable", SWE_streamnet_disable },		// [void], swe_streamnet, bool
    { "DataReady", SWE_streamnet_data_ready },		// [bool], swe_streamnet
    { "WaitAccept", SWE_streamnet_wait_accept }, 	// [swe_streamnet], swe_streamnet
    { "Close", SWE_streamnet_close }, 			// [swe_streamnet], swe_streamnet
    { "ToJson", SWE_streamnet_to_json },		// [string], swe_streamnet
    { NULL, NULL }
};

SWE_StreamNet* SWE_Stack::streamnet_create(LuaState & ll)
{
    LuaStateValidator(ll, 1);

    // result
    ll.pushTable();

    // userdata
    ll.pushString("userdata");
    auto ptr = static_cast<SWE_StreamNet**>(ll.pushUserData(sizeof(SWE_StreamNet*)));
    *ptr = new SWE_StreamNet();

    // set metatable: __gc
    ll.pushTable(0, 1);
    ll.pushFunction(SWE_streamnet_destroy).setFieldTableIndex("__gc", -2);
    ll.setMetaTableIndex(-2).setTableIndex(-3);

    // set functions
    ll.setFunctionsTableIndex(SWE_streamnet_functions, -1);

    DEBUGN(String::pointer(ptr) << ": [" << String::pointer(*ptr) << "]", 2);

    // add values
    ll.pushString("__type").pushString("swe.streamnet").setTableIndex(-3);

    return *ptr;
}

int SWE_streamnet_create(lua_State* L)
{
#ifdef SWE_DISABLE_NETWORK
    ERROR("not supported module: " << "swe.streamnet");
    return 0;
#else

    // empty params
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    SWE_StreamNet* net = SWE_Stack::streamnet_create(ll);

    if(ll.isStringIndex(2) && ll.isNumberIndex(3))
    {
	std::string server = ll.toStringIndex(2);
	int port = ll.toIntegerIndex(3);

	// SWE_StreamNet: server, port
	net->connect(server, port);

	// add values
	ll.pushString("address").pushString(server).setTableIndex(-3);
	ll.pushString("port").pushInteger(port).setTableIndex(-3);
	ll.pushString("enabled").pushBoolean(true).setTableIndex(-3);
    }

    return rescount;
#endif
}

int SWE_streamnet_destroy(lua_State* L)
{
    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    if(ll.isTopUserData())
    {
        auto ptr = static_cast<SWE_StreamNet**>(ll.getTopUserData());
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

int SWE_streamnet_local_addresses(lua_State* L)
{
#ifdef SWE_DISABLE_NETWORK
        ERROR("not supported module: " << "swe.streamnet");
        return 0;
#else
    // returned variable size
    LuaState ll(L);

    StringList addrs = StreamNetwork::localAddresses();
    ll.pushTable(addrs.size(), 0);
    int field = 1;

    for(auto & str : addrs)
	ll.pushString(str).setIndexTableIndex(field++, -2);

    return 1;
#endif
}

int SWE_streamnet_supported(lua_State* L)
{   
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

#ifdef SWE_DISABLE_NETWORK
    ll.pushBoolean(false);
#else
    ll.pushBoolean(true);
#endif

    return rescount;
}

const struct luaL_Reg SWE_streamnet_functions2[] = {
    { "IsSupported", SWE_streamnet_supported },
    { "LocalAddresses", SWE_streamnet_local_addresses },
    { NULL, NULL }
};


void SWE_StreamNet::registers(LuaState & ll)
{
    LuaStateValidator(ll, 0);

    // SWE.StreamNet
    ll.pushTable("SWE.StreamNet");
    ll.setFunctionsTableIndex(SWE_streamnet_functions2, -1);

    // set metatable: __call
    ll.pushTable(0, 1).pushFunction(SWE_streamnet_create).setFieldTableIndex("__call", -2);
    ll.setMetaTableIndex(-2).stackPop();
}

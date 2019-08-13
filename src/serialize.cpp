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

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm>

#include "engine.h"

#define MINCAPACITY 1024

void StreamBase::setconstbuf(bool f)
{
    BitFlags::set(0x00001000, f);
}

bool StreamBase::isconstbuf(void) const
{
    return BitFlags::check(0x00001000);
}

void StreamBase::setbigendian(bool f)
{
    BitFlags::set(0x80000000, f);
}

bool StreamBase::bigendian(void) const
{
    return BitFlags::check(0x80000000);
}

void StreamBase::setfail(bool f)
{
    BitFlags::set(0x00000001, f);
}

bool StreamBase::fail(void) const
{
    return BitFlags::check(0x00000001);
}

int StreamBase::get16(void)
{
    return bigendian() ? getBE16() : getLE16();
}

int StreamBase::get32(void)
{
    return bigendian() ? getBE32() : getLE32();
}

s64 StreamBase::get64(void)
{
    return bigendian() ? getBE64() : getLE64();
}


StreamBase & StreamBase::operator>> (bool & v)
{
    v = get8();
    return *this;
}

StreamBase & StreamBase::operator>> (char & v)
{
    v = get8();
    return *this;
}

StreamBase & StreamBase::operator>> (u8 & v)
{
    v = get8();
    return *this;
}

StreamBase & StreamBase::operator>> (s8 & v)
{
    v = get8();
    return *this;
}

StreamBase & StreamBase::operator>> (u16 & v)
{
    v = get16();
    return *this;
}

StreamBase & StreamBase::operator>> (s16 & v)
{
    v = get16();
    return *this;
}

StreamBase & StreamBase::operator>> (u32 & v)
{
    v = get32();
    return *this;
}

StreamBase & StreamBase::operator>> (s32 & v)
{
    v = get32();
    return *this;
}

StreamBase & StreamBase::operator>> (u64 & v)
{
    v = get64();
    return *this;
}

StreamBase & StreamBase::operator>> (s64 & v)
{
    v = get64();
    return *this;
}

StreamBase & StreamBase::operator>> (float & v)
{
    s32 intpart;
    s32 decpart;
    *this >> intpart >> decpart;

    v = intpart + (float) decpart / 100000000;
    return *this;
}

StreamBase & StreamBase::operator>> (std::string & v)
{
    u32 size = get32();
    v.resize(size);

    for(auto it = v.begin(); it != v.end(); ++it)
	*it = get8();

    return *this;
}

StreamBase & StreamBase::operator>> (Rect & v)
{
    Point & p = v;
    Size  & s = v;

    return *this >> p >> s;
}

StreamBase & StreamBase::operator>> (Point& v)
{
    return *this >> v.x >> v.y;
}

StreamBase & StreamBase::operator>> (Size & v)
{
    return *this >> v.w >> v.h;
}

StreamBase & StreamBase::operator>> (BinaryBuf & v)
{
    u32 size = get32();
    v.resize(size);

    for(auto it = v.begin(); it != v.end(); ++it)
	*it = get8();

    return *this;
}

void StreamBase::put16(u16 v)
{
    bigendian() ? putBE16(v) : putLE16(v);
}

void StreamBase::put32(u32 v)
{
    bigendian() ? putBE32(v) : putLE32(v);
}

void StreamBase::put64(u64 v)
{
    bigendian() ? putBE64(v) : putLE64(v);
}

StreamBase & StreamBase::operator<< (const bool & v)
{
    put8(v);
    return *this;
}

StreamBase & StreamBase::operator<< (const char & v)
{
    put8(v);
    return *this;
}

StreamBase & StreamBase::operator<< (const u8 & v)
{
    put8(v);
    return *this;
}

StreamBase & StreamBase::operator<< (const s8 & v)
{
    put8(v);
    return *this;
}

StreamBase & StreamBase::operator<< (const u16 & v)
{
    put16(v);
    return *this;
}

StreamBase & StreamBase::operator<< (const s16 & v)
{
    put16(v);
    return *this;
}

StreamBase & StreamBase::operator<< (const s32 & v)
{
    put32(v);
    return *this;
}

StreamBase & StreamBase::operator<< (const u32 & v)
{
    put32(v);
    return *this;
}

StreamBase & StreamBase::operator<< (const s64 & v)
{
    put64(v);
    return *this;
}

StreamBase & StreamBase::operator<< (const u64 & v)
{
    put64(v);
    return *this;
}

StreamBase & StreamBase::operator<< (const float & v)
{
    s32 intpart = static_cast<s32>(v);
    float decpart = (v - intpart) * 100000000;
    return *this << intpart << static_cast<s32>(decpart);
}

StreamBase & StreamBase::operator<< (const std::string & v)
{
    put32(v.size());

    for(auto it = v.begin(); it != v.end(); ++it)
	put8(*it);

    return *this;
}

StreamBase & StreamBase::operator<< (const Point & v)
{
    return *this << v.x << v.y;
}

StreamBase & StreamBase::operator<< (const Rect & v)
{
    const Point & p = v;
    const Size  & s = v;

    return *this << p << s;
}

StreamBase & StreamBase::operator<< (const Size & v)
{
    return *this << v.w << v.h;
}

StreamBase & StreamBase::operator<< (const BinaryBuf & v)
{
    put32(v.size());

    for(auto it = v.begin(); it != v.end(); ++it)
	put8(*it);

    return *this;
}

bool StreamBase::wait(const std::string & marker)
{
rep:
    for(auto it = marker.begin(); it != marker.end(); ++it)
    {
	if(fail()) return false;
	if(*it != get8()) goto rep;
    }

    return true;
}

StreamBuf::StreamBuf(size_t sz) : itbeg(NULL), itget(NULL), itput(NULL), itend(NULL)
{
    if(sz) realloc(sz);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    setbigendian(true); /* default: hardware endian */
#else
    setbigendian(false); /* default: hardware endian */
#endif
}

StreamBuf::~StreamBuf()
{
    if(itbeg && ! isconstbuf()) delete [] itbeg;
}

StreamBuf::StreamBuf(const StreamBuf & st) : itbeg(NULL), itget(NULL), itput(NULL), itend(NULL)
{
    if(st.isconstbuf())
    {
	itbeg = st.itbeg;
	itend = st.itend;
	itget = itbeg;
	itput = itend;
	setconstbuf(true);
	setbigendian(st.bigendian());
    }
    else
	copy(st);
}

StreamBuf::StreamBuf(const BinaryBuf & buf, int endian) : itbeg(NULL), itget(NULL), itput(NULL), itend(NULL)
{
    itbeg = (u8*) buf.data();
    itend = itbeg + buf.size();
    itget = itbeg;
    itput = itend;
    setconstbuf(true);

    if(endian == 0 || endian == 1)
	setbigendian(endian);
    else
    {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	setbigendian(true); /* default: hardware endian */
#else
        setbigendian(false); /* default: hardware endian */
#endif
    }
}

StreamBuf::StreamBuf(const u8* buf, size_t bufsz, int endian) : itbeg(NULL), itget(NULL), itput(NULL), itend(NULL)
{
    itbeg = const_cast<u8*>(buf);
    itend = itbeg + bufsz;
    itget = itbeg;
    itput = itend;
    setconstbuf(true);

    if(endian == 0 || endian == 1)
	setbigendian(endian);
    {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	setbigendian(true); /* default: hardware endian */
#else
	setbigendian(false); /* default: hardware endian */
#endif
    }
}

StreamBuf & StreamBuf::operator= (const StreamBuf & st)
{
    if(&st != this)
    {
	if(st.isconstbuf())
	{
	    itbeg = st.itbeg;
	    itend = st.itend;
	    itget = itbeg;
	    itput = itend;
	    setconstbuf(true);
	    setbigendian(st.bigendian());
	}
	else
	    copy(st);
    }
    return *this;
}

size_t StreamBuf::capacity(void) const
{
    return itend - itbeg;
}

const u8* StreamBuf::data(void) const
{
    return itget;
}

size_t StreamBuf::size(void) const
{
    return sizeg();
}

void StreamBuf::reset(void)
{
    itput = itbeg;
    itget = itbeg;
}

size_t StreamBuf::tellg(void) const
{
    return itget - itbeg;
}

size_t StreamBuf::tellp(void) const
{
    return itput - itbeg;
}

size_t StreamBuf::sizeg(void) const
{
    return itput - itget;
}

size_t StreamBuf::sizep(void) const
{
    return itend - itput;
}

void StreamBuf::realloc(size_t sz)
{
    if(isconstbuf())
    {
	FIXME("const buf reallocated");
	setconstbuf(false);
    }

    if(! itbeg)
    {
	if(sz < MINCAPACITY) sz = MINCAPACITY;

	itbeg = new u8 [sz];
	itend = itbeg + sz;
    	std::fill(itbeg, itend, 0);

	reset();
    }
    else
    if(sizep() < sz)
    {
	if(sz < MINCAPACITY) sz = MINCAPACITY;

	u8* ptr = new u8 [sz];

	std::fill(ptr, ptr + sz, 0);
	std::copy(itbeg, itput, ptr);

	itput = ptr + tellp();
	itget = ptr + tellg();

	delete [] itbeg;

	itbeg = ptr;
	itend = itbeg + sz;
    }
}

void StreamBuf::copy(const StreamBuf & sb)
{
    if(capacity() < sb.size())
	realloc(sb.size());

    std::copy(sb.itget, sb.itput, itbeg);

    itput = itbeg + sb.tellp();
    itget = itbeg + sb.tellg();

    BitFlags::reset();
    setbigendian(sb.bigendian());
}


void StreamBuf::put8(char v)
{
    if(0 == sizep())
	realloc(capacity() + capacity() / 2);

    if(sizep())
        *itput++ = v;
    else
	setfail(true);
}

int StreamBuf::get8(void)
{
    int res = 0;

    if(sizeg())
	res = 0x000000FF & *itget++;
    else
	setfail(true);

    return res;
}

int StreamBuf::getBE16(void)
{
    return (get8() << 8) | get8();
}

int StreamBuf::getLE16(void)
{
    return get8() | (get8() << 8);
}

int StreamBuf::getBE32(void)
{
    int hh = getBE16();
    int ll = getBE16();
    return (hh << 16) | ll;
}

int StreamBuf::getLE32(void)
{
    int ll = getLE16();
    int hh = getLE16();
    return (hh << 16) | ll;
}

s64 StreamBuf::getBE64(void)
{
    s64 hh = getBE32();
    s64 ll = getBE32();
    return (hh << 32) | ll;
}

s64 StreamBuf::getLE64(void)
{
    s64 ll = getBE32();
    s64 hh = getBE32();
    return (hh << 32) | ll;
}

void StreamBuf::putBE16(u16 v)
{
    put8(v >> 8);
    put8(v);
}

void StreamBuf::putLE16(u16 v)
{
    put8(v);
    put8(v >> 8);
}

void StreamBuf::putBE32(u32 v)
{
    putBE16(v >> 16);
    putBE16(v & 0xFFFF);
}

void StreamBuf::putLE32(u32 v)
{
    putLE16(v & 0xFFFF);
    putLE16(v >> 16);
}

void StreamBuf::putBE64(u64 v)
{
    putBE32(v >> 32);
    putBE32(v & 0xFFFFFFFF);
}

void StreamBuf::putLE64(u64 v)
{
    putLE32(v & 0xFFFFFFFF);
    putLE32(v >> 32);
}

BinaryBuf StreamBuf::get(size_t sz)
{
    const u8* first = itget;

    if(sz == 0 || sz > sizeg()) sz = sizeg(); 
    skip(sz);

    return BinaryBuf(first, sz);
}

void StreamBuf::put(const char* ptr, size_t sz)
{
    for(size_t it = 0; it < sz; ++it)
	*this << ptr[it];
}

void StreamBuf::skip(size_t sz)
{
    itget += sz <= sizeg() ? sz : sizeg();
}

void StreamBuf::seek(size_t sz)
{
    itget = itbeg + sz < itend ? itbeg + sz : itend;
}

StreamFile::StreamFile(const std::string & fn, const char* mode) : filemode(NULL), rw(NULL)
{
    open(fn, mode);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    setbigendian(true); /* default: hardware endian */
#else
    setbigendian(false); /* default: hardware endian */
#endif
}

StreamFile::StreamFile(const StreamFile & sf) : filemode(NULL), rw(NULL)
{
    if(sf.rw && open(sf.filename, sf.filemode))
	seek(sf.tell());
}

StreamFile::~StreamFile()
{
    close();
}

StreamFile & StreamFile::operator= (const StreamFile & sf)
{
    if(sf.rw && open(sf.filename, sf.filemode))
	seek(sf.tell());
    return *this;
}
    
bool StreamFile::open(const std::string & fn, const char* mode)
{
    close();

    if(!fn.empty() && mode)
    {
	filename = fn;
	filemode = mode;

	rw = SDL_RWFromFile(fn.c_str(), mode);

	if(! rw)
	{
	    filename.clear();
	    filemode = NULL;
	    ERROR(SDL_GetError());
	}
    }

    return rw;
}

void StreamFile::close(void)
{
    if(rw) SDL_RWclose(rw);
    rw = NULL;
}

size_t StreamFile::size(void) const
{
    if(rw)
    {
	size_t pos = SDL_RWtell(rw);
	SDL_RWseek(rw, 0, RW_SEEK_END);
	size_t len = SDL_RWseek(rw, 0, SEEK_END);
	SDL_RWseek(rw, pos, RW_SEEK_SET);
	return len;
    }
    return 0;
}

size_t StreamFile::tell(void) const
{
    return tellg();
}

void StreamFile::seek(size_t pos)
{
    if(rw) SDL_RWseek(rw, pos, RW_SEEK_SET);
}

size_t StreamFile::sizeg(void) const
{
    if(rw)
    {
	size_t pos = SDL_RWtell(rw);
	size_t len = SDL_RWseek(rw, 0, RW_SEEK_END);
	SDL_RWseek(rw, pos, RW_SEEK_SET);
	return len - pos;
    }
    return 0;
}

size_t StreamFile::tellg(void) const
{
    return rw ? SDL_RWtell(rw) : 0;
}

size_t StreamFile::sizep(void) const
{
    return sizeg();
}

size_t StreamFile::tellp(void) const
{
    return tellg();
}

void StreamFile::skip(size_t pos)
{
    if(rw) SDL_RWseek(rw, pos, RW_SEEK_CUR);
}

int StreamFile::get8(void)
{
    u8 ch = 0;
    if(rw) SDL_RWread(rw, & ch, 1, 1);
    else setfail(true);
    return ch;
}

void StreamFile::put8(char ch)
{
    if(rw) SDL_RWwrite(rw, & ch, 1, 1);
    else setfail(true);
}

int StreamFile::getBE16(void)
{
    return rw ? SDL_ReadBE16(rw) : 0;
}

int StreamFile::getLE16(void)
{
    return rw ? SDL_ReadLE16(rw) : 0;
}

int StreamFile::getBE32(void)
{
    return rw ? SDL_ReadBE32(rw) : 0;
}

int StreamFile::getLE32(void)
{
    return rw ? SDL_ReadLE32(rw) : 0;
}

s64 StreamFile::getBE64(void)
{
    return rw ? SDL_ReadBE64(rw) : 0;
}

s64 StreamFile::getLE64(void)
{
    return rw ? SDL_ReadLE64(rw) : 0;
}

void StreamFile::putBE64(u64 val)
{
    if(rw) SDL_WriteBE64(rw, val);
}

void StreamFile::putLE64(u64 val)
{
    if(rw) SDL_WriteLE64(rw, val);
}

void StreamFile::putBE32(u32 val)
{
    if(rw) SDL_WriteBE32(rw, val);
}

void StreamFile::putLE32(u32 val)
{
    if(rw) SDL_WriteLE32(rw, val);
}

void StreamFile::putBE16(u16 val)
{
    if(rw) SDL_WriteBE16(rw, val);
}

void StreamFile::putLE16(u16 val)
{
    if(rw) SDL_WriteLE16(rw, val);
}

BinaryBuf StreamFile::get(size_t sz)
{
    BinaryBuf buf(sz ? sz : sizeg());
    if(rw) SDL_RWread(rw, buf.data(), buf.size(), 1);
    return buf;
}

void StreamFile::put(const char* ptr, size_t sz)
{
    if(rw) SDL_RWwrite(rw, ptr, sz, 1);
}

StreamBuf StreamFile::toStreamBuf(size_t sz)
{
    StreamBuf sb;
    BinaryBuf buf = get(sz);
    sb.put(reinterpret_cast<const char*>(buf.data()), buf.size());
    return sb;
}

bool ZStreamBuf::read(const std::string & fn, size_t offset)
{
    StreamFile sf;
    sf.setbigendian(true);

    if(! sf.open(fn, "rb"))
    {
        ERROR("open: " << fn);
	return false;
    }

    if(offset) sf.seek(offset);

    const u32 size0 = sf.get32(); // raw size
    const u32 size1 = sf.get32(); // zip size
    BinaryBuf raw = sf.get(size1).zlibUncompress(size0);

    put(reinterpret_cast<const char*>(raw.data()), raw.size());
    seek(0);

    return ! fail();
}

bool ZStreamBuf::write(const std::string & fn, bool append) const
{
    StreamFile sf;
    sf.setbigendian(true);

    if(! sf.open(fn, append ? "ab" : "wb"))
    {
        ERROR("open: " << fn);
	return false;
    }

    BinaryBuf zip = Tools::zlibCompress(data(), size());
    if(zip.empty()) return false;

    sf.put32(size());
    sf.put32(zip.size());
    sf.put(reinterpret_cast<const char*>(zip.data()), zip.size());

    return ! sf.fail();
}

#ifndef DISABLE_NETWORK
StreamNetwork::StreamNetwork() : sd(NULL), sdset(NULL)
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    setbigendian(true); /* default: hardware endian */
#else
    setbigendian(false); /* default: hardware endian */
#endif
}

size_t StreamNetwork::timeout = 100;

StreamNetwork::StreamNetwork(const std::string & name, int port) : sd(NULL), sdset(NULL)
{
    if(!open(name, port))
        Engine::except(__FUNCTION__, "create error");

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    setbigendian(true); /* default: hardware endian */
#else
    setbigendian(false); /* default: hardware endian */
#endif
}

StreamNetwork::~StreamNetwork()
{
    close();
}

bool StreamNetwork::ready(void) const
{
    const size_t chunk = 10;

    for(size_t it = 0; it < timeout; it += chunk)
    {
	bool res = sd && sdset && 0 < SDLNet_CheckSockets(sdset, 1) && 0 < SDLNet_SocketReady(sd);
	if(res) return true;

	Tools::delay(chunk);
    }

    return false;
}

bool StreamNetwork::open(const std::string & name, int port)
{
    IPaddress ip;

    if(0 > SDLNet_ResolveHost(&ip, name.size() ? name.c_str() : NULL, port))
    {
        ERROR(SDLNet_GetError());
        return false;
    }

    close();

    sd = SDLNet_TCP_Open(&ip);
    if(sd)
    {
	sdset = SDLNet_AllocSocketSet(1);
	if(sdset)
	{
	    SDLNet_TCP_AddSocket(sdset, sd);
	    return true;
	}
    }

    ERROR(SDLNet_GetError());
    return false;
}

void StreamNetwork::close(void)
{
    if(sdset)
    {
	if(sd) SDLNet_TCP_DelSocket(sdset, sd);
    	SDLNet_FreeSocketSet(sdset);
        sdset = NULL;
    }

    if(sd)
    {
    	SDLNet_TCP_Close(sd);
    	sd = NULL;
    }
}

int StreamNetwork::recv(char *buf, int len)
{
    int total = 0;

    if(sd && buf && 0 < len)
    {
	int rcv;
	int bufsz = len;

        while((rcv = SDLNet_TCP_Recv(sd, buf, bufsz)) > 0 && rcv <= bufsz)
        {
            buf   += rcv;
            bufsz -= rcv;
	    total += rcv;
        }

        if(total != len) setfail(true);
    }

    return total;
}

int StreamNetwork::send(const char* buf, int len)
{
    int snd = 0;
    if(sd && buf && 0 < len)
    {
	snd = SDLNet_TCP_Send(sd, buf, len);
	if(snd != len) setfail(true);
    }

    return snd;
}

int StreamNetwork::get8(void)
{
    if(sd)
    {
	u8 ch = 0;

	if(1 != SDLNet_TCP_Recv(sd, & ch, 1))
	    setfail(true);

	return ch;
    }

    return 0;
}

void StreamNetwork::put8(char ch)
{
    if(sd)
    {
	if(1 != SDLNet_TCP_Send(sd, & ch, 1))
	    setfail(true);
    }
}

void StreamNetwork::skip(size_t sz)
{
    for(size_t it = 0; it < sz; ++it) get8();
}

int StreamNetwork::getBE16(void)
{
    return (get8() << 8) | get8();
}

int StreamNetwork::getLE16(void)
{
    return get8() | (get8() << 8);
}

int StreamNetwork::getBE32(void)
{
    int hh = getBE16();
    int ll = getBE16();
    return (hh << 16) | ll;
}

int StreamNetwork::getLE32(void)
{
    int ll = getLE16();
    int hh = getLE16();
    return (hh << 16) | ll;
}

s64 StreamNetwork::getBE64(void)
{
    s64 hh = getBE32();
    s64 ll = getBE32();
    return (hh << 32) | ll;
}

s64 StreamNetwork::getLE64(void)
{
    s64 ll = getBE32();
    s64 hh = getBE32();
    return (hh << 32) | ll;
}

void StreamNetwork::putBE16(u16 v)
{
    put8(v >> 8);
    put8(v);
}

void StreamNetwork::putLE16(u16 v)
{
    put8(v);
    put8(v >> 8);
}

void StreamNetwork::putBE32(u32 v)
{
    putBE16(v >> 16);
    putBE16(v & 0xFFFF);
}

void StreamNetwork::putLE32(u32 v)
{
    putLE16(v & 0xFFFF);
    putLE16(v >> 16);
}

void StreamNetwork::putBE64(u64 v)
{
    putBE32(v >> 32);
    putBE32(v & 0xFFFFFFFF);
}

void StreamNetwork::putLE64(u64 v)
{
    putLE32(v & 0xFFFFFFFF);
    putLE32(v >> 32);
}

BinaryBuf StreamNetwork::get(size_t sz)
{
    BinaryBuf res;

    if(sz)
    {
	res.resize(sz);
	int rcv = recv(reinterpret_cast<char*>(res.data()), res.size());
	res.resize(rcv);
    }
    else
    {
	while(ready())
	{
	    const size_t packet = 1024;
	    size_t bufsz = res.size();
	    res.resize(bufsz + packet);

	    int rcv = recv(reinterpret_cast<char*>(res.data() + bufsz), packet);
	    if(rcv < packet)
	    {
		res.resize(0 < rcv ? bufsz + rcv : bufsz);
	    }

	    if(fail()) break;
	}
    }

    return res;
}

void StreamNetwork::put(const char* ptr, size_t sz)
{
    send(ptr, sz);
}
#endif

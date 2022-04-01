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

#include "swe_tools.h"

namespace SWE
{
    StreamBase::StreamBase()
    {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        setbigendian(true); /* default: hardware endian */
#else
        setbigendian(false); /* default: hardware endian */
#endif
    }

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

    void StreamBase::setfail(bool f) const
    {
        const BitFlags & flags = *this;
        const_cast<BitFlags &>(flags).set(0x00000001, f);
    }

    bool StreamBase::fail(void) const
    {
        return BitFlags::check(0x00000001);
    }

    int StreamBase::get16(void) const
    {
        return bigendian() ? getBE16() : getLE16();
    }

    int StreamBase::get32(void) const
    {
        return bigendian() ? getBE32() : getLE32();
    }

    s64 StreamBase::get64(void) const
    {
        return bigendian() ? getBE64() : getLE64();
    }


    const StreamBase & StreamBase::operator>> (bool & v) const
    {
        v = get8();
        return *this;
    }

    const StreamBase & StreamBase::operator>> (char & v) const
    {
        get(& v, sizeof(v));
        return *this;
    }

    const StreamBase & StreamBase::operator>> (u8 & v) const
    {
        v = get8();
        return *this;
    }

    const StreamBase & StreamBase::operator>> (s8 & v) const
    {
        v = get8();
        return *this;
    }

    const StreamBase & StreamBase::operator>> (u16 & v) const
    {
        v = get16();
        return *this;
    }

    const StreamBase & StreamBase::operator>> (s16 & v) const
    {
        v = get16();
        return *this;
    }

    const StreamBase & StreamBase::operator>> (u32 & v) const
    {
        v = get32();
        return *this;
    }

    const StreamBase & StreamBase::operator>> (s32 & v) const
    {
        v = get32();
        return *this;
    }

    const StreamBase & StreamBase::operator>> (u64 & v) const
    {
        v = get64();
        return *this;
    }

    const StreamBase & StreamBase::operator>> (s64 & v) const
    {
        v = get64();
        return *this;
    }

    const StreamBase & StreamBase::operator>> (float & v) const
    {
        s32 intpart;
        s32 decpart;
        *this >> intpart >> decpart;
        v = intpart + (float) decpart / 100000000;
        return *this;
    }

    const StreamBase & StreamBase::operator>> (std::string & v) const
    {
        u32 size = get32();
        v.resize(size);

        for(auto it = v.begin(); it != v.end(); ++it)
            *it = get8();

        return *this;
    }

    const StreamBase & StreamBase::operator>> (Rect & v) const
    {
        Point & p = v;
        Size  & s = v;
        return *this >> p >> s;
    }

    const StreamBase & StreamBase::operator>> (Point & v) const
    {
        return *this >> v.x >> v.y;
    }

    const StreamBase & StreamBase::operator>> (Size & v) const
    {
        return *this >> v.w >> v.h;
    }

    const StreamBase & StreamBase::operator>> (BinaryBuf & v) const
    {
        u32 size = get32();
        v = get(size);
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
        put(& v, sizeof(char));
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
        put(reinterpret_cast<const char*>(v.data()), v.size());
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

    /* StreamRWops */
    StreamRWops::~StreamRWops()
    {
        close();
    }
            
    StreamRWops::StreamRWops(SDL_RWops* val) : rw(val)
    {
    }

    StreamRWops::StreamRWops(StreamRWops && srw) noexcept
    {
        rw = srw.rw;
        srw.rw = nullptr;
    }

    StreamRWops & StreamRWops::operator=(StreamRWops && srw) noexcept
    {
        rw = srw.rw;
        srw.rw = nullptr;
        return *this;
    }

    void StreamRWops::close(void)
    {
        if(rw)
        {
            SDL_RWclose(rw);
            rw = nullptr;
        }
    }

    bool StreamRWops::seek(int offset, int whence /* RW_SEEK_SET */) const
    {
        return rw ? 0 <= SDL_RWseek(rw, offset, whence) : false;
    }

    bool StreamRWops::skip(size_t len) const
    {
        return rw ? 0 <= SDL_RWseek(rw, len, RW_SEEK_CUR) : false;
    }

    size_t StreamRWops::size(void) const
    {
        if(rw)
        {
            auto pos = SDL_RWtell(rw);
	    if(0 <= pos)
	    {
		auto res = 0 <= SDL_RWseek(rw, 0, SEEK_END) ? SDL_RWtell(rw) : 0;
        	SDL_RWseek(rw, pos, RW_SEEK_SET);
        	return res;
	    }
        }

        return 0;
    }

    size_t StreamRWops::last(void) const
    {
        if(rw)
        {
            auto pos = SDL_RWtell(rw);
	    if(0 <= pos)
            {
		auto len = SDL_RWseek(rw, 0, RW_SEEK_END);
		SDL_RWseek(rw, pos, RW_SEEK_SET);
        	return len - pos;
	    }
        }

        return 0;
    }

    size_t StreamRWops::tell(void) const
    {
	auto pos = SDL_RWtell(rw);
        return rw && 0 < pos ? pos : 0;
    }

    int StreamRWops::get8(void) const
    {
        u8 res = 0;

        if(rw && 1 == SDL_RWread(rw, & res, 1, 1))
    	    return res;

    	return -1;
    }

    int StreamRWops::getBE16(void) const
    {
        return rw ? SDL_ReadBE16(rw) : 0;
    }

    int StreamRWops::getLE16(void) const
    {
        return rw ? SDL_ReadLE16(rw) : 0;
    }

    int StreamRWops::getBE32(void) const
    {
        return rw ? SDL_ReadBE32(rw) : 0;
    }

    int StreamRWops::getLE32(void) const
    {
        return rw ? SDL_ReadLE32(rw) : 0;
    }

    s64 StreamRWops::getBE64(void) const
    {
        return rw ? SDL_ReadBE64(rw) : 0;
    }

    s64 StreamRWops::getLE64(void) const
    {
        return rw ? SDL_ReadLE64(rw) : 0;
    }

    bool StreamRWops::get(void* buf, size_t sz) const
    {
        if(rw && buf && sz)
        {
            size_t last2 = last();
            size_t res = SDL_RWread(rw, buf, 1, sz);
            if(res == sz)
                return true;
            if(res != last2)
                ERROR(SDL_GetError());
        }

        return false;
    }

    BinaryBuf StreamRWops::get(size_t sz) const
    {
        BinaryBuf buf(sz ? sz : last());
        get(buf.data(), buf.size());

        return buf;
    }

    bool StreamRWops::put8(u8 val)
    {
        return rw && 1 == SDL_RWwrite(rw, & val, 1, 1);
    }

    bool StreamRWops::putBE16(u16 val)
    {
        return rw ? SDL_WriteBE16(rw, val) : false;
    }

    bool StreamRWops::putLE16(u16 val)
    {
        return rw ? SDL_WriteLE16(rw, val) : false;
    }

    bool StreamRWops::putBE32(u32 val)
    {
        return rw ? SDL_WriteBE32(rw, val) : false;
    }

    bool StreamRWops::putLE32(u32 val)
    {
        return rw ? SDL_WriteLE32(rw, val) : false;
    }

    bool StreamRWops::putBE64(u64 val)
    {
        return rw ? SDL_WriteBE64(rw, val) : false;
    }

    bool StreamRWops::putLE64(u64 val)
    {
        return rw ? SDL_WriteLE64(rw, val) : false;
    }

    bool StreamRWops::put(const void* buf, size_t sz)
    {
        if(rw && buf && sz)
        {
            size_t tell2 = tell();
            size_t res = SDL_RWwrite(rw, buf, 1, sz);
            if(res == sz)
                return true;
            if(res != tell2)
                ERROR(SDL_GetError());
        }

        return false;
    }
}

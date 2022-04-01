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
#include "swe_streamfile.h"
#include "swe_streambuf.h"

namespace SWE
{
    /* StreamBufRO */
    StreamBufRO::StreamBufRO(const BinaryBuf & bb) : sr(SDL_RWFromConstMem(bb.data(), bb.size()))
    {
        if(! sr.isValid())
        {
            ERROR(SDL_GetError());
            setfail(true);
        }
    }

    StreamBufRO::StreamBufRO(const u8* data, size_t size) : sr(SDL_RWFromConstMem(data, size))
    {
        if(! sr.isValid())
        {
            ERROR(SDL_GetError());
            setfail(true);
        }
    }

    void StreamBufRO::setBuf(const u8* data, size_t size)
    {
        sr.close();

        if(data && size)
        {
            sr = StreamRWops(SDL_RWFromConstMem(data, size));

            if(! sr.isValid())
            {
                ERROR(SDL_GetError());
                setfail(true);
            }
        }
    }

    size_t StreamBufRO::lastg(void) const
    {
        return sr.last();
    }

    size_t StreamBufRO::tellg(void) const
    {
        return sr.tell();
    }

    bool StreamBufRO::seekg(size_t pos, int whence) const
    {
        return sr.seek(pos, whence);
    }

    bool StreamBufRO::skipg(size_t len) const
    {
        return sr.skip(len);
    }

    void StreamBufRO::setBuf(const BinaryBuf & bb)
    {
        setBuf(bb.data(), bb.size());
    }

    int StreamBufRO::getBE16(void) const
    {
        if(2 > sr.last())
        {
            setfail(true);
            return 0;
        }

        return sr.getBE16();
    }

    int StreamBufRO::getLE16(void) const
    {
        if(2 > sr.last())
        {
            setfail(true);
            return 0;
        }

        return sr.getLE16();
    }

    int StreamBufRO::getBE32(void) const
    {
        if(4 > sr.last())
        {
            setfail(true);
            return 0;
        }

        return sr.getBE32();
    }

    int StreamBufRO::getLE32(void) const
    {
        if(4 > sr.last())
        {
            setfail(true);
            return 0;
        }

        return sr.getLE32();
    }

    s64 StreamBufRO::getBE64(void) const
    {
        if(8 > sr.last())
        {
            setfail(true);
            return 0;
        }

        return sr.getBE64();
    }

    s64 StreamBufRO::getLE64(void) const
    {
        if(8 > sr.last())
        {
            setfail(true);
            return 0;
        }

        return sr.getLE64();
    }

    int StreamBufRO::get8(void) const
    {
        if(1 > sr.last())
        {
            setfail(true);
            return 0;
        }

        return sr.get8();
    }

    bool StreamBufRO::get(void* buf, size_t size) const
    {
        if(sr.get(buf, size))
            return true;

        setfail(true);
        return false;
    }

    BinaryBuf StreamBufRO::get(size_t size) const
    {
        BinaryBuf buf(size ? size : sr.last());
        get(buf.data(), buf.size());

        return buf;
    }

    /* StreamBufRW */
#define MINCAPACITY 256

    StreamBufRW::StreamBufRW()
    {
        buf.reserve(MINCAPACITY);
        StreamBufRW::setBuf(buf);
    }

    StreamBufRW::StreamBufRW(const StreamBufRW & rw)
    {
        buf.reserve(MINCAPACITY);
        StreamBufRW::setBuf(rw.buf);
    }

    StreamBufRW::StreamBufRW(const BinaryBuf & bb)
    {
        buf.reserve(MINCAPACITY);
        StreamBufRW::setBuf(bb);
    }

    StreamBufRW::StreamBufRW(const u8* data, size_t size)
    {
        buf.reserve(MINCAPACITY);
        StreamBufRW::setBuf(data, size);
    }

    StreamBufRW::StreamBufRW(BinaryBuf && bb) noexcept : buf(bb), sw(SDL_RWFromMem(buf.data(), buf.size()))
    {
        buf.reserve(MINCAPACITY);
        StreamBufRO::setBuf(buf);

        if(! sw.isValid())
        {
            ERROR(SDL_GetError());
            setfail(true);
        }
    }

    StreamBufRW & StreamBufRW::operator=(const StreamBufRW & rw)
    {
        StreamBufRW::setBuf(rw.buf);
        seekp(rw.tellp());
        seekg(rw.tellg());
        return *this;
    }

    void StreamBufRW::setBuf(const u8* data, size_t size)
    {
        sr.close();
        sw.close();

        if(data && size)
        {
            buf.assign(data, data + size);
            sr = StreamRWops(SDL_RWFromConstMem(buf.data(), buf.size()));
            sw = StreamRWops(SDL_RWFromMem(buf.data(), buf.size()));

            if(! sr.isValid() || ! sw.isValid())
            {
                ERROR(SDL_GetError());
                setfail(true);
            }
        }
    }

    void StreamBufRW::setBuf(const BinaryBuf & bb)
    {
        StreamBufRW::setBuf(bb.data(), bb.size());
    }

    bool StreamBufRW::resize(size_t val)
    {
        size_t tp = tellp();
        size_t tg = tellg();
        sr.close();
        sw.close();
        buf.resize(val, 0);
        sr = StreamRWops(SDL_RWFromConstMem(buf.data(), buf.size()));
        sw = StreamRWops(SDL_RWFromMem(buf.data(), buf.size()));

        if(sr.isValid() && sw.isValid())
        {
            seekp(tp);
            seekg(tg);
        }
        else
        {
            ERROR(SDL_GetError());
            setfail(true);
            return false;
        }

        return true;
    }

    size_t StreamBufRW::lastp(void) const
    {
        return sw.last();
    }

    size_t StreamBufRW::tellp(void) const
    {
        return sw.tell();
    }

    bool StreamBufRW::seekp(size_t pos, int whence) const
    {
        return sw.seek(pos, whence);
    }

    bool StreamBufRW::skipp(size_t len) const
    {
        return sw.skip(len);
    }

    const u8* StreamBufRW::data(void) const
    {
        return buf.data() + tellg();
    }

    size_t StreamBufRW::size(void) const
    {
        return lastg();
    }

    void StreamBufRW::putBE16(u16 val)
    {
        if(sw.last() > 1 || resize(buf.size() + 2))
        {
            if(! sw.putBE16(val))
                setfail(true);
        }
    }

    void StreamBufRW::putLE16(u16 val)
    {
        if(sw.last() > 1 || resize(buf.size() + 2))
        {
            if(! sw.putLE16(val))
                setfail(true);
        }
    }

    void StreamBufRW::putBE32(u32 val)
    {
        if(sw.last() > 3 || resize(buf.size() + 4))
        {
            if(! sw.putBE32(val))
                setfail(true);
        }
    }

    void StreamBufRW::putLE32(u32 val)
    {
        if(sw.last() > 3 || resize(buf.size() + 4))
        {
            if(! sw.putLE32(val))
                setfail(true);
        }
    }

    void StreamBufRW::putBE64(u64 val)
    {
        if(sw.last() > 7 || resize(buf.size() + 8))
        {
            if(! sw.putBE64(val))
                setfail(true);
        }
    }

    void StreamBufRW::putLE64(u64 val)
    {
        if(sw.last() > 7 || resize(buf.size() + 8))
        {
            if(! sw.putLE64(val))
                setfail(true);
        }
    }

    void StreamBufRW::put8(u8 val)
    {
        if(sw.last() > 0 || resize(buf.size() + 1))
        {
            if(! sw.put8(val))
                setfail(true);
        }
    }

    bool StreamBufRW::put(const void* data, size_t size)
    {
        if(0 < size)
        {
            if(sw.last() > size - 1 || resize(buf.size() + size))
            {
                if(sw.put(data, size))
                    return true;
                setfail(true);
            }
        }

        return false;
    }

    bool ZStreamBuf::read(const std::string & fn, size_t offset)
    {
        StreamFile sf(fn, "rb");

        if(! sf.isValid())
            return false;

        if(offset) sf.seek(offset);

        const u32 size0 = sf.getBE32(); // raw size
        const u32 size1 = sf.getBE32(); // zip size
	sf.skip(4);			// reserved
        setBuf(sf.get(size1).zlibUncompress(size0));
        return ! fail();
    }

    bool ZStreamBuf::write(const std::string & fn, bool append) const
    {
        StreamFile sf(fn, append ? "ab" : "wb");

        if(! sf.isValid())
            return false;

        BinaryBuf zip = buf.zlibCompress();

        if(zip.empty()) return false;

        sf.putBE32(size());
        sf.putBE32(zip.size());
	sf.putBE32(0);
        sf.put(zip.data(), zip.size());
        return ! sf.fail();
    }
}

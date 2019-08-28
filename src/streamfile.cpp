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

#include "engine.h"
#include "streamfile.h"

StreamFile::StreamFile(const std::string & fn, const char* mode) : filemode(NULL)
{
    open(fn, mode);
}

StreamFile::StreamFile(const StreamFile & sf) : filemode(NULL)
{
    if(sf.isValid() && open(sf.filename, sf.filemode))
	seek(sf.tell());
}

StreamFile::~StreamFile()
{
    close();
}

StreamFile & StreamFile::operator= (const StreamFile & sf)
{
    if(sf.isValid() && open(sf.filename, sf.filemode))
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

	rw = StreamRWops(SDL_RWFromFile(fn.c_str(), mode));

	if(! rw.isValid())
	{
	    filename.clear();
	    filemode = NULL;

	    ERROR(fn << ", error: " << SDL_GetError());
	    return false;
	}
    }

    return true;
}

void StreamFile::close(void)
{
    rw.close();
}

StreamBuf StreamFile::toStreamBuf(size_t sz)
{
    StreamBuf sb;
    BinaryBuf buf = get(sz);
    sb.put(reinterpret_cast<const char*>(buf.data()), buf.size());
    return sb;
}

int StreamFile::getBE16(void) const
{
    if(2 > rw.last())
    {
        setfail(true);
        return 0;
    }
    return rw.getBE16();
}

int StreamFile::getLE16(void) const
{
    if(2 > rw.last())
    {
        setfail(true);
        return 0;
    }
    return rw.getLE16();
}

int StreamFile::getBE32(void) const
{
    if(4 > rw.last())
    {
        setfail(true);
        return 0;
    }
    return rw.getBE32();
}

int StreamFile::getLE32(void) const
{
    if(4 > rw.last())
    {
        setfail(true);
        return 0;
    }
    return rw.getLE32();
}

s64 StreamFile::getBE64(void) const
{
    if(8 > rw.last())
    {
        setfail(true);
        return 0;
    }
    return rw.getBE64();
}

s64 StreamFile::getLE64(void) const
{
    if(8 > rw.last())
    {
        setfail(true);
        return 0;
    }
    return rw.getLE64();
}

int StreamFile::get8(void) const
{
    if(1 > rw.last())
    {
        setfail(true);
        return 0;
    }
    return rw.get8();
}

BinaryBuf StreamFile::get(size_t size) const
{
    if(0 < size && rw.last() < size){ size = rw.last(); setfail(true); }
    return rw.get(size ? size : rw.last());
}

void StreamFile::putBE16(u16 val)
{
    if(! rw.putBE16(val))
        setfail(true);
}

void StreamFile::putLE16(u16 val)
{
    if(! rw.putLE16(val))
        setfail(true);
}

void StreamFile::putBE32(u32 val)
{
    if(! rw.putBE32(val))
        setfail(true);
}

void StreamFile::putLE32(u32 val)
{
    if(! rw.putLE32(val))
        setfail(true);
}

void StreamFile::putBE64(u64 val)
{
    if(! rw.putBE64(val))
        setfail(true);
}

void StreamFile::putLE64(u64 val)
{
    if(! rw.putLE64(val))
        setfail(true);
}

void StreamFile::put8(char val)
{
    if(! rw.put8(val))
        setfail(true);
}

void StreamFile::put(const char* data, size_t size)
{
    if(0 < size)
    {
        if(! rw.put(data, size))
                setfail(true);
    }
}

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

#include <iomanip>
#include <sstream>
#include <algorithm>

#include "swe_cstring.h"
#include "swe_tools.h"

namespace SWE
{
    BinaryBuf::BinaryBuf()
    {
    }

    BinaryBuf::BinaryBuf(size_t len, int ch) : std::vector<u8>(len, ch)
    {
    }

    BinaryBuf::BinaryBuf(const u8* ptr, size_t len) : std::vector<u8>(ptr, ptr + len)
    {
    }

    BinaryBuf::BinaryBuf(const std::vector<u8> & v) : std::vector<u8>(v)
    {
    }

    BinaryBuf::BinaryBuf(const BinaryBuf & v) : std::vector<u8>(v)
    {
    }

    BinaryBuf::BinaryBuf(BinaryBuf && v) noexcept
    {
        swap(v);
    }

    BinaryBuf::BinaryBuf(std::vector<u8> && v) noexcept
    {
        swap(v);
    }

    BinaryBuf & BinaryBuf::operator= (const BinaryBuf & v)
    {
        assign(v.begin(), v.end());
        return *this;
    }

    BinaryBuf & BinaryBuf::operator= (BinaryBuf && v) noexcept
    {
        swap(v);
        return *this;
    }

    BinaryBuf & BinaryBuf::operator= (std::vector<u8> && v) noexcept
    {
        swap(v);
        return *this;
    }

    BinaryBuf BinaryBuf::zlibUncompress(size_t real) const
    {
        return Tools::zlibUncompress(data(), size(), real);
    }

    BinaryBuf BinaryBuf::zlibCompress(void) const
    {
        return Tools::zlibCompress(data(), size());
    }

    BinaryBuf BinaryBuf::base64Decode(void) const
    {
        return Tools::base64Decode(data(), size());
    }

    BinaryBuf BinaryBuf::base64Encode(void) const
    {
        return Tools::base64Encode(data(), size());
    }

    int BinaryBuf::crc16b(void) const
    {
        return Tools::crc16b(data(), size());
    }

    u32 BinaryBuf::crc32b(void) const
    {
        return Tools::crc32b(data(), size());
    }

    std::string BinaryBuf::toString(void) const
    {
        return std::string(begin(), std::find(begin(), end(), 0));
    }

    std::string BinaryBuf::toHexString(const std::string & sep, bool prefix) const
    {
        return Tools::buffer2HexString<u8>(data(), size(), 2, sep, prefix);
    }

    BinaryBuf & BinaryBuf::append(const BinaryBuf & v)
    {
        insert(end(), v.begin(), v.end());
        return *this;
    }
}

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

#ifndef _SWE_BINARYBUF_
#define _SWE_BINARYBUF_

#include <string>
#include <vector>

#include "swe_types.h"

/// @brief пространство SWE
namespace SWE
{
    /// @brief класс бинарного массива
    class BinaryBuf : public std::vector<u8>
    {
    protected:
        BinaryBuf(const_iterator it1, const_iterator it2) : std::vector<u8>(it1, it2) {}

    public:
        BinaryBuf();
        BinaryBuf(size_t len, int ch = 0);
        BinaryBuf(const u8* ptr, size_t len);
        BinaryBuf(const std::vector<u8> &);
        BinaryBuf(const BinaryBuf &);
        BinaryBuf(BinaryBuf &&) noexcept;
        BinaryBuf(std::vector<u8> &&) noexcept;

        BinaryBuf &	operator= (const BinaryBuf &);
        BinaryBuf &	operator= (BinaryBuf &&) noexcept;
        BinaryBuf &     operator= (std::vector<u8> &&) noexcept;

        BinaryBuf	zlibUncompress(size_t real = 0) const;
        BinaryBuf	zlibCompress(void) const;

        BinaryBuf	base64Decode(void) const;
        BinaryBuf	base64Encode(void) const;

        u32		crc32b(void) const;
        int		crc16b(void) const;
        std::string	toString(void) const;
        std::string	toHexString(const std::string & sep = ", ", bool prefix = true) const;

        BinaryBuf &	append(const BinaryBuf &);
    };

} // SWE
#endif

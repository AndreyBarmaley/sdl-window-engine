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

#ifndef _SWE_STREAMBUF_
#define _SWE_STREAMBUF_

#include "swe_serialize.h"

/// @brief пространство SWE
namespace SWE
{

    class StreamBufRO : public StreamBase
    {
        void		putBE64(u64) override {}
        void		putLE64(u64) override {}
        void		putBE32(u32) override {}
        void		putLE32(u32) override {}
        void		putBE16(u16) override {}
        void 		putLE16(u16) override {}

        bool		put(const void*, size_t) override { return false; }
        void		put8(u8) override {}

        StreamBufRO(const StreamBufRO &) {}
        StreamBufRO & operator=(const StreamBufRO &) { return *this; }

    protected:
        StreamRWops	sr;

    public:
        StreamBufRO() {}
        StreamBufRO(const BinaryBuf &);
        StreamBufRO(const u8*, size_t);

        virtual void	setBuf(const u8*, size_t);
        virtual void	setBuf(const BinaryBuf &);

        size_t          lastg(void) const;
        size_t          tellg(void) const;
        bool            seekg(size_t pos, int whence = RW_SEEK_SET) const;
        bool            skipg(size_t len) const;

        size_t          tell(void) const override { return tellg(); }
        bool            skip(size_t len) const override { return skipg(len); }
	bool            seek(int offset, int whence = RW_SEEK_SET) const override { return seekg(offset, whence); }

        int		getBE16(void) const override;
        int         	getLE16(void) const override;
        int         	getBE32(void) const override;
        int         	getLE32(void) const override;
        s64         	getBE64(void) const override;
        s64         	getLE64(void) const override;

        int         	get8(void) const override;
        bool            get(void*, size_t) const override;
        BinaryBuf   	get(size_t = 0 /* all data */) const override;
    };

    class StreamBufRW : public StreamBufRO
    {
        size_t          tell(void) const override { return 0; }
        bool            skip(size_t len) const override { return false; }
	bool		seek(int offset, int whence = RW_SEEK_SET) const override { return false; }

    protected:
        BinaryBuf	buf;
        StreamRWops	sw;

        bool		resize(size_t);

    public:
        StreamBufRW();
        StreamBufRW(const StreamBufRW &);
        StreamBufRW(const BinaryBuf &);
        StreamBufRW(const u8*, size_t);
        StreamBufRW(BinaryBuf &&) noexcept;

        StreamBufRW &	operator=(const StreamBufRW &);

        void		setBuf(const u8*, size_t) override;
        void		setBuf(const BinaryBuf &) override;

        size_t          lastp(void) const;
        size_t          tellp(void) const;
        bool            seekp(size_t pos, int whence = RW_SEEK_SET) const;
        bool            skipp(size_t len) const;

        const u8*       data(void) const;
        size_t          size(void) const;

        void		putBE64(u64) override;
        void		putLE64(u64) override;
        void		putBE32(u32) override;
        void		putLE32(u32) override;
        void		putBE16(u16) override;
        void 		putLE16(u16) override;

        bool	        put(const void*, size_t) override;
        void		put8(u8) override;
    };

    class StreamBuf : public StreamBufRW
    {
    public:
        StreamBuf() {}
        StreamBuf(const BinaryBuf & bb) : StreamBufRW(bb) {}
        StreamBuf(const u8* data, size_t size) : StreamBufRW(data, size) {}
    };

    class ZStreamBuf : public StreamBuf
    {
    public:
        bool                read(const std::string &, size_t offset = 0);
        bool                write(const std::string &, bool append = false) const;
    };

} // SWE
#endif

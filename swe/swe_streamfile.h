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

#ifndef _SWE_STREAMFILE_
#define _SWE_STREAMFILE_

#include "swe_streambuf.h"

/// @brief пространство SWE
namespace SWE
{
/*
    mode:
    r  Open a file for reading. The file must exist.
    w  Create an empty file for writing.
       If a file with the same name already exists its content is erased and the file is treated as a new empty file.
    a  Append to a file.
       Writing operations append data at the end of the file.
       The file is created if it does not exist.
    r+ Open a file for update both reading and writing.
       The file must exist.
    w+ Create an empty file for both reading and writing.
       If a file with the same name already exists its content is erased and the file is treated as a new empty file.
    a+ Open a file for reading and appending.
       All writing operations are performed at the end of the file, protecting the previous content to be overwritten.
       You can reposition (fseek, rewind) the internal pointer to anywhere in the file for reading, but writing operations will move it back to the end of file.
       The file is created if it does not exist.

    In order to open a file as a binary file, a "b" character has to be included in the mode string.
    This additional "b" character can either be appended at the end of the string
    (thus making the following compound modes: "rb", "wb", "ab", "r+b", "w+b", "a+b") or be inserted between the letter and
    the "+" sign for the mixed modes ("rb+", "wb+", "ab+").
*/
    class StreamFile : public StreamBase
    {
        std::string	filename;
        const char*	filemode;
        StreamRWops	rw;

    public:
        StreamFile() : filemode(nullptr) {}
        StreamFile(const std::string &, const char* mode);
        StreamFile(const StreamFile &);
        ~StreamFile();

        StreamFile &	operator= (const StreamFile &);

        size_t		size(void) const;
        bool		open(const std::string &, const char* mode);
        void		close(void);
        bool		isValid(void) const;

        StreamBuf	toStreamBuf(size_t = 0 /* all data */);

        bool		seek(int offset, int whence = RW_SEEK_SET) const override;
        size_t		tell(void) const override;
        bool		skip(size_t len) const override;

        int		get8(void) const override;
        int		getBE16(void) const override;
        int		getLE16(void) const override;
        int		getBE32(void) const override;
        int		getLE32(void) const override;
        s64		getBE64(void) const override;
        s64		getLE64(void) const override;

        BinaryBuf	get(size_t = 0 /* all data */) const override;
        bool            get(void*, size_t) const override;

        void		put8(u8) override;
        void		putBE64(u64) override;
        void		putLE64(u64) override;
        void		putBE32(u32) override;
        void		putLE32(u32) override;
        void		putBE16(u16) override;
        void		putLE16(u16) override;
        bool		put(const void*, size_t) override;
    };

} // SWE
#endif

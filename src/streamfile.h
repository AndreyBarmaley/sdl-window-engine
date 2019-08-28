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

#include "streambuf.h"

class StreamFile : public StreamBase
{
    std::string		filename;
    const char*		filemode;
    StreamRWops         rw;

public:
    StreamFile() : filemode(NULL) {}
    StreamFile(const std::string &, const char* mode);
    StreamFile(const StreamFile &);
    ~StreamFile();

    StreamFile &	operator= (const StreamFile &);

    size_t		size(void) const { return rw.size(); }
    size_t		tell(void) const { return rw.tell(); }

    bool		open(const std::string &, const char* mode);
    void		close(void);
    bool		isValid(void) const { return rw.isValid(); }

    StreamBuf		toStreamBuf(size_t = 0 /* all data */);

    bool		seek(size_t pos) { return rw.seek(pos); }
    bool		skip(size_t len) { return rw.skip(len); }

    int			get8(void) const override;
    int			getBE16(void) const override;
    int			getLE16(void) const override;
    int			getBE32(void) const override;
    int			getLE32(void) const override;
    s64			getBE64(void) const override;
    s64			getLE64(void) const override;
    BinaryBuf		get(size_t = 0 /* all data */) const override;

    void		put8(char) override;
    void		putBE64(u64) override;
    void		putLE64(u64) override;
    void		putBE32(u32) override;
    void		putLE32(u32) override;
    void		putBE16(u16) override;
    void		putLE16(u16) override;
    void		put(const char*, size_t) override;
};

#endif

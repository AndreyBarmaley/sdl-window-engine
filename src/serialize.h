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

#ifndef _SWE_SERIALIZE_
#define _SWE_SERIALIZE_

#include <map>
#include <list>
#include <string>
#include <vector>

#include "types.h"
#include "binarybuf.h"

struct Point;
struct Rect;
struct Size;

class StreamBase : protected BitFlags
{
protected:
    virtual size_t	sizeg(void) const = 0;
    virtual size_t	sizep(void) const = 0;
    virtual size_t	tellg(void) const = 0;
    virtual size_t	tellp(void) const = 0;

    void		setconstbuf(bool);
    void		setfail(bool);

public:
    StreamBase() {}
    virtual ~StreamBase() {}

    void		setbigendian(bool);

    bool		isconstbuf(void) const;
    bool		fail(void) const;
    bool		bigendian(void) const;

    virtual void	skip(size_t) = 0;

    virtual int		getBE16(void) = 0;
    virtual int		getLE16(void) = 0;
    virtual int		getBE32(void) = 0;
    virtual int		getLE32(void) = 0;
    virtual s64		getBE64(void) = 0;
    virtual s64		getLE64(void) = 0;

    virtual void	putBE64(u64) = 0;
    virtual void	putLE64(u64) = 0;
    virtual void	putBE32(u32) = 0;
    virtual void	putLE32(u32) = 0;
    virtual void	putBE16(u16) = 0;
    virtual void	putLE16(u16) = 0;

    virtual BinaryBuf	get(size_t = 0 /* all data */) = 0;
    virtual void	put(const char*, size_t) = 0;

    bool		wait(const std::string &);

    virtual int		get8(void) = 0;
    int			get16(void);
    int			get32(void);
    s64			get64(void);

    virtual void	put8(char) = 0;
    void		put16(u16);
    void		put32(u32);
    void		put64(u64);

    StreamBase &	operator>> (bool &);
    StreamBase &	operator>> (char &);
    StreamBase &	operator>> (u8 &);
    StreamBase &	operator>> (s8 &);
    StreamBase &	operator>> (u16 &);
    StreamBase &	operator>> (s16 &);
    StreamBase &	operator>> (u32 &);
    StreamBase &	operator>> (s32 &);
    StreamBase &	operator>> (u64 &);
    StreamBase &	operator>> (s64 &);
    StreamBase &	operator>> (float &);
    StreamBase &	operator>> (std::string &);

    StreamBase &	operator>> (Rect &);
    StreamBase &	operator>> (Point &);
    StreamBase &	operator>> (Size &);
    StreamBase &	operator>> (BinaryBuf &);

    StreamBase &	operator<< (const bool &);
    StreamBase &	operator<< (const char &);
    StreamBase &	operator<< (const u8 &);
    StreamBase &	operator<< (const s8 &);
    StreamBase &	operator<< (const u16 &);
    StreamBase &	operator<< (const s16 &);
    StreamBase &	operator<< (const u32 &);
    StreamBase &	operator<< (const s32 &);
    StreamBase &	operator<< (const u64 &);
    StreamBase &	operator<< (const s64 &);
    StreamBase &	operator<< (const float &);
    StreamBase &	operator<< (const std::string &);

    StreamBase &	operator<< (const Rect &);
    StreamBase &	operator<< (const Point &);
    StreamBase &	operator<< (const Size &);
    StreamBase &	operator<< (const BinaryBuf &);

    template<class Type1, class Type2>
    StreamBase & operator>> (std::pair<Type1, Type2> & p)
    {
	return *this >> p.first >> p.second;
    }

    template<class Type>
    StreamBase & operator>> (std::vector<Type> & v)
    {
	size_t size = get32();
	v.clear();
	for(size_t it = 0; it < size; ++it){ Type t; *this >> t; v.push_back(t); }
	return *this;
    }

    template<class Type>
    StreamBase & operator>> (std::list<Type> & v)
    {
	size_t size = get32();
	v.clear();
	for(size_t it = 0; it < size; ++it){ Type t; *this >> t; v.push_back(t); }
	return *this;
    }

    template<class Type1, class Type2>
    StreamBase & operator>> (std::map<Type1, Type2> & v)
    {
	size_t size = get32();
	v.clear();
	for(size_t ii = 0; ii < size; ++ii)
	{
	    std::pair<Type1, Type2> pr;
	    *this >> pr;
	    v.insert(pr);
	}
	return *this;
    }

    template<class Type1, class Type2>
    StreamBase & operator<< (const std::pair<Type1, Type2> & p)
    {
	return *this << p.first << p.second;
    }

    template<class Type>
    StreamBase & operator<< (const std::vector<Type> & v)
    {
	put32(static_cast<u32>(v.size()));
	for(typename std::vector<Type>::const_iterator
	    it = v.begin(); it != v.end(); ++it) *this << *it;
	return *this;
    }

    template<class Type>
    StreamBase & operator<< (const std::list<Type> & v)
    {
	put32(static_cast<u32>(v.size()));
	for(typename std::list<Type>::const_iterator
	    it = v.begin(); it != v.end(); ++it) *this << *it;
	return *this;
    }

    template<class Type1, class Type2>
    StreamBase & operator<< (const std::map<Type1, Type2> & v)
    {
	put32(static_cast<u32>(v.size()));
	for(typename std::map<Type1, Type2>::const_iterator
	    it = v.begin(); it != v.end(); ++it) *this << *it;
	return *this;
    }
};

class StreamBuf : public StreamBase
{
public:
    StreamBuf(size_t = 0);
    StreamBuf(const StreamBuf &);
    StreamBuf(const BinaryBuf &, int endian = 2 /* 0: litle, 1: big, 2: platform */);
    StreamBuf(const u8*, size_t, int endian = 2 /* 0: litle, 1: big, 2: platform */);

    ~StreamBuf();

    StreamBuf &		operator= (const StreamBuf &);

    const u8*		data(void) const;
    size_t		size(void) const;
    size_t		capacity(void) const;

    void		seek(size_t);
    void		skip(size_t);

    int			get8(void);
    int			getBE16(void);
    int			getLE16(void);
    int			getBE32(void);
    int			getLE32(void);
    s64			getBE64(void);
    s64			getLE64(void);

    void		put8(char);
    void		putBE64(u64);
    void		putLE64(u64);
    void		putBE32(u32);
    void		putLE32(u32);
    void		putBE16(u16);
    void		putLE16(u16);

    BinaryBuf		get(size_t = 0 /* all data */);
    void		put(const char*, size_t);

protected:
    void		reset(void);

    size_t		tellg(void) const;
    size_t		tellp(void) const;
    size_t		sizeg(void) const;
    size_t		sizep(void) const;

    void		copy(const StreamBuf &);
    void		realloc(size_t);

    u8*			itbeg;
    u8*			itget;
    u8*			itput;
    u8*			itend;
};

class ZStreamBuf : public StreamBuf
{
public:
    bool		read(const std::string &, size_t offset = 0);
    bool		write(const std::string &, bool append = false) const;
};

class StreamFile : public StreamBase
{
    std::string		filename;
    const char*		filemode;
    SDL_RWops*		rw;

public:
    StreamFile() : filemode(NULL), rw(NULL) {}
    StreamFile(const std::string &, const char* mode);
    StreamFile(const StreamFile &);
    ~StreamFile();

    StreamFile &	operator= (const StreamFile &);

    size_t		size(void) const;
    size_t		tell(void) const;

    bool		open(const std::string &, const char* mode);
    void		close(void);
    bool		isValid(void) const { return rw; }

    StreamBuf		toStreamBuf(size_t = 0 /* all data */);

    void		seek(size_t);
    void		skip(size_t);

    int			get8(void);
    int			getBE16(void);
    int			getLE16(void);
    int			getBE32(void);
    int			getLE32(void);
    s64			getBE64(void);
    s64			getLE64(void);

    void		put8(char);
    void		putBE64(u64);
    void		putLE64(u64);
    void		putBE32(u32);
    void		putLE32(u32);
    void		putBE16(u16);
    void		putLE16(u16);

    BinaryBuf		get(size_t = 0 /* all data */);
    void		put(const char*, size_t);

protected:
    size_t		sizeg(void) const;
    size_t		sizep(void) const;
    size_t		tellg(void) const;
    size_t		tellp(void) const;
};

#ifndef DISABLE_NETWORK
class StreamNetwork : public StreamBase
{
    TCPsocket		sd;
    SDLNet_SocketSet	sdset;

    StreamNetwork(const StreamNetwork &) {}
    StreamNetwork &	operator= (const StreamNetwork &){ return *this; }

public:
    StreamNetwork();
    StreamNetwork(const std::string &, int);
    ~StreamNetwork();

    bool		open(const std::string &, int);
    void		close(void);

    void		skip(size_t);

    int			get8(void);
    int			getBE16(void);
    int			getLE16(void);
    int			getBE32(void);
    int			getLE32(void);
    s64			getBE64(void);
    s64			getLE64(void);

    void		put8(char);
    void		putBE64(u64);
    void		putLE64(u64);
    void		putBE32(u32);
    void		putLE32(u32);
    void		putBE16(u16);
    void		putLE16(u16);

    BinaryBuf		get(size_t = 0 /* all data */);
    void		put(const char*, size_t);
    static void		setReadyTimeout(size_t ms) { timeout = ms; }

protected:
    size_t		sizeg(void) const { return 0; }
    size_t		sizep(void) const { return 0; }
    size_t		tellg(void) const { return 0; }
    size_t		tellp(void) const { return 0; }


    int			recv(char*, int);
    int			send(const char*, int);
    bool		ready(void) const;

    static size_t	timeout;
};

#endif
#endif

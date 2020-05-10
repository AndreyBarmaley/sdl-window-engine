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

#include "swe_types.h"
#include "swe_binarybuf.h"

namespace SWE
{

    struct Point;
    struct Rect;
    struct Size;

    /* StreamBase */
    class StreamBase : protected BitFlags
    {
    protected:
        void		setconstbuf(bool);
        void		setfail(bool) const;

    public:
        StreamBase();
        virtual ~StreamBase() {}

        void		setbigendian(bool);

        bool		isconstbuf(void) const;
        bool		fail(void) const;
        bool		bigendian(void) const;

        virtual int	getBE16(void) const = 0;
        virtual int	getLE16(void) const = 0;
        virtual int	getBE32(void) const = 0;
        virtual int	getLE32(void) const = 0;
        virtual s64	getBE64(void) const = 0;
        virtual s64	getLE64(void) const = 0;

        virtual void	putBE64(u64) = 0;
        virtual void	putLE64(u64) = 0;
        virtual void	putBE32(u32) = 0;
        virtual void	putLE32(u32) = 0;
        virtual void	putBE16(u16) = 0;
        virtual void	putLE16(u16) = 0;

        virtual BinaryBuf	get(size_t = 0 /* all data */) const = 0;
        virtual void		put(const char*, size_t) = 0;

        bool		wait(const std::string &);

        virtual int	get8(void) const = 0;
        int		get16(void) const;
        int		get32(void) const;
        s64		get64(void) const;

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

            for(size_t it = 0; it < size; ++it)
            {
                Type t;
                *this >> t;
                v.push_back(t);
            }

            return *this;
        }

        template<class Type>
        StreamBase & operator>> (std::list<Type> & v)
        {
            size_t size = get32();
            v.clear();

            for(size_t it = 0; it < size; ++it)
            {
                Type t;
                *this >> t;
                v.push_back(t);
            }

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

    /* StreamRWops */
    class StreamRWops
    {
        StreamRWops(const StreamRWops &) : rw(NULL) {}
        StreamRWops & operator=(const StreamRWops &)
        {
            return *this;
        }

    protected:
        SDL_RWops*		rw;

    public:
        ~StreamRWops()
        {
            close();
        }

        StreamRWops(SDL_RWops* val = NULL) : rw(val) {}
        StreamRWops(StreamRWops && srw) noexcept
        {
            rw = srw.rw;
            srw.rw = NULL;
        }

        StreamRWops & operator=(StreamRWops && srw) noexcept
        {
            rw = srw.rw;
            srw.rw = NULL;
            return *this;
        }

        void		close(void);

        size_t		size(void) const;
        size_t              last(void) const;
        size_t              tell(void) const;
        bool                seek(int offset, int whence = RW_SEEK_SET);
        bool                skip(size_t);

        bool		isValid(void) const
        {
            return rw;
        }

        int                 get8(void) const;
        int                 getBE16(void) const;
        int                 getLE16(void) const;
        int                 getBE32(void) const;
        int                 getLE32(void) const;
        s64                 getBE64(void) const;
        s64                 getLE64(void) const;
        BinaryBuf           get(size_t = 0 /* all data */) const;

        bool		put8(char);
        bool		putBE16(u16);
        bool		putLE16(u16);
        bool		putBE32(u32);
        bool		putLE32(u32);
        bool		putBE64(u64);
        bool		putLE64(u64);
        bool		put(const char*, size_t);
    };

} // SWE
#endif

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
#include <unordered_map>

#include "swe_types.h"
#include "swe_binarybuf.h"

/// @brief пространство SWE
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

        virtual BinaryBuf get(size_t = 0 /* all data */) const = 0;
        virtual bool	get(void*, size_t) const = 0;
        virtual bool	put(const void*, size_t) = 0;

        bool		wait(const std::string &);

        virtual int	get8(void) const = 0;
        int		get16(void) const;
        int		get32(void) const;
        s64		get64(void) const;

        virtual void	put8(u8) = 0;
        void		put16(u16);
        void		put32(u32);
        void		put64(u64);

        virtual size_t  tell(void) const { return 0; }
        virtual bool    skip(size_t) const { return false; }
        virtual bool    seek(int offset, int whence = RW_SEEK_SET) const { return false; }

        const StreamBase &	operator>> (bool &) const;
        const StreamBase &	operator>> (char &) const;
        const StreamBase &	operator>> (u8 &) const;
        const StreamBase &	operator>> (s8 &) const;
        const StreamBase &	operator>> (u16 &) const;
        const StreamBase &	operator>> (s16 &) const;
        const StreamBase &	operator>> (u32 &) const;
        const StreamBase &	operator>> (s32 &) const;
        const StreamBase &	operator>> (u64 &) const;
        const StreamBase &	operator>> (s64 &) const;
        const StreamBase &	operator>> (float &) const;
        const StreamBase &	operator>> (std::string &) const;

        const StreamBase &	operator>> (Rect &) const;
        const StreamBase &	operator>> (Point &) const;
        const StreamBase &	operator>> (Size &) const;
        const StreamBase &	operator>> (BinaryBuf &) const;

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
        const StreamBase & operator>> (std::pair<Type1, Type2> & p) const
        {
            return *this >> p.first >> p.second;
        }

        template<class Type>
        const StreamBase & operator>> (std::vector<Type> & v) const
        {
            size_t size = get32();
            v.clear(); v.reserve(size);
            for(size_t it = 0; it < size; ++it)
            {
                Type t; *this >> t; v.emplace_back(t);
            }
            return *this;
        }

        template<class Type>
        const StreamBase & operator>> (std::list<Type> & v) const
        {
            size_t size = get32(); v.clear();
            for(size_t it = 0; it < size; ++it)
            {
                Type t; *this >> t; v.emplace_back(t);
            }
            return *this;
        }

        template<class Type1, class Type2>
        const StreamBase & operator>> (std::map<Type1, Type2> & v) const
        {
            size_t size = get32(); v.clear();
            for(size_t ii = 0; ii < size; ++ii)
            {
                Type1 t; *this >> t; *this >> v[t];
            }
            return *this;
        }

        template<class Type1, class Type2, class... Args>
        const StreamBase & operator>> (std::unordered_map<Type1, Type2, Args...> & v) const
        {
            size_t size = get32(); v.clear();
            for(size_t ii = 0; ii < size; ++ii)
            {
                Type1 t; *this >> t; *this >> v[t];
            }
            return *this;
        }

        template<class Type1, class Type2>
        StreamBase & operator<< (const std::pair<Type1, Type2> & p)
        {
            return *this << p.first << p.second;
        }

        template<typename InputIterator>
        StreamBase & push(InputIterator first, InputIterator last)
        {
            for(auto it = first; it != last; ++it)
                *this << *it;

            return *this;
        }

        template<class Type>
        StreamBase & operator<< (const std::vector<Type> & v)
        {
            put32(v.size());
            return push(std::begin(v), std::end(v));
        }

        template<class Type>
        StreamBase & operator<< (const std::list<Type> & v)
        {
            put32(v.size());
            return push(std::begin(v), std::end(v));
        }

        template<class Type1, class Type2>
        StreamBase & operator<< (const std::map<Type1, Type2> & v)
        {
            put32(v.size());
            return push(std::begin(v), std::end(v));
        }

        template<class Type1, class Type2, class... Args>
        StreamBase & operator<< (const std::unordered_map<Type1, Type2, Args...> & v)
        {
            put32(v.size());
            return push(std::begin(v), std::end(v));
        }
    };

    /* StreamRWops */
    class StreamRWops
    {
        StreamRWops(const StreamRWops &) {};
        StreamRWops & operator=(const StreamRWops &){ return *this; };

    protected:
        SDL_RWops*		rw;

    public:
        StreamRWops(SDL_RWops* val = nullptr);
        StreamRWops(StreamRWops && srw) noexcept;
        ~StreamRWops();

        StreamRWops &	operator=(StreamRWops && srw) noexcept;

        void		close(void);

        size_t		size(void) const;
        size_t          last(void) const;
        size_t          tell(void) const;
        bool            skip(size_t) const;
        bool            seek(int offset, int whence) const;

        bool		isValid(void) const { return rw; }

        int             get8(void) const;
        int             getBE16(void) const;
        int             getLE16(void) const;
        int             getBE32(void) const;
        int             getLE32(void) const;
        s64             getBE64(void) const;
        s64             getLE64(void) const;
        bool		get(void*, size_t) const;
        BinaryBuf       get(size_t = 0 /* all data */) const;

        bool		put8(u8);
        bool		putBE16(u16);
        bool		putLE16(u16);
        bool		putBE32(u32);
        bool		putLE32(u32);
        bool		putBE64(u64);
        bool		putLE64(u64);
        bool		put(const void*, size_t);
    };

} // SWE
#endif

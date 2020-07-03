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

#ifndef _SWE_TYPES_
#define _SWE_TYPES_

#define MAXU16   		0xFFFF
#define MAXU32   		0xFFFFFFFF

#define ARRAY_COUNT(A)	       	sizeof(A) / sizeof(A[0])
#define ARRAY_COUNT_END(A)   	A + ARRAY_COUNT(A)

#ifdef OLDENGINE
#define SDLENGINE		12
#else
#define SDLENGINE		20
#endif

#include "SDL.h"
#ifndef DISABLE_TTF
#include "SDL_ttf.h"
#endif
#ifndef DISABLE_IMAGE
#include "SDL_image.h"
#endif
#ifndef DISABLE_AUDIO
#include "SDL_mixer.h"
#endif
#ifndef DISABLE_NETWORK
#include "SDL_net.h"
#endif

#include <vector>

typedef Sint8           s8;
typedef Uint8           u8;
typedef Sint16          s16;
typedef Uint16          u16;
typedef Sint32          s32;
typedef Uint32          u32;
typedef Sint64          s64;
typedef Uint64          u64;

#ifndef PATH_MAX
#define PATH_MAX	4096
#endif

#if defined __SYMBIAN32__
#undef PATH_MAX
#define PATH_MAX FILENAME_MAX
namespace std
{
    int			c_abs(int x);
    float		c_abs(float x);
    double		c_abs(double x);
    int			c_isspace(char c);

#define isspace(c) 	c_isspace(c)
#define abs(x) 		c_abs(x)
}
using namespace std;
#endif

#if defined __MINGW32CE__
#undef PATH_MAX
#define PATH_MAX	255
#endif

#if defined __MINGW32__
#define S_IFSOCK 0140000
#define S_IFLNK  0120000

#define S_ISLNK(m)      (((m) & S_IFMT) == S_IFLNK)
#define S_ISSOCK(m)     (((m) & S_IFMT) == S_IFSOCK)
#endif

#if defined __MINGW32CE__
//#define S_IRWXU 00700
//#define S_IRUSR 00400
//#define S_IWUSR 00200
//#define S_IXUSR 00100

#define S_IRWXG 00070
#define S_IRGRP 00040
#define S_IWGRP 00020
#define S_IXGRP 00010

#define S_IRWXO 00007
#define S_IROTH 00004
#define S_IWOTH 00002
#define S_IXOTH 00001
#endif

#define _(s)            SWE::Translation::gettext(s)
#define _n(a,b,c)       SWE::Translation::ngettext(a,b,c)

namespace SWE
{
    class StreamBase;

    struct packshort
    {
    protected:
        u8 v1, v2;
	
    public:
        packshort(u16 val = 0);
        packshort(u8 val1, u8 val2);

        u16		operator()(void) const;
        u16		value(void) const;

        const u8 &	val1(void) const;
        const u8 &	val2(void) const;

        packshort &	set1(u8);
        packshort &	set2(u8);
        void		setvalue(u16);

        bool		operator< (const packshort &) const;
        bool		operator> (const packshort &) const;
        bool		operator== (const packshort &) const;
        bool		operator!= (const packshort &) const;
    };

    StreamBase & operator<< (StreamBase &, const packshort &);
    const StreamBase & operator>> (const StreamBase &, packshort &);

    struct packint
    {
	~packint() {}

        u32		operator()(void) const { return value(); }

        virtual u32	value(void) const = 0;
        virtual void	setvalue(u32) = 0;

        bool		operator< (const packint &) const;
        bool		operator> (const packint &) const;
        bool		operator== (const packint &) const;
        bool		operator!= (const packint &) const;
    };

    StreamBase & operator<< (StreamBase &, const packint &);
    const StreamBase & operator>> (const StreamBase &, packint &);

    struct packint2 : packint
    {
    protected:
        u16 v1, v2;
	
    public:
        packint2(u32 val = 0);
        packint2(u16 val1, u16 val2);

        u32		value(void) const override;
        void		setvalue(u32) override;

        const u16 &	val1(void) const;
        const u16 &	val2(void) const;

        packint2 &	set1(u16);
        packint2 &	set2(u16);
    };

    struct packint4 : packint
    {
    protected:
        packshort v1, v2;
	
    public:
        packint4(u32 val = 0);
        packint4(u16 val1, u16 val2);
        packint4(u8 val1, u8 val2, u8 val3, u8 val4);

        u32		value(void) const override;
        void		setvalue(u32) override;

        const u8 &	val1(void) const;
        const u8 &	val2(void) const;
        const u8 &	val3(void) const;
        const u8 &	val4(void) const;

        packint4 &	set1(u8);
        packint4 &	set2(u8);
        packint4 &	set3(u8);
        packint4 &	set4(u8);
    };

    class BitFlags
    {
        u32		state;

        friend StreamBase & operator<< (StreamBase &, const BitFlags &);
        friend const StreamBase & operator>> (const StreamBase &, BitFlags &);

    public:
        BitFlags(int v = 0) : state(v) {}

        size_t	operator()(void) const
        {
            return state;
        }
        size_t	value(void) const
        {
            return state;
        }

        void        set(size_t v, bool f)
        {
            if(f) set(v);
            else reset(v);
        }
        void        set(size_t v)
        {
            state |= v;
        }
        void	switched(size_t v)
        {
            set(v, ! check(v));
        }
        void        reset(size_t v)
        {
            state &= ~v;
        }
        void        reset(void)
        {
            state = 0;
        }
        bool        check(size_t v) const
        {
            return state & v;
        }

        int		countBits(void) const;
        std::vector<int> toVector(void) const;
    };

    StreamBase & operator<< (StreamBase &, const BitFlags &);
    const StreamBase & operator>> (const StreamBase &, BitFlags &);

} // SWE
#endif

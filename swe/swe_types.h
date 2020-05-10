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

namespace SWE
{

#define _(s)            Translation::gettext(s)
#define _n(a,b,c)       Translation::ngettext(a,b,c)

    class StreamBase;

    struct packshort
    {
    protected:
        u16 val;

        friend StreamBase & operator<< (StreamBase &, const packshort &);
        friend StreamBase & operator>> (StreamBase &, packshort &);

    public:
        packshort(int v = 0) : val(v) {}
        packshort(int val1, int val2) : val((0xFF00 & (val1 << 8)) | (0x00FF & val2)) {}

        int operator()(void) const
        {
            return val;
        }
        const u16 & value(void) const
        {
            return val;
        }

        int val1(void) const
        {
            return 0xFF & (value() >> 8);
        }
        int val2(void) const
        {
            return 0xFF & value();
        }

        packshort & set1(int v)
        {
            val = (val | 0xFF00) & (0x00FF | (0xFF00 & (v << 8)));
            return *this;
        }
        packshort & set2(int v)
        {
            val = (val | 0x00FF) & (0xFF00 | (0x00FF & v));
            return *this;
        }

        void setvalue(u16 v)
        {
            val = v;
        }

        bool operator< (const packshort & p) const
        {
            return val < p.val;
        }
        bool operator> (const packshort & p) const
        {
            return val > p.val;
        }
        bool operator== (const packshort & p) const
        {
            return val == p.val;
        }
        bool operator!= (const packshort & p) const
        {
            return val != p.val;
        }
    };

    StreamBase & operator<< (StreamBase &, const packshort &);
    StreamBase & operator>> (StreamBase &, packshort &);

    class packint
    {
    protected:
        u32 val;

        friend StreamBase & operator<< (StreamBase &, const packint &);
        friend StreamBase & operator>> (StreamBase &, packint &);

    public:
        packint(int v = 0) : val(v) {}

        int operator()(void) const
        {
            return val;
        }
        const u32 & value(void) const
        {
            return val;
        }

        void setvalue(u32 v)
        {
            val = v;
        }

        bool operator< (const packint & p) const
        {
            return val < p.val;
        }
        bool operator> (const packint & p) const
        {
            return val > p.val;
        }
        bool operator== (const packint & p) const
        {
            return val == p.val;
        }
        bool operator!= (const packint & p) const
        {
            return val != p.val;
        }
    };

    StreamBase & operator<< (StreamBase &, const packint &);
    StreamBase & operator>> (StreamBase &, packint &);

    struct packint4 : public packint
    {
        packint4(int val = 0) : packint(val) {}
        packint4(int val1, int val2, int val3, int val4) : packint((0xFF000000 & (val1 << 24)) | (0x00FF0000 & (val2 << 16))  | (0x0000FF00 & (val3 << 8)) | (0x000000FF & val4)) {}

        int val1(void) const
        {
            return 0xFF & (value() >> 24);
        }
        int val2(void) const
        {
            return 0xFF & (value() >> 16);
        }
        int val3(void) const
        {
            return 0xFF & (value() >> 8);
        }
        int val4(void) const
        {
            return 0xFF & value();
        }

        packint4 & set1(int v)
        {
            val = (val | 0xFF000000) & (0x00FFFFFF | (0xFF000000 & (v << 24)));
            return *this;
        }
        packint4 & set2(int v)
        {
            val = (val | 0x00FF0000) & (0xFF00FFFF | (0x00FF0000 & (v << 16)));
            return *this;
        }
        packint4 & set3(int v)
        {
            val = (val | 0x0000FF00) & (0xFFFF00FF | (0x0000FF00 & (v <<  8)));
            return *this;
        }
        packint4 & set4(int v)
        {
            val = (val | 0x000000FF) & (0xFFFFFF00 | (0x000000FF &  v));
            return *this;
        }
    };

    struct packint2 : public packint
    {
        packint2(int val = 0) : packint(val) {}
        packint2(int val1, int val2) : packint((0xFFFF0000 & (val1 << 16)) | (0x0000FFFF & val2)) {}

        int val1(void) const
        {
            return 0xFFFF & (value() >> 16);
        }
        int val2(void) const
        {
            return 0xFFFF & value();
        }

        packint2 & set1(int v)
        {
            val = (val | 0xFFFF0000) & (0x0000FFFF | (0xFFFF0000 & (v << 16)));
            return *this;
        }
        packint2 & set2(int v)
        {
            val = (val | 0x0000FFFF) & (0xFFFF0000 | (0x0000FFFF & v));
            return *this;
        }
    };

    class BitFlags
    {
        u32		state;

        friend StreamBase & operator<< (StreamBase &, const BitFlags &);
        friend StreamBase & operator>> (StreamBase &, BitFlags &);

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
    StreamBase & operator>> (StreamBase &, BitFlags &);

} // SWE
#endif

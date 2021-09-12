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

#include <cmath>
#include <numeric>
#include <cstring>
#include <iterator>
#include <algorithm>
#include <zlib.h>

#include "swe_display.h"
#include "swe_types.h"
#include "swe_tools.h"

namespace SWE
{

    u32 Tools::ticks(void)
    {
        return SDL_GetTicks();
    }

    void Tools::delay(unsigned int ms)
    {
        SDL_Delay(ms);
    }

    int Tools::countBits(unsigned long val)
    {
        int res = 0;

        for(unsigned long itr = 1; itr; itr <<= 1)
            if(val & itr) ++res;

        return res;
    }
            
    int Tools::signValue(int s)
    {   
        return (s < 0 ? -1 : (s > 0 ? 1 : 0));
    }

    int Tools::rand(int min, int max)
    {
        if(min > max) std::swap(min, max);
        return min + std::rand() / (RAND_MAX + 1.0) * (max - min + 1);
    }

    float Tools::randf(float min, float max)
    {
        if(min > max) std::swap(min, max);
        return min + std::rand() * (max - min) / static_cast<float>(RAND_MAX);
    }

    u32 Tools::crc32b(const std::string & str)
    {
	return crc32b(reinterpret_cast<const u8*>(str.data()), str.size(), 0xEDB88320);
    }

    u32 Tools::crc32b(const u8* ptr, size_t size)
    {
	return crc32b(ptr, size, 0xEDB88320);
    }

    u32 Tools::crc32b(const u8* ptr, size_t size, u32 magic)
    {
	u32 res = std::accumulate(ptr, ptr + size, 0xFFFFFFFF, [=](u32 crc, int val)
	{
    	    crc ^= val;

    	    for(int bit = 0; bit < 8; ++bit)
    	    {
        	u32 mask = crc & 1 ? 0xFFFFFFFF : 0;
        	crc = (crc >> 1) ^ (magic & mask);
    	    }

	    return crc;
	});

        return ~res;
    }

    int Tools::crc16b(const std::string & str)
    {
	return crc16b(reinterpret_cast<const u8*>(str.data()), str.size(), 0x8320);
    }

    int Tools::crc16b(const u8* ptr, size_t size)
    {
	return crc16b(ptr, size, 0x8320);
    }

    int Tools::crc16b(const u8* ptr, size_t size, u16 magic)
    {
	u16 res = std::accumulate(ptr, ptr + size, 0xFFFF, [=](u16 crc, int val)
	{
    	    crc ^= val;

    	    for(int bit = 0; bit < 8; ++bit)
    	    {
        	u16 mask = crc & 1 ? 0xFFFF : 0;
        	crc = (crc >> 1) ^ (magic & mask);
    	    }

	    return crc;
	});

        return ~res;
    }

    std::pair<int, float> Tools::modf(float num)
    {
        std::pair<int, float> res;
        float intpart = 0;
        res.second = modff(num, & intpart);
        res.first = intpart;
        return res;
    }

    BinaryBuf Tools::zlibUncompress(const char* ptr, size_t size, size_t real)
    {
        return zlibUncompress(reinterpret_cast<const u8*>(ptr), size, real);
    }

    BinaryBuf Tools::zlibCompress(const char* ptr, size_t size)
    {
        return zlibCompress(reinterpret_cast<const u8*>(ptr), size);
    }

    const char* zlibError(int ret)
    {
	switch(ret)
	{
	    case Z_STREAM_END: return "end";
	    case Z_NEED_DICT: return "dict";
	    case Z_ERRNO: return "errno";
	    case Z_STREAM_ERROR: return "stream";
	    case Z_DATA_ERROR: return "data";
	    case Z_MEM_ERROR: return "mem";
	    case Z_BUF_ERROR: return "buf";
	    case Z_VERSION_ERROR: return "version";
	}
	return "";
    }

    BinaryBuf Tools::zlibCompress(const u8* ptr, size_t size)
    {
        BinaryBuf res;

        if(ptr && size)
        {
            res.resize(compressBound(size));
            uLong dstsz = res.size();
            int ret = ::compress(reinterpret_cast<Bytef*>(res.data()), &dstsz,
                                 reinterpret_cast<const Bytef*>(ptr), size);

            if(ret == Z_OK)
                res.resize(dstsz);
            else
            {
                res.clear();
                ERROR("zlib error: " << zlibError(ret) << "(" << ret << ")");
            }
        }

        return res;
    }

    BinaryBuf Tools::zlibUncompress(const u8* ptr, size_t size, size_t real)
    {
        BinaryBuf res;

        if(ptr && size)
        {
            res.resize(real ? real : size * 7);
            uLong dstsz = res.size();
            int ret = Z_BUF_ERROR;

            while(Z_BUF_ERROR ==
                  (ret = ::uncompress(reinterpret_cast<Bytef*>(res.data()), &dstsz,
                                      reinterpret_cast<const Bytef*>(ptr), size)))
            {
                dstsz = res.size() * 2;
                res.resize(dstsz);
            }

            if(ret == Z_OK)
                res.resize(dstsz);
            else
            {
                res.clear();
                ERROR("zlib error: " << zlibError(ret) << "(" << ret << ")");
            }
        }

        return res;
    }

    u32 base64DecodeChar(u32 v)
    {
        if(v == '+')
            return 62;

        if(v == '/')
            return 63;

        if('0' <= v && v <= '9')
            return v - '0' + 52;

        if('A' <= v && v <= 'Z')
            return v - 'A';

        if('a' <= v && v <= 'z')
            return v - 'a' + 26;

        return 0;
    }

    u32 base64EncodeChar(u32 v)
    {
        // 0 <=> 25
        if(v <= ('Z' - 'A'))
            return v + 'A';

        // 26 <=> 51
        if(v <= (26 + ('z' - 'a')))
            return v + 'a' - 26;

        // 52 <=> 61
        if(v <= (52 + ('9' - '0')))
            return v + '0' - 52;

        if(v == 62)
            return '+';

        if(v == 63)
            return '/';

        return 0;
    }

    BinaryBuf Tools::base64Decode(const char* src)
    {
        size_t srcsz = src ? std::strlen(src) : 0;
        return base64Decode(reinterpret_cast<const u8*>(src), srcsz);
    }

    BinaryBuf Tools::base64Decode(const u8* src, size_t srcsz)
    {
        BinaryBuf dst;

        if(0 < srcsz && srcsz % 4 == 0)
        {
            size_t dstsz = 3 * srcsz / 4;

            if(src[srcsz - 1] == '=') dstsz--;
            if(src[srcsz - 2] == '=') dstsz--;

            dst.reserve(dstsz);

            for(size_t ii = 0; ii < srcsz; ii += 4)
            {
                u32 sextet_a = base64DecodeChar(src[ii]);
                u32 sextet_b = base64DecodeChar(src[ii + 1]);
                u32 sextet_c = base64DecodeChar(src[ii + 2]);
                u32 sextet_d = base64DecodeChar(src[ii + 3]);
                u32 triple = (sextet_a << 18) + (sextet_b << 12) + (sextet_c << 6) + sextet_d;

                if(dst.size() < dstsz) dst.push_back((triple >> 16) & 0xFF);

                if(dst.size() < dstsz) dst.push_back((triple >> 8) & 0xFF);

                if(dst.size() < dstsz) dst.push_back(triple & 0xFF);
            }
        }
        else
            ERROR("incorrect size buf: " << srcsz);

        return dst;
    }

    BinaryBuf Tools::base64Encode(const char* src)
    {
        size_t srcsz = src ? std::strlen(src) : 0;
        return base64Encode(reinterpret_cast<const u8*>(src), srcsz);
    }

    BinaryBuf Tools::base64Encode(const u8* src, size_t srcsz)
    {
        BinaryBuf dst;
        size_t dstsz = 4 * srcsz / 3 + 1;
        dst.reserve(dstsz);

        for(size_t ii = 0; ii < srcsz; ii += 3)
        {
            u32 b1 = src[ii];
            u32 b2 = ii + 1 < srcsz ? src[ii + 1] : 0;
            u32 b3 = ii + 2 < srcsz ? src[ii + 2] : 0;
            u32 triple = (b1 << 16) | (b2 << 8) | b3;

            dst.push_back(base64EncodeChar(0x3F & (triple >> 18)));
            dst.push_back(base64EncodeChar(0x3F & (triple >> 12)));
            dst.push_back(ii + 1 < srcsz ? base64EncodeChar(0x3F & (triple >> 6)) : '=');
            dst.push_back(ii + 2 < srcsz ? base64EncodeChar(0x3F & triple) : '=');
        }

        return dst;
    }

    Points Tools::renderCircle(const Point & center, int radius, bool fill)
    {
        Points res;
        res.reserve(fill ? 4 * radius* radius : 8 * radius);
        int x = radius;
        int y = 0;
        int fixRadius = 1 - x;

        while(x >= y)
        {
            if(fill)
            {
                // center + Point(-x, y)       center + Point(x, y)
                // center + Point(-x, -y)      center + Point(x, -y)
                for(int xx = center.x - x; xx <= center.x + x; ++xx)
                    res << Point(xx, center.y + y) << Point(xx, center.y - y);

                // center + Point(y, -x)       center + Point(y, x)
                // center + Point(-y, -x)      center + Point(-y, x)
                for(int yy = center.y - x; yy <= center.y + x; ++yy)
                    res << Point(center.x + y, yy) << Point(center.x - y, yy);
            }
            else
            {
                res <<
                    center + Point(x, y) << center + Point(y, x) <<
                    center + Point(-x, y) << center + Point(-y, x) <<
                    center + Point(-x, -y) << center + Point(-y, -x) <<
                    center + Point(x, -y) << center + Point(y, -x);
            }

            y++;

            if(fixRadius < 0)
                fixRadius += 2 * y + 1;
            else
            {
                x--;
                fixRadius += 2 * (y - x + 1);
            }
        }

        return res;
    }

    Points Tools::renderLine(const Point & pt1, const Point & pt2, int step)
    {
        Points res;
        res.reserve(Point::distance(pt1, pt2));
        res.push_back(pt1);
	if(pt1 == pt2) return res;

        int dx = std::abs(pt2.x - pt1.x);
        int dy = std::abs(pt2.y - pt1.y);

        int ns = std::div((dx > dy ? dx : dy), 2).quot;
	int ii = 0;

        Point pt(pt1);

        while(pt != pt2)
        {
            if(dx > dy)
            {
                pt.x < pt2.x ? ++pt.x : --pt.x;
                ns -= dy;
            }
            else
            {
                pt.y < pt2.y ? ++pt.y : --pt.y;
                ns -= dx;
            }

            if(ns < 0)
            {
                if(dx > dy)
                {
                    pt.y < pt2.y ? ++pt.y : --pt.y;
                    ns += dx;
                }
                else
                {
                    pt.x < pt2.x ? ++pt.x : --pt.x;
                    ns += dy;
                }
            }

            if(2 > step || 0 == (ii % step) || pt == pt2)
		res.push_back(pt);

	    ii++;
        }

        return res;
    }

    /* Timer */
    struct TimerDeleter
    {
        void operator()(SDL_TimerID* ptr)
        {
            SDL_RemoveTimer(*ptr);
            std::default_delete<SDL_TimerID>() (ptr);
        }
    };

    Timer::Timer(const SDL_TimerID & id) : ptr(new SDL_TimerID{id})
    {
    }

    void Timer::destroy(void)
    {
        ptr.reset();
    }

    Timer Timer::create(u32 interval, u32(*fn)(u32, void*), void* param)
    {
        return Timer(SDL_AddTimer(interval, fn, param));
    }

    bool Timer::isValid(void) const
    {
        return ptr.get();
    }

    /* TickTrigger */
    TickTrigger::TickTrigger() : latest(0)
    {
	reset();
    }

    bool TickTrigger::check(u32 ms, u32 period) const
    {
        if(0 < period && 0 < latest && ms - latest >= period)
        {
            latest = ms;
            return true;
        }
        return false;
    }

    void TickTrigger::reset(void)
    {
        latest = Tools::ticks() - Display::timeStart();
    }

    void TickTrigger::disabled(bool f)
    {
        if(f) latest = 0; else reset();
    }

    /* packshort */
    packshort::packshort(u16 val) : v1(0), v2(0)
    {
	setvalue(val);
    }

    packshort::packshort(u8 val1, u8 val2) : v1(0), v2(0)
    {
	set1(val1).set2(val2);
    }

    const u8 & packshort::val1(void) const
    {
	return v1;
    }

    const u8 & packshort::val2(void) const
    {
	return v2;
    }

    packshort & packshort::set1(u8 val)
    {
	v1 = val;
	return *this;
    }

    packshort & packshort::set2(u8 val)
    {
	v2 = val;
	return *this;
    }

    u16 packshort::operator()(void) const
    {
	return value();
    }

    u16 packshort::value(void) const
    {
	return (static_cast<u16>(val1()) << 8) | static_cast<u16>(val2());
    }

    void packshort::setvalue(u16 val)
    {
	set1(0xFF & (val >> 8));
	set2(0xFF & val);
    }

    bool packshort::operator< (const packshort & p) const
    {
	return value() < p.value();
    }

    bool packshort::operator> (const packshort & p) const
    {
	return value() > p.value();
    }

    bool packshort::operator== (const packshort & p) const
    {
	return value() == p.value();
    }

    bool packshort::operator!= (const packshort & p) const
    {
	return value() != p.value();
    }

    StreamBase & operator<< (StreamBase & sb, const packshort & st)
    {
        return sb << st.value();
    }

    const StreamBase & operator>> (const StreamBase & sb, packshort & st)
    {
	u16 val16; sb >> val16; st.setvalue(val16);
	return sb;
    }

    /* packint */
    bool packint::operator< (const packint & p) const
    {
	return value() < p.value();
    }

    bool packint::operator> (const packint & p) const
    {
	return value() > p.value();
    }

    bool packint::operator== (const packint & p) const
    {
	return value() == p.value();
    }

    bool packint::operator!= (const packint & p) const
    {
	return value() != p.value();
    }

    StreamBase & operator<< (StreamBase & sb, const packint & st)
    {
        return sb << st.value();
    }

    const StreamBase & operator>> (const StreamBase & sb, packint & st)
    {
	u32 val32; sb >> val32; st.setvalue(val32);
	return sb;
    }

    /* packint2 */
    packint2::packint2(u32 val) : v1(0), v2(0)
    {
	setvalue(val);
    }

    packint2::packint2(u16 val1, u16 val2) : v1(0), v2(0)
    {
	set1(val1).set2(val2);
    }

    const u16 & packint2::val1(void) const
    {
	return v1;
    }

    const u16 & packint2::val2(void) const
    {
	return v2;
    }

    packint2 & packint2::set1(u16 val)
    {
	v1 = val;
	return *this;
    }

    packint2 & packint2::set2(u16 val)
    {
	v2 = val;
	return *this;
    }

    u32 packint2::value(void) const
    {
	return (static_cast<u32>(val1()) << 16) | static_cast<u32>(val2());
    }

    void packint2::setvalue(u32 val)
    {
	set1(0xFFFF & (val >> 16));
	set2(0xFFFF & val);
    }

    /* packint4 */
    packint4::packint4(u32 val) : v1(0), v2(0)
    {
	setvalue(val);
    }

    packint4::packint4(u16 val1, u16 val2) : v1(0), v2(0)
    {
	v1.setvalue(val1);
	v2.setvalue(val2);
    }

    packint4::packint4(u8 val1, u8 val2, u8 val3, u8 val4) : v1(0), v2(0)
    {
	v1.set1(val1).set2(val2);
	v2.set1(val3).set2(val4);
    }

    const u8 & packint4::val1(void) const
    {
	return v1.val1();
    }

    const u8 & packint4::val2(void) const
    {
	return v1.val2();
    }

    const u8 & packint4::val3(void) const
    {
	return v2.val1();
    }

    const u8 & packint4::val4(void) const
    {
	return v2.val2();
    }

    packint4 & packint4::set1(u8 val)
    {
	v1.set1(val);
	return *this;
    }

    packint4 & packint4::set2(u8 val)
    {
	v1.set2(val);
	return *this;
    }

    packint4 & packint4::set3(u8 val)
    {
	v2.set1(val);
	return *this;
    }

    packint4 & packint4::set4(u8 val)
    {
	v2.set2(val);
	return *this;
    }

    u32 packint4::value(void) const
    {
	return (static_cast<u32>(v1()) << 16) | static_cast<u32>(v2());
    }

    void packint4::setvalue(u32 val)
    {
	v1.setvalue(0xFFFF & (val >> 16));
	v2.setvalue(0xFFFF & val);
    }

    /* BitFlags */
    StreamBase & operator<< (StreamBase & sb, const BitFlags & st)
    {
        return sb << st.state;
    }

    const StreamBase & operator>> (const StreamBase & sb, BitFlags & st)
    {
        return sb >> st.state;
    }

    int BitFlags::countBits(void) const
    {
        int res = 0;

        for(int ii = 0; ii < 31; ++ii)
            if(check(static_cast<int>(1) << ii)) res++;

        return res;
    }

    std::vector<int> BitFlags::toVector(void) const
    {
        std::vector<int> res;
        res.reserve(32);

        for(int ii = 0; ii < 31; ++ii)
        {
            int spec = static_cast<int>(1) << ii;

            if(check(spec)) res.push_back(spec);
        }

        return res;
    }

#ifdef SWE_WITH_ICONV
#include <iconv.h>
    std::string Tools::stringEncode(const std::string & str, const char* charset)
    {
        iconv_t cd;

        if(! charset || (iconv_t)(-1) == (cd = iconv_open("utf-8", charset)))
            return str;

        std::string res(str);
        size_t inbytesleft = str.size();
        size_t outbytesleft = inbytesleft * 2 + 1;
        const char* inbuf = str.c_str();
        char* outbuf1 = new char [outbytesleft];
        char* outbuf2 = outbuf1;
#if defined(__MINGW32CE__)
        size_t reslen = iconv(cd, const_cast<char**>(&inbuf), &inbytesleft, &outbuf1, &outbytesleft);
#elif defined(__FreeBSD__) || defined (__MINGW32__)  || defined (__MINGW64__)
        size_t reslen = iconv(cd, &inbuf, &inbytesleft, &outbuf1, &outbytesleft);
#else
        size_t reslen = iconv(cd, const_cast<char**>(&inbuf), &inbytesleft, &outbuf1, &outbytesleft);
#endif
        iconv_close(cd);

        if(reslen != (size_t)(-1))
            res = std::string(outbuf2, outbuf1 - outbuf2);

        delete [] outbuf2;
        return res;
    }
#else
    std::string cp1251_to_utf8(const std::string & in)
    {
        const u32 table_1251[] =
        {
            0x82D0, 0x83D0, 0x9A80E2, 0x93D1, 0x9E80E2, 0xA680E2, 0xA080E2, 0xA180E2,
            0xAC82E2, 0xB080E2, 0x89D0, 0xB980E2, 0x8AD0, 0x8CD0, 0x8BD0, 0x8FD0,
            0x92D1, 0x9880E2, 0x9980E2, 0x9C80E2, 0x9D80E2, 0xA280E2, 0x9380E2, 0x9480E2,
            0, 0xA284E2, 0x99D1, 0xBA80E2, 0x9AD1, 0x9CD1, 0x9BD1, 0x9FD1,
            0xA0C2, 0x8ED0, 0x9ED1, 0x88D0, 0xA4C2, 0x90D2, 0xA6C2, 0xA7C2,
            0x81D0, 0xA9C2, 0x84D0, 0xABC2, 0xACC2, 0xADC2, 0xAEC2, 0x87D0,
            0xB0C2, 0xB1C2, 0x86D0, 0x96D1, 0x91D2, 0xB5C2, 0xB6C2, 0xB7C2,
            0x91D1, 0x9684E2, 0x94D1, 0xBBC2, 0x98D1, 0x85D0, 0x95D1, 0x97D1,
            0x90D0, 0x91D0, 0x92D0, 0x93D0, 0x94D0, 0x95D0, 0x96D0, 0x97D0,
            0x98D0, 0x99D0, 0x9AD0, 0x9BD0, 0x9CD0, 0x9DD0, 0x9ED0, 0x9FD0,
            0xA0D0, 0xA1D0, 0xA2D0, 0xA3D0, 0xA4D0, 0xA5D0, 0xA6D0, 0xA7D0,
            0xA8D0, 0xA9D0, 0xAAD0, 0xABD0, 0xACD0, 0xADD0, 0xAED0, 0xAFD0,
            0xB0D0, 0xB1D0, 0xB2D0, 0xB3D0, 0xB4D0, 0xB5D0, 0xB6D0, 0xB7D0,
            0xB8D0, 0xB9D0, 0xBAD0, 0xBBD0, 0xBCD0, 0xBDD0, 0xBED0, 0xBFD0,
            0x80D1, 0x81D1, 0x82D1, 0x83D1, 0x84D1, 0x85D1, 0x86D1, 0x87D1,
            0x88D1, 0x89D1, 0x8AD1, 0x8BD1, 0x8CD1, 0x8DD1, 0x8ED1, 0x8FD1
        };
        std::string res;
        res.reserve(in.size() * 2 + 1);

        for(auto it = in.begin(); it != in.end(); ++it)
        {
            if(*it & 0x80)
            {
                const size_t index = *it & 0x7f;

                if(index < ARRAY_COUNT(table_1251))
                {
                    const u32 & v = table_1251[index];
                    res.append(1, v);
                    res.append(1, v >> 8);

                    if(v & 0xFFFF0000) res.append(1, v >> 16);
                }
            }
            else
                res.append(1, *it);
        }

        return res;
    }

    std::string Tools::stringEncode(const std::string & str, const char* charset)
    {
        if(charset)
        {
            if(0 == std::strcmp(charset, "cp1251"))
                return cp1251_to_utf8(str);
        }

        ERROR("not supported");
        return str;
    }
#endif
}

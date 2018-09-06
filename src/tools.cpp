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
#include <cstring>
#include <algorithm>
#include <zlib.h>

#include "engine.h"

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

u32 crc32i(u32 crc, int val)
{
    crc ^= val;

    for(int bit = 0; bit < 8; ++bit)
    {
        u32 mask = crc & 1 ? 0xFFFFFFFF : 0;
        crc = (crc >> 1) ^ (0xEDB88320 & mask);
    }

    return crc;
}

u32 Tools::crc32b(const char* str)
{
    u32 crc = 0xFFFFFFFF;

    while(str && *str)
    {
	crc = crc32i(crc, *str);
	str++;
    }

    return ~crc;
}

u32 Tools::crc32b(const u8* ptr, size_t size)
{
    u32 crc = 0xFFFFFFFF;

    for(const u8* it = ptr; it < ptr + size; ++it)
	crc = crc32i(crc, *it);

    return ~crc;
}

u16 crc16i(u16 crc, int val)
{
    crc ^= val;

    for(int bit = 0; bit < 8; ++bit)
    {
        u16 mask = crc & 1 ? 0xFFFF : 0;
        crc = (crc >> 1) ^ (0x8320 & mask);
    }

    return crc;
}

int Tools::crc16b(const char* str)
{
    u16 crc = 0xFFFF;

    while(str && *str)
    {
	crc = crc16i(crc, *str);
	str++;
    }

    return ~crc;
}

int Tools::crc16b(const u8* ptr, size_t size)
{
    u16 crc = 0xFFFF;

    for(const u8* it = ptr; it < ptr + size; ++it)
	crc = crc16i(crc, *it);

    return ~crc;
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
            ERROR("zlib error: " << ret);
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
            ERROR("zlib error: " << ret);
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
    if(26 <= v && v <= 26 + ('z' - 'a'))
        return v + 'a' - 26;

    // 52 <=> 61
    if(52 <= v && v <= 52 + ('9' - '0'))
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
    {
	ERROR("incorrect size buf: " << srcsz);
    }

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

Timer::Timer(const SDL_TimerID & id) : ptr(std::make_shared<SDL_TimerID>(id))
{
}

Timer::~Timer()
{
    destroy();
}

void Timer::destroy(void)
{
    if(ptr.unique() && isValid())
    {
        SDL_RemoveTimer(*ptr);
        *ptr = 0;
    }
}

Timer Timer::create(u32 interval, u32 (*fn)(u32, void*), void* param)
{
    return Timer(SDL_AddTimer(interval, fn, param));
}

Points Tools::renderCircle(const Point & center, int radius, bool fill, bool sort)
{
    Points res;
    res.reserve(fill ? 4 * radius * radius : 8 * radius);

    int x = radius;
    int y = 0;
    int fixRadius = 1 - x;

    while (x >= y)
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

    if(sort)
	std::sort(res.begin(), res.end());

    return res;
}

Points Tools::renderLine(const Point & pt1, const Point & pt2, int step)
{
    Points res;
    res.reserve(Point::approximateDistance(pt1, pt2));

    const int dx = std::abs(pt2.x - pt1.x);
    const int dy = std::abs(pt2.y - pt1.y);

    int ns = std::div((dx > dy ? dx : dy), 2).quot;
    Point pt(pt1);

    for(int i = 0; i <= (dx > dy ? dx : dy); ++i)
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

        if(0 == (i % step)) res.push_back(pt);
    }

    return res;
}

StreamBase & operator<< (StreamBase & sb, const packint & st)
{
    return sb << st.val;
}

StreamBase & operator>> (StreamBase & sb, packint & st)
{
    return sb >> st.val;
}

StreamBase & operator<< (StreamBase & sb, const BitFlags & st)
{
    return sb << st.state;
}

StreamBase & operator>> (StreamBase & sb, BitFlags & st)
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

#ifdef WITH_ICONV
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

#if defined(__FreeBSD__) || defined (__MINGW32__)  || defined (__MINGW64__)
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
std::string Tools::stringEncode(const std::string & str, const char* charset)
{
    return str;
}
#endif

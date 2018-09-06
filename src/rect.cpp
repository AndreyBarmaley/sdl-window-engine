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

#include <climits>
#include <algorithm>
#include <iterator>
#include <sstream>

#include "rect.h"

bool Point::operator== (const Point & pt) const
{
    return (x == pt.x && y == pt.y);
}

bool Point::operator!= (const Point & pt) const
{
    return !(*this == pt);
}

Point & Point::operator+= (const Point & pt)
{
    x += pt.x;
    y += pt.y;

    return *this;
}

Point & Point::operator-= (const Point & pt)
{
    x -= pt.x;
    y -= pt.y;

    return *this;
}

Point Point::operator+ (const Point & pt) const
{
    return Point(x + pt.x, y + pt.y);
}

Point Point::operator- (const Point & pt) const
{
    return Point(x - pt.x, y - pt.y);
}

Point Point::operator* (const Point & pt) const
{
    return Point(x * pt.x, y * pt.y);
}

Point Point::operator/ (const Point & pt) const
{
    return Point(x / pt.x, y / pt.y);
}

Point Point::operator* (int val) const
{
    return Point(x * val, y * val);
}

Point Point::operator/ (int val) const
{
    return Point(x / val, y / val);
}

bool Point::operator< (const Point & pt) const
{
    return x < pt.x || (x == pt.x && y < pt.y);
}

int Point::approximateDistance(const Point & pt1, const Point & pt2)
{
    Point res = pt1 - pt2;
    return std::max(std::abs(res.x), std::abs(res.y));
}

std::string Point::toString(void) const
{
    std::ostringstream os;
    os << "x: " << x << ", " << "y: " << y;
    return os.str();
}

bool ZPoint::operator== (const ZPoint & pt) const
{
    return x == pt.x && y == pt.y && z == pt.z;
}

bool ZPoint::operator!= (const ZPoint & pt) const
{
    return !(*this == pt);
}

ZPoint & ZPoint::operator+= (const ZPoint & pt)
{
    x += pt.x;
    y += pt.y;
    z += pt.z;

    return *this;
}

ZPoint & ZPoint::operator-= (const ZPoint & pt)
{
    x -= pt.x;
    y -= pt.y;
    z -= pt.z;

    return *this;
}

ZPoint ZPoint::operator+ (const ZPoint & pt) const
{
    return ZPoint(x + pt.x, y + pt.y, z + pt.z);
}

ZPoint ZPoint::operator- (const ZPoint & pt) const
{
    return ZPoint(x - pt.x, y - pt.y, z - pt.z);
}

ZPoint ZPoint::operator* (const ZPoint & pt) const
{
    return ZPoint(x * pt.x, y * pt.y, z * pt.z);
}

ZPoint ZPoint::operator/ (const ZPoint & pt) const
{
    return ZPoint(x / pt.x, y / pt.y, z / pt.z);
}

ZPoint ZPoint::operator* (int val) const
{
    return ZPoint(x * val, y * val, z * val);
}

ZPoint ZPoint::operator/ (int val) const
{
    return ZPoint(x / val, y / val, z / val);
}

bool ZPoint::operator< (const ZPoint & pt) const
{
    return x < pt.x || (x == pt.x && (y < pt.y || (y == pt.y && z < pt.z)));
}

std::string ZPoint::toString(void) const
{
    std::ostringstream os;
    os << "x: " << x << ", " << "y: " << y << ", " << "z: " << z;
    return os.str();
}

bool Size::operator== (const Size & sz) const
{
    return (w == sz.w && h == sz.h);
}

bool Size::operator!= (const Size & sz) const
{
    return !(*this == sz);
}

Size & Size::operator+= (const Size & sz)
{
    w += sz.w;
    h += sz.h;

    return *this;
}

Size & Size::operator-= (const Size & sz)
{
    w -= sz.w;
    h -= sz.h;

    return *this;
}

Size Size::operator+ (const Size & sz) const
{
    return Size(w + sz.w, h + sz.h); 
}

Size Size::operator- (const Size & sz) const
{
    return Size(w - sz.w, h - sz.h);
}

Size Size::operator* (const Size & sz) const
{
    return Size(w * sz.w, h * sz.h); 
}

Size Size::operator/ (const Size & sz) const
{
    return Size(w / sz.w, h / sz.h);
}

Size Size::operator* (int val) const
{
    return Size(w * val, h * val); 
}

Size Size::operator/ (int val) const
{
    return Size(w / val, h / val);
}

bool Size::operator< (const Size & sz) const
{
    return w < sz.w || h < sz.h;
}

bool Size::isEmpty(void) const
{
    return 0 == w || 0 == h;
}

std::string Size::toString(void) const
{
    std::ostringstream os;
    os << "width: " << w << ", " << "height: " << h;
    return os.str();
}

Rect::Rect(int rx, int ry, int rw, int rh) : Point(rx, ry), Size(rw, rh)
{
}

Rect::Rect(const Point & pt, const Size & sz) : Point(pt), Size(sz)
{
}

Rect::Rect(const Point & pt1, const Point & pt2)
{
    x = pt1.x < pt2.x ? pt1.x : pt2.x;
    y = pt1.y < pt2.y ? pt1.y : pt2.y;
    w = (pt1.x < pt2.x ? pt2.x - pt1.x : pt1.x - pt2.x) + 1;
    h = (pt1.y < pt2.y ? pt2.y - pt1.y : pt1.y - pt2.y) + 1;
}

bool Rect::operator== (const Rect & rt) const
{
    return (x == rt.x && y == rt.y && w == rt.w && h == rt.h);
}

bool Rect::operator!= (const Rect & rt) const
{
    return !(*this == rt);
}

Rect Rect::operator+ (const Point & pt) const
{
    return Rect(x + pt.x, y + pt.y, w, h);
}

Rect Rect::operator- (const Point & pt) const
{
    return Rect(x - pt.x, y - pt.y, w, h);
}

Rect Rect::operator+ (const Size & sz) const
{
    return Rect(x, y, w + sz.w, h + sz.h);
}

Rect Rect::operator- (const Size & sz) const
{
    return Rect(x, y, w - sz.w, h - sz.h);
}

Rect Rect::operator+ (const Rect & rt) const
{
    return Rect(x + rt.y, y + rt.y, w + rt.w, h + rt.h);
}

Rect Rect::operator- (const Rect & rt) const
{
    return Rect(x - rt.x, y - rt.y, w - rt.w, h - rt.h);
}

bool Rect::operator& (const Point & pt) const
{
    return !(pt.x < x || pt.y < y || pt.x >= (x + w) || pt.y >= (y + h));
}

bool Rect::operator& (const Rect & rt) const
{
    return ! (x > rt.x + rt.w || x + w < rt.x || y > rt.y + rt.h || y + h < rt.y);
}

Point Rect::topLeft(void) const
{
    return Point(x, y);
}

Point Rect::topRight(void) const
{
    return Point(x + w - 1, y);
}

Point Rect::bottomLeft(void) const
{
    return Point(x, y + h - 1);
}

Point Rect::bottomRight(void) const
{
    return Point(x + w - 1, y + h - 1);
}

Rect Rect::intersection(const Rect & rt1, const Rect & rt2)
{
    Rect res;

    res.x = rt1.x <= rt2.x ? rt2.x : rt1.x;
    res.y = rt1.y <= rt2.y ? rt2.y : rt1.y;

    res.w = rt1.x + rt1.w <= rt2.x + rt2.w ? rt1.x + rt1.w - res.x : rt2.x + rt2.w - res.x;
    res.h = rt1.y + rt1.h <= rt2.y + rt2.h ? rt1.y + rt1.h - res.y : rt2.y + rt2.h - res.y;

    return res;
}

std::string Rect::toString(void) const
{
    std::ostringstream os;
    os << Point::toString() << ", " << Size::toString();
    return os.str();
}

void Rect::setPoint(const Point & pt)
{
    x = pt.x;
    y = pt.y;
}

void Rect::setSize(const Size & sz)
{
    w = sz.w;
    h = sz.h;
}

Rect Points::around(void) const
{
    Rect res;

    if(1 < size())
    {
	const Point & pt1 = at(0);
	const Point & pt2 = at(1);

	res = Rect(pt1, pt2);

	for(const_iterator
	    it = begin() + 2; it != end(); ++it)
	{
	    if((*it).x < res.x) res.x = (*it).x;
	    else
	    if((*it).x > res.x + res.w) res.w = (*it).x - res.x + 1;

	    if((*it).y < res.y) res.y = (*it).y;
	    else
	    if((*it).y > res.y + res.h) res.h = (*it).y - res.y + 1;
	}
    }

    return res;
}

Points & Points::operator<< (const Point & pt)
{
    push_back(pt);
    return *this;
}

Rect Rects::around(void) const
{
    Rect res;

    if(size())
    {
	const_iterator it = begin();
	res = *it;

	++it;

	for(; it != end(); ++it)
	{
	    const Rect & rt1 = *it;
	    const Rect & rt2 = res;
	    Rect & rt3 = res;

    	    rt3.x = rt1.x < rt2.x ? rt1.x : rt2.x;
	    rt3.y = rt1.y < rt2.y ? rt1.y : rt2.y;
	    rt3.w = rt1.x + rt1.w > rt2.x + rt2.w ? rt1.x + rt1.w - rt3.x : rt2.x + rt2.w - rt3.x;
    	    rt3.h = rt1.y + rt1.h > rt2.y + rt2.h ? rt1.y + rt1.h - rt3.y : rt2.y + rt2.h - rt3.y;
	}
    }

    return res;
}

Point Rect::toPoint(void) const
{
    return Point(x, y);
}

Size Rect::toSize(void) const
{
    return Size(w, h);
}

int Rects::index(const Point & pt) const
{
    for(auto it = begin(); it != end(); ++it)
	if(*it & pt) return std::distance(begin(), it);
    return -1;
}

Rects & Rects::operator<< (const Rect & rt)
{
    push_back(rt);
    return *this;
}

Rect::Rect(const SDL_Rect & rt) : Point(rt.x, rt.y), Size(rt.w, rt.h)
{
}

SDL_Rect Rect::toSDLRect(void) const
{
    SDL_Rect res;
    res.x = x; res.y = y; res.w = w; res.h = h;

    return res;
}

Point operator+ (const Point & pt, const Size & sz)
{
    return Point(pt.x + sz.w, pt.y + sz.h);
}

Point operator- (const Point & pt, const Size & sz)
{
    return Point(pt.x - sz.w, pt.y - sz.h);
}

Point operator* (const Point & pt, const Size & sz)
{
    return Point(pt.x * sz.w, pt.y * sz.h);
}

Point operator/ (const Point & pt, const Size & sz)
{
    return Point(pt.x / sz.w, pt.y / sz.h);
}

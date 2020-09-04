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

#include "swe_rect.h"
#include "swe_tools.h"

namespace SWE
{
    /* Point */
    Point::Point(std::initializer_list<int> args) : x(0), y(0)
    {
	auto it = std::begin(args);

	if(std::end(args) != it)
	{
    	    x = *it;
    	    it = std::next(it);
	}

	if(std::end(args) != it)
	{
    	    y = *it;
    	    //it = std::next(it);
	}
    }

    Point Point::parse(const std::string & str, int sep)
    {
        Point res;
        StringList list = String::split(str, sep);

        if(1 < list.size())
        {
            auto it = list.begin();
            res.x = String::toInt(*it);
            it++;
            res.y = String::toInt(*it);
        }

        return res;
    }

    bool Point::isNull(void) const
    {
        return 0 == x && 0 == y;
    }

    bool Point::inABC(const SWE::Point & a, const SWE::Point & b, const SWE::Point & c) const
    {
	int a1 = (a.x - x) * (b.y - a.y) - (b.x - a.x) * (a.y - y);
	int b1 = (b.x - x) * (c.y - b.y) - (c.x - b.x) * (b.y - y);
	int c1 = (c.x - x) * (a.y - c.y) - (a.x - c.x) * (c.y - y);

	return ((a1 >= 0 && b1 >= 0 && c1 >= 0) || (a1 < 0 && b1 < 0 && c1 < 0));
    }

    bool Point::operator== (const Point & pt) const
    {
        return x == pt.x && y == pt.y;
    }

    bool Point::operator!= (const Point & pt) const
    {
        return x != pt.x || y != pt.y;
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

    int Point::distance(const Point & pt1, const Point & pt2)
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

    /* ZPoint */
    ZPoint::ZPoint(std::initializer_list<int> args) : z(0)
    {
	auto it = std::begin(args);

	if(std::end(args) != it)
	{
    	    x = *it;
    	    it = std::next(it);
	}

	if(std::end(args) != it)
	{
    	    y = *it;
    	    it = std::next(it);
	}

	if(std::end(args) != it)
	{
    	    z = *it;
    	    //it = std::next(it);
	}
    }

    ZPoint ZPoint::parse(const std::string & str, int sep)
    {
        ZPoint res;
        StringList list = String::split(str, sep);

        if(2 < list.size())
        {
            auto it = list.begin();
            res.x = String::toInt(*it);
            it++;
            res.y = String::toInt(*it);
            it++;
            res.z = String::toInt(*it);
        }

        return res;
    }

    bool ZPoint::operator== (const ZPoint & pt) const
    {
        return x == pt.x && y == pt.y && z == pt.z;
    }

    bool ZPoint::operator!= (const ZPoint & pt) const
    {
        return x != pt.x || y != pt.y || z != pt.z;
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

    std::string ZPoint::toString(void) const
    {
        std::ostringstream os;
        os << "x: " << x << ", " << "y: " << y << ", " << "z: " << z;
        return os.str();
    }

    /* Size */
    Size::Size(std::initializer_list<int> args) : w(0), h(0)
    {
	auto it = std::begin(args);

	if(std::end(args) != it)
	{
    	    w = *it;
    	    it = std::next(it);
	}
    
	if(std::end(args) != it)
	{
    	    h = *it;
    	    //it = std::next(it);
	}
    }

    Size Size::parse(const std::string & str, int sep)
    {
        Size res;
        StringList list = String::split(str, sep);

        if(1 < list.size())
        {
            auto it = list.begin();
            res.w = String::toInt(*it);
            it++;
            res.h = String::toInt(*it);
        }

        return res;
    }

    bool Size::operator== (const Size & sz) const
    {
        return w == sz.w && h == sz.h;
    }

    bool Size::operator!= (const Size & sz) const
    {
        return w != sz.w || h != sz.h;
    }

    bool Size::operator<= (const Size & sz) const
    {
        return *this == sz || *this < sz;
    }

    bool Size::operator>= (const Size & sz) const
    {
        return *this == sz || *this > sz;
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

    bool Size::operator> (const Size & sz) const
    {
        return sz < *this;
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

    void fixedNegativeSize(Rect & rt)
    {
        if(rt.w < 0)
        {
            rt.x = rt.x + rt.w + 1;
            rt.w = std::abs(rt.w);
        }

        if(rt.h < 0)
        {
            rt.y = rt.y + rt.y  + 1;
            rt.h = std::abs(rt.h);
        }
    }

    /* Rect */
    Rect::Rect(int rx, int ry, int rw, int rh) : Point(rx, ry), Size(rw, rh)
    {
        fixedNegativeSize(*this);
    }

    Rect::Rect(const Point & pt, const Size & sz) : Point(pt), Size(sz)
    {
        fixedNegativeSize(*this);
    }

    Rect::Rect(const Point & pt1, const Point & pt2) : Point(pt1), Size(1, 1)
    {
        if(pt2.x < x)
        {
            w += x - pt2.x;
            x = pt2.x;
        }
        else if(pt2.x > x + w)
            w = pt2.x + 1 - x;

        if(pt2.y < y)
        {
            h += y - pt2.y;
            y = pt2.y;
        }
        else if(pt2.y > y + h)
            h = pt2.y + 1 - y;
    }

    Rect::Rect(std::initializer_list<int> args)
    {
	auto it = std::begin(args);

	if(std::end(args) != it)
	{
    	    x = *it;
    	    it = std::next(it);
	}

	if(std::end(args) != it)
	{
    	    y = *it;
    	    it = std::next(it);
	}

	if(std::end(args) != it)
	{
    	    w = *it;
    	    it = std::next(it);
	}
    
	if(std::end(args) != it)
	{
    	    h = *it;
    	    //it = std::next(it);
	}
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
	return contains(pt);
    }

    bool Rect::operator& (const Rect & rt) const
    {
	return intersects(rt);
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

    bool Rect::contains(const Point & pt) const
    {
        return !(pt.x < x || pt.y < y || pt.x >= (x + w) || pt.y >= (y + h));
    }


    bool Rect::contains(const Rect & rt) const
    {
	return contains(rt.topLeft()) && contains(rt.topRight()) &&
		    contains(rt.bottomLeft()) && contains(rt.bottomRight());
    }

    Rect Rect::intersected(const Rect & rt) const
    {
	Rect res;
	intersection(*this, rt, &res);
	return res;
    }

    bool Rect::intersects(const Rect & rt) const
    {
        return !(x >= rt.x + rt.w || x + w <= rt.x || y >= rt.y + rt.h || y + h <= rt.y);
    }

    // max
    Rect Rect::around(const Rect & rt1, const Rect & rt2)
    {
	Rect rt3;
        rt3.x = rt1.x < rt2.x ? rt1.x : rt2.x;
        rt3.y = rt1.y < rt2.y ? rt1.y : rt2.y;
        rt3.w = rt1.x + rt1.w > rt2.x + rt2.w ? rt1.x + rt1.w - rt3.x : rt2.x + rt2.w - rt3.x;
        rt3.h = rt1.y + rt1.h > rt2.y + rt2.h ? rt1.y + rt1.h - rt3.y : rt2.y + rt2.h - rt3.y;
	return rt3;
    }

    bool Rect::intersection(const Rect & rt1, const Rect & rt2, Rect* res)
    {
        if(rt1.intersects(rt2))
        {
            if(res)
            {
                res->x = rt1.x <= rt2.x ? rt2.x : rt1.x;
                res->y = rt1.y <= rt2.y ? rt2.y : rt1.y;
                res->w = rt1.x + rt1.w <= rt2.x + rt2.w ? rt1.x + rt1.w - res->x : rt2.x + rt2.w - res->x;
                res->h = rt1.y + rt1.h <= rt2.y + rt2.h ? rt1.y + rt1.h - res->y : rt2.y + rt2.h - res->y;
            }

            return true;
        }

        return false;
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

        if(size())
        {
            auto it = begin();
            res.setPoint(*it++);
            res.setSize(Size(1, 1));

            for(; it != end(); ++it)
            {
                const Point & pt = *it;

                if(pt.x < res.x)
                {
                    res.w += res.x - pt.x;
                    res.x = pt.x;
                }
                else if(pt.x > res.x + res.w)
                    res.w = pt.x + 1 - res.x;

                if(pt.y < res.y)
                {
                    res.h += res.y - pt.y;
                    res.y = pt.y;
                }
                else if(pt.y > res.y + res.h)
                    res.h = pt.y + 1 - res.y;
            }
        }

        return res;
    }


    Points & Points::push_back(const Point & pt)
    {
        std::vector<Point>::push_back(pt);
        return *this;
    }

    Points & Points::push_back(const Points & pts)
    {
        insert(end(), pts.begin(), pts.end());
        return *this;
    }

    Points & Points::operator<< (const Point & pt)
    {
        return push_back(pt);
    }

    Points & Points::operator<< (const Points & pts)
    {
        return push_back(pts);
    }

    std::string Points::toString(void) const
    {
	StringList res;
	for(auto & pt : *this)
	    res.push_back(SWE::StringFormat("[%1,%2]").arg(pt.x).arg(pt.y));
	return res.join(", ");
    }

    Rect Rects::around(void) const
    {
        Rect res;
        auto it = begin();
        res = *it++;

        for(; it != end(); ++it)
        {
            const Rect & rt = *it;

            if(rt.x < res.x)
            {
                res.w += res.x - rt.x;
                res.x = rt.x;
            }

            if(rt.x + rt.w > res.x + res.w)
                res.w = rt.x + rt.w - res.x;

            if(rt.y < res.y)
            {
                res.h += res.y - rt.y;
                res.y = rt.y;
            }

            if(rt.y + rt.h > res.y + res.h)
                res.h = rt.y + rt.h - res.y;
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

    Rects & Rects::push_back(const Rect & tt)
    {
        std::vector<Rect>::push_back(tt);
        return *this;
    }

    Rects & Rects::push_back(const Rects & rts)
    {
        insert(end(), rts.begin(), rts.end());
        return *this;
    }

    Rects & Rects::operator<< (const Rects & rts)
    {
        return push_back(rts);
    }

    Rects & Rects::operator<< (const Rect & rt)
    {
        return push_back(rt);
    }

    std::string Rects::toString(void) const
    {
	StringList res;
	for(auto & rt : *this)
	    res.push_back(SWE::StringFormat("[%1,%2,%3,%4]").arg(rt.x).arg(rt.y).arg(rt.w).arg(rt.h));
	return res.join(", ");
    }

    Rect::Rect(const SDL_Rect & rt) : Point(rt.x, rt.y), Size(rt.w, rt.h)
    {
    }

    SDL_Rect Rect::toSDLRect(void) const
    {
        SDL_Rect res;
        res.x = x;
        res.y = y;
        res.w = w;
        res.h = h;
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

    struct PointComp
    {
        bool operator()(const Point & lhs, const Point & rhs) const
        {
            return lhs.y < rhs.y || (lhs.y == rhs.y && (lhs.x < rhs.x));
        }
    };

    Polygon::Polygon(const Points & v)
    {
        for(auto it1 = v.begin(); it1 != v.end(); ++it1)
        {
            auto it2 = std::next(it1);
            if(it2 != v.end()) push_back(Tools::renderLine(*it1, *it2));
        }

        if(size() && back() != front())
            push_back(Tools::renderLine(v.back(), v.front()));

        std::sort(begin(), end(), PointComp());
        resize(std::distance(begin(), std::unique(begin(), end())));
    }

    bool Polygon::operator& (const Point & pt) const
    {
        Rect ar = around();

        if(ar & pt)
        {
            // check: move up
            bool intersection = false;

            for(int y = pt.y - 1; y >= ar.y; --y)
            {
                if(end() != std::find(begin(), end(), Point(pt.x, y)))
                {
                    intersection = true;
                    break;
                }
            }

            if(! intersection)
                return false;

            // check; move down
            intersection = false;

            for(int y = pt.y + 1; y <= ar.y + ar.h; ++y)
            {
                if(end() != std::find(begin(), end(), Point(pt.x, y)))
                {
                    intersection = true;
                    break;
                }
            }

            if(! intersection)
                return false;

            // check: move left
            intersection = false;

            for(int x = pt.x - 1; x >= ar.x; --x)
            {
                if(end() != std::find(begin(), end(), Point(x, pt.y)))
                {
                    intersection = true;
                    break;
                }
            }

            if(! intersection)
                return false;

            // check: move right
            intersection = false;

            for(int x = pt.x + 1; x <= ar.x + ar.w; ++x)
            {
                if(end() != std::find(begin(), end(), Point(x, pt.y)))
                {
                    intersection = true;
                    break;
                }
            }

            return intersection;
        }

        return false;
    }
}

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

#ifndef _SWE_RECT_
#define _SWE_RECT_

#include <string>

#include "sharedvector.h"
#include "types.h"

struct Size
{
    int w, h;

    Size() : w(0), h(0) {}
    Size(int sw, int sh) : w(sw), h(sh) {}

    bool	isEmpty(void) const;

    bool	operator== (const Size &) const;
    bool	operator!= (const Size &) const;
    bool	operator<= (const Size &) const;
    bool	operator>= (const Size &) const;

    Size &	operator+= (const Size &);
    Size &	operator-= (const Size &);

    Size	operator+ (const Size &) const;
    Size	operator- (const Size &) const;

    Size	operator* (const Size &) const;
    Size	operator/ (const Size &) const;
    Size	operator* (int) const;
    Size	operator/ (int) const;

    bool	operator< (const Size &) const;
    bool	operator> (const Size &) const;

    std::string	toString(void) const;
};

struct Point
{
    int x, y;

    Point() : x(0), y(0) {}
    Point(const Size & sz) : x(sz.w), y(sz.h) {}
    Point(int px, int py) : x(px), y(py) {}

    bool	isNull(void) const;

    bool	operator== (const Point &) const;
    bool	operator!= (const Point &) const;

    Point &	operator+= (const Point &);
    Point &	operator-= (const Point &);

    Point	operator+ (const Point &) const;
    Point	operator- (const Point &) const;

    Point	operator* (const Point &) const;
    Point	operator/ (const Point &) const;
    Point	operator* (int) const;
    Point	operator/ (int) const;

    Size	toSize(void) const { return Size(x, y); }
    std::string	toString(void) const;

    static int	distance(const Point &, const Point &);
};

struct ZPoint : public Point
{
    int z;

    ZPoint() : z(0) {}
    ZPoint(int px, int py, int pz) : Point(px, py), z(pz) {}
    ZPoint(const Point & pt, int pz) : Point(pt), z(pz) {}

    bool	operator== (const ZPoint &) const;
    bool	operator!= (const ZPoint &) const;

    ZPoint &	operator+= (const ZPoint &);
    ZPoint &	operator-= (const ZPoint &);

    ZPoint	operator+ (const ZPoint &) const;
    ZPoint	operator- (const ZPoint &) const;

    ZPoint	operator* (const ZPoint &) const;
    ZPoint	operator/ (const ZPoint &) const;
    ZPoint	operator* (int) const;
    ZPoint	operator/ (int) const;

    std::string	toString(void) const;
};

struct Rect : public Point, public Size
{
    Rect() {}
    Rect(int, int, int, int);
    Rect(const Point &, const Size &);
    Rect(const Point &, const Point &);
    Rect(const SDL_Rect &);

    bool	operator== (const Rect &) const;
    bool	operator!= (const Rect &) const;

    Rect	operator+ (const Point &) const;
    Rect	operator- (const Point &) const;
    Rect	operator+ (const Size &) const;
    Rect	operator- (const Size &) const;
    Rect	operator+ (const Rect &) const;
    Rect	operator- (const Rect &) const;

    Point	topLeft(void) const;
    Point	topRight(void) const;
    Point	bottomLeft(void) const;
    Point	bottomRight(void) const;

    void	setPoint(const Point &);
    void	setSize(const Size &);

    Point	toPoint(void) const;
    Size	toSize(void) const;

    SDL_Rect	toSDLRect(void) const;

    // Rect include Point
    bool	operator& (const Point &) const;
    // Rect intersects Rect
    bool	operator& (const Rect &) const;

    static bool	intersection(const Rect &, const Rect &, Rect* res = NULL);
    std::string	toString(void) const;
};

struct Points : std::vector<Point>
{
    Points() {}
    Points(const std::vector<Point> & v) : std::vector<Point>(v) {}
    Points(const Points & v) : std::vector<Point>(v) {}
    Points(Points && v) { swap(v); }

    Points & operator= (const Points & v) { assign(v.begin(), v.end()); return *this; }
    Points & operator= (Points && v) { swap(v); return *this; }

    Rect	around(void) const;

    Points &	push_back(const Point &);
    Points &	push_back(const Points &);

    Points &	operator<< (const Point &);
    Points &	operator<< (const Points &);
};

struct Rects : std::vector<Rect>
{
    Rects() {}
    Rects(const std::vector<Rect> & v) : std::vector<Rect>(v) {}
    Rects(const Rects & v) : std::vector<Rect>(v) {}
    Rects(Rects && v) { swap(v); }

    Rects & operator= (const Rects & v) { assign(v.begin(), v.end()); return *this; }
    Rects & operator= (Rects && v) { swap(v); return *this; }

    int 	index(const Point &) const;
    Rect	around(void) const;

    Rects &	push_back(const Rect &);
    Rects &	push_back(const Rects &);

    Rects &	operator<< (const Rect &);
    Rects &	operator<< (const Rects &);
};

struct Polygon : Points
{
    Polygon() {}
    Polygon(const Points &);

    bool	operator& (const Point &) const;
};

Point operator+ (const Point &, const Size &);
Point operator- (const Point &, const Size &);
Point operator* (const Point &, const Size &);
Point operator/ (const Point &, const Size &);

#endif

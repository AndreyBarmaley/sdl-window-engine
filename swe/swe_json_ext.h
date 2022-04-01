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

#ifndef _SWE_JSONWRAPPER_EXT_
#define _SWE_JSONWRAPPER_EXT_

#ifdef SWE_WITH_JSON

#include "swe_json.h"
#include "swe_colors.h"
#include "swe_cunicode_color.h"
#include "swe_cstring.h"
#include "swe_rect.h"

/// @brief пространство SWE
namespace SWE
{
    const JsonValue & operator>> (const JsonValue &, Point &);
    const JsonValue & operator>> (const JsonValue &, ZPoint &);
    const JsonValue & operator>> (const JsonValue &, Size &);
    const JsonValue & operator>> (const JsonValue &, Rect &);

    JsonArray & operator<< (JsonArray &, const Point &);
    JsonArray & operator<< (JsonArray &, const ZPoint &);
    JsonArray & operator<< (JsonArray &, const Size &);
    JsonArray & operator<< (JsonArray &, const Rect &);

    JsonObject & operator<< (JsonObject &, const Point &);
    JsonObject & operator<< (JsonObject &, const ZPoint &);
    JsonObject & operator<< (JsonObject &, const Size &);
    JsonObject & operator<< (JsonObject &, const Rect &);

    namespace JsonPack
    {
        JsonArray	point(const Point &);
        JsonArray	zpoint(const ZPoint &);
        JsonArray	size(const Size &);
        JsonArray	rect(const Rect &);

        JsonArray	points(const Points &);
        JsonArray	rects(const Rects &);
        JsonArray	stringList(const StringList &);
        JsonObject	fbColors(const FBColors &);

        template<typename T>
        JsonArray stdList(const std::list<T> & v)
        {
            JsonArray ja;

            for(auto it = v.begin(); it != v.end(); ++it)
                ja << *it;

            return ja;
        }

        template<typename T>
        JsonArray stdVector(const std::vector<T> & v)
        {
            JsonArray ja;

            for(auto it = v.begin(); it != v.end(); ++it)
                ja << *it;

            return ja;
        }
    }

    namespace JsonUnpack
    {
        Point	point(const JsonArray &);
        ZPoint	zpoint(const JsonArray &);
        Size	size(const JsonArray &);
        Rect	rect(const JsonArray &);

        Points	points(const JsonArray &);
        Rects	rects(const JsonArray &);

        Point	point(const JsonObject &, const std::string & = "point", const Point & def = Point());
        ZPoint	zpoint(const JsonObject &, const std::string & = "zpoint", const ZPoint & def = ZPoint());
        Size	size(const JsonObject &, const std::string & = "size", const Size & def = Size());
        Rect	rect(const JsonObject &, const std::string & = "rect", const Rect & def = Rect());
        Color	color(const JsonObject &, const std::string & = "color", const Color::color_t & def = Color::Transparent);

        FBColors fbColors(const JsonObject &, const std::string & = "colors", const FBColors & def = FBColors());
	UnicodeColor unicodeColor(const JsonObject &, const std::string & = "uc", const UnicodeColor & def = UnicodeColor());
    }

} // SWE
#endif
#endif

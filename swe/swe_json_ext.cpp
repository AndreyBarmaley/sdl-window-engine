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

#ifdef SWE_WITH_JSON

#include "swe_json_ext.h"

namespace SWE
{

    /* JsonArray */
    JsonArray & operator<< (JsonArray & ja, const Point & st)
    {
        ja.addArray(JsonPack::point(st));
        return ja;
    }

    JsonArray & operator<< (JsonArray & ja, const ZPoint & st)
    {
        ja.addArray(JsonPack::zpoint(st));
        return ja;
    }

    JsonArray & operator<< (JsonArray & ja, const Size & st)
    {
        ja.addArray(JsonPack::size(st));
        return ja;
    }

    JsonArray & operator<< (JsonArray & ja, const Rect & st)
    {
        ja.addArray(JsonPack::rect(st));
        return ja;
    }

    const JsonArray & operator>> (const JsonArray & ja, Point & st)
    {
        if(1 < ja.size())
        {
            st.x = ja.getInteger(0);
            st.y = ja.getInteger(1);
        }

        return ja;
    }

    const JsonArray & operator>> (const JsonArray & ja, ZPoint & st)
    {
        if(1 < ja.size())
        {
            st.x = ja.getInteger(0);
            st.y = ja.getInteger(1);

            if(2 < ja.size())
                st.y = ja.getInteger(2);
        }

        return ja;
    }

    const JsonArray & operator>> (const JsonArray & ja, Size & st)
    {
        if(1 < ja.size())
        {
            st.w = ja.getInteger(0);
            st.h = ja.getInteger(1);
        }

        return ja;
    }

    const JsonArray & operator>> (const JsonArray & ja, Rect & st)
    {
        if(3 < ja.size())
        {
            st.x = ja.getInteger(0);
            st.y = ja.getInteger(1);
            st.w = ja.getInteger(2);
            st.h = ja.getInteger(3);
        }

        return ja;
    }

    /* JsonObject */
    JsonObject & operator<< (JsonObject & jo, const Point & st)
    {
        JsonArray ja;
        jo.addArray("point", ja << st);
        return jo;
    }

    JsonObject & operator<< (JsonObject & jo, const ZPoint & st)
    {
        JsonArray ja;
        jo.addArray("zpoint", ja << st);
        return jo;
    }

    JsonObject & operator<< (JsonObject & jo, const Size & st)
    {
        JsonArray ja;
        jo.addArray("size", ja << st);
        return jo;
    }

    JsonObject & operator<< (JsonObject & jo, const Rect & st)
    {
        JsonArray ja;
        jo.addArray("rect", ja << st);
        return jo;
    }

    const JsonObject & operator>> (const JsonObject & jo, Point & st)
    {
        st = JsonUnpack::point(jo);
        return jo;
    }

    const JsonObject & operator>> (const JsonObject & jo, ZPoint & st)
    {
        st = JsonUnpack::zpoint(jo);
        return jo;
    }

    const JsonObject & operator>> (const JsonObject & jo, Size & st)
    {
        st = JsonUnpack::size(jo);
        return jo;
    }

    const JsonObject & operator>> (const JsonObject & jo, Rect & st)
    {
        st = JsonUnpack::rect(jo);
        return jo;
    }

    /* JsonPack */
    JsonArray JsonPack::point(const Point & st)
    {
        return JsonArray({st.x, st.y});
    }

    JsonArray JsonPack::zpoint(const ZPoint & st)
    {
        return JsonArray({st.x, st.y, st.z});
    }

    JsonArray JsonPack::size(const Size & st)
    {
        return JsonArray({st.w, st.h});
    }

    JsonArray JsonPack::rect(const Rect & st)
    {
        return JsonArray({st.x, st.y, st.w, st.h});
    }

    JsonArray JsonPack::points(const Points & st)
    {
        return stdVector<Point>(st);
    }

    JsonArray JsonPack::rects(const Rects & st)
    {
        return stdVector<Rect>(st);
    }

    JsonArray JsonPack::stringList(const StringList & v)
    {
        return stdList<std::string>(v);
    }

    JsonObject JsonPack::fbColors(const FBColors & v)
    {
        JsonObject jo;
        jo.addString("fg", v.fgcolor().toString());
        jo.addString("bg", v.bgcolor().toString());
        return jo;
    }

    /* JsonUnpack */
    Point JsonUnpack::point(const JsonArray & ja)
    {
        Point st;
        ja >> st;
        return st;
    }

    ZPoint JsonUnpack::zpoint(const JsonArray & ja)
    {
        ZPoint st;
        ja >> st;
        return st;
    }

    Size JsonUnpack::size(const JsonArray & ja)
    {
        Size st;
        ja >> st;
        return st;
    }

    Rect JsonUnpack::rect(const JsonArray & ja)
    {
        Rect st;
        ja >> st;
        return st;
    }

    Point JsonUnpack::point(const JsonObject & jo, const std::string & key, const Point & def)
    {
        if(jo.hasKey(key))
        {
            Point st;
            auto ja = jo.getArray(key);

            if(ja)
            {
                *ja >> st;
                return st;
            }
        }

        return def;
    }

    ZPoint JsonUnpack::zpoint(const JsonObject & jo, const std::string & key, const ZPoint & def)
    {
        if(jo.hasKey(key))
        {
            ZPoint st;
            auto ja = jo.getArray(key);

            if(ja)
            {
                *ja >> st;
                return st;
            }
        }

        return def;
    }

    Size JsonUnpack::size(const JsonObject & jo, const std::string & key, const Size & def)
    {
        if(jo.hasKey(key))
        {
            Size st;
            auto ja = jo.getArray(key);

            if(ja)
            {
                *ja >> st;
                return st;
            }
        }

        return def;
    }

    Rect JsonUnpack::rect(const JsonObject & jo, const std::string & key, const Rect & def)
    {
        if(jo.hasKey(key))
        {
            Rect st;
            auto ja = jo.getArray(key);

            if(ja)
            {
                *ja >> st;
                return st;
            }
        }

        return def;
    }

    Color JsonUnpack::color(const JsonObject & jo, const std::string & key, const Color::color_t & def)
    {
        if(jo.hasKey(key))
            return Color(jo.getString(key));

        return Color(def);
    }

    FBColors JsonUnpack::fbColors(const JsonObject & jo, const std::string & key, const FBColors & def)
    {
        auto jv = jo.getValue(key);
	if(jv)
	{
	    if(jv->isObject())
	    {
		auto jo2 = static_cast<const JsonObject*>(jv);
		auto fg = Color(jo2->getString("fg", "black")).toColorIndex();
		auto bg = Color(jo2->getString("bg", "transparent")).toColorIndex();
		return FBColors(fg, bg);
	    }

	    if(jv->isArray())
	    {
		auto ja = static_cast<const JsonArray*>(jv);
		auto fg = 0 < ja->size() ? Color(ja->getString(0)).toColorIndex() : SWE::Color::Black;
		auto bg = 1 < ja->size() ? Color(ja->getString(1)).toColorIndex() : SWE::Color::Transparent;
		return FBColors(fg, bg);
	    }

	    return FBColors(Color(jo.getString(key)).toColorIndex());
	}

	return def;
    }

    UnicodeColor JsonUnpack::unicodeColor(const JsonObject & jo, const std::string & key, const UnicodeColor & def)
    {
        if(jo.hasKey(key))
	    return UnicodeColor(jo.getInteger("symbol"), JsonUnpack::fbColors(jo, "colors"));

	return def;
    }

    Points JsonUnpack::points(const JsonArray & ja)
    {
        Points st;
        st.resize(ja.size());

        for(int index = 0; index < ja.size(); ++index)
        {
            auto ar = ja.getArray(index);

            if(ar) *ar >> st[index];
        }

        return st;
    }

    Rects JsonUnpack::rects(const JsonArray & ja)
    {
        Rects st;
        st.resize(ja.size());

        for(int index = 0; index < ja.size(); ++index)
        {
            auto ar = ja.getArray(index);

            if(ar) *ar >> st[index];
        }

        return st;
    }
}

#endif

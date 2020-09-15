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

#include <numeric>
#include <iterator>
#include <algorithm>

#include "swe_fontset.h"
#include "swe_cunicode_color.h"

namespace SWE
{

    /* UnicodeColor */
    UnicodeColor::UnicodeColor() : std::pair<FBColors, u16>(FBColors(Color::Black, Color::White), 0)
    {
    }

    UnicodeColor::UnicodeColor(int sym, const Color & cl) : std::pair<FBColors, u16>(FBColors(cl.toColorIndex()), sym)
    {
    }

    UnicodeColor::UnicodeColor(int sym, const FBColors & fbc) : std::pair<FBColors, u16>(FBColors(fbc), sym)
    {
    }

    bool UnicodeColor::isUnicode(int uc) const
    {
        return uc == second;
    }

    int  UnicodeColor::unicode(void) const
    {
        return second;
    }

    const FBColors & UnicodeColor::colors(void) const
    {
        return first;
    }

    int UnicodeColor::value(void) const
    {
        return (colors().value() << 16) | unicode();
    }

    ColorIndex UnicodeColor::fgindex(void) const
    {
        return ColorIndex(colors().fg());
    }

    ColorIndex UnicodeColor::bgindex(void) const
    {
        return ColorIndex(colors().bg());
    }

    Color UnicodeColor::fgcolor(void) const
    {
        return fgindex().toColor();
    }

    Color UnicodeColor::bgcolor(void) const
    {
        return bgindex().toColor();
    }

    void UnicodeColor::unicode(int sym)
    {
        second = sym;
    }

    void UnicodeColor::colors(const FBColors & fbc)
    {
        first = fbc;
    }

    void UnicodeColor::fgindex(const ColorIndex & col)
    {
        first.setfg(col);
    }

    void UnicodeColor::bgindex(const ColorIndex & col)
    {
        first.setbg(col);
    }

    bool UnicodeColor::operator< (const UnicodeColor & uc) const
    {
        return colors() < uc.colors() ||
               (colors() == uc.colors() && unicode() < uc.unicode());
    }

    bool UnicodeColor::operator> (const UnicodeColor & uc) const
    {
        return colors() > uc.colors() ||
               (colors() == uc.colors() && unicode() > uc.unicode());
    }

    bool UnicodeColor::operator== (const UnicodeColor & uc) const
    {
        return colors() == uc.colors() && unicode() == uc.unicode();
    }

    bool UnicodeColor::operator!= (const UnicodeColor & uc) const
    {
        return colors() != uc.colors() || unicode() != uc.unicode();
    }

    /* UCString */
    UCString::UCString(const UnicodeString & ustr, const ColorIndex & fg) : defcols(fg)
    {
        reserve(ustr.size());
        *this << ustr;
    }

    UCString::UCString(const UnicodeString & ustr, const FBColors & fbc) : defcols(fbc)
    {
        reserve(ustr.size());
        *this << ustr;
    }

    UCString::UCString(const_iterator it1, const_iterator it2) : std::vector<UnicodeColor>(it1, it2)
    {
    }

    UCString::UCString(const FBColors & fbc) : defcols(fbc)
    {
    }

    UCString::UCString(const std::vector<UnicodeColor> & v) : std::vector<UnicodeColor>(v)
    {
    }

    UCString::UCString(const UCString & v) : std::vector<UnicodeColor>(v)
    {
    }

    UCString::UCString(UCString && v) noexcept
    {
        swap(v);
    }

    UCString & UCString::operator= (const UCString & v)
    {
        std::vector<UnicodeColor>::assign(v.begin(), v.end());
        return *this;
    }

    UCString & UCString::operator= (UCString && v) noexcept
    {
        swap(v);
        return *this;
    }

    std::string UCString::toString(void) const
    {
        return toUnicodeString().toString();
    }

    UnicodeString UCString::toUnicodeString(void) const
    {
        UnicodeString res;
        res.reserve(size());

        for(const UnicodeColor & uc : *this)
            res.push_back(uc.unicode());

        return res;
    }

    UCString & UCString::operator<< (const FBColors & fbc)
    {
        defcols = fbc;
        return *this;
    }

    UCString & UCString::operator<< (const ColorIndex & col)
    {
        defcols.setfg(col);
        return *this;
    }

    UCString & UCString::operator<< (const UnicodeString & ustr)
    {
        for(const auto & uc : ustr)
            push_back(UnicodeColor(uc, defcols));

        return *this;
    }

    UCString & UCString::operator<< (const UnicodeColor & uc)
    {
        push_back(uc);
        return *this;
    }

    UCString & UCString::operator<< (const UCString & ucs)
    {
        append(ucs);
        return *this;
    }

    struct tagpos : std::pair<size_t, size_t>
    {
        tagpos() : std::pair<size_t, size_t>(std::string::npos, 0) {}
        tagpos(size_t pos, size_t len) : std::pair<size_t, size_t>(pos, len) {}

        const size_t & pos(void) const
        {
            return first;
        }
        const size_t & len(void) const
        {
            return second;
        }
    };

    // [default:color1:color2] set default fg,bg
    // [colors:color1:color2] set current colors fg, bg
    // [color:color1] set current color fg
    // parse tocken string: "test [color:red]red[color:default] default [color:yellow:red]yellow_red"
    UCString UCString::parseUnicode(const UnicodeString & us, FBColors defcols)
    {
        UCString ucs;
        ucs.reserve(us.size());
        ucs << defcols;
        int brackets = std::count(us.begin(), us.end(), '[');

        if(0 < brackets)
        {
            std::vector<tagpos> tags;
            tags.reserve(brackets);
            auto pos1 = us.begin();
            auto pos2 = us.end();

            while(us.end() != (pos2 = std::find(pos1, us.end(), '[')))
            {
                if(us.end() != (pos1 = std::find(pos2 + 1, us.end(), ']')))
                {
                    pos2 = pos1 - 1;

                    while(*pos2 != '[') pos2--;

                    tags.push_back(tagpos(std::distance(us.begin(), pos2), pos1 - pos2 + 1));
                    pos1++;
                }
            }

            size_t pos = 0;

            for(auto it = tags.begin(); it != tags.end(); ++it)
            {
                tagpos & tag = *it;
                UnicodeList tokens = us.substr(tag.pos() + 1, tag.len() - 2).split(':');

                if(pos < tag.pos())
                    ucs << us.substr(pos, tag.pos() - pos);

                if(1 < tokens.size())
                {
                    if(tokens.front() == "color" && 2 == tokens.size())
                    {
                        if(tokens.back() == "default")
                            ucs << defcols;
                        else
                            ucs << Color(tokens.back().toString()).toColorIndex();
                    }
                    else if(tokens.front() == "colors" && 3 == tokens.size())
                    {
                        tokens.pop_front();
                        const ColorIndex & fg = Color(tokens.front().toString()).toColorIndex();
                        const ColorIndex & bg = Color(tokens.back().toString()).toColorIndex();
                        ucs << FBColors(fg, bg);
                    }
                    else if(tokens.front() == "default" && 3 == tokens.size())
                    {
                        tokens.pop_front();
                        const ColorIndex & fg = Color(tokens.front().toString()).toColorIndex();
                        const ColorIndex & bg = Color(tokens.back().toString()).toColorIndex();
                        defcols = FBColors(fg, bg);
                        ucs << defcols;
                    }
                    else
                    {
                        ERROR("unknown token: " << tokens.front().toString());
                        ucs << us.substr(tag.pos(), tag.len());
                    }
                }
                else
                    ucs << us.substr(tag.pos(), tag.len());

                pos = tag.pos() + tag.len();
            }

            ucs << us.substr(pos);
        }
        else
            ucs << us;

        return ucs;
    }

    UCStringList UCString::split(int sep) const
    {
        UCStringList list;
        auto pos1 = begin();
        auto pos2 = end();

        while(static_cast<size_t>(std::distance(begin(), pos1)) < size() &&
              end() != (pos2 = std::find_if(pos1, end(), [&](const UnicodeColor & uc){ return uc.isUnicode(sep); })))
        {
            list.push_back(UCString(pos1, pos2));
            pos1 = pos2 + 1;
        }

        // tail
        if(static_cast<size_t>(std::distance(begin(), pos1)) < size())
            list.push_back(UCString(pos1, pos2));

        return list;
    }

    UCStringList UCString::splitWidth(const FontRender & frs, int width) const
    {
        UCStringList res;
        UCStringList list = split('\n');

        for(auto it = list.begin(); it != list.end(); ++it)
        {
            UCString & ucs = *it;
            Size sz = frs.unicodeSize(ucs.toUnicodeString());

            if(sz.w <= width)
                res << ucs;
            else
            {
                auto it1 = ucs.begin();
                auto it2 = it1 + 1;

                while(it2 != ucs.end())
                {
                    for(; it2 != ucs.end(); ++it2)
                    {
                        UCString substr = ucs.substr(std::distance(ucs.begin(), it1), std::distance(it1, it2));

                        if(substr.size())
                        {
                            sz = frs.unicodeSize(substr.toUnicodeString());

                            if(sz.w > width) break;
                        }
                    }

                    if(it2 == ucs.end())
                        res << ucs.substr(std::distance(ucs.begin(), it1), std::distance(it1, it2));
                    else
                    {
                        auto it3 = (*it2).unicode() != 0x20 ? it2 - 1 : it2;

                        while(it3 != it1 && (*it3).unicode() != 0x20) --it3;

                        if(it3 != it1) it2 = it3;

                        res << ucs.substr(std::distance(ucs.begin(), it1), std::distance(it1, it2));
                        it1 = (*it2).unicode() == 0x20 ? it2 + 1 : it2;
                        it2 = it1 != ucs.end() ? it1 + 1 : ucs.end();
                    }
                }
            }
        }

        return res;
    }

    int UCString::index(int ch) const
    {
        auto it = std::find_if(begin(), end(), [&](const UnicodeColor & uc){ return uc.isUnicode(ch); });
        return it != end() ? std::distance(begin(), it) : -1;
    }

    UCString UCString::substr(size_t pos, int len) const
    {
        UCString res;

        if(len && pos < size())
        {
            size_t pos2 = 0 < len && pos + len < size() ? len : size() - pos;
            res.resize(pos2, UnicodeColor());
            std::copy(begin() + pos, begin() + pos + pos2, res.begin());
        }

        return res;
    }

    UCStringList UCString::wrap(int width) const
    {
        const int sep = 0x20;
        size_t pos1 = 0;
        size_t pos2 = pos1 + width - 1;
        UCStringList list;

        while(pos1 < size())
        {
            if(pos2 + 1 < size())
            {
                for(; pos2 > pos1; --pos2)
                    if(operator[](pos2).isUnicode(sep)) break;

                list << substr(pos1, pos2 != pos1 ? pos2 - pos1 : width);
            }
            else
                list << substr(pos1);

            pos1 = pos2 + 1;
            pos2 = pos1 + width - 1;

            if(pos2 >= size()) pos2 = size() - 1;
        }

        return list;
    }

    UnicodeColor UCString::at(size_t pos) const
    {
        //UnicodeColor res;
        return pos < size() ? std::vector<UnicodeColor>::at(pos) : UnicodeColor();
    }

    size_t UCString::length(void) const
    {
        return std::vector<UnicodeColor>::size();
    }

    size_t UCString::size(void) const
    {
        return length();
    }

    void UCString::assign(const UnicodeString & us, const FBColors & fbc)
    {
        defcols = fbc;
        reserve(us.size());
        *this << us;
    }

    UCString & UCString::append(const UCString & src)
    {
        int sz = size();
        resize(size() + src.size(), UnicodeColor());
        std::copy(src.begin(), src.end(), begin() + sz);
        return *this;
    }

    /* UCStringList */
    UCStringList::UCStringList()
    {
    }

    UCStringList::UCStringList(const UnicodeList & ul, const FBColors & fbc)
    {
        append(ul, fbc);
    }

    UCStringList::UCStringList(const UnicodeList & ul, const ColorIndex & col)
    {
        append(ul, col);
    }

    UCStringList::UCStringList(const std::list<UCString> & v) : std::list<UCString>(v)
    {
    }

    UCStringList::UCStringList(const UCStringList & v) : std::list<UCString>(v)
    {
    }

    UCStringList::UCStringList(UCStringList && v) noexcept
    {
        swap(v);
    }

    UCStringList & UCStringList::operator= (const UCStringList & v)
    {
        assign(v.begin(), v.end());
        return *this;
    }

    UCStringList & UCStringList::operator= (UCStringList && v) noexcept
    {
        swap(v);
        return *this;
    }

    size_t UCStringList::maxStringWidth(void) const
    {
        if(empty()) return 0;

        auto it = std::max_element(begin(), end(),
			    [](auto & str1, auto & str2) { return str1.size() < str2.size(); });
        return it != end() ? (*it).size() : front().size();
    }

    size_t UCStringList::totalStringsWidth(void) const
    {
	return std::accumulate(begin(), end(), 0,
        	    [](size_t v, const UCString & str){ return v + str.size(); });
    }


    UCString UCStringList::join(void) const
    {
        UCString res;

        for(auto it = begin(); it != end(); ++it)
            res.append(*it);

        return res;
    }

    UCString UCStringList::join(const UCString & sep) const
    {
        UCString res;

        for(auto it = begin(); it != end(); ++it)
        {
            res.append(*it);
            auto next = it;

            if(++next != end())
                res.append(sep);
        }

        return res;
    }

    UCStringList & UCStringList::append(const UnicodeList & ul, const ColorIndex & col)
    {
        append(ul, FBColors(col));
        return *this;
    }

    UCStringList & UCStringList::append(const UnicodeList & ul, const FBColors & fbc)
    {
        for(auto it = ul.begin(); it != ul.end(); ++it)
            push_back(UCString(*it, fbc));

        return *this;
    }

    UCStringList & UCStringList::append(const UCString & v)
    {
        push_back(v);
        return *this;
    }
 
    UCStringList & UCStringList::append(const UCStringList & ucsl)
    {
        insert(end(), ucsl.begin(), ucsl.end());
        return *this;
    }

    UCStringList & UCStringList::operator<< (const UCString & ucs)
    {
        return append(ucs);
    }

    UCStringList & UCStringList::operator<< (const UCStringList & ucsl)
    {
        return append(ucsl);
    }
}

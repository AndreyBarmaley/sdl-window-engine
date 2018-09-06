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

#include <algorithm>
#include <iterator>

#include "fontset.h"
#include "cunicode_color.h"

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

std::string UCString::toString(void) const
{
    return toUnicodeString().toString();
}

UnicodeString UCString::toUnicodeString(void) const
{
    UnicodeString res;
    res.reserve(size());
    for(const_iterator it = begin(); it != end(); ++it)
	res.push_back((*it).unicode());
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
    for(auto it = ustr.begin(); it != ustr.end(); ++it)
	push_back(UnicodeColor(*it, defcols));
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

    const size_t & pos(void) const { return first; }
    const size_t & len(void) const { return second; }
};

// parse tocken string: "test [color:red]red[color:default] default [color:yellow:red]yellow_red"
UCString UCString::parseUnicode(const UnicodeString & us, const FBColors & defcols)
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
		    ucs << (tokens.back() == "default" ? defcols : FBColors(Color(tokens.back().toString()).toColorIndex()));
		}
		else
		if(tokens.front() == "colors")
		{
		    if(3 == tokens.size())
		    {
			tokens.pop_front();
			const Color & fgcol = Color(tokens.front().toString());
			const Color & bgcol = Color(tokens.back().toString());
			ucs << FBColors(fgcol.toColorIndex(), bgcol.toColorIndex());
		    }
		    else
		    {
			ucs << defcols;
		    }
		}
		else
		{
		    ERROR("unknown token: " << tokens.front().toString());
		    ucs << us.substr(tag.pos(), tag.len());
		}
	    }
	    else
	    {
		ucs << us.substr(tag.pos(), tag.len());
	    }

	    pos = tag.pos() + tag.len();
	}

	ucs << us.substr(pos);
    }
    else
    {
	ucs << us;
    }

    return ucs;
}

UCStringList UCString::split(int sep) const
{
    UCStringList list;
    auto pos1 = begin();
    auto pos2 = end();

    while(static_cast<size_t>(std::distance(begin(), pos1)) < size() &&
        end() != (pos2 = std::find_if(pos1, end(), std::bind2nd(std::mem_fun_ref(&UnicodeColor::isUnicode), sep))))
    {
        list.push_back(UCString(pos1, pos2));
        pos1 = pos2 + 1;
    }

    // tail
    if(static_cast<size_t>(std::distance(begin(), pos1)) < size())
    {
        list.push_back(UCString(pos1, pos2));
    }

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

UnicodeColor UCString::at(size_t pos) const
{
    //UnicodeColor res;
    return pos < size() ? SharedVector<UnicodeColor>::at(pos) : UnicodeColor();
}

size_t UCString::length(void) const
{
    return SharedVector<UnicodeColor>::size();
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
size_t UCStringList::maxStringWidth(void) const
{
    size_t res = 0;
    for(const_iterator it = begin(); it != end(); ++it)
        if((*it).size() > res) res = (*it).size();
    return res;
}

size_t UCStringList::totalStringsWidth(void) const
{
    size_t res = 0;
    for(const_iterator it = begin(); it != end(); ++it)
        res += (*it).size();
    return res;
}


UCString UCStringList::join(void) const
{
    UCString res;

    for(const_iterator
        it = begin(); it != end(); ++it)
        res.append(*it);

    return res;
}

UCString UCStringList::join(const UCString & sep) const
{
    UCString res;

    for(const_iterator
        it = begin(); it != end(); ++it)
    {
        res.append(*it);
        const_iterator next = it;

        if(++next != end())
            res.append(sep);
    }

    return res;
}

void UCStringList::append(const UnicodeList & ul, const ColorIndex & col)
{
    append(ul, FBColors(col));
}

void UCStringList::append(const UnicodeList & ul, const FBColors & fbc)
{
    for(auto it = ul.begin(); it != ul.end(); ++it)
        push_back(UCString(*it, fbc));
}

void UCStringList::append(const UCStringList & ucsl)
{
    insert(end(), ucsl.begin(), ucsl.end());
}

UCStringList & UCStringList::operator<< (const UCString & ucs)
{
    push_back(ucs);
    return *this;
}

UCStringList & UCStringList::operator<< (const UCStringList & ucsl)
{
    append(ucsl);
    return *this;
}

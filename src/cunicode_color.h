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

#ifndef _SWE_UNICODECOLOR_
#define _SWE_UNICODECOLOR_

#include <utility>

#include "colors.h"
#include "cunicode.h"

class UnicodeColor : std::pair<FBColors, u16> /* bg col 8 bit, fg col 8 bit, sym 16 bit */
{
public:
    UnicodeColor() : std::pair<FBColors, u16>(FBColors(Color::Black, Color::White), 0) {}
    UnicodeColor(int sym, const Color & cl) : std::pair<FBColors, u16>(FBColors(cl.toColorIndex()), sym) {}
    UnicodeColor(int sym, const FBColors & fbc) : std::pair<FBColors, u16>(FBColors(fbc), sym) {}

    bool		visible(void) const { return unicode() >= 0x20; }

    bool		operator< (const UnicodeColor & uc) const;
    bool		operator> (const UnicodeColor & uc) const;
    bool		operator== (const UnicodeColor & uc) const;
    bool		operator!= (const UnicodeColor & uc) const;

    bool 		isUnicode(int uc) const { return uc == second; }
    int 		unicode(void) const { return second; }
    const FBColors &	colors(void) const { return first; }
    int 		value(void) const { return (colors().value() << 16) | unicode(); }

    ColorIndex 		fgindex(void) const { return ColorIndex(colors().fg()); }
    ColorIndex 		bgindex(void) const { return ColorIndex(colors().bg()); }

    Color 		fgcolor(void) const { return fgindex().toColor(); }
    Color 		bgcolor(void) const { return bgindex().toColor(); }

    void		unicode(int sym) { second = sym; }
    void		fgindex(const ColorIndex & col) { first.setfg(col); }
    void		bgindex(const ColorIndex & col) { first.setbg(col); }
};

class UCStringList;
class FontRender;

class UCString : protected SharedVector<UnicodeColor>
{
    FBColors		defcols;

public:
    UCString(const FBColors & fbc = FBColors(Color::Black)) : defcols(fbc) {}
    UCString(const_iterator it1, const_iterator it2) : SharedVector<UnicodeColor>(it1, it2) {}

    UCString(const UnicodeString &, const FBColors & fbc = FBColors(Color::Black));
    UCString(const UnicodeString &, const ColorIndex &);

    UCString & operator<< (const FBColors &);
    UCString & operator<< (const ColorIndex &);
    UCString & operator<< (const UnicodeString &);
    UCString & operator<< (const UnicodeColor &);
    UCString & operator<< (const UCString &);

    void		assign(const UnicodeString &, const FBColors &);
    UCString &		append(const UCString &);
    UCStringList	split(int sep) const;
    UCStringList	splitWidth(const FontRender &, int width) const;
    UCString		substr(size_t pos, int len) const;
    UnicodeString	toUnicodeString(void) const;
    std::string 	toString(void) const;

    static UCString	parseUnicode(const UnicodeString &, const FBColors & def = FBColors());

    UnicodeColor	at(size_t) const;
    size_t		length(void) const;
    size_t		size(void) const { return length(); }
};

class UCStringList : public SharedList<UCString>
{
public:
    UCStringList() {}
    UCStringList(const SharedList<UCString> & list) : SharedList<UCString>(list) {}
    UCStringList(const UnicodeList & ul, const FBColors & fbc) { append(ul, fbc); }
    UCStringList(const UnicodeList & ul, const ColorIndex & col) { append(ul, col); }

    size_t              maxStringWidth(void) const;
    size_t              totalStringsWidth(void) const;

    UCString		join(void) const;
    UCString		join(const UCString &) const;

    void                append(const UnicodeList &, const FBColors &);
    void                append(const UnicodeList &, const ColorIndex &);
    void                append(const UCStringList &);

    UCStringList &	operator<< (const UCString &);
    UCStringList &	operator<< (const UCStringList &);
};

#endif

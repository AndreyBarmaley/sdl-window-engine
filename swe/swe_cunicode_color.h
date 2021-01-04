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

#ifndef _SWE_UNICODE_COLOR_
#define _SWE_UNICODE_COLOR_

#include <utility>

#include "swe_colors.h"
#include "swe_cunicode.h"

/// @brief пространство SWE
namespace SWE
{
    /// @brief класс цветного unicode символа
    class UnicodeColor : std::pair<FBColors, u16> /* bg col 8 bit, fg col 8 bit, sym 16 bit */
    {
    public:
        UnicodeColor();
        UnicodeColor(int sym, const Color & cl);
        UnicodeColor(int sym, const FBColors & fbc);

        bool		operator< (const UnicodeColor &) const;
        bool		operator> (const UnicodeColor &) const;
        bool		operator== (const UnicodeColor &) const;
        bool		operator!= (const UnicodeColor &) const;

        bool 		isUnicode(int uc) const;
        int 		unicode(void) const;
        const FBColors & colors(void) const;
        int 		value(void) const;

        ColorIndex 	fgindex(void) const;
        ColorIndex 	bgindex(void) const;
        Color 		fgcolor(void) const;
        Color 		bgcolor(void) const;

        void		unicode(int sym);
        void		colors(const FBColors &);
        void		fgindex(const ColorIndex &);
        void		bgindex(const ColorIndex &);
    };

    class UCStringList;
    class FontRender;

    /// @brief класс цветной unicode строки
    class UCString : protected std::vector<UnicodeColor>
    {
        FBColors		defcols;

    protected:
        UCString(const_iterator it1, const_iterator it2);

    public:
        UCString(const FBColors & fbc = FBColors(Color::Black));
        UCString(const UnicodeString &, const FBColors & fbc = FBColors(Color::Black));
        UCString(const UnicodeString &, const ColorIndex &);
        UCString(const std::vector<UnicodeColor> & v);
        UCString(const UCString & v);
        UCString(UCString && v) noexcept;

        UCString &	operator= (const UCString & v);
        UCString &	operator= (UCString && v) noexcept;

        UCString &	operator<< (const FBColors &);
        UCString &	operator<< (const ColorIndex &);
        UCString &	operator<< (const UnicodeString &);
        UCString &	operator<< (const UnicodeColor &);
        UCString &	operator<< (const UCString &);

        void		assign(const UnicodeString &, const FBColors &);
        UCString &	append(const UCString &);
        UCStringList	split(int sep) const;
        UCStringList	splitWidth(const FontRender &, int width) const;
        UCStringList	wrap(int) const;

        int             index(int) const;
        UCString	substr(size_t pos, int len = -1) const;
        UnicodeString	toUnicodeString(void) const;
        std::string 	toString(void) const;

        static UCString	parseUnicode(const UnicodeString &, FBColors def = FBColors());

        UnicodeColor	at(size_t) const;
        size_t		length(void) const;
        size_t		size(void) const;
    };

    /// @brief класс список цветных unicode строк
    class UCStringList : public std::list<UCString>
    {
    public:
        UCStringList();
        UCStringList(const UnicodeList & ul, const FBColors & fbc);
        UCStringList(const UnicodeList & ul, const ColorIndex & col);
        UCStringList(const std::list<UCString> & v);
        UCStringList(const UCStringList & v);
        UCStringList(UCStringList && v) noexcept;

        UCStringList &	operator= (const UCStringList & v);
        UCStringList &	operator= (UCStringList && v) noexcept;

        size_t          maxStringWidth(void) const;
        size_t          totalStringsWidth(void) const;

        UCString	join(void) const;
        UCString	join(const UCString &) const;

        UCStringList &	append(const UCString &);
        UCStringList &	append(const UnicodeList &, const FBColors &);
        UCStringList &	append(const UnicodeList &, const ColorIndex &);
        UCStringList &	append(const UCStringList &);

        UCStringList &	operator<< (const UCString &);
        UCStringList &	operator<< (const UCStringList &);
    };

} // SWE
#endif

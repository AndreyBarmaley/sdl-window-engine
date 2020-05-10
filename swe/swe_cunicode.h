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

#ifndef _SWE_CUNICODE_
#define _SWE_CUNICODE_

#include <string>
#include <vector>
#include <list>

#include "swe_types.h"

namespace SWE
{

    class UnicodeList;
    class FontRender;

    class UnicodeString : public std::u16string
    {
    public:
        UnicodeString();
        UnicodeString(size_t len, int ch);
        UnicodeString(const std::u16string & v);
        UnicodeString(const UnicodeString & v);
        UnicodeString(const_iterator it1, const_iterator it2);
        UnicodeString(std::u16string && v) noexcept;
        UnicodeString(UnicodeString && v) noexcept;
        UnicodeString(const std::string & v);
        UnicodeString(const char* v);

        UnicodeString &	operator= (const UnicodeString & v);
        UnicodeString &	operator= (UnicodeString && v) noexcept;
        UnicodeString &	operator= (std::u16string && v) noexcept;

        bool		operator== (const std::string &) const;
        bool		operator!= (const std::string &) const;

        void		assign(const std::string &);
        void		assign(const_iterator it1, const_iterator it2);

        UnicodeString &	append(int);
        UnicodeString &	append(const UnicodeString &);
        UnicodeList	split(int sep) const;
        UnicodeList	splitWidth(const FontRender &, int width) const;
        UnicodeList	wrap(int) const;

        static std::list<UnicodeString>
        split(const UnicodeString & str, const UnicodeString & sep);


        int		index(int) const;
        UnicodeString	substr(size_t, int = -1) const;
        std::string	toString(void) const;
        std::string     toHexString(const std::string & sep = ", ", bool prefix = true) const;

        UnicodeString   ucFirst(void) const;
        UnicodeString	toLower(void) const;
        UnicodeString	toUpper(void) const;
        UnicodeString	firstLower(void) const;
        UnicodeString	firstUpper(void) const;
    };

    class UnicodeFormat : public UnicodeString
    {
        int		cur;

    public:
        UnicodeFormat(const char*);

        UnicodeFormat &	arg(const UnicodeString &);
        UnicodeFormat &	arg(const std::string &);
        UnicodeFormat &	arg(const char*);
        UnicodeFormat &	arg(int);
        UnicodeFormat &	arg(double, int prec);
    };

    class UnicodeList : public std::list<UnicodeString>
    {
    public:
        UnicodeList();
        UnicodeList(const StringList & v);
        UnicodeList(const std::list<UnicodeString> & v);
        UnicodeList(const UnicodeList & v);
        UnicodeList(UnicodeList && v) noexcept;

        UnicodeList &	operator= (const UnicodeList & v);
        UnicodeList &	operator= (UnicodeList && v) noexcept;

        size_t		maxStringWidth(void) const;
        size_t		totalStringsWidth(void) const;

        UnicodeString   join(void) const;
        UnicodeString   join(const UnicodeString &) const;

        UnicodeList &	append(const UnicodeString &);
        UnicodeList &	append(const StringList &);
        UnicodeList &	append(const UnicodeList &);

        UnicodeList &	operator<< (const UnicodeString &);
        UnicodeList &	operator<< (const StringList &);
        UnicodeList &	operator<< (const UnicodeList &);

        StringList	toStringList(void) const;
    };

} // SWE
#endif

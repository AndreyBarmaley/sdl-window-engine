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

#ifndef _SWE_CSTRING_
#define _SWE_CSTRING_

#include <list>
#include <string>
#include <functional>

#include "swe_types.h"

/// @brief пространство SWE
namespace SWE
{
    /// @brief класс списка строк
    struct StringList : std::list<std::string>
    {
        StringList();
        StringList(const std::list<std::string> &);
	StringList(const std::initializer_list<const char*> &);
        StringList(const StringList &);

        StringList(StringList &&) noexcept;
        StringList(std::list<std::string> &&) noexcept;

        StringList &	operator= (const StringList &);
        StringList &	operator= (StringList &&) noexcept;
        StringList &	operator= (std::list<std::string> &&) noexcept;

        size_t		maxStringWidth(void) const;
        size_t		totalStringsWidth(void) const;

        std::string     join(void) const;
        std::string     join(const std::string &) const;

        StringList &	append(const std::string &);
        StringList &	append(const StringList &);

        StringList &	operator<< (const std::string &);
        StringList &	operator<< (const StringList &);
    };

    namespace String
    {
	const char*	Bool(bool);
        std::string     ucFirst(std::string);
        std::string     toLower(std::string);
        std::string     toUpper(std::string);
        int             toInt(const std::string &, bool* = nullptr);
        long int        toLong(const std::string &, bool* = nullptr);
        double          toDouble(const std::string &, bool* = nullptr);
        int		index(const std::string &, int);

        std::string     trimmed(const std::string &);
        std::string	chomp(const std::string &);
        std::string     time(void);
        std::string     time(const time_t &);
        std::string	strftime(const std::string &);
        std::string	strftime(const std::string &, const time_t &);

        std::string	escaped(const std::string &, bool quote = false);
        std::string	unescaped(std::string);

        bool		compareInSensitive(const std::string &, const std::string &);

        std::string     replace(const std::string &, const char*, const std::string &);
        std::string     replace(const std::string &, const char*, int);

        StringList      split(const std::string &, int);
	StringList	split(const std::string & str, std::function<bool(int)>);
        StringList      split(const std::string & str, const std::string & sep);

        std::string     hex(u64 value, int width = 8);
        std::string     hex64(u64 value);
        std::string	pointer(const void*);

        std::string     number(int);
        std::string     number(double, int prec);

	const char*	sign(int);
    }

    /// @brief класс форматной строки
    class StringFormat : public std::string
    {
        int		cur;

    public:
        StringFormat(const std::string &, size_t reserver = 0);

        StringFormat &	arg(const std::string &);
        StringFormat &	arg(const char*);
        StringFormat &	arg(int);
        StringFormat &	arg(double, int prec);

        StringFormat &	replace(const char*, int);
        StringFormat &	replace(const char*, const std::string &);
        StringFormat &	replace(const char*, double, int prec);
    };

} // SWE
#endif

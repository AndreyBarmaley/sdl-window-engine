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

#include <cctype>
#include <ctime>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include "swe_tools.h"
#include "swe_systems.h"
#include "swe_fontset.h"
#include "swe_cunicode.h"

namespace SWE
{

    UnicodeString::UnicodeString()
    {
    }

    UnicodeString::UnicodeString(size_t len, int ch)
    {
        reserve(len + 1);
        std::u16string::assign(len, ch);
    }

    UnicodeString::UnicodeString(const std::u16string & v) : std::u16string(v)
    {
    }

    UnicodeString::UnicodeString(const UnicodeString & v) : std::u16string(v)
    {
    }

    UnicodeString::UnicodeString(const_iterator it1, const_iterator it2) : std::u16string(it1, it2)
    {
    }

    UnicodeString::UnicodeString(std::u16string && v) noexcept
    {
        swap(v);
    }

    UnicodeString::UnicodeString(UnicodeString && v) noexcept
    {
        swap(v);
    }

    UnicodeString::UnicodeString(const std::string & v)
    {
        assign(v);
    }

    UnicodeString::UnicodeString(const char* v)
    {
        if(v) assign(v);
    }

    UnicodeString & UnicodeString::operator= (const UnicodeString & v)
    {
        std::u16string::assign(v.begin(), v.end());
        return *this;
    }

    UnicodeString & UnicodeString::operator= (UnicodeString && v) noexcept
    {
        swap(v);
        return *this;
    }

    UnicodeString & UnicodeString::operator= (std::u16string && v) noexcept
    {
        swap(v);
        return *this;
    }

    void UnicodeString::assign(const std::string & utf8)
    {
        clear();
        reserve(utf8.size() + 1);

        // convert utf8 to utf16
        for(std::string::const_iterator
            it = utf8.begin(); it < utf8.end(); ++it)
        {
            u16 ch = static_cast<u8>(*it);

            if(ch >= 0xF0)
            {
                if(utf8.end() - it > 3)
                {
                    ch  =  static_cast<u16>(*it++ & 0x07) << 18;
                    ch |=  static_cast<u16>(*it++ & 0x3F) << 12;
                    ch |=  static_cast<u16>(*it++ & 0x3F) << 6;
                    ch |=  static_cast<u16>(*it & 0x3F);
                }
                else break;
            }
            else if(ch >= 0xE0)
            {
                if(utf8.end() - it > 2)
                {
                    ch  =  static_cast<u16>(*it++ & 0x0F) << 12;
                    ch |=  static_cast<u16>(*it++ & 0x3F) << 6;
                    ch |=  static_cast<u16>(*it & 0x3F);
                }
                else break;
            }
            else if(ch >= 0xC0)
            {
                if(utf8.end() - it > 1)
                {
                    ch  =  static_cast<u16>(*it++ & 0x1F) << 6;
                    ch |=  static_cast<u16>(*it & 0x3F);
                }
                else break;
            }

            switch(ch)
            {
                case '\n':
                    push_back(ch);
                    // ERROR("FIXME: " << "skip <endline> for string; " << utf8);
                    break;

                case '\t':
                    resize(size() + 4, 0x20);
                    break;

                default:
                    push_back(ch);
                    break;
            }
        }
    }

    void UnicodeString::assign(const_iterator it1, const_iterator it2)
    {
        std::u16string::assign(it1, it2);
    }

    int UnicodeString::index(int ch) const
    {
        const_iterator it = std::find(begin(), end(), ch);
        return it != end() ? std::distance(begin(), it) : -1;
    }

    UnicodeString & UnicodeString::append(int val)
    {
        push_back(val);
        return *this;
    }

    UnicodeString UnicodeString::ucFirst(void) const
    {
        UnicodeString res(size(), 0);

        if(size()) std::transform(begin(), begin() + 1, res.begin(), ::towupper);

        return res;
    }

    UnicodeString UnicodeString::toLower(void) const
    {
        UnicodeString res(size(), 0);
        std::transform(begin(), end(), res.begin(), ::towlower);
        return res;
    }

    UnicodeString UnicodeString::toUpper(void) const
    {
        UnicodeString res(size(), 0);
        std::transform(begin(), end(), res.begin(), ::towupper);
        return res;
    }

    UnicodeString UnicodeString::firstLower(void) const
    {
        UnicodeString res = *this;

        if(res.size()) res[0] = ::towlower(res[0]);

        return res;
    }

    UnicodeString UnicodeString::firstUpper(void) const
    {
        UnicodeString res = *this;

        if(res.size()) res[0] = ::towupper(res[0]);

        return res;
    }

    std::string UnicodeString::toString(void) const
    {
        std::string str;
        str.reserve(2 * size());

        // utf16 to utf8
        for(const_iterator it = begin(); it != end(); ++it)
        {
            if(*it < 128)
                str.append(1, static_cast<char>(*it));
            else if(*it < 2048)
            {
                str.append(1, static_cast<char>(192 + ((*it - (*it % 64)) / 64)));
                str.append(1, static_cast<char>(128 + (*it % 64)));
            }
            else
            {
                str.append(1, static_cast<char>(224 + ((*it - (*it % 4096)) / 4096)));
                str.append(1, static_cast<char>(128 + (((*it % 4096) - (*it % 64)) / 64)));
                str.append(1, static_cast<char>(128 + (*it % 64)));
            }
        }

        return str;
    }

    std::string UnicodeString::toHexString(const std::string & sep, bool prefix) const
    {
        if(size())
        {
            StringList list;

            for(const_iterator it = begin(); it != end(); ++it)
            {
                std::ostringstream os;

                if(prefix) os << "0x";

                os << std::setw(4) << std::setfill('0') << std::uppercase << std::hex << static_cast<int>(*it);
                list << os.str();
            }

            return list.join(sep);
        }

        return "";
    }

    UnicodeString & UnicodeString::append(const UnicodeString & src)
    {
        int sz = size();
        resize(size() + src.size(), 0);
        std::copy(src.begin(), src.end(), begin() + sz);
        return *this;
    }

    std::list<UnicodeString>
    UnicodeString::split(const UnicodeString & str, const UnicodeString & sep)
    {
        return Tools::AdvancedSplit<UnicodeString>(str, sep);
    }

    UnicodeList UnicodeString::split(int sep) const
    {
        UnicodeList list;
        const_iterator pos1 = begin();
        const_iterator pos2 = end();

        while(static_cast<size_t>(std::distance(begin(), pos1)) < size() &&
              end() != (pos2 = std::find(pos1, end(), sep)))
        {
            list << UnicodeString(pos1, pos2);
            pos1 = pos2 + 1;
        }

        // tail
        if(static_cast<size_t>(std::distance(begin(), pos1)) < size())
            list << UnicodeString(pos1, pos2);

        return list;
    }

    UnicodeList UnicodeString::splitWidth(const FontRender & frs, int width) const
    {
        UnicodeList res;
        UnicodeList list = split('\n');

        for(auto it = list.begin(); it != list.end(); ++it)
        {
            UnicodeString & ustr = *it;
            Size sz = frs.unicodeSize(ustr);

            if(sz.w <= width)
                res << ustr;
            else
            {
                auto it1 = ustr.begin();
                auto it2 = it1 + 1;

                while(it2 != ustr.end())
                {
                    for(; it2 != ustr.end(); ++it2)
                    {
                        UnicodeString substr = ustr.substr(std::distance(ustr.begin(), it1), std::distance(it1, it2));

                        if(substr.size())
                        {
                            sz = frs.unicodeSize(substr);

                            if(sz.w > width) break;
                        }
                    }

                    if(it2 == ustr.end())
                        res << ustr.substr(std::distance(ustr.begin(), it1), std::distance(it1, it2));
                    else
                    {
                        auto it3 = *it2 != 0x20 ? it2 - 1 : it2;

                        while(it3 != it1 && *it3 != 0x20) --it3;

                        if(it3 != it1) it2 = it3;

                        res << ustr.substr(std::distance(ustr.begin(), it1), std::distance(it1, it2));
                        it1 = *it2 == 0x20 ? it2 + 1 : it2;
                        it2 = it1 != ustr.end() ? it1 + 1 : ustr.end();
                    }
                }
            }
        }

        return res;
    }

    UnicodeList UnicodeString::wrap(int width) const
    {
        const int sep = 0x20;
        size_t pos1 = 0;
        size_t pos2 = pos1 + width - 1;
        UnicodeList list;

        while(pos1 < size())
        {
            if(pos2 + 1 < size())
            {
                for(; pos2 > pos1; --pos2)
                    if(operator[](pos2) == sep) break;

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

    UnicodeString UnicodeString::substr(size_t pos, int len) const
    {
        UnicodeString res;

        if(len && pos < size())
        {
            size_t pos2 = 0 < len && pos + len < size() ? len : size() - pos;
            res = UnicodeString(pos2, 0);
            std::copy(begin() + pos, begin() + pos + pos2, res.begin());
        }

        return res;
    }

    bool UnicodeString::operator== (const std::string & str) const
    {
        return std::equal(begin(), end(), str.begin());
    }

    bool UnicodeString::operator!= (const std::string & str) const
    {
        return ! std::equal(begin(), end(), str.begin());
    }

    /* UnicodeFormat */
    UnicodeFormat::UnicodeFormat(const char* str)
        : UnicodeString(str), cur(1)
    {
    }

    UnicodeFormat & UnicodeFormat::arg(const UnicodeString & val)
    {
        iterator it1 = begin();
        iterator it2 = end();

        while(true)
        {
            it1 = std::find(it1, end(), '%');

            if(it1 == end() || it1 + 1 == end())
            {
                cur++;
                return *this;
            }

            if(std::isdigit(*(it1 + 1)))
            {
                it2 = std::find_if(it1 + 1, end(), std::not1(std::ptr_fun<int, int>(std::isdigit)));
                int argc = String::toInt(substr(std::distance(begin(), it1 + 1), it2 - it1 - 1).toString());

                if(0 == argc)
                {
                    ERROR("template not found: " << "\"" << UnicodeString::toString() << "\"" << ", arg: " << val.toString());
                    return *this;
                }

                if(cur == argc) break;
            }

            it1++;
        }

        UnicodeString res;
        res.reserve((size() + val.size()) * 2);
        res.append(substr(0, std::distance(begin(), it1))).append(UnicodeString(val)).append(substr(std::distance(begin(), it2)));
        res.swap(*this);
        return arg(val);
    }

    UnicodeFormat & UnicodeFormat::arg(const std::string & val)
    {
        return arg(UnicodeString(val));
    }

    UnicodeFormat & UnicodeFormat::arg(const char* val)
    {
        return arg(UnicodeString(val ? val : "(null)"));
    }

    UnicodeFormat & UnicodeFormat::arg(int val)
    {
        return arg(String::number(val));
    }

    UnicodeFormat & UnicodeFormat::arg(double val, int prec)
    {
        return arg(String::number(val, prec));
    }

    /* UnicodeList */
    UnicodeList::UnicodeList()
    {
    }

    UnicodeList::UnicodeList(const StringList & v)
    {
        append(v);
    }

    UnicodeList::UnicodeList(const std::list<UnicodeString> & v) : std::list<UnicodeString>(v)
    {
    }

    UnicodeList::UnicodeList(const UnicodeList & v) : std::list<UnicodeString>(v)
    {
    }

    UnicodeList::UnicodeList(UnicodeList && v) noexcept
    {
        swap(v);
    }

    UnicodeList & UnicodeList::operator= (const UnicodeList & v)
    {
        assign(v.begin(), v.end());
        return *this;
    }

    UnicodeList & UnicodeList::operator= (UnicodeList && v) noexcept
    {
        swap(v);
        return *this;
    }

    size_t UnicodeList::maxStringWidth(void) const
    {
        size_t res = 0;

        for(const_iterator it = begin(); it != end(); ++it)
            if((*it).size() > res) res = (*it).size();

        return res;
    }

    size_t UnicodeList::totalStringsWidth(void) const
    {
        size_t res = 0;

        for(const_iterator it = begin(); it != end(); ++it)
            res += (*it).size();

        return res;
    }

    UnicodeString UnicodeList::join(void) const
    {
        UnicodeString res;

        for(const_iterator
            it = begin(); it != end(); ++it)
            res.append(*it);

        return res;
    }

    UnicodeString UnicodeList::join(const UnicodeString & sep) const
    {
        UnicodeString res;

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

    UnicodeList & UnicodeList::operator<< (const UnicodeString & us)
    {
        return append(us);
    }

    UnicodeList & UnicodeList::operator<< (const StringList & sl)
    {
        return append(sl);
    }

    UnicodeList & UnicodeList::operator<< (const UnicodeList & ul)
    {
        return append(ul);
    }

    UnicodeList & UnicodeList::append(const StringList & list)
    {
        for(StringList::const_iterator
            it = list.begin(); it != list.end(); ++it)
            push_back(*it);

        return *this;
    }

    UnicodeList & UnicodeList::append(const UnicodeList & list)
    {
        insert(end(), list.begin(), list.end());
        return *this;
    }

    UnicodeList & UnicodeList::append(const UnicodeString & v)
    {
        push_back(v);
        return *this;
    }

    StringList UnicodeList::toStringList(void) const
    {
        StringList res;

        for(auto it = begin(); it != end(); ++it)
            res << (*it).toString();

        return res;
    }
}

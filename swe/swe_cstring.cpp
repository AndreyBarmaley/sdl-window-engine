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

#include <ctime>
#include <cctype>
#include <cstring>
#include <climits>
#include <sstream>
#include <iomanip>
#include <numeric>
#include <algorithm>

#include "swe_tools.h"
#include "swe_systems.h"
#include "swe_cstring.h"

namespace SWE
{

    const char* String::Bool(bool f)
    {
	return f ? "true" : "false";
    }

    std::string String::escaped(const std::string & str, bool quote)
    {
        std::ostringstream os;

        // start quote
        if(quote)
            os << "\"";

        // variants: \\, \", \/, \t, \n, \r, \f, \b
        for(auto & ch : str)
        {
            switch(ch)
            {
                case '\\': os << "\\\\"; break;
                case '"':  os << "\\\""; break;
                case '/':  os << "\\/"; break;
                case '\t': os << "\\t"; break;
                case '\n': os << "\\n"; break;
                case '\r': os << "\\r"; break;
                case '\f': os << "\\f"; break;
                case '\b': os << "\\b"; break;
                default: os << ch; break;
            }
        }

        // end quote
        if(quote)
            os << "\"";

        return os.str();
    }

    std::string String::unescaped(std::string str)
    {
        if(str.size() < 2)
            return str;

        // variants: \\, \", \/, \t, \n, \r, \f, \b
        for(auto it = str.begin(); it != str.end(); ++it)
        {
            auto itn = std::next(it);
            if(itn == str.end()) break;

            if(*it == '\\')
            {
                switch(*itn)
                {
                    case '\\': str.erase(itn); break;
                    case '"': str.erase(itn); *it = '"'; break;
                    case '/': str.erase(itn); *it = '/'; break;
                    case 't': str.erase(itn); *it = '\t'; break;
                    case 'n': str.erase(itn); *it = '\n'; break;
                    case 'r': str.erase(itn); *it = '\r'; break;
                    case 'f': str.erase(itn); *it = '\f'; break;
                    case 'b': str.erase(itn); *it = '\b'; break;
                    default: break;
                }
            }
        }

        return str;
    }

    bool compareInSensChar(const char & c1, const char & c2)
    {
        return c1 != c2 ? (std::tolower(c1) == std::tolower(c2)) : true;
    }

    bool String::compareInSensitive(const std::string & str1, const std::string & str2)
    {
        return ((str1.size() == str2.size()) &&
                std::equal(str1.begin(), str1.end(), str2.begin(), & compareInSensChar));
    }

    std::string String::ucFirst(std::string str)
    {
        if(! str.empty())
            std::transform(str.begin(), str.begin() + 1, str.begin(), ::toupper);

        return str;
    }

    std::string String::toLower(std::string str)
    {
        if(! str.empty())
            std::transform(str.begin(), str.end(), str.begin(), ::tolower);

        return str;
    }

    std::string String::toUpper(std::string str)
    {
        if(! str.empty())
            std::transform(str.begin(), str.end(), str.begin(), ::toupper);

        return str;
    }

    std::string String::chomp(const std::string & str)
    {
        if(str.size())
        {
            auto pos = str.find_last_not_of("\n \t");

            if(pos != std::string::npos)
                return str.substr(0, pos + 1);
        }

        return str;
    }

    std::string String::trimmed(const std::string & str)
    {
        if(! str.empty())
        {
            auto it1 = str.begin();
            while(it1 != str.end() && std::isspace(*it1)) ++it1;

            auto it2 = str.end() - 1;
            while(it2 != str.begin() && std::isspace(*it2)) --it2;

            return std::string(it1, it2 + 1);
        }

        return str;
    }

    std::string String::hex(u64 value, int width)
    {
        std::ostringstream stream;
        stream << "0x" << std::setw(width) << std::setfill('0') << std::hex << value;
        return stream.str();
    }

    std::string String::hex64(u64 value)
    {
        return hex(value, 16);
    }

    const char* String::sign(int val)
    {
	if(val < 0) return "-";
	if(val > 0) return "+";
	return "";
    }

    std::string String::pointer(const void* ptr)
    {
        return ptr ? hex(reinterpret_cast<uintptr_t>(ptr), sizeof(ptr)) : "nullptr";
    }

    int String::toInt(const std::string & str, bool* ok)
    {
        long int res = toLong(str, ok);

        if(res > INT_MAX)
            ERROR("overload INT_MAX");

        return res;
    }

    long int String::toLong(const std::string & str, bool* ok)
    {
        long int res = 0;

        if(ok)
        {
            std::istringstream ss(str);

            // hex
            if(str.size() > 2 && str[0] == '0' && std::tolower(str[1]) == 'x' &&
               std::all_of(str.begin() + 2, str.end(), [](int ch){ return std::isxdigit(ch); }))
                ss >> std::hex;

            ss >> res;
            *ok = !ss.fail();
        }
        else
        {
#if defined(__MINGW32CE__)
            std::istringstream ss(str);
            ss >> res;
#else
            res = std::stol(str, nullptr, 0);
#endif
        }

        return res;
    }

    double String::toDouble(const std::string & str, bool* ok)
    {
        double res = 0;

        if(ok)
        {
            std::istringstream ss(str);
            ss >> res;
            *ok = !ss.fail();
        }
        else
        {
#if defined(__MINGW32CE__)
            std::istringstream ss(str);
            ss >> res;
#else
            res = std::stod(str, nullptr);
#endif
        }

        return res;
    }

    int String::index(const std::string & str, int ch)
    {
        size_t pos = str.find(ch);
        return pos != std::string::npos ? pos : -1;
    }

    std::string String::time(void)
    {
        time_t raw;
        std::time(&raw);
        return String::time(raw);
    }

    std::string String::time(const time_t & raw)
    {
	return strftime("%X", raw);
    }

    std::string String::strftime(const std::string & format)
    {
        time_t raw;
        std::time(& raw);
	return strftime(format, raw);
    }

    std::string String::strftime(const std::string & format, const time_t & raw)
    {
	const size_t len = format.size() < 32 ? 64 : format.size() * 2;
	std::unique_ptr<char[]> buf(new char[len]);
        struct tm* timeinfo = std::localtime(&raw);
        std::strftime(buf.get(), len - 1, format.c_str(), timeinfo);
        return std::string(buf.get());
    }

    std::string String::number(int value)
    {
#if defined(__MINGW32CE__)
        std::ostringstream os;
        os << value;
        return os.str();
#else
        return std::to_string(value);
#endif
    }

    std::string String::number(double value, int prec)
    {
        if(prec)
        {
            std::ostringstream os;
            os << std::fixed << std::setprecision(prec) << value;
            return os.str();
        }

#if defined(__MINGW32CE__)
        return number(value, 8);
#else
        return std::to_string(value);
#endif
    }

    std::string String::replace(const std::string & src, const char* pred, const std::string & val)
    {
        std::string res = src;
        size_t pos = std::string::npos;

        while(std::string::npos != (pos = res.find(pred))) res.replace(pos, std::strlen(pred), val);

        return res;
    }

    std::string String::replace(const std::string & src, const char* pred, int val)
    {
        return replace(src, pred, number(val));
    }

    StringList String::split(const std::string & str, const std::string & sep)
    {
        return Tools::AdvancedSplit<std::string>(str, sep);
    }

    StringList String::split(const std::string & str, std::function<bool(int)> fn)
    {
        StringList list;
        auto pos1 = str.begin();
        auto pos2 = str.end();

        while(pos1 != str.end())
	{
	    pos2 = std::find_if(pos1, str.end(), fn);
	    if(pos2 == str.end()) break;

            list << str.substr(std::distance(str.begin(), pos1), std::distance(pos1, pos2));
            pos1 = std::next(pos2);
        }

        // tail
        if(pos1 != str.end())
            list << str.substr(std::distance(str.begin(), pos1), std::distance(pos1, str.end()));

        return list;
    }

    StringList String::split(const std::string & str, int sep)
    {
	return split(str, [=](int ch){ return ch == sep; });
    }

    /* StringList */
    StringList::StringList()
    {
    }

    StringList::StringList(const std::list<std::string> & v) : std::list<std::string>(v)
    {
    }

    StringList::StringList(const StringList & v) : std::list<std::string>(v)
    {
    }

    StringList::StringList(const std::initializer_list<const char*> & list)
    {
        assign(list.begin(), list.end());
    }

    StringList::StringList(std::list<std::string> && v) noexcept
    {
        swap(v);
    }

    StringList::StringList(StringList && v) noexcept
    {
        swap(v);
    }

    StringList & StringList::operator= (const StringList & v)
    {
        assign(v.begin(), v.end());
        return *this;
    }

    StringList & StringList::operator= (StringList && v) noexcept
    {
        swap(v);
        return *this;
    }

    StringList & StringList::operator= (std::list<std::string> && v) noexcept
    {
        swap(v);
        return *this;
    }

    /* StringFormat */
    StringFormat::StringFormat(const std::string & str, size_t reserver)
        : cur(1)
    {
        if(reserver) reserve(reserver);
        append(str);
    }

    StringFormat & StringFormat::arg(const char* val)
    {
        return arg(val ? std::string(val) : std::string("(null)"));
    }

    StringFormat & StringFormat::arg(const std::string & val)
    {
        auto it1 = begin();
        auto it2 = end();

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
                it2 = std::find_if(it1 + 1, end(), [](int ch){ return ! std::isdigit(ch); });
                int argc = String::toInt(substr(std::distance(begin(), it1 + 1), it2 - it1 - 1));

                if(0 == argc)
                {
                    ERROR("template not found: " << "\"" << *this << "\"" << ", arg: " << val);
                    return *this;
                }

                if(cur == argc) break;
            }

            it1++;
        }

        std::string res;
        res.reserve((size() + val.size()) * 2);
        res.append(substr(0, std::distance(begin(), it1))).append(val).append(substr(std::distance(begin(), it2)));
        std::swap(*this, res);
        return arg(val);
    }

    StringFormat & StringFormat::arg(int val)
    {
        return arg(String::number(val));
    }

    StringFormat & StringFormat::arg(double val, int prec)
    {
        return arg(String::number(val, prec));
    }

    StringFormat & StringFormat::replace(const char* id, const std::string & val)
    {
	std::string str = String::replace(*this, id, val);
	std::swap(*this, str);
	return *this;
    }

    StringFormat & StringFormat::replace(const char* id, int val)
    {
        return replace(id, String::number(val));
    }

    StringFormat & StringFormat::replace(const char* id, double val, int prec)
    {
        return replace(id, String::number(val, prec));
    }

    StringList & StringList::operator<< (const std::string & str)
    {
        return append(str);
    }

    StringList & StringList::operator<< (const StringList & sl)
    {
        return append(sl);
    }

    size_t StringList::maxStringWidth(void) const
    {
	if(empty()) return 0;

	auto it = std::max_element(begin(), end(),
			    [](auto & str1, auto & str2) { return str1.size() < str2.size(); });
	return it != end() ? (*it).size() : front().size();
    }

    size_t StringList::totalStringsWidth(void) const
    {
        return std::accumulate(begin(), end(), 0,
                    [](size_t v, const std::string & str){ return v + str.size(); });
    }

    StringList & StringList::append(const std::string & val)
    {
        push_back(val);
        return *this;
    }

    StringList & StringList::append(const StringList & list)
    {
        insert(end(), list.begin(), list.end());
        return *this;
    }

    std::string StringList::join(void) const
    {
        std::ostringstream os;
	std::copy(begin(), end(), std::ostream_iterator<std::string>(os));
        return os.str();
    }

    std::string StringList::join(const std::string & sep) const
    {
        std::ostringstream os;

        for(auto it = begin(); it != end(); ++it)
        {
            os << *it;
            if(std::next(it) != end())
                os << sep;
        }

        return os.str();
    }

}

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
#include <climits>

#include "tools.h"
#include "systems.h"
#include "cstring.h"

std::string String::escapeChar(const std::string & str, int ch)
{
    if(str.empty()) return str;

    StringList list = String::split(str, ch);
    list.remove("");

    return list.join(std::string(1, '\\').append(1, ch));
}

bool compareInSensChar(const char & c1, const char & c2)
{
    return c1 != c2 ? (std::tolower(c1) == std::tolower(c2)) : true;
}
 
bool String::compareInSensitive(const std::string & str1, const std::string &str2)
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

std::string String::pointer(const void* ptr)
{
    return hex(reinterpret_cast<uintptr_t>(ptr), sizeof(ptr));
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
	    str.end() == std::find_if(str.begin() + 2, str.end(), std::not1(std::ptr_fun<int, int>(std::isxdigit))))
    	ss >> std::hex;

	ss >> res;
	*ok = !ss.fail();
    }
    else
	res = std::stol(str, NULL, 0);

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
	res = std::stod(str, NULL);

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

std::string String::time(time_t raw)
{

    char buf [13] = { 0 };
    struct tm* tmi = std::localtime(&raw);
    std::strftime(buf, sizeof(buf) - 1, "%X", tmi);

    return std::string(buf);
}

std::string String::strftime(const std::string & format)
{
    time_t raw;
    std::time(& raw);
    char buf[PATH_MAX];

    struct tm* timeinfo = std::localtime(&raw);
    std::memset(buf, 0, sizeof(buf));
    std::strftime(buf, sizeof(buf) - 1, format.c_str(), timeinfo);

    return std::string(buf);
}

std::string String::number(int value)
{
    return std::to_string(value);
}

std::string String::number(double value, int prec)
{
    if(prec)
    {
	std::ostringstream os;
	os << std::fixed << std::setprecision(prec) << value;
	return os.str();
    }

    return std::to_string(value);
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

std::list<std::string>
split(const std::string & str, const std::string & sep) 
{
    return Tools::AdvancedSplit<std::string>(str, sep);
}

StringList String::split(const std::string & str, int sep)
{
    StringList list;
    size_t pos1 = 0;
    size_t pos2 = std::string::npos;

    while(pos1 < str.size() &&
	std::string::npos != (pos2 = str.find(sep, pos1)))
    {
        list << str.substr(pos1, pos2 - pos1);
        pos1 = pos2 + 1;
    }

    // tail
    if(pos1 < str.size())
        list << str.substr(pos1, str.size() - pos1);

    return list;
}

StringFormat::StringFormat(const char* str, size_t reserver)
    : cur(1)
{
    if(reserver) reserve(reserver);
    if(str) append(str);
}

StringFormat & StringFormat::arg(const char* val)
{
    return arg(val ? std::string(val) : std::string("(null)"));
}

StringFormat & StringFormat::arg(const std::string & val)
{
    iterator it1 = begin();
    iterator it2 = end();

    while(true)
    {
	it1 = std::find(it1, end(), '%');
	if(it1 == end() || it1 + 1 == end()){ cur++; return *this; }

	if(std::isdigit(*(it1 + 1)))
	{
    	    it2 = std::find_if(it1 + 1, end(), std::not1(std::ptr_fun<int, int>(std::isdigit)));
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
    size_t res = 0;
    for(const_iterator it = begin(); it != end(); ++it)
	if((*it).size() > res) res = (*it).size();
    return res;
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

    for(const_iterator it = begin(); it != end(); ++it)
        os << *it;

    return os.str();
}

std::string StringList::join(const std::string & sep) const
{
    std::ostringstream os;

    for(const_iterator
        it = begin(); it != end(); ++it)
    {
        os << *it;
        const_iterator next = it;

        if(++next != end())
            os << sep;
    }

    return os.str();
}


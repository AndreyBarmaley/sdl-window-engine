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

#ifndef _SWE_SYSTEM_
#define _SWE_SYSTEM_

#include <ostream>
#include <string>
#include <vector>

class LogWrapper
{
protected:
    static std::ostream* os;
    static std::string   id;

public:
    LogWrapper();
    ~LogWrapper();

    static	void init(const std::string &);

    template<class T>
    LogWrapper & operator<< (const T & t)
    {
        if(os) *os << t;
        return *this;
    }
};

std::string shortPrettyName(const std::string &);

#define COUT(x) LogWrapper() << x << "\n";
#define PRETTY(x, y) COUT(String::time() << ": [" << x << "]\t" << shortPrettyName(__PRETTY_FUNCTION__) << ": " << y)

#define VERBOSE(x) PRETTY("VERBOSE", x)
#define ERROR(x) PRETTY("ERROR", x)
#define FIXME(x) PRETTY("FIXME", x)

#ifdef BUILD_DEBUG
#define DEBUG(x) PRETTY("DEBUG", x)
#else
#define DEBUG(x)   ;
#endif

#include "binarybuf.h"
#include "cstring.h"

namespace Systems
{
    int		setEnvironment(const char* name, const char* value);
    const char*	environment(const char*);

#ifdef WITH_DLL
    void*	openLib(const std::string &);
    void	closeLib(void*);
    void*	procAddressLib(void*, const std::string &);
    const char* suffixLib(void);
#endif

    bool	makeDirectory(const std::string &);
    std::string	concatePath(const std::string &, const std::string &);
    std::string	concatePath(const std::string &, const std::string &, const std::string &);
    std::string	concatePath2(std::string, const char*);
    StringList  readDir(const std::string &, const std::string & = "", bool sensitive = false);

    std::string dirname(const std::string &);
    std::string basename(const std::string &);

    std::string nulFile(void);
    std::string homeDirectory(const std::string &);
    StringList	shareDirectories(const std::string &);

    bool	isFile(const std::string &, bool writable = false);
    bool	isDirectory(const std::string &, bool writable = false);
    int		remove(const std::string &);

    void	setLocale(int, const char*);
    std::string messageLocale(int length /* 1, 2, 3 */);

    bool	saveFile(const BinaryBuf &, const std::string &);
    BinaryBuf	readFile(const std::string &);

    bool	saveString2File(const std::string &, const std::string &);
    bool	readFile2String(const std::string &, std::string &);

    int		GetCommandOptions(int argc, char* const argv[], const char* optstring);
    char*	GetOptionsArgument(void);

    bool	isEmbeded(void);
}

#endif

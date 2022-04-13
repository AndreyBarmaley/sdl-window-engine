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

#include <string>
#include <vector>
#include <sstream>
#include <utility>

#if defined(__MINGW32CE__)
#include <locale.h>
#else
#include <clocale>
#endif

#include "swe_binarybuf.h"
#include "swe_cstring.h"

/// @brief пространство SWE
namespace SWE
{
    class LogWrapper
    {
    protected:
        std::ostringstream os;

    public:
        LogWrapper(){}
        ~LogWrapper();

        static void init(const std::string &, const char* = nullptr);

/*
        template<typename Value>
        LogWrapper & cout(Value&& val)
        {
            if(os) *os << std::forward<Value>(val);
            return *this;
        }

        template<typename Value, typename... Values>
        LogWrapper & cout(Value&& val, Values&&... vals)
        {
            if(os) *os << std::forward<Value>(val);
            return cout(std::forward<Values>(vals)...);
        }

        template<typename... Values>
        LogWrapper & cout(Values&&... vals)
        {
            return cout(std::forward<Values>(vals)...);
        }
*/

        template<typename Value>
        LogWrapper & operator<< (const Value & val)
        {
            os << val;
            return *this;
        }
    };

    namespace Engine
    {
        int	debugMode(void);
    }

    std::string shortPrettyName(const std::string &);


    namespace Systems
    {
        int		setEnvironment(const char* name, const char* value);
        const char*	environment(const char*);

#ifdef SWE_DLOPEN
        void*		openLib(const std::string &);
        bool		closeLib(void*);
        void*		procAddressLib(void*, const std::string &);
        const char* 	suffixLib(void);
#endif

        bool		makeDirectory(const std::string &, int mode = 0);
        std::string	concatePath(const std::string &, const std::string &);
        std::string	concatePath(const std::string &, const std::string &, const std::string &);
        std::string	concatePath(const StringList &);
        StringList  	findFiles(const std::string &, const std::string & = "", bool sensitive = false);
        StringList  	readDir(const std::string &, bool fullpath = true);

        std::string 	dirname(const std::string &);
        std::string 	basename(const std::string &);

        std::string 	nulFile(void);
        std::string 	homeDirectory(const std::string &);
        StringList	shareDirectories(const std::string &);

        void		assetsInit(void);
        const StringList &
    			assetsList(void);

        bool		isFile(const std::string &, bool writable = false);
        bool		isDirectory(const std::string &, bool writable = false);
        int		remove(const std::string &);

        void		setLocale(int, const char*);
        std::string 	messageLocale(int length /* 1, 2, 3 */);

        bool		saveFile(const BinaryBuf &, const std::string &, int offset = 0 /* -1 end */);
        BinaryBuf	readFile(const std::string &, size_t offset = 0, size_t size = 0);

        bool		saveString2File(const std::string &, const std::string &);
        bool		readFile2String(const std::string &, std::string &);

        int		GetCommandOptions(int argc, char* const argv[], const char* optstring);
        char*		GetOptionsArgument(void);

        bool		isEmbeded(void);
        size_t		memoryUsage(void);
    }

} // SWE

#define COUT(x) SWE::LogWrapper() << x << "\n";

#if defined(SWE_DEBUG_MESSAGES) || defined(SWE_DEBUG)
 #if defined(__WIN32__) || defined(__MINGW32__)
  #undef SWE_LOG_COLORED
 #endif
 #if defined(SWE_LOG_COLORED)
  // colored console
  #define PRETTY3(x, y, c) COUT(SWE::String::time() << ": " << x << "\t\x1B[35m" << SWE::shortPrettyName(__PRETTY_FUNCTION__) << ": " << c << y << "\033[0m")
  #define PRETTY(x, y) PRETTY3(x, y, "\x1B[36m")
  #define VERBOSE(x) { PRETTY("\x1B[31m[VERBOSE]", x); }
  #define ERROR(x)   { PRETTY3("\x1B[93m[ERROR]", x, "\x1B[93m"); }
  #define FIXME(x)   { PRETTY("\x1B[32m[FIXME]", x); }
  #define DEBUG(x)   { if(SWE::Engine::debugMode()) PRETTY("\x1B[34m[DEBUG]", x); }
  #define DEBUGN(x,n){ if(n <= SWE::Engine::debugMode()) PRETTY("\x1B[34m[DEBUG]", x); }
 #else
  #define PRETTY(x, y) COUT(SWE::String::time() << ": " << x << "\t" << SWE::shortPrettyName(__PRETTY_FUNCTION__) << ": " << y)
  #define VERBOSE(x) { PRETTY("[VERBOSE]", x); }
  #define ERROR(x)   { PRETTY("[ERROR]", x); }
  #define FIXME(x)   { PRETTY("[FIXME]", x); }
  #define DEBUG(x)   { if(SWE::Engine::debugMode()) PRETTY("[DEBUG]", x); }
  #define DEBUGN(x,n){ if(n <= SWE::Engine::debugMode()) PRETTY("[DEBUG]", x); }
 #endif
#else
 #define PRETTY(x, y) COUT(SWE::String::time() << ": " << x << "\t" << SWE::shortPrettyName(__PRETTY_FUNCTION__) << ": " << y)
 #define VERBOSE(x) { PRETTY("[VERBOSE]", x); }
 #define ERROR(x)   { PRETTY("[ERROR]", x); }
 #define FIXME(x)   { PRETTY("[FIXME]", x); }
 #define DEBUG(x)   {}
 #define DEBUGN(x)  {}
#endif

#endif

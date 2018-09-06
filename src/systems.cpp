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

#include <locale>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cctype>
#include <cstdlib>
#include <clocale>

#if defined(__MINGW32CE__) || defined(__MINGW32__) || defined(__WIN32__) || defined(__WIN64__)
#include <windows.h>
#include <shellapi.h>
#undef ERROR
#else
#include <dlfcn.h>
#endif

#if ! defined(__MINGW32CE__)
#include <unistd.h>
#endif

#include "systems.h"

#if defined(__MINGW32CE__)
#undef Shell_NotifyIcon
extern "C" {
    BOOL WINAPI Shell_NotifyIcon(DWORD, PNOTIFYICONDATAW);
}

extern HICON screen_icn;
extern HINSTANCE SDL_Instance;
extern HWND SDL_Window;
#endif

#if defined(__WIN32__)
#include <io.h>
#endif

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#if defined(__SYMBIAN32__)
#define SEPARATOR '\\'
#elif defined(__WIN32__)
#define SEPARATOR '\\'
#else
#define SEPARATOR '/'
#endif

std::ostream* LogWrapper::os = NULL;
std::string   LogWrapper::id;

#if defined(__SYMBIAN32__)
LogWrapper::init(const std::string & app){}
LogWrapper::LogWrapper(){}
LogWrapper::~LogWrapper(){}
#elif defined(ANDROID)
#include <android/log.h>
namespace
{
    static std::ostringstream oslog;
}

void LogWrapper::init(const std::string & app)
{
    id = app;
}

LogWrapper::LogWrapper()
{
    oslog.str("");
    os = & oslog;
}

LogWrapper::~LogWrapper()
{
    __android_log_print(ANDROID_LOG_INFO, id.c_str(), "%s", oslog.str().c_str());
}
#elif defined(__MINGW32__)
namespace
{
    static std::ofstream osfile;
}

void LogWrapper::init(const std::string & app)
{
    id = std::string(app).append(".log");
    osfile.open(id.c_str(), std::fstream::app);
}

LogWrapper::LogWrapper()
{
    if(osfile.is_open())
	os = & osfile;
}

LogWrapper::~LogWrapper(){}
#else
void LogWrapper::init(const std::string & app)
{
    id = app;
}

LogWrapper::LogWrapper()
{
    os = & std::cerr;
}

LogWrapper::~LogWrapper(){}
#endif


int Systems::setEnvironment(const char* name, const char* value)
{
#ifdef OLDENGINE
    std::string str(name);
    str.append("=").append(value);
    return SDL_putenv(const_cast<char *>(str.c_str()));
#else
    return SDL_setenv(name, value, 1);
#endif
}

const char* Systems::environment(const char* name)
{
    return SDL_getenv(name);
}

bool Systems::makeDirectory(const std::string & path)
{
    if(Systems::isDirectory(path))
	return true;

    std::string root = Systems::dirname(path);

    // make recursive
    if(! Systems::isDirectory(root))
	makeDirectory(root);

    int ret = 0;

#if defined(__SYMBIAN32__)
    ret = mkdir(path.c_str(), S_IRWXU);
#elif defined(__WIN32__)
    ret = mkdir(path.c_str());
#else
    ret = mkdir(path.c_str(), S_IRWXU);
#endif

    if(ret != 0)
	ERROR("errno: " << ret);

    return ret == 0;
}

std::string Systems::concatePath(const std::string & str1, const std::string & str2)
{
    if(! str1.empty() && str1[str1.size() - 1] == SEPARATOR)
	return std::string(str1).append(str2);

    return std::string(str1).append(1, SEPARATOR).append(str2);
}

std::string Systems::concatePath(const std::string & str1, const std::string & str2, const std::string & str3)
{
    return concatePath(concatePath(str1, str2), str3);
}

std::string Systems::concatePath2(std::string str, const char* ptr)
{
    return concatePath(str, ptr);
}

std::string Systems::dirname(const std::string & str)
{
    if(str.size())
    {
        size_t pos = str.rfind(SEPARATOR);

        if(std::string::npos == pos)
            return std::string(".");
        else if(pos == 0)
            return std::string("./");
        else if(pos == str.size() - 1)
            return dirname(str.substr(0, str.size() - 1));
        else
            return str.substr(0, pos);
    }

    return str;
}

std::string Systems::basename(const std::string & str)
{
    if(str.size())
    {
        size_t pos = str.rfind(SEPARATOR);

        if(std::string::npos == pos ||
           pos == 0) return str;
        else if(pos == str.size() - 1)
            return basename(str.substr(0, str.size() - 1));
        else
            return str.substr(pos + 1);
    }

    return str;
}

bool Systems::isFile(const std::string & name, bool writable)
{
#if defined(ANDROID)
    return 0 == access(name.c_str(), writable ? W_OK : R_OK);
#else
    struct stat fs;

    if(stat(name.c_str(), &fs) || !S_ISREG(fs.st_mode))
        return false;

    return 0 == access(name.c_str(), writable ? W_OK : R_OK);
#endif
}

bool Systems::isDirectory(const std::string & name, bool writable)
{
#if defined (ANDROID)
    return 0 == access(name.c_str(), writable ? W_OK : R_OK);
#else
    struct stat fs;

    if(stat(name.c_str(), &fs) || !S_ISDIR(fs.st_mode))
        return false;

    return 0 == access(name.c_str(), writable ? W_OK : R_OK);
#endif
}

int Systems::remove(const std::string & file)
{
    if(isDirectory(file))
        return -1;

    return unlink(file.c_str());
}

bool Systems::saveString2File(const std::string & str, const std::string & file)
{
    SDL_RWops* rw = SDL_RWFromFile(file.c_str(), "wb");

    if(rw && SDL_RWwrite(rw, str.c_str(), str.size(), 1) == 1)
    {
        SDL_RWclose(rw);
        return true;
    }

    ERROR(SDL_GetError());
    return false;
}

bool Systems::saveFile(const BinaryBuf & raw, const std::string & file)
{
    SDL_RWops* rw = SDL_RWFromFile(file.c_str(), "wb");

    if(rw && SDL_RWwrite(rw, raw.data(), raw.size(), 1) == 1)
    {
        SDL_RWclose(rw);
        return true;
    }

    ERROR(SDL_GetError());
    return false;
}

bool Systems::readFile2String(const std::string & file, std::string & res)
{
    SDL_RWops* rw = SDL_RWFromFile(file.c_str(), "rb");

    if(rw && SDL_RWseek(rw, 0, RW_SEEK_END) != -1)
    {
	res.clear();
        res.resize(SDL_RWtell(rw));
        SDL_RWseek(rw, 0, RW_SEEK_SET);
        SDL_RWread(rw, & res[0], res.size(), 1);
        SDL_RWclose(rw);

	return true;
    }
    else
	ERROR(SDL_GetError());

    return false;
}

BinaryBuf Systems::readFile(const std::string & file)
{
    BinaryBuf buf;
    SDL_RWops* rw = SDL_RWFromFile(file.c_str(), "rb");

    if(rw && SDL_RWseek(rw, 0, RW_SEEK_END) != -1)
    {
        buf.resize(SDL_RWtell(rw));
        SDL_RWseek(rw, 0, RW_SEEK_SET);
        SDL_RWread(rw, buf.data(), buf.size(), 1);
        SDL_RWclose(rw);
    }
    else
	ERROR(SDL_GetError());

    return buf;
}

bool Systems::isEmbeded(void)
{
#if defined(__MINGW32CE__) || defined(ANDROID) || defined(__SYMBIAN32__)
    return true;
#endif
    return false;
}

void Systems::setLocale(int category, const char* locale)
{
#if defined(ANDROID)
    setlocale(category, locale);
#else
    std::setlocale(category, locale);
#endif
}

std::string Systems::nulFile(void)
{
#if defined(__MINGW32CE__) || defined(__MINGW32__) || defined(__WIN32__) || defined(__WIN64__)
    return "nul";
#else
    return "/dev/null";
#endif
}

std::string Systems::messageLocale(int length /* 1, 2, 3 */)
{
    std::string locname;
#if defined(__MINGW32CE__) || defined(__MINGW32__)
    char* clocale = std::setlocale(LC_MONETARY, NULL);
#elif defined(ANDROID)
    char* clocale = setlocale(LC_MESSAGES, NULL);
#else
    char* clocale = std::setlocale(LC_MESSAGES, NULL);
#endif

    if(clocale)
    {
        locname = String::toLower(clocale);
        // 3: en_us.utf-8
        // 2: en_us
        // 1: en
        if(length < 3)
        {
            StringList list = String::split(locname, length < 2 ? '_' : '.');
            return list.empty() ? locname : list.front();
        }
    }
    return locname;
}

std::string Systems::homeDirectory(const std::string & prog)
{
    std::string res;
#ifndef OLDENGINE
    char* path = SDL_GetPrefPath("", prog.c_str());

    if(path)
    {
	res = path;
	SDL_free(path);
    }
    else
#endif
    if(Systems::environment("HOME"))
        res = Systems::concatePath(Systems::environment("HOME"), std::string(".").append(prog));
    else
    if(Systems::environment("APPDATA"))
        res = Systems::concatePath(Systems::environment("APPDATA"), prog);

    return res;
}

StringList Systems::shareDirectories(const std::string & prog)
{
    StringList dirs;

#if defined(ANDROID)
    const char* internal = SDL_AndroidGetInternalStoragePath();
    if(internal) dirs.push_back(Systems::concatePath(internal, prog));

    if(SDL_ANDROID_EXTERNAL_STORAGE_WRITE & SDL_AndroidGetExternalStorageState())
    {
        const char* external = SDL_AndroidGetExternalStoragePath();
        if(external) dirs.push_back(Systems::concatePath(external, prog));
    }

    const char* dir1 = "/storage/sdcard0";
    if(isDirectory(dir1, true))
	dirs.push_back(Systems::concatePath(dir1, prog));

    const char* dir2 = "/storage/sdcard1";
    if(isDirectory(dir2, true))
	dirs.push_back(Systems::concatePath(dir2, prog));
#else
    dirs.push_back(homeDirectory(prog));
#endif

    return dirs;
}

bool findFilterContent(const std::string & content, const std::string & filter, bool sensitive)
{
    if(sensitive)
    {
    	if(std::string::npos == content.find(filter))
	    return false;
    }
    else
    {
    	if(std::string::npos == String::toLower(content).find(String::toLower(filter)))
	    return false;
    }

    return true;
}

StringList Systems::readDir(const std::string & path, const std::string & filter, bool sensitive)
{
    StringList res;

    DIR* dp = opendir(path.c_str());
    if(! dp)
    {
	ERROR("can't read directory: " << path);
	return res;
    }

    struct dirent* ep;
    while(NULL != (ep = readdir(dp)))
    {
        const std::string fullname = Systems::concatePath(path, ep->d_name);

        if(Systems::isDirectory(fullname))
	{
	    // skip /. and /..
	    if(ep->d_name[0] != '.' || (ep->d_name[1] && ep->d_name[1] != '.'))
		res << Systems::readDir(fullname, filter, sensitive);
        }
	else
	if(Systems::isFile(fullname))
	{
    	    if(filter.empty() ||
		findFilterContent(ep->d_name, filter, sensitive))
        	res.push_back(fullname);
	}
    }

    closedir(dp);
    return res;
}

int Systems::GetCommandOptions(int argc, char* const argv[], const char* optstring)
{
#if defined(__MINGW32CE__)
    return -1;
#else
    return getopt(argc, argv, optstring);
#endif
}

char* Systems::GetOptionsArgument(void)
{
#if defined(__MINGW32CE__)
    return NULL;
#else
    return optarg;
#endif
}

#include "cstring.h"
std::string shortPrettyName(const std::string & name)
{
    size_t end = name.rfind('(');
    if(end == std::string::npos) end = name.size();
    size_t begin = 0;

    for(size_t it = end; it; --it)
    {
	if(name[it] == 0x20)
	{
	    begin = it + 1;
	    break;
	}
    }

    return name.substr(begin, end - begin);
}

#ifdef WITH_DLL
void* Systems::openLib(const std::string & file)
{
#if defined(__WIN32__) || defined(__WIN64__)
    return LoadLibrary(file.c_str());
#else
    return dlopen(file.c_str(), RTLD_LAZY);
#endif
}

void Systems::closeLib(void* lib)
{
#if defined(__WIN32__) || defined(__WIN64__)
    FreeLibrary((HINSTANCE) lib);
#else
    dlclose(lib);
#endif
}

void* Systems::procAddressLib(void* lib, const std::string & func)
{
#if defined(__WIN32__) || defined(__WIN64__)
    return GetProcAddress((HINSTANCE) lib, func.c_str());
#else
    return dlsym(lib, func.c_str());
#endif
}

const char* Systems::suffixLib(void)
{
#if defined(__WIN32__) || defined(__WIN64__)
    return ".dll";
#else
    return ".so";
#endif
}
#endif // WITH_DLL

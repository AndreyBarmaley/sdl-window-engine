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
#include <algorithm>

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

#include "swe_streamfile.h"
#include "swe_systems.h"

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

namespace SWE
{

    std::string SEPARATOR(void)
    {
#if defined(__SYMBIAN32__)
        return "\\";
#elif defined(__MINGW32CE__)
        return "/";
#elif defined(__WIN32__)
        return "\\\\";
#endif
        return "/";
    }

    std::ostream* LogWrapper::os = nullptr;
    std::string   LogWrapper::id;

#if defined(__SYMBIAN32__)
    LogWrapper::init(const std::string & app, const char* arg0) {}
    LogWrapper::LogWrapper() {}
    LogWrapper::~LogWrapper() {}
#elif defined(ANDROID)
#include <android/log.h>
    namespace
    {
        static std::ostringstream oslog;
    }

    void LogWrapper::init(const std::string & app, const char* arg0)
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

    void LogWrapper::init(const std::string & app, const char* arg0)
    {
        id = arg0 ?
             Systems::concatePath(Systems::dirname(arg0), app).append(".txt") :
             std::string(app).append(".txt");
        osfile.open(id.c_str(), std::fstream::app);

        if(! osfile.is_open())
            std::cerr << "error open file: " << id << std::endl;
    }

    LogWrapper::LogWrapper()
    {
        if(osfile.is_open())
            os = & osfile;
        else
            os = & std::clog;
    }

    LogWrapper::~LogWrapper()
    {
        if(os) os->flush();
    }
#else
    void LogWrapper::init(const std::string & app, const char* arg0)
    {
        id = app;
    }

    LogWrapper::LogWrapper()
    {
        os = & std::clog;
    }

    LogWrapper::~LogWrapper() {}
#endif

    int Systems::setEnvironment(const char* name, const char* value)
    {
#ifdef SWE_SDL12
        std::string str(name);
        str.append("=").append(value);
        return SDL_putenv(const_cast<char*>(str.c_str()));
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

    std::string Systems::concatePath(const StringList & list)
    {
        std::string path;

        for(auto it = list.begin(); it != list.end(); ++it)
            path = concatePath(path, *it);

        return path;
    }

    std::string Systems::concatePath(const std::string & str1, const std::string & str2)
    {
        std::string sep = SEPARATOR();

        if(str1.empty())
            return str2;

        if(str2.empty())
            return str1;

        if(str1.size() >= sep.size() &&
           str1.substr(str1.size() - sep.size()) == sep)
            return std::string(str1).append(str2);

        return std::string(str1).append(sep).append(str2);
    }

    std::string Systems::concatePath(const std::string & str1, const std::string & str2, const std::string & str3)
    {
        return concatePath(concatePath(str1, str2), str3);
    }

    std::string dirname2(const std::string & str)
    {
        if(str.size())
        {
            std::string sep = SEPARATOR();
            size_t pos = str.rfind(sep);

            if(std::string::npos == pos)
#if defined(__MINGW32CE__)
                return "/";

#elif defined(__MINGW32__)
                return str;
#else
                return std::string(".");
#endif
            else if(pos == 0)
                return sep;
            else if(pos == str.size() - sep.size())
                return dirname2(str.substr(0, str.size() - sep.size()));

            return str.substr(0, pos);
        }

        return str;
    }

    std::string Systems::dirname(const std::string & str)
    {
#if defined(__MINGW32CE__)
        return dirname2(String::replace(str, "\\", "/"));
#elif defined(__MINGW32__)
        return dirname2(String::escapeChar(str, '\\'));
#endif
        return dirname2(str);
    }

    std::string Systems::basename(const std::string & str)
    {
        if(str.size())
        {
            std::string sep = SEPARATOR();
            size_t pos = str.rfind(sep);

            if(std::string::npos == pos)
                return str;
            else if(pos == str.size() - sep.size())
                return basename(str.substr(0, str.size() - sep.size()));
            else
                return str.substr(pos + sep.size());
        }

        return str;
    }

    namespace
    {
        StringList assetsRes;
    }

    void Systems::assetsInit(void)
    {
        const char* list = "assets.list";
        std::string str;
        Systems::readFile2String(list, str);

        if(str.size())
        {
            assetsRes << String::split(str, 0x0A);
            DEBUG("items: " << assetsRes.size());
        }
    }

    const StringList & Systems::assetsList(void)
    {
        return assetsRes;
    }

    bool Systems::isFile(const std::string & name, bool writable)
    {
#if defined(ANDROID)

        if(assetsRes.end() != std::find(assetsRes.begin(), assetsRes.end(), name))
            return true;

#endif
        struct stat fs;

        if(stat(name.c_str(), &fs) || !S_ISREG(fs.st_mode))
            return false;

        return 0 == access(name.c_str(), writable ? W_OK : R_OK);
    }

    bool Systems::isDirectory(const std::string & name, bool writable)
    {
#if defined(__MINGW32__)

        if(name.back() == ':')
            return isDirectory(std::string(name).append(SEPARATOR()));

#endif
        struct stat fs;

        if(stat(name.c_str(), &fs) || !S_ISDIR(fs.st_mode))
            return false;

        return 0 == access(name.c_str(), writable ? W_OK : R_OK);
    }

    int Systems::remove(const std::string & file)
    {
        if(isDirectory(file))
            return -1;

        return unlink(file.c_str());
    }

    bool Systems::saveFile(const BinaryBuf & raw, const std::string & file, int offset /* default: 0, endpos : -1 */)
    {
        SDL_RWops* rw = nullptr;

        if(0 < offset)
        {
            // open a file for update both reading and writing
            rw = SDL_RWFromFile(file.c_str(), "r+b");
            SDL_RWseek(rw, offset, RW_SEEK_SET);
        }
        else if(0 > offset)
        {
            // writing operations append data at the end of the file
            rw = SDL_RWFromFile(file.c_str(), "ab");
        }
        else
            // create an empty file for writing
            rw = SDL_RWFromFile(file.c_str(), "wb");

        if(rw)
        {
            bool res = SDL_RWwrite(rw, raw.data(), raw.size(), 1) == 1;
            SDL_RWclose(rw);
            return res;
        }

        ERROR(SDL_GetError());
        return false;
    }

    BinaryBuf Systems::readFile(const std::string & file, size_t offset, size_t size)
    {
        BinaryBuf buf;
        SDL_RWops* rw = SDL_RWFromFile(file.c_str(), "rb");

        if(rw)
        {
            SDL_RWseek(rw, 0, RW_SEEK_END);
            size_t filesz = SDL_RWtell(rw);

            if(offset > filesz)
            {
                ERROR("offset out of range, file size: " << filesz);
                return buf;
            }

            // fixed block size
            if(offset + size > filesz)
                size = filesz - offset;
            else
                size = filesz;

            if(0 == size)
            {
                SDL_RWclose(rw);
                DEBUG("file empty, nothing to read");
                return buf;
            }

            buf.resize(size);
            SDL_RWseek(rw, offset, RW_SEEK_SET);

            if(SDL_RWread(rw, buf.data(), buf.size(), 1) != 1)
            {
                ERROR(SDL_GetError());
                buf.clear();
            }

            SDL_RWclose(rw);
            return buf;
        }

        ERROR(SDL_GetError());
        return buf;
    }

    bool Systems::saveString2File(const std::string & str, const std::string & file)
    {
        if(str.empty())
            return false;

        return saveFile(BinaryBuf(reinterpret_cast<const u8*>(str.c_str()), str.size()), file);
    }

    bool Systems::readFile2String(const std::string & file, std::string & str)
    {
        BinaryBuf bb = readFile(file);

        if(bb.size())
        {
            str = bb.toString();
            return true;
        }

        return false;
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
#elif defined(__MINGW32CE__)
        // setlocale(category, locale);
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
#if defined(ANDROID)
        char* clocale = setlocale(LC_MESSAGES, nullptr);
#elif defined(__MINGW32CE__) || defined(__MINGW32__)
        char* clocale = nullptr;
#else
        char* clocale = std::setlocale(LC_MESSAGES, nullptr);
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
#ifndef SWE_SDL12
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
            else if(Systems::environment("APPDATA"))
                res = Systems::concatePath(Systems::environment("APPDATA"), prog);

#if defined(__MINGW32__)
        res = String::escapeChar(res, '\\');
#endif
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
        std::string home = homeDirectory(prog);

        if(home.size()) dirs.push_back(home);

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

    StringList Systems::findFiles(const std::string & path, const std::string & filter, bool sensitive)
    {
#if defined(__MINGW32__)

        if(path.back() == ':')
            return findFiles(std::string(path).append(SEPARATOR()), filter, sensitive);

#endif
        StringList res;
        DIR* dp = opendir(path.c_str());

        if(! dp)
        {
            ERROR("can't read directory: " << path);
            return res;
        }

        struct dirent* ep;

        while(nullptr != (ep = readdir(dp)))
        {
            const std::string fullname = concatePath(path, ep->d_name);

            if(Systems::isDirectory(fullname))
            {
                // skip /. and /..
                if(ep->d_name[0] != '.' || (ep->d_name[1] && ep->d_name[1] != '.'))
                    res << findFiles(fullname, filter, sensitive);
            }
            else if(Systems::isFile(fullname))
            {
                if(filter.empty() ||
                   findFilterContent(ep->d_name, filter, sensitive))
                    res.push_back(fullname);
            }
        }

        closedir(dp);
        return res;
    }

    StringList Systems::readDir(const std::string & path, bool fullpath)
    {
#if defined(__MINGW32__)

        if(path.back() == ':')
            return readDir(std::string(path).append(SEPARATOR()), fullpath);

#endif
        StringList res;
        DIR* dp = opendir(path.c_str());

        if(! dp)
        {
            ERROR("can't read directory: " << path);
            return res;
        }

        struct dirent* ep;

        while(nullptr != (ep = readdir(dp)))
            if(0 != strcmp(ep->d_name, "."))
                res.push_back(fullpath ? concatePath(path, ep->d_name) : ep->d_name);

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
        return nullptr;
#else
        return optarg;
#endif
    }

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

#ifdef SWE_DLOPEN
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
#endif // SWE_DLOPEN

    /* fix char */
#if defined(__MINGW32CE__)

#define is_ll_hh(a, ll, hh) (ll <= a && hh >= a)

#define is_00_08(a) is_ll_hh(a, 0x00, 0x08)
#define is_09_0d(a) is_ll_hh(a, 0x09, 0x0d)
#define is_0e_1f(a) is_ll_hh(a, 0x0e, 0x1f)
#define is_21_2f(a) is_ll_hh(a, 0x21, 0x2f)
#define is_30_39(a) is_ll_hh(a, 0x30, 0x39)
#define is_3a_40(a) is_ll_hh(a, 0x3a, 0x40)
#define is_41_46(a) is_ll_hh(a, 0x41, 0x46)
#define is_47_5a(a) is_ll_hh(a, 0x47, 0x5a)
#define is_5b_60(a) is_ll_hh(a, 0x5b, 0x60)
#define is_61_66(a) is_ll_hh(a, 0x61, 0x66)
#define is_67_7a(a) is_ll_hh(a, 0x67, 0x7a)
#define is_7b_7e(a) is_ll_hh(a, 0x7b, 0x7e)

#define is_20(a)    (0x20 == a)
#define is_7f(a)    (0x7f == a)

    int iscntrl(int a)
    {
        return (is_00_08(a) || is_09_0d(a) || is_0e_1f(a) || is_7f(a));
    }
    int isspace(int a)
    {
        return (is_09_0d(a) || is_20(a));
    }
    int isupper(int a)
    {
        return (is_41_46(a) || is_47_5a(a));
    }
    int isslower(int a)
    {
        return (is_61_66(a) || is_67_7a(a));
    }
    int isalpha(int a)
    {
        return (is_41_46(a) || is_47_5a(a) || is_61_66(a) || is_67_7a(a));
    }
    int isdigit(int a)
    {
        return (is_30_39(a));
    }
    int isxdigit(int a)
    {
        return (is_30_39(a) || is_41_46(a) || is_61_66(a));
    }
    int isalnum(int a)
    {
        return (isalpha(a) || isdigit(a));
    }
    int ispunct(int a)
    {
        return (is_21_2f(a) || is_3a_40(a) || is_5b_60(a) || is_7b_7e(a));
    }
    int isgraph(int a)
    {
        return (ispunct(a) || isalnum(a));
    }
    int isprint(int a)
    {
        return (isgraph(a) || is_20(a));
    }
#endif

    size_t Systems::memoryUsage(void)
    {
#if defined(__SYMBIAN32__)
        return 0;
#elif defined(__WIN32__)
        static MEMORYSTATUS ms;
        ZeroMemory(&ms, sizeof(ms));
        ms.dwLength = sizeof(MEMORYSTATUS);
        GlobalMemoryStatus(&ms);
        return (ms.dwTotalVirtual - ms.dwAvailVirtual);
#elif defined(__LINUX__)
        unsigned int size = 0;
        std::ostringstream os;
        os << "/proc/" << getpid() << "/statm";
        std::ifstream fs(os.str().c_str());

        if(fs.is_open())
        {
            fs >> size;
            fs.close();
        }

        return size * getpagesize();
#else
        return 0;
#endif
    }
}

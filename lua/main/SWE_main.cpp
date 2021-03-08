/***************************************************************************
 *   Copyright (C) 2019 by SWE team <sdl.window.engine@gmail.com>          *
 *                                                                         *
 *   Part of the SWE Lua:                                                  *
 *   https://github.com/AndreyBarmaley/SWE_lua                             *
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

#include <unistd.h>
#include <functional>

#include "../src/SWE_global.h"

extern "C" {
    int luaopen_SWE(lua_State* L);
}

bool findUserStorageFolder(const std::string & str)
{
    if(0 == str.compare(0, 8, "/storage"))
    {
	if(! Systems::isDirectory(str)) Systems::makeDirectory(str);
	if(Systems::isDirectory(str)) return true;
    }
    return false;
}

void printHelp(const char* prog)
{
    COUT("Usage: " << prog << " <file> | <path>\n" <<
            "\t<file> - starting script (default start.lua or main.lua)\n" <<
            "\t<path> - path to starting script or directory (if default starting script exists there)\n\n" <<
            "\tSWE bindings version: " << SWE_LUA_VERSION << "\n" <<
            "\tSWE bindings license: " << SWE_LUA_LICENSE << "\n" <<
            "\tbuild with " << LUA_VERSION << "\n");
}

int main(int argc, char** argv)
{
    Systems::setLocale(LC_ALL, "");
    Systems::setLocale(LC_NUMERIC, "C"); // decimal point

    try
    {
	const char* app = "SWE_lua";
	std::string cwd = Systems::dirname(argv[0]);

	LogWrapper::init(app, argv[0]);
	Engine::setDebugMode(true);

#if defined (_GNU_SOURCE) && ! defined (ANDROID) && ! defined(__MINGW32CE__)
	if(cwd == ".")
	{
	    char* ptr = get_current_dir_name();
	    if(ptr)
	    {
		cwd = ptr;
		free(ptr);
	    }
	}
#endif

	const char* start1 = "start.lua";
        const char* start2 = "main.lua";
        const char* params = 1 < argc ? argv[1] : Systems::environment("SWE_START");

	std::string runfile;
	StringList dirs = Systems::shareDirectories(app);

#ifdef ANDROID
	Systems::assetsInit();

	// sync assets
	auto it = std::find_if(dirs.rbegin(), dirs.rend(), findUserStorageFolder);
	if(it != dirs.rend())
	{
	    const StringList & assets = Systems::assetsList();
	    for(auto as = assets.begin(); as != assets.end(); ++as)
	    {
		if((*as).size() < 5 || (*as).substr((*as).size() - 4, 4) != ".lua") continue;

		std::string body;
		if(Systems::readFile2String(*as, body))
		{
		    std::string dstfile = Systems::concatePath(*it, *as);
		    std::string dstdir = Systems::dirname(dstfile);

		    if(! Systems::isDirectory(dstdir)) Systems::makeDirectory(dstdir);
		    if(*as == start1) runfile = dstfile;

		    Systems::saveString2File(body, dstfile);
		    DEBUG("sync: " << *as << " => " << dstfile);
		}
	    }
	}
#else
        // params priority
        if(params)
	{
            if(0 == strcmp(params, "--help") || 0 == strcmp(params, "-h"))
            {
                printHelp(argv[0]);
                return EXIT_SUCCESS;
            }

            if(Systems::isFile(params))
                std::swap(start1, params);

	    if(Systems::isDirectory(params))
            {
                for(auto str : { "start.lua", "main.lua" })
                {
		    std::string path = Systems::concatePath(params, str);
                    if(Systems::isFile(path))
                    {
                        runfile = path;
                        break;
                    }
                }
            }
        }

        if(runfile.empty())
        {
            // safe run start2
	    if(! Systems::isFile(start1) && Systems::isFile(start2))
                std::swap(start1, start2);

	    if(Systems::isFile(start1))
	        runfile = start1;
        }

	if(cwd.size()) 
	    dirs.push_back(cwd);
#endif

	for(auto it = dirs.begin(); it != dirs.end(); ++it)
	{
	    DEBUG("shares: " << *it);

	    if(runfile.empty())
	    {
		std::string check = Systems::concatePath(*it, start1);
		if(Systems::isFile(check))
		{
		    DEBUG("start found: " << check);
		    runfile = check;
		    break;
		}
	    }
	}

	// check params
	if(runfile.empty())
	{
	    ERROR("starting file not found");
	    return EXIT_FAILURE;
	}

#ifdef ANDROID
	cwd = Systems::dirname(runfile);
	if(cwd.size()) chdir(cwd.c_str());
#endif

        LuaState ll = LuaState::newState();

	if(! ll.L())
	{
	    ERROR("LuaState failure");
	    return EXIT_FAILURE;
	}

	// register directories
	if(Systems::isDirectory(cwd))
	    ll.registerDirectory(cwd);

	const std::string crd = Systems::dirname(runfile);
	if(crd.size() && Systems::isDirectory(crd))
	    ll.registerDirectory(crd);

        luaopen_SWE(ll.L());

	// set SWE.getcwd
	if(ll.pushTable("SWE").isTopTable())
	{
	    DEBUG("set SWE.getcwd: " << cwd);
	    ll.pushString(cwd).setFieldTableIndex("getcwd", -2);

	    DEBUG("set SWE.runfile: " << runfile);
	    ll.pushString(runfile).setFieldTableIndex("runfile", -2);
	}
    	ll.stackPop();

#ifdef WITH_VIDEOCAM_FFMPEG
	DEBUG("build with ffmpeg capture");
#endif

#ifdef WITH_VIDEOCAM_FIREWIRE
	DEBUG("build with firewire capture");
#endif

	// change print -> SWE.Print
#ifdef ANDROID
	if(ll.pushTable("SWE").isTopTable())
	{
	    if(ll.pushTable("_G").isTopTable())
	    {
		if(ll.getFieldTableIndex("Print", -2).isTopFunction())
		{
		    ll.setFieldTableIndex("print", -2);
		}
    		else
		{
		    ll.stackPop();
		}
	    }
    	    ll.stackPop();
	}
    	ll.stackPop();
#endif

        std::string err, trace;
        if(! ll.doFile(runfile))
	{
            err = ll.getTopString();
            trace = ll.stackTrace();
	}

        LuaState::closeState(ll);

        if(! err.empty())
            ERROR(err);

        if(! trace.empty())
            ERROR(trace);
    }
    catch(Engine::exception &)
    {
    }

    return EXIT_SUCCESS;
}

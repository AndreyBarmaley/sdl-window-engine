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
#include <sys/stat.h>
#include <sstream>

#include "SWE_keys.h"
#include "SWE_rect.h"
#include "SWE_tools.h"
#include "SWE_audio.h"
#include "SWE_color.h"
#include "SWE_signal.h"
#include "SWE_global.h"
#include "SWE_window.h"
#include "SWE_texture.h"
#include "SWE_videocam.h"
#include "SWE_terminal.h"
#include "SWE_binarybuf.h"
#include "SWE_streamnet.h"
#include "SWE_streambuf.h"
#include "SWE_randomhit.h"
#include "SWE_streamfile.h"
#include "SWE_fontrender.h"
#include "SWE_translation.h"
#include "SWE_unicodestring.h"

int SWE_window_init_mobile(lua_State* L)
{
    // params: string title, bool landscape
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    std::string title = ll.toStringIndex(1);
    bool landscape =  ll.toBooleanIndex(2);
    Window* win = DisplayScene::rootWindow();

    // check also initialized
    if(win && ((landscape && win->width() > win->height()) || (! landscape && win->width() < win->height())))
    {
	SWE_Scene::clean(ll, true);
	DEBUG("display found: " << win->size().toString());

	win->setVisible(true);

	// push to stack
	if(! SWE_Scene::window_pushtop(ll, *win))
	    ll.pushNil();

	return rescount;
    }

    SWE_Scene::clean(ll, false);
    DEBUG("display init: " << (landscape ? "landscape" : "portrait"));

    if(Display::init(title, landscape))
    {
	SWE_Stack::window_create(ll, Point(0, 0), Display::size(), NULL);
    }
    else
    {
	ERROR("display init failed");
	ll.pushNil();
    }

    return rescount;
}

int SWE_window_init_simple(lua_State* L)
{
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    int params = ll.stackSize();
    std::string title = ll.toStringIndex(1);
    Size winsz = 3 > params ? Size(0, 0) : Size(ll.toIntegerIndex(2), ll.toIntegerIndex(3));
    bool fullscreen = 4 > params ? false : ll.toBooleanIndex(4);
    Window* win = DisplayScene::rootWindow();

    // check also initialized
    if(win && (winsz.isEmpty() || winsz == win->size()))
    {
	SWE_Scene::clean(ll, true);
	DEBUG("display found: " << win->size().toString());

	win->setVisible(true);

	// push to stack
	if(! SWE_Scene::window_pushtop(ll, *win))
	{
	    ERROR("display is loss");
	    ll.pushNil();
	}

	return rescount;
    }
    
    SWE_Scene::clean(ll, false);
    DEBUG("display init: " << winsz.toString());

    if(Display::init(title, winsz, fullscreen))
    {
	SWE_Stack::window_create(ll, Point(0, 0), Display::size(), NULL);
    }
    else
    {
	ERROR("display init failed");
	ll.pushNil();
    }

    return rescount;
}

int SWE_window_init_extend(lua_State* L)
{
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    std::string title;
    Size winsz, rendersz;
    bool fullscreen = false;
    bool accel = false;
    bool resized = false;

    // iterate table
    ll.pushNil();
    while(ll.nextTableIndex(-2))
    {
        std::string key = ll.toStringIndex(-2);

        if(key == "title")
            title = ll.toStringIndex(-1);
        else
        if(key == "window")
            winsz = SWE_Size::get(ll, -1, __FUNCTION__);
        else
        if(key == "render")
            rendersz = SWE_Size::get(ll, -1, __FUNCTION__);
        else
        if(key == "fullscreen")
            fullscreen = ll.toBooleanIndex(-1);
        else
        if(key == "accel")
            accel = ll.toBooleanIndex(-1);
        else
        if(key == "resized")
            resized = ll.toBooleanIndex(-1);

        // pop value
        ll.stackPop();
    }

    SWE_Scene::clean(ll, false);
    DEBUG("display init: " << winsz.toString());

    if(Display::init(title, winsz, rendersz, fullscreen, accel, resized))
    {
	SWE_Stack::window_create(ll, Point(0, 0), Display::size(), NULL);
    }
    else
    {
	ERROR("display init failed");
	ll.pushNil();
    }

    return rescount;
}

int SWE_window_init(lua_State* L)
{
    LuaState ll(L);

    // simple params: string title, int width, int height, bool fullscreen
    // mobile params: string title, bool landscape
    // extend params: table { "title":string, "window":size, "render":size, "fullscreen":bool, "accel":bool, "resized":bool }

    if(ll.isTopTable())
	return SWE_window_init_extend(L);

    if(ll.isBooleanIndex(2))
	return SWE_window_init_mobile(L);

    return SWE_window_init_simple(L);
}


int SWE_terminal_init(lua_State* L)
{
    // params: string title, table swe_fontrender, int cols, int rows
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    std::string title = ll.toStringIndex(1);
    SWE_FontRender* frs = SWE_FontRender::get(ll, 2, __FUNCTION__);
    int cols = ll.toIntegerIndex(3);
    int rows = ll.toIntegerIndex(4);

    if(frs)
    {
        if(Display::init(title, frs->size() * Size(cols, rows), false))
        {
	    SWE_Stack::terminal_create(ll, *frs, cols, rows, NULL);
        }
	else
	{
	    ERROR("display init failed");
	    ll.pushNil();
	}
    }
    else
    {
	ERROR("terminal init failed");
	ll.pushNil();
    }

    return rescount;
}

int SWE_quit(lua_State* L)
{
    // params: none
    DEBUG("the end");
    Engine::quit();

    return 0;
}

int SWE_display_window(lua_State* L)
{
    // params: none
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(Window* win = DisplayScene::rootWindow())
	SWE_Scene::window_pushtop(ll, *win);
    else
    {
	ERROR("display not found");
	ll.pushNil();
    }

    return rescount;
}

int SWE_display_dirty(lua_State* L)
{
    // params: none
    DisplayScene::setDirty(true);

    return 0;
}

int SWE_display_size(lua_State* L)
{
    // params: none
    LuaState ll(L);

    const Size & size = Display::size();
    ll.pushInteger(size.w);
    ll.pushInteger(size.h);
    ll.pushBoolean(Display::isFullscreenWindow());

    return 3;
}

int SWE_get_version(lua_State* L)
{
    // params: none
    LuaState ll(L);
    ll.pushInteger(SWE_LUA_VERSION);
    return 1;
}

int SWE_display_videomodes(lua_State* L)
{
    // params: none
    LuaState ll(L);

    bool landscape = ll.isBooleanIndex(1) ? ll.toBooleanIndex(1) : true;
    auto modes = Display::hardwareVideoModes(landscape);

    for(auto & mode : modes)
    {
	ll.pushTable();
	ll.pushInteger(mode.w).setFieldTableIndex("width", -2);
	ll.pushInteger(mode.h).setFieldTableIndex("height", -2);
    }

    return modes.size();
}

int SWE_loop(lua_State* L)
{
    // params: swe_window
    LuaState ll(L);

    if(auto win = SWE_Window::get(ll, 1, __FUNCTION__))
    {
	int res = win->exec();
	ll.pushInteger(res);
    }
    else
    {
	ERROR("userdata empty");
	ll.pushInteger(0);
    }

    return 1;
}

int SWE_dump(lua_State* L)
{
    // params: none
    LuaState(L).stackDump();
    return 0;
}

int SWE_cursor_show(lua_State* L)
{
    // params: none
    Display::hardwareCursorShow();
    return 0;
}

int SWE_cursor_hide(lua_State* L)
{
    // params: none
    Display::hardwareCursorHide();
    return 0;
}

int SWE_print(lua_State* L)
{
    // params: 
    LuaState ll(L);
    std::ostringstream os;

    for(int index = 1; index <= ll.stackSize(); ++index)
    {
	if(1 < index) os << ", ";
	os << ll.toStringIndex(index);
    }

    COUT(String::time() << ": [" << "PRINT" << "]\t" << os.str());
    return 0;
}

int SWE_error(lua_State* L)
{
    // params: 
    LuaState ll(L);
    std::ostringstream os;

    for(int index = 1; index <= ll.stackSize(); ++index)
    {
	if(1 < index) os << " ";
	os << ll.toStringIndex(index);
    }

    ERROR(os.str());
    return 0;
}

int SWE_debug(lua_State* L)
{
    // params: 
    LuaState ll(L);
    std::ostringstream os;

    for(int index = 1; index <= ll.stackSize(); ++index)
    {
	if(1 < index) os << " ";
	os << ll.toStringIndex(index);
    }

    DEBUG(os.str());
    return 0;
}

int SWE_cursor_load(lua_State* L)
{
    // params: table texture, int offsetx, int offsety
    LuaState ll(L);

    int params = ll.stackSize();
    int offx = 2 > params ? 0 : ll.toIntegerIndex(2);
    int offy = 3 > params ? 0 : ll.toIntegerIndex(3);

    if(SWE_Texture* ptr = SWE_Texture::get(ll, 1, __FUNCTION__))
    {
	DisplayScene::setCursor(*ptr, Point(offx, offy));
    }
    else
    {
	ERROR("userdata empty");
    }

    return 0;
}

int SWE_cursor_info(lua_State* L)
{
    // params: none
    LuaState ll(L);

    ll.pushTable();

    Point pos = Display::mouseCursorPosition();
    u32 btn = Display::mouseButtonState();

    ll.pushInteger(pos.x).setFieldTableIndex("posx", -3);
    ll.pushInteger(pos.y).setFieldTableIndex("posy", -3);
    ll.pushInteger(btn).setFieldTableIndex("buttons", -3);

    return 1;
}

int SWE_find_resource(lua_State* L)
{
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    // params: string resource
    if(! ll.isTopString())
    {
	ERROR("string not found");
	ll.pushBoolean(false);
	return rescount;
    }

    std::string resname = ll.getTopString();
    std::string filename = SWE_Tools::findResource(ll, resname);

    if(Systems::isFile(filename))
    {
        DEBUG(filename);
        ll.pushString(filename);
    }
    else
    {
        ll.pushNil();
    }

    return rescount;
}

int SWE_register_resources(lua_State* L)
{
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    // params: string directory
    if(! ll.isTopString())
    {
	ERROR("string not found");
	ll.pushBoolean(false);
	return rescount;
    }

    std::string name = ll.getTopString();

    if(! Systems::isDirectory(name))
    {
	std::string name2 = SWE_Tools::toRunningPath(ll, name);
	if(Systems::isDirectory(name2)) std::swap(name, name2);
    }

    if(! Systems::isDirectory(name))
    {
	std::string name2 = SWE_Tools::toCurrentPath(ll, name);
	if(Systems::isDirectory(name2)) std::swap(name, name2);
    }

    if(Systems::isDirectory(name))
    {
	DEBUG(name);
	bool res = SWE_Tools::addResourceDir(ll, name);
	ll.pushBoolean(res);
    }
    else
    {
	ERROR("directory not found: " << name);
	ll.pushBoolean(false);
    }

    return rescount;
}

int SWE_register_directory(lua_State* L)
{
    // params: string directory
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(! ll.isTopString())
    {
	ERROR("string not found");
	ll.pushBoolean(false);
	return rescount;
    }

    std::string name = ll.getTopString();

    if(! Systems::isDirectory(name))
    {
	std::string name2 = SWE_Tools::toRunningPath(ll, name);
	if(Systems::isDirectory(name2)) std::swap(name, name2);
    }

    if(! Systems::isDirectory(name))
    {
	std::string name2 = SWE_Tools::toCurrentPath(ll, name);
	if(Systems::isDirectory(name2)) std::swap(name, name2);
    }

    if(Systems::isDirectory(name))
    {
	DEBUG(name);
	bool res = ll.registerDirectory(name);
	ll.pushBoolean(res);
    }
    else
    {
	ERROR("directory not found: " << name);
	ll.pushBoolean(false);
    }

    return rescount;
}

int SWE_system_delay(lua_State* L)
{
    // params: string directory
    LuaState ll(L);

    if(ll.isTopInteger())
    {
	int ms = ll.getTopInteger();
	Tools::delay(ms);
    }
    else
    {
	ERROR("integer not found");
    }

    return 0;
}

int SWE_system_mkdir(lua_State* L)
{
    // params: string directory
    LuaState ll(L);

    if(ll.isTopString())
    {
	std::string name = ll.getTopString();

	if(! Systems::isDirectory(name))
	    Systems::makeDirectory(name);

	ll.pushBoolean(Systems::isDirectory(name));
    }
    else
    {
	ERROR("string not found");
	ll.pushBoolean(false);
    }

    return 1;
}

int SWE_system_memory_usage(lua_State* L)
{
    // params: none
    LuaState ll(L);

    ll.pushInteger(Systems::memoryUsage());
    return 1;
}

int SWE_system_mobile_osname(lua_State* L)
{
    // params: none
    LuaState ll(L);

#if defined ANDROID
    ll.pushString("android");
#elif defined __MINGW32CE__
    ll.pushString("wince");
#elif __SYMBIAN32__
    ll.pushString("symbian");
#else
    ll.pushNil();
#endif
    return 1;
}

int SWE_system_current_directory(lua_State* L)
{
    // params: string directory
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    ll.pushTable("SWE");
    ll.getFieldTableIndex("getcwd", -1, false);

    if(! ll.isTopString())
    {
	ERROR("SWE.getcwd not found");
	ll.stackPop().pushString("./");
    }
    // rmove table
    ll.stackRemoveIndex(-2);

    return rescount;
}

int SWE_system_share_directories(lua_State* L)
{
    // params: empty
    LuaState ll(L);
    StringList dirs = Systems::shareDirectories("SWE_lua");

    // getcwd
    dirs.push_back(SWE_Tools::toCurrentPath(ll, ""));

    for(auto & str : dirs)
	ll.pushString(str);

    return dirs.size();
}

int SWE_system_read_directory(lua_State* L)
{
    // params: string directory
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(! ll.isTopString())
    {
	ERROR("string not found");
	ll.pushTable();
	return rescount;
    }

    std::string dirpath = ll.getTopString();
    if(! Systems::isDirectory(dirpath))
    {
	std::string dirpath2 = SWE_Tools::toCurrentPath(ll, dirpath);
	if(Systems::isDirectory(dirpath2)) std::swap(dirpath, dirpath2);
    }

    if(! Systems::isDirectory(dirpath))
    {
	std::string dirpath2 = SWE_Tools::toRunningPath(ll, dirpath);
	if(Systems::isDirectory(dirpath2)) std::swap(dirpath, dirpath2);
    }

    // result
    ll.pushTable();

    if(Systems::isDirectory(dirpath))
    {
	DEBUG(dirpath);

	for(auto & path : Systems::readDir(dirpath, true))
	{
	    if(Systems::isDirectory(path))
		ll.pushString("directory");
	    else
	    if(Systems::isFile(path))
		ll.pushString("file");
	    else
		ll.pushString("unknown");
	    ll.setFieldTableIndex(path, -2);
	}
    }
    else
    {
	ERROR("directory not found: " << dirpath);
    }

    // return table
    return rescount;
}

int SWE_system_concate_path(lua_State* L)
{
    // params: string list
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    int params = ll.stackSize();
    StringList list;

    for(int it = 1; it <= params; ++it)
	if(ll.isStringIndex(it))
	    list.push_back(ll.toStringIndex(it));

    ll.pushString(Systems::concatePath(list));

    return rescount;
}

int SWE_system_dirname_basename(lua_State* L)
{
    // params: string directory
    const int rescount = 2;
    LuaStateDefine(ll, L, rescount);

    if(ll.isTopString())
    {
	std::string path = ll.getTopString();

	ll.pushString(Systems::dirname(path));
	ll.pushString(Systems::basename(path));
    }
    else
    {
	ERROR("string not found");
	ll.pushNil().pushNil();
    }

    return rescount;
}

int SWE_system_file_stat(lua_State* L)
{
    // params: string directory
    LuaState ll(L);

    if(! ll.isTopString())
    {
	ERROR("string not found");
	ll.pushNil();
	return 1;
    }

    std::string path = ll.getTopString();
    struct stat st;

    if(0 > stat(path.c_str(), & st))
    {
        ERROR("get stat error: " << path);
	ll.pushNil();
	return 1;
    }

    ll.pushTable();
    ll.pushInteger(st.st_mode).setFieldTableIndex("mode", -2);
    ll.pushInteger(st.st_uid).setFieldTableIndex("uid", -2);
    ll.pushInteger(st.st_gid).setFieldTableIndex("gid", -2);
    ll.pushInteger(st.st_size).setFieldTableIndex("size", -2);
    ll.pushInteger(st.st_atime).setFieldTableIndex("atime", -2);
    ll.pushInteger(st.st_mtime).setFieldTableIndex("mtime", -2);
    ll.pushInteger(st.st_ctime).setFieldTableIndex("ctime", -2);
    ll.pushInteger(st.st_nlink).setFieldTableIndex("nlink", -2);

    int type = '-';
    bool isdir = false;

    if(S_ISSOCK(st.st_mode))
    {
	ll.pushString("sock").setFieldTableIndex("type", -2);
	type = 's';
    }
    else
    if(S_ISLNK(st.st_mode))
    {
	ll.pushString("link").setFieldTableIndex("type", -2);
	type = 'l';
    }
    else
    if(S_ISBLK(st.st_mode))
    {
	ll.pushString("block").setFieldTableIndex("type", -2);
	type = 'b';
    }
    else
    if(S_ISCHR(st.st_mode))
    {
	ll.pushString("char").setFieldTableIndex("type", -2);
	type = 'c';
    }
    else
    if(S_ISFIFO(st.st_mode))
    {
	ll.pushString("fifo").setFieldTableIndex("type", -2);
	type = 'p';
    }
    else
    if(S_ISDIR(st.st_mode))
    {
	ll.pushString("dir").setFieldTableIndex("type", -2);
	type = 'd';
	isdir = true;
    }
    else
    if(S_ISREG(st.st_mode))
    {
	ll.pushString("file").setFieldTableIndex("type", -2);
	type = '-';
    }

    std::string access = StringFormat("%1%2%3%4%5%6%7%8%9%10").arg(std::string(1, type)).
	arg(st.st_mode & S_IRUSR ? "r" : "-").arg(st.st_mode & S_IWUSR ? "w" : "-").arg(st.st_mode & S_IXUSR ? "x" : "-").
	arg(st.st_mode & S_IRGRP ? "r" : "-").arg(st.st_mode & S_IWGRP ? "w" : "-").arg(st.st_mode & S_IXGRP ? "x" : "-").
	arg(st.st_mode & S_IROTH ? "r" : "-").arg(st.st_mode & S_IWOTH ? "w" : "-").arg(st.st_mode & S_IXOTH ? "x" : "-");

    ll.pushBoolean(isdir).setFieldTableIndex("isdir", -2);
    ll.pushString(access).setFieldTableIndex("access", -2);
    ll.pushString(path).setFieldTableIndex("path", -2);

    return 1;
}

int SWE_render_screenshot(lua_State* L)
{
    // params: string filename
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(ll.isTopString())
    {
	bool res = Display::renderScreenshot(SWE_Tools::toCurrentPath(ll, ll.getTopString()));
	ll.pushBoolean(res);
    }
    else
    {
	ERROR("string not found");
	ll.pushBoolean(false);
    }

    return rescount;
}

int SWE_string_encode_utf8(lua_State* L)
{
    // params: string str, string charset
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    int params = ll.stackSize();

    if(2 > params || ! ll.isStringIndex(1) || ! ll.isStringIndex(2))
    {
        ERROR("require minimum params: " << "string content, string charset");
	ll.pushNil();
    }
    else
    {
	std::string content = ll.toStringIndex(1);
	std::string charset = ll.toStringIndex(2);
	ll.pushString(Tools::stringEncode(content, charset.c_str()));
    }

    return rescount;
}

int SWE_system_tick(lua_State* L)
{
    // params: none
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    ll.pushNumber(Tools::ticks() - Display::timeStart());
    return rescount;
}

int SWE_table_json(lua_State* L)
{
    // params: string json
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(ll.isTableIndex(-1))
    {
	ll.pushString(ll.toJsonStringTableIndex(-1));
    }
    else
    {
	ERROR("table not found");
	ll.pushNil();
    }

    return rescount;
}

int SWE_json_parse(lua_State* L)
{
    // params: string json
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(! ll.isTopString())
    {
	ERROR("string not found");
	ll.pushNil();
	return rescount;
    }

    JsonContentString jcs(ll.getTopString());

    if(jcs.isArray())
    {
	JsonArray ja = jcs.toArray();
	SWE_Tools::pushJsonArray(ll, & ja);
    }
    else
    if(jcs.isObject())
    {
	JsonObject jo = jcs.toObject();
	SWE_Tools::pushJsonObject(ll, & jo);
    }
    else
    {
	ERROR("bad json");
	ll.pushNil();
    }

    return rescount;
}

int SWE_push_event(lua_State* L)
{
    // params: int code, pointer data, table window
    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    int params = ll.stackSize();
    int code = ll.toIntegerIndex(1);
    void* data = NULL;
    auto  dst = 3 > params || ! ll.isTableIndex(3) ? NULL : SWE_Window::get(ll, 3, __FUNCTION__);

    // store ref
    if(! ll.isNilIndex(2))
    {
	ll.pushValueIndex(2);
	int objRef = luaL_ref(ll.L(), LUA_REGISTRYINDEX);
	data = reinterpret_cast<void*>(objRef);
	DEBUG("create ref object: " << String::hex(objRef));
    }

    DisplayScene::pushEvent(dst, code, data);

    if(data)
    {
	// main window
	Window* win = DisplayScene::rootWindow();
	DisplayScene::pushEvent(win, Signal::LuaUnrefAction, data);
    }

    return rescount;
}

int SWE_set_debug(lua_State* L)
{
    // params: bool
    LuaState ll(L);

    Engine::setDebugMode(ll.toBooleanIndex(1));
    return 0;
}

int SWE_system_run_command(lua_State* L)
{
#if defined(__MINGW32CE__)
    ERROR("not implemented");
    return 0;
#else
    // params: string list
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    int params = ll.stackSize();
    std::string cmdParams;

    for(int it = 1; it <= params; ++it)
	cmdParams.append(ll.toStringIndex(it)).append(1, 0x20);

    DEBUG("`" << cmdParams << "'");
    FILE* pipe = popen(cmdParams.c_str(), "r");

    // result table
    ll.pushTable();

    if(pipe)
    {
	char buffer[512];
	int index = 1;

	while(! std::feof(pipe))
	{
    	    if(std::fgets(buffer, sizeof(buffer), pipe))
		ll.pushInteger(index++).pushString(String::chomp(buffer)).setTableIndex(-3);
	}

	pclose(pipe);
    }
    else
    {
	ERROR("popen error");
    }

    return rescount;
#endif
}

int SWE_display_keyboard(lua_State* L)
{
    // params: bool
    LuaState ll(L);

#ifndef SWE_SDL12
    if(ll.toBooleanIndex(1))
    {
	SDL_StartTextInput();
    }
    else
    {
	SDL_StopTextInput();
    }
#endif

    return 0;
}

int SWE_display_handleevents(lua_State* L)
{
    // params: int interval
    LuaState ll(L);

    u32 interval = ll.isNumberIndex(1) ? ll.toIntegerIndex(1) : 0;
    DisplayScene::handleEvents(interval);

    return 0;
}

/*
int SWE_debug_test(lua_State* L)
{
    // params: int interval
    LuaState ll(L);

    if(ll.isTableIndex(1))
    {
	JsonObject params = ll.toJsonObjectTableIndex(1);
	DEBUG(params.toString());
	DEBUG("dev: " << "`" << params.getString("device") << "'");
    }
    else
    {
	ERROR("table not found");
    }

    return 0;
}
*/

// library interface
const struct luaL_Reg SWE_functions[] = {
    { "DisplayInit", SWE_window_init }, // [table swe_window], string title, int width, int height
    { "TerminalInit",SWE_terminal_init},// [table swe_terminal], string title, table swe_fontrender, int cols, int rows
    { "Dump", SWE_dump }, 		// [void], void or object
    { "Debug", SWE_debug },		// [void], params
    { "Print", SWE_print },		// [void], params
    { "Error", SWE_error },		// [void], params
    { "MainLoop", SWE_loop }, 		// [int window result code], table: window
    { "CursorShow", SWE_cursor_show },	// [void], void
    { "CursorHide", SWE_cursor_hide },	// [void], void
    { "CursorLoad", SWE_cursor_load },	// [void], table swe_texture, int offsetx, int offsety
    { "CursorInfo", SWE_cursor_info },	// [table cursor], void
    { "JsonParse", SWE_json_parse },	// [table], string json
    { "TableToJson", SWE_table_json },	// [string], table
    { "PushEvent", SWE_push_event },	// [void], int code, pointer data, table window
    { "SetDebug", SWE_set_debug }, 	// [void], bool
    { "GetVersion", SWE_get_version }, 	// [int], void
    { "DisplayWindow", SWE_display_window }, 			// [table swe_window], void
    { "DisplayDirty", SWE_display_dirty }, 			// [void], void
    { "DisplayVideoModes", SWE_display_videomodes }, 		// [string list], void
    { "DisplaySize", SWE_display_size }, 			// [int list], void
    { "DisplayKeyboard", SWE_display_keyboard }, 		// [void], bool show
    { "DisplayHandleEvents", SWE_display_handleevents }, 	// [void], int interval
    { "RenderScreenshot", SWE_render_screenshot }, 		// [bool], string filename
    { "RegisterLuaDirectory", SWE_register_directory }, 	// [bool], string directory
    { "RegisterResourceDirectory", SWE_register_resources }, 	// [bool], string directory
    { "FindResource", SWE_find_resource },                      // [string], string resource
    { "SystemSleep", SWE_system_delay }, 			// [void], int
    { "SystemMakeDirectory", SWE_system_mkdir },		// [bool], string directory
    { "SystemCurrentDirectory", SWE_system_current_directory },	// [string], void
    { "SystemShareDirectories", SWE_system_share_directories },	// [string list], void
    { "SystemReadDirectory", SWE_system_read_directory },	// [table list], string directory
    { "SystemFileStat", SWE_system_file_stat },			// [table], string filename
    { "SystemDirnameBasename", SWE_system_dirname_basename },	// [string], string
    { "SystemConcatePath", SWE_system_concate_path },		// [string], string list
    { "SystemMemoryUsage", SWE_system_memory_usage },		// [number], void
    { "SystemMobileOs", SWE_system_mobile_osname },		// [string], void
    { "SystemRunCommand", SWE_system_run_command },		// [table], string cmd, string params, string params
    { "SystemTick", SWE_system_tick },				// [number], void
    { "StringEncodeToUTF8", SWE_string_encode_utf8 },		// [string], string str, string from charset
    // { "DebugTest", SWE_debug_test },
    { NULL, NULL }
};

extern "C" {
 //name of this function is not flexible
 int luaopen_SWE(lua_State* L)
 {
    LuaState ll(L);
    if(ll.version() < 501)
    {
	ERROR("minimum Lua API version 5.1");
	return 0;
    }

    ll.pushTable("SWE");
    ll.getFieldTableIndex("version", -1, false);
    int swever = ll.getTopNumber();

    if(SWE_LUA_VERSION == swever)
    {
	// SWE also loaded
	ll.stackPop();
	DEBUG("also loaded: " << swever);
	return 1;
    }

    ll.stackPop();

    // set version
    ll.pushInteger(SWE_LUA_VERSION).setFieldTableIndex("version", -2);
    ll.pushString(SWE_LUA_LICENSE).setFieldTableIndex("license", -2);
    // set functions
    ll.setFunctionsTableIndex(SWE_functions, -1);
    // set metatable: __gc
    ll.pushTable(0, 1).pushFunction(SWE_quit).setFieldTableIndex("__gc", -2);
    ll.setMetaTableIndex(-2);

    // SWE.Audio
    SWE_Audio::registers(ll);
    // SWE.Color
    SWE_Color::registers(ll);
    // SWE.Key
    SWE_Key::registers(ll);
    // SWE.FontRender
    SWE_FontRender::registers(ll);
    // SWE.Scene
    SWE_Scene::registers(ll);
    // SWE.Window
    SWE_Window::registers(ll);
    // SWE.Polygon
    SWE_Polygon::registers(ll);
    // SWE.Texture
    SWE_Texture::registers(ll);
    // SWE.BinaryBuf
    SWE_BinaryBuf::registers(ll);
    // SWE.StreamNet
    SWE_StreamNet::registers(ll);
    // SWE.StreamBuf
    SWE_StreamBuf::registers(ll);
    // SWE.StreamFile
    SWE_StreamFile::registers(ll);
    // SWE.RandomHit
    SWE_RandomHit::registers(ll);
    // SWE.Point
    SWE_Point::registers(ll);
    // SWE.Size
    SWE_Size::registers(ll);
    // SWE.Rect
    SWE_Rect::registers(ll);
    // SWE.Signal
    SWE_Signal::registers(ll);
    // SWE.Terminal
    SWE_Terminal::registers(ll);
    // SWE.VideoCam
    SWE_VideoCam::registers(ll);
    // SWE.Translation
    SWE_Translation::registers(ll);
    // SWE.UnicodeString
    SWE_UnicodeString::registers(ll);

    bool res = Engine::init();
    if(! res) ERROR("libswe init failed");

    DEBUG("usage " << LUA_RELEASE << ", " << "bindings version: " << SWE_LUA_VERSION);
    return 1;
 }
}

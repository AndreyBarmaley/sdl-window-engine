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

#include "SWE_rect.h"
#include "SWE_tools.h"
#include "SWE_fontrender.h"
#include "SWE_unicodestring.h"

int SWE_fontrender_create_psf(lua_State*);
int SWE_fontrender_create_ttf(lua_State*);
int SWE_fontrender_destroy(lua_State*);

SWE_FontRender* SWE_FontRender::get(LuaState & ll, int tableIndex, const char* funcName)
{
    LuaStateValidator(ll, 0);

    if(! ll.isTableIndex(tableIndex) ||
	0 != ll.popFieldTableIndex("__type", tableIndex).compare("swe.fontrender"))
    {
        ERROR(funcName << ": " << "table not found, index: " << tableIndex);
        return NULL;
    }

    if(! ll.getFieldTableIndex("userdata", tableIndex).isTopUserData())
    {
        ERROR(funcName << ": " << "not userdata, index: " << tableIndex << ", " << ll.getTopTypeName());
	ll.stackPop();
        return NULL;
    }

    auto ptr = static_cast<SWE_FontRender**>(ll.getTopUserData());
    ll.stackPop();

    return ptr ? *ptr : NULL;
}

/////////////////////////////////////////////////////////////////////
int SWE_fontrender_to_json(lua_State* L)
{
    // params: swe_fontrender
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_FontRender::get(ll, 1, __FUNCTION__))
    {
	const std::string font = ll.getFieldTableIndex("font", 1).getTopString();

	if(font == "system")
	{
	    ll.stackPop(1);

    	    std::string str = StringFormat("{\"type\":\"%1\",\"font\":\"%2\"}").arg("swe.fontrender").arg(font);
    	    ll.pushString(str);
	}
	else
	{
	    int size = ll.getFieldTableIndex("size", 1).getTopInteger();
	    std::string blended = ll.getFieldTableIndex("blended", 1).getTopString();
	    int style = ll.getFieldTableIndex("style", 1).getTopInteger();
	    int hinting = ll.getFieldTableIndex("hinting", 1).getTopInteger();

	    ll.stackPop(5);

    	    std::string str = StringFormat("{\"type\":\"%1\",\"font\":\"%2\",\"size\":%3,\"blended\":%4,\"style\":%5,\"hinting\":%6}").
		arg("swe.fontrender").arg(font).arg(size).arg(blended).arg(style).arg(hinting);
    	    ll.pushString(str);
	}
    }
    else
    {
	ERROR("userdata empty");
	ll.pushNil();
    }

    return rescount;
}

int SWE_fontrender_tostring(lua_State* L)
{
    // params: swe_fontrender
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_FontRender::get(ll, 1, __FUNCTION__))
    {
	const std::string font = ll.getFieldTableIndex("font", 1).getTopString();

	if(font == "system")
	{
	    // remove field
	    ll.stackPop(1);
    	    ll.pushString(StringFormat("%1[%2]").arg("swe.fontrender").arg(font));
	}
	else
	{
	    int size = ll.getFieldTableIndex("size", 1).getTopInteger();
	    std::string blended = ll.getFieldTableIndex("blended", 1).getTopString();
	    int style = ll.getFieldTableIndex("style", 1).getTopInteger();
	    int hinting = ll.getFieldTableIndex("hinting", 1).getTopInteger();

	    ll.stackPop(5);
    	    ll.pushString(StringFormat("%1[%2,%3,%4,%5,%6]").arg("swe.fontrender").arg(font).arg(size).arg(blended).arg(style).arg(hinting));
	}
    }
    else
    {
	ERROR("userdata empty");
	ll.pushNil();
    }

    return rescount;
}

int SWE_fontrender_symbol_advance(lua_State* L)
{
    // params: swe_fontrender, symbol int
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_FontRender* frs = SWE_FontRender::get(ll, 1, __FUNCTION__))
    {
	int symbol = ll.toIntegerIndex(2);
        int width = frs->symbolAdvance(symbol);

	// width
	ll.pushInteger(width);
    }
    else
    {
	ERROR("userdata empty");
	ll.pushInteger(0);
    }

    return rescount;
}

int SWE_fontrender_string_size(lua_State* L)
{
    // params: swe_fontrender, string, bool
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_FontRender* frs = SWE_FontRender::get(ll, 1, __FUNCTION__))
    {
	std::string str = ll.toStringIndex(2);
	bool direction = ll.toBooleanIndex(3);

        Size res = frs->stringSize(str, direction);
        SWE_Stack::size_create(ll, res.w, res.h);
    }
    else
    {
	ERROR("userdata empty");
        ll.pushNil();
    }

    return rescount;
}

int SWE_fontrender_ucstring_size(lua_State* L)
{
    // params: swe_fontrender, string, bool
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    SWE_FontRender* frs = SWE_FontRender::get(ll, 1, __FUNCTION__);
    SWE_UnicodeString* ustr = SWE_UnicodeString::get(ll, 2, __FUNCTION__);

    if(frs && ustr)
    {
	bool direction = ll.toBooleanIndex(3);

        Size res = frs->unicodeSize(*ustr, direction);
        SWE_Stack::size_create(ll, res.w, res.h);
    }
    else
    {
	ERROR("userdata empty");
        ll.pushNil();
    }

    return rescount;
}

int SWE_fontrender_split_stringwidth(lua_State* L)
{
    // params: swe_fontrender, int width
    // returned table
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    if(SWE_FontRender* frs = SWE_FontRender::get(ll, 1, __FUNCTION__))
    {
	std::string str = ll.toStringIndex(2);
	int width = ll.toIntegerIndex(3);
	StringList list = frs->splitStringWidth(str, width);

	ll.pushTable(list.size(), 0);
	unsigned int seqindex = 1;
    
	for(auto & str : list)
	{
	    ll.pushString(str);
    	    ll.setIndexTableIndex(seqindex++, -2);
	}

	return rescount;
    }

    ERROR("userdata empty");
    return 0;
}

const struct luaL_Reg SWE_fontrender_functions[] = {
    { "ToJson", SWE_fontrender_to_json },		      // [string], swe_fontrender
    { "SymbolAdvance", SWE_fontrender_symbol_advance },       // [int], swe_fontrender, symbol integer
    { "SplitStringWidth", SWE_fontrender_split_stringwidth }, // [list string], swe_fontrender, string, int width
    { "StringSize", SWE_fontrender_string_size },             // [swe.size], swe_fontrender, string, bool
    { "UnicodeStringSize", SWE_fontrender_ucstring_size },    // [swe.size], swe_fontrender, string, bool
    { NULL, NULL }
};

SWE_FontRender* SWE_Stack::fontrender_create(LuaState & ll, const FontRender* frs, const std::string & font)
{
    LuaStateValidator(ll, 1);

    // result
    ll.pushTable();

    // set: tostring
    ll.pushTable(0, 1);
    ll.pushFunction(SWE_fontrender_tostring).setFieldTableIndex("__tostring", -2);
    ll.setMetaTableIndex(-2);

    // set userdata
    ll.pushString("userdata");
    auto ptr = static_cast<SWE_FontRender**>(ll.pushUserData(sizeof(SWE_FontRender*)));
    *ptr = (SWE_FontRender*) frs;

    // set metatable: __gc to userdata
    ll.pushTable(0, 1);
    ll.pushFunction(SWE_fontrender_destroy).setFieldTableIndex("__gc", -2);
    ll.setMetaTableIndex(-2).setTableIndex(-3);

    // set functions
    ll.setFunctionsTableIndex(SWE_fontrender_functions, -1);

    if(frs == & systemFont())
        DEBUG("used system font: FontAltC8x16");

    DEBUG(String::pointer(ptr) << ": [" << String::pointer(*ptr) << "]" << ", " << "fontId: " << String::hex((*ptr)->id().value(), 4));

    ll.pushString("__type").pushString("swe.fontrender").setTableIndex(-3);
    ll.pushString("font").pushString(font).setTableIndex(-3);
    ll.pushString("size").pushInteger(frs->id().size()).setTableIndex(-3);
    ll.pushString("blended").pushInteger(frs->id().property().render()).setTableIndex(-3);
    ll.pushString("style").pushInteger(frs->id().property().style()).setTableIndex(-3);
    ll.pushString("hinting").pushInteger(frs->id().property().hinting()).setTableIndex(-3);
    ll.pushString("fixedWidth").pushInteger(frs->symbolAdvance(0x20)).setTableIndex(-3);
    ll.pushString("lineHeight").pushInteger(frs->lineSkipHeight()).setTableIndex(-3);

    return *ptr;
}

int SWE_fontrender_create_ttf(lua_State* L)
{
    // SWE.FontRender(self, string, int fsz, int render, int style, int hinting)
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    int params = ll.stackSize();
    if(3 > params || ! ll.isStringIndex(2) || ! ll.isNumberIndex(3))
    {
	ERROR("require minimum params: " << "string font, int size");
	ll.pushNil();
	return rescount;
    }

    std::string font = ll.toStringIndex(2);
    const FontRender* frs = NULL;

    if(! Systems::isFile(font))
    {
	std::string font2 = SWE_Tools::findResource(ll, font);
	if(Systems::isFile(font2)) std::swap(font, font2);
    }

    if(Systems::isFile(font))
    {
	DEBUG(font);

	int fontsz = ll.toIntegerIndex(3);
	auto blended = static_cast<CharRender>(4 > params ? RenderBlended : ll.toIntegerIndex(4));
	int style = 5 > params ? StyleNormal : ll.toIntegerIndex(5);
	auto hinting = static_cast<CharHinting>(6 > params ? HintingNormal : ll.toIntegerIndex(6));

	// SWE_FontRender: font, size, blend, style, hinting
	frs = new FontRenderTTF(font, fontsz, blended, style, hinting);
	if(! frs->isValid())
	{
	    delete frs;
	    frs = & systemFont();
	    font = "system";
	}
    }
    else
    {
	ERROR("file not found: " << font);

	// SWE_FontRender: system
	frs = & systemFont();
	font = "system";
    }

    SWE_Stack::fontrender_create(ll, frs, font);
    return rescount;
}

int SWE_fontrender_create_psf(lua_State* L)
{
    // empty params
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    int params = ll.stackSize();
    if(4 > params || ! ll.isStringIndex(2) || ! ll.isNumberIndex(3) || ! ll.isNumberIndex(4))
    {
	ERROR("require minimum params: " << "string font, int width, int height");
	ll.pushNil();
	return rescount;
    }

    std::string font = ll.toStringIndex(2);
    const FontRender* frs = NULL;

    if(! Systems::isFile(font))
    {
	std::string font2 = SWE_Tools::findResource(ll, font);
	if(Systems::isFile(font2)) std::swap(font, font2);
    }

    if(Systems::isFile(font))
    {
	DEBUG(font);

	int fsw = ll.toIntegerIndex(3);
	int fsh = ll.toIntegerIndex(4);

	// SWE_FontRender: font, size
	frs = new FontRenderPSF(font, Size(fsw, fsh));
	if(! frs->isValid())
	{
	    delete frs;
	    frs = & systemFont();
	    font = "system";
	}
    }
    else
    {
	ERROR("file not found: " << font);

	// SWE_FontRender: system
	frs = & systemFont();
	font = "system";
    }

    SWE_Stack::fontrender_create(ll, frs, font);
    return rescount;
}

int SWE_fontrender_create_sys(lua_State* L)
{
    // empty params
    const int rescount = 1;
    LuaStateDefine(ll, L, rescount);

    SWE_Stack::fontrender_create(ll, & systemFont(), "system");
    return rescount;
}

int SWE_fontrender_destroy(lua_State* L)
{
    const int rescount = 0;
    LuaStateDefine(ll, L, rescount);

    if(ll.isTopUserData())
    {
        auto ptr = static_cast<SWE_FontRender**>(ll.getTopUserData());
        if(ptr && *ptr)
        {
            DEBUG(String::pointer(ptr) << ": [" << String::pointer(*ptr) << "]");

	    if((FontRender*) *ptr != (FontRender*) & systemFont())
	    {
		if((*ptr)->isTTF())
        	{
		    delete (FontRenderTTF*) *ptr;
		}
        	else
		{
		    delete (FontRenderPSF*) *ptr;
		}
	    }

            *ptr = NULL;
        }
        else
        {
            ERROR("userdata empty");
        }
    }
    else
    {
        ERROR("not userdata");
    }

    return rescount;
}

void SWE_FontRender::registers(LuaState & ll)
{
    LuaStateValidator(ll, 0);

    // SWE.Align constants
    ll.pushTable("SWE.Align");
    ll.pushInteger(AlignLeft).setFieldTableIndex("Left", -2);
    ll.pushInteger(AlignRight).setFieldTableIndex("Right", -2);
    ll.pushInteger(AlignTop).setFieldTableIndex("Top", -2);
    ll.pushInteger(AlignBottom).setFieldTableIndex("Bottom", -2);
    ll.pushInteger(AlignCenter).setFieldTableIndex("Center", -2);
    ll.stackPop();
    
    // SWE.Font constants
    ll.pushTable("SWE.Font");
    ll.pushInteger(RenderDefault).setFieldTableIndex("RenderDefault", -2);
    ll.pushInteger(RenderSolid).setFieldTableIndex("RenderSolid", -2);
    ll.pushInteger(RenderBlended).setFieldTableIndex("RenderBlended", -2);
    ll.pushInteger(RenderShaded).setFieldTableIndex("RenderShaded", -2);
    ll.pushInteger(StyleDefault).setFieldTableIndex("StyleDefault", -2);
    ll.pushInteger(StyleNormal).setFieldTableIndex("StyleNormal", -2);
    ll.pushInteger(StyleBold).setFieldTableIndex("StyleBold", -2);
    ll.pushInteger(StyleItalic).setFieldTableIndex("StyleItalic", -2);
    ll.pushInteger(StyleUnderLine).setFieldTableIndex("StyleUnderLine", -2);
    ll.pushInteger(StyleStrikeThrough).setFieldTableIndex("StyleStrikeThrough", -2);
    ll.pushInteger(HintingDefault).setFieldTableIndex("HintingDefault", -2);
    ll.pushInteger(HintingNormal).setFieldTableIndex("HintingNormal", -2);
    ll.pushInteger(HintingLight).setFieldTableIndex("HintingLight", -2);
    ll.pushInteger(HintingMono).setFieldTableIndex("HintingMono", -2);
    ll.pushInteger(HintingNone).setFieldTableIndex("HintingNone", -2);
    ll.stackPop();

    // SWE.FontRender
    ll.pushTable("SWE.FontRender");
    // set metatable: __call
    ll.pushTable(0, 1).pushFunction(SWE_fontrender_create_ttf).setFieldTableIndex("__call", -2);
    ll.setMetaTableIndex(-2).stackPop();

    ll.pushTable("SWE.FontRender.System");
    // set metatable: __call
    ll.pushTable(0, 1).pushFunction(SWE_fontrender_create_sys).setFieldTableIndex("__call", -2);
    ll.setMetaTableIndex(-2).stackPop();
}

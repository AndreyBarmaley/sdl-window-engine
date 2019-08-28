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

#include <algorithm>
#include <unordered_map>
#include <iterator>

#include "engine.h"
#include "fontset.h"

struct UnicodeAll : UnicodeColor
{
    UnicodeAll() : UnicodeColor(0, FBColors(Color::Transparent)) {}
};

class CharsetID : public std::pair<FontID, UnicodeColor>
{
    const FontID & fs(void) const
    {
	return first;
    }

    const UnicodeColor & uc(void) const
    {
	return second;
    }

public:
    CharsetID() : std::pair<FontID, UnicodeColor>() {}
    CharsetID(const FontID & fi, const UnicodeColor & uc) : std::pair<FontID, UnicodeColor>(fi, uc) {}


    bool operator< (const CharsetID & ci) const
    {
	return fs() < ci.fs() ||
	    (fs() == ci.fs() && uc() < ci.uc());
    }

    bool operator> (const CharsetID & ci) const
    {
	return fs() > ci.fs() ||
	    (fs() == ci.fs() && uc() > ci.uc());
    }

    bool operator== (const CharsetID & ci) const
    {
	return fs() == ci.fs() && (uc() == ci.uc() || UnicodeAll() == ci.uc());
    }

    u64 value(void) const
    {
	u64 v1 = fs().value();
	u64 v2 = uc().value();

	return (v1 << 32) | v2;
    }
    
    std::string toString(void) const
    {
        return StringFormat("fontid: %1, fontsz: %2, chars: %3, color: %4").
                arg(String::hex(fs().font())).
                arg(fs().size()).
                arg(String::hex(uc().unicode(), 2)).
                arg(uc().fgcolor().toString());
    }
};

namespace
{
    struct HasherCID
    {
	size_t operator() (const CharsetID & cid) const
	{
	    return std::hash<u64>()(cid.value());
	}
    };

    std::unordered_map<CharsetID, Texture, HasherCID> fontsCache;
}

void FontsCache::clear(void)
{
    fontsCache.clear();
}

void FontsCache::dump(void)
{
    VERBOSE("size: " << fontsCache.size());
    for(auto it = fontsCache.begin(); it != fontsCache.end(); ++it)
	VERBOSE((*it).first.toString());
}

void FontsCache::erase(void)
{
    if(render)
	    fontsCache.erase(CharsetID(render->id(), UnicodeAll()));
}

Texture FontsCache::renderCharset(int ch, const Color & col)
{
    Texture res;

    if(render)
    {
	    CharsetID cid(render->id(), UnicodeColor(ch, col));

    	    auto it = fontsCache.find(cid);
    	    if(it != fontsCache.end())
	    {
		res = (*it).second;
	    }
	    else
	    {
		res = Display::createTexture(render->renderCharset(ch, col));
		if(res.isValid()) fontsCache[cid] = res;
	    }
    }

    return res;
}

void FontRender::clearCache(void)
{
    FontsCache().clear();
}

void FontRender::dumpCache(void)
{
    FontsCache().dump();
}

StringList FontRender::splitStringWidth(const std::string & as, int width) const
{
    StringList res;
    StringList list = String::split(as, '\n');

    if(isValid())
    for(auto it = list.begin(); it != list.end(); ++it)
    {
	std::string & str = *it;
	Size sz = stringSize(str);

	if(sz.w <= width)
	    res << str;
	else
	{
	    auto it1 = str.begin();
	    auto it2 = it1 + 1;

	    while(it2 != str.end())
	    {
		for(; it2 != str.end(); ++it2)
		{
		    std::string substr = str.substr(std::distance(str.begin(), it1), std::distance(it1, it2));

		    if(substr.size())
		    {
			sz = stringSize(substr);
			if(sz.w > width) break;
		    }
		}

		if(it2 == str.end())
		    res << str.substr(std::distance(str.begin(), it1), std::distance(it1, it2));
		else
		{
		    auto it3 = *it2 != 0x20 ? it2 - 1 : it2;
		    while(it3 != it1 && *it3 != 0x20) --it3;
		    if(it3 != it1) it2 = it3;

		    res << str.substr(std::distance(str.begin(), it1), std::distance(it1, it2));

		    it1 = *it2 == 0x20 ? it2 + 1 : it2;
		    it2 = it1 != str.end() ? it1 + 1 : str.end();
		}
	    }
	}
    }

    return res;
}

UnicodeList FontRender::splitUnicodeWidth(const UnicodeString & us, int width) const
{
    return isValid() ? us.splitWidth(*this, width) : UnicodeList();
}

UCStringList FontRender::splitUCStringWidth(const UCString & us, int width) const
{
    return isValid() ? us.splitWidth(*this, width) : UCStringList();
}

void FontRender::renderString(const std::string & str, const Color & col, const Point & pos, Surface & sf) const
{
    int offset = 0;

    for(auto it = str.begin(); it != str.end(); ++it)
    {
        Surface ch = renderCharset(*it, col);
        ch.blit(ch.rect(), Rect(Point(pos.x + offset, pos.y), ch.size()), sf);
        offset += ch.width();
    }
}

struct SDLFont
{
    TTF_Font*	raw;

    SDLFont(TTF_Font* ptr = NULL) : raw(ptr)
    {
    }

    ~SDLFont()
    {
	if(raw) TTF_CloseFont(raw);
    }
};

FontRenderTTF::FontRenderTTF(const std::string & fn, int size, bool blend, int style, int hinting)
{
    open(fn, size, blend, style, hinting);
}

FontRenderTTF::FontRenderTTF(const BinaryBuf & buf, int size, bool blend, int style, int hinting)
{
    load(buf, size, blend, style, hinting);
}

FontRenderTTF::~FontRenderTTF()
{
    if(ptr.unique())
	FontsCache(this).erase();
}

void FontRenderTTF::reset(void)
{
    ptr.reset();
    FontRender::fsz = Size(0, 0);
}

bool FontRenderTTF::load(const BinaryBuf & raw, int size, bool blend, int style, int hinting)
{
    fid.reset();
    SDL_RWops *rw = SDL_RWFromConstMem(raw.data(), raw.size());
    if(rw)
    {
	TTF_Font* ttf = TTF_OpenFontRW(rw, 1, size);
	if(ttf)
	{
	    ptr = std::make_shared<SDLFont>(ttf);
	    fid = FontID(raw.crc16b(), size, blend, style, hinting);
	    TTF_SetFontStyle(toSDLFont(), style);
#ifndef OLDENGINE
	    TTF_SetFontHinting(toSDLFont(), hinting);
#endif
	    FontRender::fsz = Size(symbolAdvance(0x20), lineSkipHeight());
	    return true;
	}
    }

    ERROR(SDL_GetError());
    return false;
}

bool FontRenderTTF::open(const std::string & fn, int size, bool blend, int style, int hinting)
{
    fid.reset();

    TTF_Font* ttf = TTF_OpenFont(fn.c_str(), size);
    if(ttf)
    {
	ptr = std::make_shared<SDLFont>(ttf);
	fid = FontID(Tools::crc16b(fn.c_str()), size, blend, style, hinting);
	TTF_SetFontStyle(toSDLFont(), style);
#ifndef OLDENGINE
	TTF_SetFontHinting(toSDLFont(), hinting);
#endif
	FontRender::fsz = Size(symbolAdvance(0x20), lineSkipHeight());
	return true;
    }

    ERROR(SDL_GetError());
    return false;
}

TTF_Font* FontRenderTTF::toSDLFont(void) const
{
    SDLFont* res = ptr.get();
    return res ? res->raw : NULL;
}

int FontRenderTTF::lineSkipHeight(void) const
{
    return isValid() ? TTF_FontLineSkip(toSDLFont()) : 0;
}


int FontRenderTTF::symbolAdvance(int sym) const
{
    int res = 0;
    if(isValid())
    {
	if(0 != TTF_GlyphMetrics(toSDLFont(), sym, NULL, NULL, NULL, NULL, & res))
	    ERROR(SDL_GetError());
    }
    return res;
}

Size FontRenderTTF::char1Size(int ch) const
{
    char str[2]; str[0] = ch; str[1] = 0;
    int w = 0; int h = 0;

    if(isValid())
    {
	if(0 != TTF_SizeUTF8(toSDLFont(), & str[0], & w, & h))
    	    ERROR(SDL_GetError());
    }

    return Size(w, h);
}

Size FontRenderTTF::char2Size(int ch) const
{
    u16 str[2]; str[0] = ch; str[1] = 0;
    int w = 0; int h = 0;

    if(isValid())
    {
	if(0 != TTF_SizeUNICODE(toSDLFont(), & str[0], & w, & h))
    	    ERROR(SDL_GetError());
    }

    return Size(w, h);
}

Size FontRenderTTF::stringSize(const std::string & str, bool horizontal) const
{
    int w = 0;
    int h = 0;

    if(isValid())
    {
	if(horizontal)
	{
	    if(0 != TTF_SizeUTF8(toSDLFont(), str.c_str(), & w, & h))
		ERROR(SDL_GetError());
	}
	else
	{
	    for(auto it = str.begin(); it != str.end(); ++it)
	    {
		Size sz = char1Size(*it);
		if(sz.w > w) w = sz.w;
		h = h + sz.h;
	    }
	}
    }

    return Size(w, h);
}

Size FontRenderTTF::unicodeSize(const UnicodeString & ustr, bool horizontal) const
{
    int w = 0;
    int h = 0;

    if(isValid())
    {
	if(horizontal)
	{
	    if(ustr.capacity() > ustr.size())
	    {
		UnicodeString & ustr2 = const_cast<UnicodeString &>(ustr);
		ustr2.push_back(0);
		const char16_t* ptr = & ustr2[0];

		if(0 != TTF_SizeUNICODE(toSDLFont(), reinterpret_cast<const Uint16*>(ptr), & w, & h))
		    ERROR(SDL_GetError());

		ustr2.pop_back();
	    }
	    else
	    {
		UnicodeString ustr2;
		ustr2.append(ustr).append(0);
		const char16_t* ptr = & ustr2[0];

		if(0 != TTF_SizeUNICODE(toSDLFont(), reinterpret_cast<const Uint16*>(ptr), & w, & h))
		    ERROR(SDL_GetError());
	    }
	}
	else
	{
	    for(auto it = ustr.begin(); it != ustr.end(); ++it)
	    {
		Size sz = char2Size(*it);
		if(sz.w > w) w = sz.w;
		h = h + sz.h;
	    }
	}
    }

    return Size(w, h);
}

Surface FontRenderTTF::renderCharset(int ch, const Color & col) const
{
    u16 buf[2] = { L'\0', L'\0' }; buf[0] = ch;
    SDL_Surface* sf = NULL;

    if(isValid())
    {
	if(fid.blend())
	    sf = TTF_RenderUNICODE_Blended(toSDLFont(), buf, col.toSDLColor());
	else
	    sf = TTF_RenderUNICODE_Solid(toSDLFont(), buf, col.toSDLColor());

	if(sf != NULL)
	    return Surface(sf);
	else
	    ERROR(SDL_GetError());
    }

    return Surface();
}

FontRenderPSF::FontRenderPSF(const std::string & fn, const Size & sz) : FontRender(sz), buf(Systems::readFile(fn))
{
    fid = FontID(Tools::crc16b(fn.c_str()), sz.h, false, StyleNormal, HintingNormal);
}

FontRenderPSF::FontRenderPSF(const u8* ptr, size_t len, const Size & sz) : FontRender(sz), buf(BinaryBuf(ptr, len))
{
    fid = FontID(Tools::crc16b(ptr, len), sz.h, false, StyleNormal, HintingNormal);
}

int FontRenderPSF::lineSkipHeight(void) const
{
    return size().h;
}


int FontRenderPSF::symbolAdvance(int sym) const
{
    return size().w;
}

Size FontRenderPSF::fixedSize(size_t len, bool horizontal) const
{
    Size newSize = size();

    if(horizontal)
	newSize.w *= len;
    else
	newSize.h *= len;

    return newSize;
}

Size FontRenderPSF::stringSize(const std::string & str, bool horizontal) const
{
    return fixedSize(str.size(), horizontal);
}

Size FontRenderPSF::unicodeSize(const UnicodeString & ustr, bool horizontal) const
{
    return fixedSize(ustr.size(), horizontal);
}

Surface FontRenderPSF::renderCharset(int ch, const Color & cl) const
{
    Surface res(fsz);

    if(UnicodeColor(ch, cl).visible())
    {
	size_t offsetx = ch * fsz.w * fsz.h / 8; // bits -> byte

	if(offsetx < buf.size())
	{
	    for(int yy = 0; yy < fsz.h; ++yy)
	    {
    		size_t offsety = yy * fsz.w / 8; // bits -> byte

    		if(offsetx + offsety < buf.size())
    		{
        	    int line = *(buf.data() + offsetx + offsety);
        	    for(int xx = 0; xx < fsz.w; ++xx)
        	    {
            		if(0x80 & (line << xx))
                	    res.fill(Rect(xx, yy, 1, 1), cl);
        	    }
		}
		else
		{
		    ERROR("pixel out of range: " << offsetx + offsety << ", " << "size: " << buf.size());
		}
	    }
	}
	else
	{
	    ERROR("char out of range: " << ch);
	}
    }

    return res;
}

#include "fontpsf.h"

FontAltC8x16::FontAltC8x16() : FontRenderPSF(_fontpsf_altc_8x16.data, sizeof(_fontpsf_altc_8x16.data), Size(_fontpsf_altc_8x16.width, _fontpsf_altc_8x16.height))
{
}

const FontRenderSystem & systemFont(void)
{
    static FontRenderSystem sysfont;
    return sysfont;
}

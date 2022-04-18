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
#include <iterator>

#include "swe_display.h"
#include "swe_fontset.h"

namespace SWE
{
    /* CharProperty */
    CharProperty::CharProperty(const CharRender & blend, int style, const CharHinting & hinting) : val(0)
    {
	/* blended 2 bit, style 4 bit, hinting 2 bit */
        setRender(RenderDefault == blend ? RenderSolid : blend);
        setStyle(StyleDefault == style ? StyleNormal : style);
        setHinting(HintingDefault == hinting ? HintingNormal : hinting);
    }

    int CharProperty::operator()(void) const
    {   
        return val;
    }   

    CharRender CharProperty::render(void) const
    {   
        switch(0x03 & (val >> 6))
	{
	    case 1:	return RenderBlended;
	    case 2:	return RenderShaded;
	    default: break;
	}
	return RenderSolid;
    }

    int CharProperty::style(void) const
    {   
        return 0x0F & (val >> 2);
    }

    CharHinting CharProperty::hinting(void) const
    {   
        switch(0x03 & (val))
	{
	    case TTF_HINTING_NORMAL:	return HintingNormal;
	    case TTF_HINTING_LIGHT:	return HintingLight;
	    case TTF_HINTING_MONO:	return HintingMono;
	    case TTF_HINTING_NONE:	return HintingNone;
	    default: break;
	}
	return HintingNormal;
    }

    void CharProperty::reset(void)
    {
        val = 0;
    }

    void CharProperty::setRender(const CharRender & v)
    {
        val &= ~(0x03 << 6);
        if(v != RenderDefault) val |= (v & 0x03) << 6;
    }

    void CharProperty::setStyle(int v)
    {
        val &= ~(0x0f << 2);
        if(v != StyleDefault) val |= (v & 0x0f) << 2;
    }
        
    void CharProperty::setHinting(const CharHinting & v)
    {
        val &= ~(0x03);
        if(v != HintingDefault) val |= (v & 0x03);
    }

    bool CharProperty::operator< (const CharProperty & cp) const
    {
	return val < cp.val;
    }

    bool CharProperty::operator!= (const CharProperty & cp) const
    {
	return val != cp.val;
    }

    /* FontID */
    FontID::FontID(int id, int sz, const CharProperty & cp)
    {
        setId(id);
        setSize(sz);
        setProperty(cp);
    }

    int FontID::operator()(void) const
    {   
        return value();
    }

    int FontID::value(void) const
    {   
        return (static_cast<int>(val1) << 16) | (static_cast<int>(val2) << 8) | static_cast<int>(val3.val);
    }

    bool FontID::operator<(const FontID & fd) const
    {
	return value() < fd.value();
    }

    bool FontID::operator>(const FontID & fd) const
    {
	return value() > fd.value();
    }

    bool FontID::operator==(const FontID & fd) const
    {
	return value() == fd.value();
    }

    int FontID::id(void) const
    {
        return val1;
    }
        
    int FontID::size(void) const
    {
        return val2;
    }
        
    const CharProperty & FontID::property(void) const
    {
        return val3;
    }
        
    void FontID::reset(void)
    {
	val1 = 0; val2 = 0; val3.reset();
    }
    
    void FontID::setId(int v)
    {
        val1 = v;
    }

    void FontID::setSize(int v)
    {
	val2 = v;
    }
        
    void FontID::setProperty(const CharProperty & v)
    {
	val3 = v;
    }

    /// @private
    struct UnicodeAll : UnicodeColor
    {
        UnicodeAll() : UnicodeColor(0, FBColors(Color::Transparent)) {}
    };

    /// @private
    struct CharsetID : public std::pair<FontID, UnicodeColor>
    {
        CharsetID() : std::pair<FontID, UnicodeColor>() {}
        CharsetID(const FontID & fi, const UnicodeColor & uc) : std::pair<FontID, UnicodeColor>(fi, uc) {}

        const FontID & fs(void) const
        {
            return first;
        }
        const UnicodeColor & uc(void) const
        {
            return second;
        }

        FontID & fs(void)
        {
            return first;
        }
        UnicodeColor & uc(void)
        {
            return second;
        }

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
                   arg(String::hex(fs().id())).
                   arg(fs().size()).
                   arg(String::hex(uc().unicode(), 2)).
                   arg(uc().fgcolor().toString());
        }
    };

    /// @private
    struct HasherCID
    {
        size_t operator()(const CharsetID & cid) const
        {
            return std::hash<u64>()(cid.value());
        }
    };

    swe_unordered_map<CharsetID, Texture, HasherCID> fontsCache;
}

void SWE::FontsCache::clear(void)
{
    DEBUG("items: " << fontsCache.size());
    fontsCache.clear();
}

void SWE::FontsCache::dump(void)
{
    VERBOSE("size: " << fontsCache.size());

    for(auto it = fontsCache.begin(); it != fontsCache.end(); ++it)
        VERBOSE((*it).first.toString());
}

void SWE::FontsCache::erase(void)
{
    if(render)
        fontsCache.erase(CharsetID(render->id(), UnicodeAll()));
}

SWE::Texture SWE::FontsCache::renderCharset(int ch, const Color & col, const CharRender & blend, int style, const CharHinting & hinting)
{
    Texture res;

    if(render)
    {
        CharsetID cid(render->id(), UnicodeColor(ch, col));
        CharProperty cp;

        if(RenderDefault != blend) cp.setRender(blend);
        if(StyleDefault != style) cp.setStyle(style);
        if(HintingDefault != hinting) cp.setHinting(hinting);

        cid.fs().setProperty(cp);

        auto it = fontsCache.find(cid);

        if(it != fontsCache.end())
            res = (*it).second;
        else
        {
            res = Display::createTexture(render->renderCharset(ch, col, blend, style, hinting));

            if(res.isValid()) fontsCache[cid] = res;
        }
    }

    return res;
}

void SWE::FontRender::clearCache(void)
{
    FontsCache().clear();
}

void SWE::FontRender::dumpCache(void)
{
    FontsCache().dump();
}

SWE::StringList SWE::FontRender::splitStringWidth(const std::string & as, int width) const
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

SWE::UnicodeList SWE::FontRender::splitUnicodeWidth(const UnicodeString & us, int width) const
{
    return isValid() ? us.splitWidth(*this, width) : UnicodeList();
}

SWE::UCStringList SWE::FontRender::splitUCStringWidth(const UCString & us, int width) const
{
    return isValid() ? us.splitWidth(*this, width) : UCStringList();
}

void SWE::FontRender::renderString(const std::string & str, const Color & col, const Point & pos, Surface & sf) const
{
    int offset = 0;

    for(auto it = str.begin(); it != str.end(); ++it)
    {
        Surface ch = renderCharset(*it, col, RenderDefault, StyleDefault, HintingDefault);
        ch.blit(ch.rect(), Rect(Point(pos.x + offset, pos.y), ch.size()), sf);
        offset += ch.width();
    }
}

#ifndef SWE_DISABLE_TTF
SWE::FontRenderTTF::FontRenderTTF(const std::string & fn, size_t fsz, const CharRender & blend, int style, const CharHinting & hinting)
    : ptr{nullptr, TTF_CloseFont}
{
    open(fn, fsz, blend, style, hinting);
}

SWE::FontRenderTTF::FontRenderTTF(const BinaryBuf & buf, size_t fsz, const CharRender & blend, int style, const CharHinting & hinting)
    : ptr{nullptr, TTF_CloseFont}
{
    load(buf, fsz, blend, style, hinting);
}

SWE::FontRenderTTF::~FontRenderTTF()
{
    if(ptr.unique())
        FontsCache(this).erase();
}

void SWE::FontRenderTTF::reset(void)
{
    ptr.reset();
    FontRender::fontSize = Size(0, 0);
}

bool SWE::FontRenderTTF::load(const BinaryBuf & raw, size_t fsz, const CharRender & blend, int style, const CharHinting & hinting)
{
    fid.reset();

    if(SDL_RWops* rw = SDL_RWFromConstMem(raw.data(), raw.size()))
    {
        if(TTF_Font* ttf = TTF_OpenFontRW(rw, 1, fsz))
        {
            ptr.reset(ttf, TTF_CloseFont);
            fid = FontID(raw.crc16b(), fsz, CharProperty(blend, style, hinting));
#ifdef SWE_DEBUG_MESSAGES
            FontRender::fontSize = Size(symbolAdvance(0x20), lineSkipHeight());
	    auto prop = fid.property();
	    DEBUG("binary" << SWE::StringFormat(", id: %1, size: %2, render: %3, style: %4, hinting: %5").
			    arg(String::hex(fid.id(), 4)).arg(fid.size()).arg(prop.render()).arg(prop.style()).arg(prop.hinting()));
#endif
            return true;
        }
    }

    ERROR(SDL_GetError());
    return false;
}

bool SWE::FontRenderTTF::open(const std::string & fn, size_t fsz, const CharRender & blend, int style, const CharHinting & hinting)
{
    fid.reset();

    if(TTF_Font* ttf = TTF_OpenFont(fn.c_str(), fsz))
    {
        ptr.reset(ttf, TTF_CloseFont);
        fid = FontID(Tools::crc16b(fn.c_str()), fsz, CharProperty(blend, style, hinting));
#ifdef SWE_DEBUG_MESSAGES
        FontRender::fontSize = Size(symbolAdvance(0x20), lineSkipHeight());
	auto prop = fid.property();
	    DEBUG("binary" << SWE::StringFormat(", id: %1, size: %2, render: %3, style: %4, hinting: %5").
			    arg(String::hex(fid.id(), 4)).arg(fid.size()).arg(prop.render()).arg(prop.style()).arg(prop.hinting()));
#endif
        return true;
    }

    ERROR(SDL_GetError());
    return false;
}

void SWE::FontRenderTTF::setShadedBackground(const Color & col)
{
    shaded = col;
}

const SWE::FontID & SWE::FontRenderTTF::id(void) const
{
    return fid;
}

bool SWE::FontRenderTTF::isValid(void) const
{
    return toSDLFont();
}

bool SWE::FontRenderTTF::isTTF(void) const
{
    return true;
}

TTF_Font* SWE::FontRenderTTF::toSDLFont(void) const
{
    return ptr.get();
}

int SWE::FontRenderTTF::lineSkipHeight(void) const
{
    return isValid() ? TTF_FontLineSkip(toSDLFont()) : 0;
}


int SWE::FontRenderTTF::symbolAdvance(int sym) const
{
    int res = 0;

    if(isValid())
    {
        if(0 != TTF_GlyphMetrics(toSDLFont(), sym, nullptr, nullptr, nullptr, nullptr, & res))
            ERROR(SDL_GetError());
    }

    return res;
}

SWE::Size SWE::FontRenderTTF::char1Size(int ch) const
{
    char str[2];
    str[0] = ch;
    str[1] = 0;
    int w = 0;
    int h = 0;

    if(isValid())
    {
        if(0 != TTF_SizeUTF8(toSDLFont(), & str[0], & w, & h))
            ERROR(SDL_GetError());
    }

    return Size(w, h);
}

SWE::Size SWE::FontRenderTTF::char2Size(int ch) const
{
    u16 str[2];
    str[0] = ch;
    str[1] = 0;
    int w = 0;
    int h = 0;

    if(isValid())
    {
        if(0 != TTF_SizeUNICODE(toSDLFont(), & str[0], & w, & h))
            ERROR(SDL_GetError());
    }

    return Size(w, h);
}

SWE::Size SWE::FontRenderTTF::stringSize(const std::string & str, bool horizontal) const
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

SWE::Size SWE::FontRenderTTF::unicodeSize(const UnicodeString & ustr, bool horizontal) const
{
    int w = 0;
    int h = 0;

    if(isValid())
    {
        if(horizontal)
        {
            if(0 == ustr.back())
            {
                if(1 < ustr.size())
                {
                    const char16_t* ptr = & ustr[0];

                    if(0 != TTF_SizeUNICODE(toSDLFont(), reinterpret_cast<const Uint16*>(ptr), & w, & h))
                        ERROR(SDL_GetError());
                }
            }
            else if(ustr.capacity() > ustr.size())
            {
                UnicodeString & ustr2 = const_cast<UnicodeString &>(ustr);
                ustr2.push_back(0);
                Size res = unicodeSize(ustr2, horizontal);
                ustr2.pop_back();
                return res;
            }
            else
            {
                UnicodeString ustr2;
                ustr2.reserve(ustr.size() + 1);
                ustr2.append(ustr);
                return unicodeSize(ustr2, horizontal);
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

SWE::Surface SWE::FontRenderTTF::renderString(const std::string & str, const Color & col, const CharRender & blend, int style, const CharHinting & hinting) const
{
    TTF_Font* ttf = toSDLFont();

    if(ttf && str.size())
    {
        SDL_Surface* sf = nullptr;
        CharProperty cp = fid.property();

        TTF_SetFontStyle(ttf, StyleDefault == style ? cp.style() : style);
#ifndef SWE_SDL12
        TTF_SetFontHinting(ttf, HintingDefault == hinting ? cp.hinting() : hinting);
#endif

        int render = RenderDefault == blend ? cp.render() : blend;
        switch(render)
        {
            default:
                sf = TTF_RenderUTF8_Solid(ttf, str.c_str(), col.toSDLColor());
                break;
            case RenderBlended:
                sf = TTF_RenderUTF8_Blended(ttf, str.c_str(), col.toSDLColor());
                break;
            case RenderShaded:
                sf = TTF_RenderUTF8_Shaded(ttf, str.c_str(), col.toSDLColor(), shaded.toSDLColor());
            break;
        }

        if(sf)
            return Surface(sf);

        ERROR(SDL_GetError());
    }

    return Surface();
}

SWE::Surface SWE::FontRenderTTF::renderUnicode(const UnicodeString & ustr, const Color & col, const CharRender & blend, int style, const CharHinting & hinting) const
{
    TTF_Font* ttf = toSDLFont();

    if(ttf && ustr.size())
    {
        SDL_Surface* sf = nullptr;
        const char16_t* ptr = & ustr[0];
        CharProperty cp = fid.property();

        TTF_SetFontStyle(ttf, StyleDefault == style ? cp.style() : style);
#ifndef SWE_SDL12
        TTF_SetFontHinting(ttf, HintingDefault == hinting ? cp.hinting() : hinting);
#endif

        int render = RenderDefault == blend ? cp.render() : blend;
        switch(render)
        {
            default:
                sf = TTF_RenderUNICODE_Solid(ttf, reinterpret_cast<const Uint16*>(ptr), col.toSDLColor());
                break;
            case RenderBlended:
                sf = TTF_RenderUNICODE_Blended(ttf, reinterpret_cast<const Uint16*>(ptr), col.toSDLColor());
                break;
            case RenderShaded:
                sf = TTF_RenderUNICODE_Shaded(ttf, reinterpret_cast<const Uint16*>(ptr), col.toSDLColor(), shaded.toSDLColor());
                break;
        }

        if(sf)
            return Surface(sf);

        ERROR(SDL_GetError());
    }

    return Surface();
}

SWE::Surface SWE::FontRenderTTF::renderCharset(int ch, const Color & col, const CharRender & blend, int style, const CharHinting & hinting) const
{
    TTF_Font* ttf = toSDLFont();

    if(ttf && 0x20 < ch)
    {
        u16 buf[2] = { L'\0', L'\0' };
        buf[0] = ch;
        SDL_Surface* sf = nullptr;
        CharProperty cp = fid.property();

        TTF_SetFontStyle(ttf, StyleDefault == style ? cp.style() : style);
#ifndef SWE_SDL12
        TTF_SetFontHinting(ttf, HintingDefault == hinting ? cp.hinting() : hinting);
#endif

        int render = RenderDefault == blend ? cp.render() : blend;
        switch(render)
        {
            default:
            sf = TTF_RenderUNICODE_Solid(ttf, buf, col.toSDLColor());
            break;
            case RenderBlended:
                sf = TTF_RenderUNICODE_Blended(ttf, buf, col.toSDLColor());
                break;
            case RenderShaded:
                sf = TTF_RenderUNICODE_Shaded(ttf, buf, col.toSDLColor(), shaded.toSDLColor());
                break;
        }

        if(sf)
            return Surface(sf);

        ERROR(SDL_GetError());
    }

    return Surface(FontRender::fontSize);
}
#endif

SWE::FontRenderPSF::FontRenderPSF(const std::string & fn, const Size & sz) : FontRender(sz), buf(Systems::readFile(fn))
{
    fid = FontID(Tools::crc16b(fn.c_str()), sz.h);
}

SWE::FontRenderPSF::FontRenderPSF(const u8* ptr, size_t len, const Size & sz) : FontRender(sz), buf(BinaryBuf(ptr, len))
{
    fid = FontID(Tools::crc16b(ptr, len), sz.h);
}

int SWE::FontRenderPSF::lineSkipHeight(void) const
{
    return size().h;
}

int SWE::FontRenderPSF::symbolAdvance(int sym) const
{
    return size().w;
}

SWE::Size SWE::FontRenderPSF::fixedSize(size_t len, bool horizontal) const
{
    Size newSize = size();

    if(horizontal)
        newSize.w *= len;
    else
        newSize.h *= len;

    return newSize;
}

SWE::Size SWE::FontRenderPSF::stringSize(const std::string & str, bool horizontal) const
{
    return fixedSize(str.size(), horizontal);
}

SWE::Size SWE::FontRenderPSF::unicodeSize(const UnicodeString & ustr, bool horizontal) const
{
    return fixedSize(ustr.size(), horizontal);
}

SWE::Surface SWE::FontRenderPSF::renderCharset(int ch, const Color & cl, const CharRender & blend, int style, const CharHinting & hinting) const
{
    auto & fsz = FontRender::size();
    Surface res(fsz);

    if(0x20 < ch)
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
                    ERROR("pixel out of range: " << offsetx + offsety << ", " << "size: " << buf.size());
            }
        }
        else
            ERROR("char out of range: " << ch);
    }

    return res;
}

#include "swe_fontpsf.h"

namespace SWE
{
    /// @brief встроенный шрифт altc_8x16
    FontAltC8x16::FontAltC8x16() : FontRenderPSF(_fontpsf_altc_8x16.data, sizeof(_fontpsf_altc_8x16.data), Size(_fontpsf_altc_8x16.width, _fontpsf_altc_8x16.height))
    {
    }

    const FontRenderSystem & systemFont(void)
    {
        static FontRenderSystem sysfont;
        return sysfont;
    }
}

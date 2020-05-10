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

#include "swe_display.h"
#include "swe_fontset.h"

namespace SWE
{
    /* CharsetProperty */
    CharsetProperty::CharsetProperty(int blend, int style, int hinting) : val(0)
    {
        setRender(blend);
        setStyle(style);
        setHinting(hinting);
    }

    int CharsetProperty::operator()(void) const
    {   
        return val;
    }   

    int CharsetProperty::render(void) const
    {   
        return 0x03 & (val >> 6);
    }   

    int CharsetProperty::style(void) const
    {   
        return 0x0F & (val >> 2);
    }   

    int CharsetProperty::hinting(void) const
    {   
        return 0x0003 & (val);
    }   

    void CharsetProperty::reset(void)
    {
        val = 0;
    }

    void CharsetProperty::setRender(int v)
    {
        val &= ~(0x03 << 6);
    
        if(v)
            val |= (v & 0x03) << 6;
    }

    void CharsetProperty::setStyle(int v)
    {
        val &= ~(0x0F << 2);

        if(v)
            val |= (v & 0x0F) << 2;
    }
        
    void CharsetProperty::setHinting(int v)
    {
        val &= ~(0x03);

        if(v)
            val |= (v & 0x03);
    }

    /* FontID */
    FontID::FontID(int id, int sz, const CharsetProperty & cp)
    {
        setId(id);
        setSize(sz);
        setProperty(cp());
    }
        
    int FontID::id(void) const
    {
        return val1();
    }
        
    int FontID::size(void) const
    {
        return packshort(val2()).val1();
    }
        
    CharsetProperty FontID::property(void) const
    {
        CharsetProperty res;
        res.val = packshort(val2()).val2();
        return res;
    }
        
    void FontID::reset(void)
    {
        setvalue(0);
    }
    
    void FontID::setId(int v)
    {
        set1(v);
    }

    void FontID::setSize(int v)
    {
        set2(packshort(val2()).set1(v).value());
    }
        
    void FontID::setProperty(int v)
    {
        set2(packshort(val2()).set2(v).value());
    }

    /* UnicodeAll */
    struct UnicodeAll : UnicodeColor
    {
        UnicodeAll() : UnicodeColor(0, FBColors(Color::Transparent)) {}
    };

    /* CharsetID */
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

#ifndef DISABLE_TTF
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
#endif

    struct HasherCID
    {
        size_t operator()(const CharsetID & cid) const
        {
            return std::hash<u64>()(cid.value());
        }
    };

    std::unordered_map<CharsetID, Texture, HasherCID> fontsCache;
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

SWE::Texture SWE::FontsCache::renderCharset(int ch, const Color & col, int blend, int style, int hinting)
{
    Texture res;

    if(render)
    {
        CharsetID cid(render->id(), UnicodeColor(ch, col));
        CharsetProperty cp;

        if(0 <= blend) cp.setRender(blend);

        if(0 <= style) cp.setStyle(style);

        if(0 <= hinting) cp.setHinting(hinting);

        if(cp()) cid.fs().setProperty(cp());

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
        Surface ch = renderCharset(*it, col, -1, -1, -1);
        ch.blit(ch.rect(), Rect(Point(pos.x + offset, pos.y), ch.size()), sf);
        offset += ch.width();
    }
}

#ifndef DISABLE_TTF
SWE::FontRenderTTF::FontRenderTTF(const std::string & fn, int size, int blend, int style, int hinting)
{
    open(fn, size, blend, style, hinting);
}

SWE::FontRenderTTF::FontRenderTTF(const BinaryBuf & buf, int size, int blend, int style, int hinting)
{
    load(buf, size, blend, style, hinting);
}

SWE::FontRenderTTF::~FontRenderTTF()
{
    if(ptr.unique())
        FontsCache(this).erase();
}

void SWE::FontRenderTTF::reset(void)
{
    ptr.reset();
    FontRender::fsz = Size(0, 0);
}

bool SWE::FontRenderTTF::load(const BinaryBuf & raw, int size, int blend, int style, int hinting)
{
    fid.reset();
    SDL_RWops* rw = SDL_RWFromConstMem(raw.data(), raw.size());

    if(rw)
    {
        TTF_Font* ttf = TTF_OpenFontRW(rw, 1, size);

        if(ttf)
        {
            ptr = std::make_shared<SDLFont>(ttf);
            fid = FontID(raw.crc16b(), size, CharsetProperty(blend, style, hinting));
            FontRender::fsz = Size(symbolAdvance(0x20), lineSkipHeight());
            return true;
        }
    }

    ERROR(SDL_GetError());
    return false;
}

bool SWE::FontRenderTTF::open(const std::string & fn, int size, int blend, int style, int hinting)
{
    fid.reset();
    TTF_Font* ttf = TTF_OpenFont(fn.c_str(), size);

    if(ttf)
    {
        ptr = std::make_shared<SDLFont>(ttf);
        fid = FontID(Tools::crc16b(fn.c_str()), size, CharsetProperty(blend, style, hinting));
        FontRender::fsz = Size(symbolAdvance(0x20), lineSkipHeight());
        return true;
    }

    ERROR(SDL_GetError());
    return false;
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
    SDLFont* res = ptr.get();
    return res ? res->raw : NULL;
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
        if(0 != TTF_GlyphMetrics(toSDLFont(), sym, NULL, NULL, NULL, NULL, & res))
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

SWE::Surface SWE::FontRenderTTF::renderString(const std::string & str, const Color & col, int blend, int style, int hinting) const
{
    TTF_Font* ttf = toSDLFont();

    if(ttf && str.size())
    {
        SDL_Surface* sf = NULL;
        CharsetProperty cp = fid.property();

        if(0 > blend)
            blend = cp.render();

        if(0 > style)
            style = cp.style();

        if(0 > hinting)
            hinting = cp.hinting();

        TTF_SetFontStyle(ttf, style);
#ifndef OLDENGINE
        TTF_SetFontHinting(ttf, hinting);
#endif
        sf = blend ? TTF_RenderUTF8_Blended(ttf, str.c_str(), col.toSDLColor()) :
             TTF_RenderUTF8_Solid(ttf, str.c_str(), col.toSDLColor());

        if(sf != NULL)
            return Surface(sf);
        else
            ERROR(SDL_GetError());
    }

    return Surface();
}

SWE::Surface SWE::FontRenderTTF::renderUnicode(const UnicodeString & ustr, const Color & col, int blend, int style, int hinting) const
{
    TTF_Font* ttf = toSDLFont();

    if(ttf && ustr.size())
    {
        SDL_Surface* sf = NULL;
        const char16_t* ptr = & ustr[0];
        CharsetProperty cp = fid.property();

        if(0 > blend)
            blend = cp.render();

        if(0 > style)
            style = cp.style();

        if(0 > hinting)
            hinting = cp.hinting();

        TTF_SetFontStyle(ttf, style);
#ifndef OLDENGINE
        TTF_SetFontHinting(ttf, hinting);
#endif
        sf = blend ? TTF_RenderUNICODE_Blended(ttf, reinterpret_cast<const Uint16*>(ptr), col.toSDLColor()) :
             TTF_RenderUNICODE_Solid(ttf, reinterpret_cast<const Uint16*>(ptr), col.toSDLColor());

        if(sf != NULL)
            return Surface(sf);
        else
            ERROR(SDL_GetError());
    }

    return Surface();
}

SWE::Surface SWE::FontRenderTTF::renderCharset(int ch, const Color & col, int blend, int style, int hinting) const
{
    TTF_Font* ttf = toSDLFont();

    if(ttf && 0x20 < ch)
    {
        u16 buf[2] = { L'\0', L'\0' };
        buf[0] = ch;
        SDL_Surface* sf = NULL;
        CharsetProperty cp = fid.property();

        if(0 > blend)
            blend = cp.render();

        if(0 > style)
            style = cp.style();

        if(0 > hinting)
            hinting = cp.hinting();

        TTF_SetFontStyle(ttf, style);
#ifndef OLDENGINE
        TTF_SetFontHinting(ttf, hinting);
#endif

        if(blend)
            sf = TTF_RenderUNICODE_Blended(ttf, buf, col.toSDLColor());
        else
            sf = TTF_RenderUNICODE_Solid(ttf, buf, col.toSDLColor());

        if(sf != NULL)
            return Surface(sf);
        else
            ERROR(SDL_GetError());
    }

    return Surface(fsz);
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

SWE::Surface SWE::FontRenderPSF::renderCharset(int ch, const Color & cl, int blend, int style, int hinting) const
{
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
    FontAltC8x16::FontAltC8x16() : FontRenderPSF(_fontpsf_altc_8x16.data, sizeof(_fontpsf_altc_8x16.data), Size(_fontpsf_altc_8x16.width, _fontpsf_altc_8x16.height))
    {
    }

    const FontRenderSystem & systemFont(void)
    {
        static FontRenderSystem sysfont;
        return sysfont;
    }
}

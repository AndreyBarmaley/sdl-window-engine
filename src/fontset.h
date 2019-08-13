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

#ifndef _SWE_FONTRENDER_
#define _SWE_FONTRENDER_

#include "types.h"
#include "surface.h"
#include "binarybuf.h"
#include "cunicode_color.h"

enum align_t { AlignNone, AlignLeft, AlignRight, AlignTop, AlignBottom, AlignCenter };

struct FontID : packint2 /* font id 16 bit, font blend 1 bit, style 4 bit, hinting 2, font size 9 bit */
{
    FontID() : packint2(0) {}
    FontID(int id, int sz, bool blend, int style, int hinting) : packint2(id, 0)
    {
	int val = 0x01FF & sz;
	if(blend) val |= 0x8000;
	if(style) val |= (style & 0x000F) << 11;
	if(hinting) val |= (hinting & 0x0003) << 9;
	set2(val);
    }

    int                 font(void) const { return val1(); }
    int                 size(void) const { return 0x01FF & val2(); }
    int                 style(void) const { return 0x000F & (val2() >> 11); }
    int                 hinting(void) const { return 0x0003 & (val2() >> 9); }
    bool                blend(void) const { return 0x8000 & val2(); }
    void                reset(void) { setvalue(0); }
};

class FontRender
{
protected:
    Size	fsz;

public:
    FontRender() {}
    FontRender(const Size & sz) : fsz(sz) {}
    virtual ~FontRender() {}

    virtual const FontID &
			id(void) const = 0;
    virtual bool	isValid(void) const = 0;
    virtual bool	isTTF(void) const = 0;

    virtual Size	stringSize(const std::string &, bool horizontal = true) const = 0;
    virtual Size	unicodeSize(const UnicodeString &, bool horizontal = true) const = 0;

    virtual int		symbolAdvance(int) const = 0;
    virtual int		lineSkipHeight(void) const = 0;

    virtual Surface	renderCharset(int, const Color &) const = 0;

    //
    UCStringList	splitUCStringWidth(const UCString &, int) const;
    UnicodeList		splitUnicodeWidth(const UnicodeString &, int) const;
    StringList		splitStringWidth(const std::string &, int) const;

    const Size &	size(void) const { return fsz; }

    static void		clearCache(void);
    static void		dumpCache(void);

    void 		renderString(const std::string &, const Color &, const Point &, Surface &) const;
};

class CharsetID;

class FontsCache
{
    const FontRender*	render;

public:
    FontsCache(const FontRender* font = NULL) : render(font) {}

    static void		clear(void);
    static void		dump(void);
    void		erase(void);

    Texture		renderCharset(int ch, const Color &);
};

struct SDLFont;

#if (SDL_VERSIONNUM(SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION, SDL_TTF_PATCHLEVEL) <= SDL_VERSIONNUM(2, 0, 10))
#define TTF_STYLE_STRIKETHROUGH 0x08
#define TTF_HINTING_NORMAL 0
#define TTF_HINTING_LIGHT  1
#define TTF_HINTING_MONO   2
#define TTF_HINTING_NONE   3
#endif

enum { StyleNormal = TTF_STYLE_NORMAL, StyleBold = TTF_STYLE_BOLD, StyleItalic = TTF_STYLE_ITALIC, StyleUnderLine = TTF_STYLE_UNDERLINE, StyleStrikeThrough = TTF_STYLE_STRIKETHROUGH };
enum { HintingNormal = TTF_HINTING_NORMAL, HintingLight = TTF_HINTING_LIGHT, HintingMono = TTF_HINTING_MONO, HintingNone = TTF_HINTING_NONE };

class FontRenderTTF : public FontRender
{
    std::shared_ptr<SDLFont> ptr;
    FontID	fid;

    Size	char1Size(int ch) const;
    Size	char2Size(int ch) const;

public:
    FontRenderTTF() {}
    FontRenderTTF(const std::string &, int size, bool blend = false, int style = StyleNormal, int hinting = HintingNormal);
    FontRenderTTF(const BinaryBuf &, int size, bool blend = false, int style = StyleNormal, int hinting = HintingNormal);
    ~FontRenderTTF();

    void        reset(void);

    bool        open(const std::string &, int, bool blend = false, int style = StyleNormal, int hinting = HintingNormal);
    bool        load(const BinaryBuf &, int, bool blend = false, int style = StyleNormal, int hinting = HintingNormal);

    const FontID &
		id(void) const override { return fid; }
    bool	isValid(void) const override { return toSDLFont(); }
    bool	isTTF(void) const override { return true; }
    TTF_Font*	toSDLFont(void) const;

    Size	stringSize(const std::string &, bool horizontal = true) const override;
    Size	unicodeSize(const UnicodeString &, bool horizontal = true) const override;

    int		symbolAdvance(int sym) const override;
    int		lineSkipHeight(void) const override;

    Surface	renderCharset(int, const Color &) const override;
};

class FontRenderPSF : public FontRender
{
    BinaryBuf	buf;
    FontID	fid;

    Size	fixedSize(size_t, bool) const;

public:
    FontRenderPSF(const u8*, size_t, const Size &);
    FontRenderPSF(const std::string &, const Size &);

    const FontID &
		id(void) const override { return fid; }
    bool	isValid(void) const override { return buf.size(); }
    bool	isTTF(void) const override { return false; }

    Size	stringSize(const std::string &, bool horizontal = true) const override;
    Size	unicodeSize(const UnicodeString &, bool horizontal = true) const override;

    int		symbolAdvance(int sym) const override;
    int		lineSkipHeight(void) const override;

    Surface	renderCharset(int, const Color &) const override;
};

class FontAltC8x16 : public FontRenderPSF
{
public:
    FontAltC8x16();
};

#define FontRenderSystem	FontAltC8x16

const FontRenderSystem & systemFont(void);

#endif

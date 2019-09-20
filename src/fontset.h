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

#if (SDL_VERSIONNUM(SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION, SDL_TTF_PATCHLEVEL) <= SDL_VERSIONNUM(2, 0, 10))
#define TTF_STYLE_STRIKETHROUGH 0x08
#define TTF_HINTING_NORMAL 0
#define TTF_HINTING_LIGHT  1
#define TTF_HINTING_MONO   2
#define TTF_HINTING_NONE   3
#endif

enum { StyleNormal = TTF_STYLE_NORMAL, StyleBold = TTF_STYLE_BOLD, StyleItalic = TTF_STYLE_ITALIC, StyleUnderLine = TTF_STYLE_UNDERLINE, StyleStrikeThrough = TTF_STYLE_STRIKETHROUGH };
enum { HintingNormal = TTF_HINTING_NORMAL, HintingLight = TTF_HINTING_LIGHT, HintingMono = TTF_HINTING_MONO, HintingNone = TTF_HINTING_NONE };

struct CharsetID;
struct SDLFont;

struct CharsetProperty
{
    /* unused 1 bit, blended 1 bit, style 4 bit, hinting 2 bit */
    u8                  val;

    CharsetProperty(int v) : val(v)
    {
    }

    CharsetProperty() : val(0)
    {
        setBlended(false);
        setStyle(StyleNormal);
        setHinting(HintingNormal);
    }

    CharsetProperty(bool blend, int style, int hinting) : val(0)
    {
        setBlended(blend);
        setStyle(style);
        setHinting(hinting);
    }

    int                 operator() (void) const { return val; }

    bool                blended(void) const { return 0x01 & (val >> 6); }
    int                 style(void) const { return 0x0F & (val >> 2); }
    int                 hinting(void) const { return 0x0003 & (val); }
    void                reset(void) { val = 0; }

    void                setBlended(bool v) { val &= ~(0x01 << 6); val |= (v & 0x01) << 6; }
    void                setStyle(int v) { val &= ~(0x0F << 2); val |= (v & 0x0F) << 2; }
    void                setHinting(int v) { val &= ~(0x03); val |= (v & 0x03); }
};

struct FontID
{
    /* font id 16 bit, font size 9 bit, charset property 7 bit */
    u32			val;

    FontID() : val(0)
    {
    }

    FontID(int id, int sz, const CharsetProperty & cp = CharsetProperty()) : val(0)
    {
	setId(id);
	setSize(sz);
	setProperty(cp());
    }

    const u32 &		value(void) const { return val; }

    bool		operator< (const FontID & fid) const { return val < fid.val; }
    bool		operator> (const FontID & fid) const { return val > fid.val; }
    bool		operator== (const FontID & fid) const { return val == fid.val; }
    bool		operator!= (const FontID & fid) const { return val != fid.val; }

    int                 id(void) const { return 0x0000FFFF & (val >> 16); }
    int                 size(void) const { return 0x000001FF & (val >> 7); }
    CharsetProperty     property(void) const { return CharsetProperty(0x0000007F & val); }

    void                reset(void) { val = 0; }

    void		setId(int v) { val &= ~(0x0000FFFF << 16); val |= (v & 0xFFFF) << 16; }
    void		setSize(int v) { val &= ~(0x000001FF << 7); val |= (v & 0x01FF) << 7; }
    void		setProperty(int v) { val &= ~(0x0000007F); val |= (v & 0x0000007F); }
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

    virtual Surface	renderCharset(int, const Color &, int blend, int style, int hinting) const = 0;

    //
    UCStringList	splitUCStringWidth(const UCString &, int) const;
    UnicodeList		splitUnicodeWidth(const UnicodeString &, int) const;
    StringList		splitStringWidth(const std::string &, int) const;

    const Size &	size(void) const { return fsz; }

    static void		clearCache(void);
    static void		dumpCache(void);

    void 		renderString(const std::string &, const Color &, const Point &, Surface &) const;
};

class FontsCache
{
    const FontRender*	render;

public:
    FontsCache(const FontRender* font = NULL) : render(font) {}

    static void		clear(void);
    static void		dump(void);
    void		erase(void);

    Texture		renderCharset(int ch, const Color & col) { return renderCharset(ch, col, -1, -1, -1); }
    Texture		renderCharset(int ch, const Color &, int blend, int style, int hinting);
};



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

    Surface	renderCharset(int, const Color &, int blend, int style, int hinting) const override;
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

    Surface	renderCharset(int, const Color &, int blend, int style, int hinting) const override;
};

class FontAltC8x16 : public FontRenderPSF
{
public:
    FontAltC8x16();
};

#define FontRenderSystem	FontAltC8x16

const FontRenderSystem & systemFont(void);

#endif

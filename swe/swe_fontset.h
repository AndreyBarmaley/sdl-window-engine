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

#include "swe_types.h"
#include "swe_surface.h"
#include "swe_binarybuf.h"
#include "swe_cunicode_color.h"

namespace SWE
{

    enum align_t { AlignNone, AlignLeft, AlignRight, AlignTop, AlignBottom, AlignCenter };

    struct CharsetID;

#if (SDL_VERSIONNUM(SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION, SDL_TTF_PATCHLEVEL) <= SDL_VERSIONNUM(2, 0, 10))
#define TTF_STYLE_STRIKETHROUGH 0x08
#define TTF_HINTING_NORMAL 0
#define TTF_HINTING_LIGHT  1
#define TTF_HINTING_MONO   2
#define TTF_HINTING_NONE   3
#endif

#ifdef DISABLE_TTF
#define TTF_STYLE_NORMAL        0
#define TTF_STYLE_BOLD          0x01
#define TTF_STYLE_ITALIC        0x02
#define TTF_STYLE_UNDERLINE     0x04
#define TTF_STYLE_STRIKETHROUGH 0x08
#define TTF_HINTING_NORMAL 0
#define TTF_HINTING_LIGHT  1
#define TTF_HINTING_MONO   2
#define TTF_HINTING_NONE   3
#endif

    enum PropertyRender { RenderSolid = 0, RenderBlended = 1, RenderShaded = 2 };
    enum PropertyStyle { StyleNormal = TTF_STYLE_NORMAL, StyleBold = TTF_STYLE_BOLD, StyleItalic = TTF_STYLE_ITALIC, StyleUnderLine = TTF_STYLE_UNDERLINE, StyleStrikeThrough = TTF_STYLE_STRIKETHROUGH };
    enum PropertyHinting { HintingNormal = TTF_HINTING_NORMAL, HintingLight = TTF_HINTING_LIGHT, HintingMono = TTF_HINTING_MONO, HintingNone = TTF_HINTING_NONE };

    struct CharsetProperty
    {
        /* blended 2 bit, style 4 bit, hinting 2 bit */
        u8                  val;

        CharsetProperty(int blend = RenderSolid, int style = StyleNormal, int hinting = HintingNormal);

        int	operator()(void) const;
        int	render(void) const;
        int	style(void) const;
        int	hinting(void) const;
        void	reset(void);
        void	setRender(int v);
        void	setStyle(int v);
        void	setHinting(int v);
    };

    struct FontID
    {
	u16	val1;
	u8	val2;
	u8	val3;

        /* font id 16 bit, font size 8 bit, charset property 8 bit */
        FontID() {}
        FontID(int id, int sz, const CharsetProperty & cp = CharsetProperty());

        int	operator()(void) const;
        int	value(void) const;
	bool	operator<(const FontID &) const;
	bool	operator>(const FontID &) const;
	bool	operator==(const FontID &) const;

        int 	id(void) const;
        int 	size(void) const;

        CharsetProperty property(void) const;

        void 	reset(void);
        void 	setId(int v);
        void 	setSize(int v);
        void 	setProperty(int v);
    };

    class FontRender
    {
    protected:
        Size		fsz;

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

        virtual int	symbolAdvance(int = 0x20) const = 0;
        virtual int	lineSkipHeight(void) const = 0;

        virtual Surface	renderCharset(int, const Color &, int blend = -1, int style = -1, int hinting = -1) const = 0;

        //
        UCStringList	splitUCStringWidth(const UCString &, int) const;
        UnicodeList	splitUnicodeWidth(const UnicodeString &, int) const;
        StringList	splitStringWidth(const std::string &, int) const;

        const Size &	size(void) const
        {
            return fsz;
        }

        static void	clearCache(void);
        static void	dumpCache(void);

        void 		renderString(const std::string &, const Color &, const Point &, Surface &) const;
    };

    class FontsCache
    {
        const FontRender*	render;

    public:
        FontsCache(const FontRender* font = NULL) : render(font) {}

        static void	clear(void);
        static void	dump(void);
        void		erase(void);

        Texture		renderCharset(int ch, const Color & col)
        {
            return renderCharset(ch, col, -1, -1, -1);
        }

        Texture		renderCharset(int ch, const Color &, int blend, int style, int hinting);
    };

#ifndef DISABLE_TTF
    class FontRenderTTF : public FontRender
    {
        std::shared_ptr<TTF_Font> ptr;
        FontID		fid;

        Size		char1Size(int ch) const;
        Size		char2Size(int ch) const;

    public:
        FontRenderTTF() {}
        FontRenderTTF(const std::string &, int size, int blend = RenderSolid, int style = StyleNormal, int hinting = HintingNormal);
        FontRenderTTF(const BinaryBuf &, int size, int blend = RenderSolid, int style = StyleNormal, int hinting = HintingNormal);
        ~FontRenderTTF();

        void        	reset(void);

        bool        	open(const std::string &, int, int blend = RenderSolid, int style = StyleNormal, int hinting = HintingNormal);
        bool        	load(const BinaryBuf &, int, int blend = RenderSolid, int style = StyleNormal, int hinting = HintingNormal);

        const FontID &	id(void) const override;
        bool		isValid(void) const override;
        bool		isTTF(void) const override;

        TTF_Font*	toSDLFont(void) const;

        Size		stringSize(const std::string &, bool horizontal = true) const override;
        Size		unicodeSize(const UnicodeString &, bool horizontal = true) const override;

        int		symbolAdvance(int sym) const override;
        int		lineSkipHeight(void) const override;

        Surface	renderCharset(int, const Color &, int blend = -1, int style = -1, int hinting = -1) const override;
        // render ttf string
        Surface	renderString(const std::string &, const Color &, int blend = -1, int style = -1, int hinting = -1) const;
        Surface	renderUnicode(const UnicodeString &, const Color &, int blend = -1, int style = -1, int hinting = -1) const;
    };
#endif

    class FontRenderPSF : public FontRender
    {
        BinaryBuf	buf;
        FontID		fid;

        Size		fixedSize(size_t, bool) const;

    public:
        FontRenderPSF(const u8*, size_t, const Size &);
        FontRenderPSF(const std::string &, const Size &);

        const FontID &
        id(void) const override
        {
            return fid;
        }

        bool isValid(void) const override
        {
            return buf.size();
        }

        bool isTTF(void) const override
        {
            return false;
        }

        Size		stringSize(const std::string &, bool horizontal = true) const override;
        Size		unicodeSize(const UnicodeString &, bool horizontal = true) const override;

        int		symbolAdvance(int sym) const override;
        int		lineSkipHeight(void) const override;

        Surface	renderCharset(int, const Color &, int blend = -1, int style = -1, int hinting = -1) const override;
    };

    class FontAltC8x16 : public FontRenderPSF
    {
    public:
        FontAltC8x16();
    };

#define FontRenderSystem	FontAltC8x16

    const FontRenderSystem & systemFont(void);

} // SWE
#endif

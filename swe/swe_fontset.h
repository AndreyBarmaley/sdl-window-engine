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

/// @brief пространство SWE
namespace SWE
{

    /// @enum SWE::AlignType
    /// @brief перечисление типа выравнивания
    enum AlignType
    {
        AlignLeft,      ///< по левому краю
        AlignRight,     ///< по правому краю
        AlignTop,       ///< по верхнему краю
        AlignBottom,    ///< по нижнему краю
        AlignCenter     ///< по центру
    };

    struct CharsetID;

#if (SDL_VERSIONNUM(SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION, SDL_TTF_PATCHLEVEL) <= SDL_VERSIONNUM(2, 0, 10))
#define TTF_STYLE_STRIKETHROUGH 0x08
#define TTF_HINTING_NORMAL 0
#define TTF_HINTING_LIGHT  1
#define TTF_HINTING_MONO   2
#define TTF_HINTING_NONE   3
#endif

#ifdef SWE_DISABLE_TTF
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

    /// @enum SWE::CharRender
    /// @brief перечисление типа рендера
    enum CharRender
    {
        RenderDefault = -1,     ///< по умолчанию (используется значение заданное при инициализации)
        RenderSolid = 0,        ///< тип solid
        RenderBlended = 1,      ///< тип blended
        RenderShaded = 2        ///< тип shaded
    };

    /// @enum SWE::CharStyle
    /// @brief перечисление типа стиля шрифта
    enum CharStyle
    {
        StyleDefault = -1,                              ///< по умолчанию (используется значение заданное при инициализации)
        StyleNormal = TTF_STYLE_NORMAL,                 ///< стиль normal
        StyleBold = TTF_STYLE_BOLD,                     ///< стиль bold
        StyleItalic = TTF_STYLE_ITALIC,                 ///< стиль italic
        StyleUnderLine = TTF_STYLE_UNDERLINE,           ///< стиль under line
        StyleStrikeThrough = TTF_STYLE_STRIKETHROUGH    ///< стиль strike through
    };

    /// @enum SWE::CharHinting
    /// @brief перечисление типа сглаживания контура
    enum CharHinting
    {
        HintingDefault = -1,                            ///< по умолчанию (используется значение заданное при инициализации)
        HintingNormal = TTF_HINTING_NORMAL,             ///< тип normal
        HintingLight = TTF_HINTING_LIGHT,               ///< тип light
        HintingMono = TTF_HINTING_MONO,                 ///< тип mono
        HintingNone = TTF_HINTING_NONE                  ///< тип none
    };

    /// @brief свойства отрисовка символа
    struct CharProperty
    {
        /* blended 2 bit, style 4 bit, hinting 2 bit */
        u8			val;

        CharProperty(const CharRender & = RenderSolid, int style = StyleNormal, const CharHinting & = HintingNormal);

        int			operator()(void) const;
        CharRender		render(void) const;
        int			style(void) const;
        CharHinting		hinting(void) const;
        void			reset(void);
        void			setRender(const CharRender &);
        void			setStyle(int);
        void			setHinting(const CharHinting &);

	bool			operator<(const CharProperty & cp) const;
	bool			operator!=(const CharProperty &) const;
    };

    /// @brief класс идентификации шрифта
    struct FontID
    {
	u16			val1;
	u8			val2;
	CharProperty		val3;

        /* font id 16 bit, font size 8 bit, charset property 8 bit */
        FontID() : val1(0), val2(0) {}
        FontID(int id, int sz, const CharProperty & cp = CharProperty());

        int			operator()(void) const;
        int			value(void) const;
	bool			operator<(const FontID &) const;
	bool			operator>(const FontID &) const;
	bool			operator==(const FontID &) const;

        int 			id(void) const;
        int 			size(void) const;

        const CharProperty & property(void) const;

        void 			reset(void);
        void 			setId(int v);
        void 			setSize(int v);
        void 			setProperty(const CharProperty &);
    };

    /// @brief базовый класс шрифта
    class FontRender
    {
    protected:
        Size			fontSize;

    public:
        FontRender() {}
        FontRender(const Size & fsz) : fontSize(fsz) {}
        virtual ~FontRender() {}

        virtual const FontID &	id(void) const = 0;
        virtual bool		isValid(void) const = 0;
        virtual bool		isTTF(void) const = 0;

        virtual Size		stringSize(const std::string &, bool horizontal = true) const = 0;
        virtual Size		unicodeSize(const UnicodeString &, bool horizontal = true) const = 0;

        virtual int		symbolAdvance(int = 0x20) const = 0;
        virtual int		lineSkipHeight(void) const = 0;

        virtual Surface		renderCharset(int, const Color &, const CharRender & = RenderDefault, int style = StyleDefault, const CharHinting & = HintingDefault) const = 0;

        //
        UCStringList		splitUCStringWidth(const UCString &, int) const;
        UnicodeList		splitUnicodeWidth(const UnicodeString &, int) const;
        StringList		splitStringWidth(const std::string &, int) const;

        const Size &		size(void) const
        {
            return fontSize;
        }

        static void		clearCache(void);
        static void		dumpCache(void);

        void 			renderString(const std::string &, const Color &, const Point &, Surface &) const;
    };

    /// @brief класс кеширования спрайтов символов
    class FontsCache
    {
        const FontRender*	render;

    public:
        FontsCache(const FontRender* font = nullptr) : render(font) {}

        static void		clear(void);
        static void		dump(void);
        void			erase(void);

        Texture			renderCharset(int ch, const Color & col)
        {
            return renderCharset(ch, col, RenderDefault, StyleDefault, HintingDefault);
        }

        Texture			renderCharset(int ch, const Color &, const CharRender &, int style, const CharHinting &);
    };

#ifndef SWE_DISABLE_TTF
    /// @brief базовый класс рендера TTF шрифта
    class FontRenderTTF : public FontRender
    {
        std::shared_ptr<TTF_Font> ptr;
        FontID			fid;
        Color                   shaded;

        Size			char1Size(int ch) const;
        Size			char2Size(int ch) const;

    public:
        FontRenderTTF() : ptr{nullptr, TTF_CloseFont} {}
        FontRenderTTF(const std::string &, size_t fsz, const CharRender & = RenderSolid, int style = StyleNormal, const CharHinting & = HintingNormal);
        FontRenderTTF(const BinaryBuf &, size_t fsz, const CharRender & = RenderSolid, int style = StyleNormal, const CharHinting & = HintingNormal);
        ~FontRenderTTF();

        void                    setShadedBackground(const Color &);
        void        		reset(void);

        bool        		open(const std::string &, size_t fsz, const CharRender & = RenderSolid, int style = StyleNormal, const CharHinting & = HintingNormal);
        bool        		load(const BinaryBuf &, size_t fsz, const CharRender & = RenderSolid, int style = StyleNormal, const CharHinting & = HintingNormal);

        const FontID &		id(void) const override;
        bool			isValid(void) const override;
        bool			isTTF(void) const override;

        TTF_Font*		toSDLFont(void) const;

        Size			stringSize(const std::string &, bool horizontal = true) const override;
        Size			unicodeSize(const UnicodeString &, bool horizontal = true) const override;

        int			symbolAdvance(int sym) const override;
        int			lineSkipHeight(void) const override;

        Surface			renderCharset(int, const Color &, const CharRender & = RenderDefault, int style = StyleDefault, const CharHinting & = HintingDefault) const override;
        // render ttf string
        Surface			renderString(const std::string &, const Color &, const CharRender & = RenderDefault, int style = StyleDefault, const CharHinting & = HintingDefault) const;
        Surface			renderUnicode(const UnicodeString &, const Color &, const CharRender & = RenderDefault, int style = StyleDefault, const CharHinting & = HintingDefault) const;
    };
#endif

    /// @brief базовый класс рендера PSF шрифта
    class FontRenderPSF : public FontRender
    {
        BinaryBuf		buf;
        FontID			fid;

        Size			fixedSize(size_t, bool) const;

    public:
        FontRenderPSF(const u8*, size_t, const Size &);
        FontRenderPSF(const std::string &, const Size &);

        const FontID &		id(void) const override
        {
            return fid;
        }

        bool 			isValid(void) const override
        {
            return buf.size();
        }

        bool 			isTTF(void) const override
        {
            return false;
        }

        Size			stringSize(const std::string &, bool horizontal = true) const override;
        Size			unicodeSize(const UnicodeString &, bool horizontal = true) const override;

        int			symbolAdvance(int sym) const override;
        int			lineSkipHeight(void) const override;

        Surface			renderCharset(int, const Color &, const CharRender & = RenderDefault, int style = StyleDefault, const CharHinting & = HintingDefault) const override;
    };

    /// @brief встроенный системный рендер PSF шрифта
    class FontAltC8x16 : public FontRenderPSF
    {
    public:
        FontAltC8x16();
    };

#define FontRenderSystem	FontAltC8x16

    /// @brief альяс на встроенный рендер PSF шрифта
    /// @see FontAltC8x16
    const FontRenderSystem & 	systemFont(void);

} // SWE
#endif

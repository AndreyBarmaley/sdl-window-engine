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

#ifndef _SWE_COLORS_
#define _SWE_COLORS_

#include <string>
#include <utility>

#include "swe_types.h"
#include "swe_tools.h"

/// @brief пространство SWE
namespace SWE
{

    class Color;
    class ColorIndex;

    /// @brief базовый класс цвета
    class ARGB : public packint4
    {
    public:
        ARGB() : packint4(SDL_ALPHA_OPAQUE, 0, 0, 0) {}
        ARGB(int a, int r, int g, int b) : packint4(a, r, g, b) {}
        ARGB(int val) : packint4(val) {}
        ARGB(const std::string &);
	virtual ~ARGB() {}

        int	a(void) const;
        int	r(void) const;
        int	g(void) const;
        int	b(void) const;

        void setA(int v);
        void setR(int v);
        void setG(int v);
        void setB(int v);

        void setARGB(int v);
        int  getARGB(void) const;
        bool hasAlpha(void) const;
    };

    /// @brief класс цвета
    class Color : public ARGB
    {
    public:
	/// @brief набор html цветовых кодов
        enum color_t
        {
            Black = 0, Navy, DarkBlue, MediumBlue, Blue, DarkGreen, Green, Teal, DarkCyan, DeepSkyBlue,
            DarkTurquoise, MediumSpringGreen, Lime, SpringGreen, Aqua, Cyan, MidnightBlue, DodgerBlue, LightSeaGreen, ForestGreen,
            SeaGreen, DarkSlateGray, LimeGreen, MediumSeaGreen, Turquoise, RoyalBlue, SteelBlue, DarkSlateBlue, MediumTurquoise, Indigo,
            DarkOliveGreen, CadetBlue, CornflowerBlue, MediumAquamarine, DimGray, SlateBlue, OliveDrab, SlateGray, LightSlateGray, MediumSlateBlue,
            LawnGreen, Chartreuse, Aquamarine, Maroon, Purple, Olive, Gray, SkyBlue, LightSkyBlue, BlueViolet,
            DarkRed, DarkMagenta, SaddleBrown, DarkSeaGreen, LightGreen, MediumPurple, DarkViolet, PaleGreen, DarkOrchid, Amethyst,
            YellowGreen, Sienna, Brown, DarkGray, LightBlue, GreenYellow, PaleTurquoise, LightSteelBlue, PowderBlue, FireBrick,
            DarkGoldenrod, MediumOrchid, RosyBrown, DarkKhaki, Silver, MediumVioletRed, IndianRed, Peru, Chocolate, Tan,
            LightGrey, Thistle, Orchid, Goldenrod, PaleVioletRed, Crimson, Gainsboro, Plum, BurlyWood, LightCyan,
            Lavender, DarkSalmon, Violet, PaleGoldenrod, LightCoral, Khaki, AliceBlue, Honeydew, Azure, SandyBrown,
            Wheat, Beige, WhiteSmoke, MintCream, GhostWhite, Salmon, AntiqueWhite, Linen, LightGoldenrodYellow, OldLace,
            Red, Fuchsia, Magenta, DeepPink, OrangeRed, Tomato, HotPink, Coral, DarkOrange, LightSalmon,
            Orange, LightPink, Pink, Gold, PeachPuff, NavajoWhite, Moccasin, Bisque, MistyRose, BlanchedAlmond,
            PapayaWhip, LavenderBlush, Seashell, Cornsilk, LemonChiffon, FloralWhite, Snow, Yellow, LightYellow, Ivory,
            White, Transparent /* ColorIndex: 141 */
        };

        Color() {}
        explicit Color(const ARGB & argb) : ARGB(argb) {}
        Color(int r, int g, int b, int a = SDL_ALPHA_OPAQUE) : ARGB(a, r, g, b) {}
#ifdef SWE_SDL12
        Color(const SDL_Color & col) : ARGB(col.unused, col.r, col.g, col.b) {}
#else
        Color(const SDL_Color & col) : ARGB(col.a, col.r, col.g, col.b) {}
#endif
        Color(const std::string & str) : ARGB(str) {}
        Color(int color_t, int a = SDL_ALPHA_OPAQUE);

        SDL_Color	toSDLColor(void) const;
        ColorIndex	toColorIndex(void) const;
        std::string	toString(void) const;
        std::string	toHexString(void) const;

        bool		isTransparent(void) const;
        bool		isColor(color_t) const;

        static Color	   transparent(void);
        static Color	   colorKey(void);
        static const char* name(color_t);
    };

    /// @brief класс индекса цвета
    class ColorIndex
    {
        u32		val;
        bool		isValid(void) const;

    public:
        ColorIndex(int color_t = Color::Black);

        u32		operator()(void) const;
        ColorIndex &	operator= (int v);

        u32		index(void) const;

        bool            operator< (const ColorIndex &) const;
        bool            operator> (const ColorIndex &) const;
        bool		operator== (const ColorIndex &) const;
        bool		operator!= (const ColorIndex &) const;

        bool		isTransparent(void) const;
        Color		toColor(void) const;
    };

    /// @brief класс пары двух цветов (foreground, background)
    struct FBColors : packshort
    {
        FBColors(const ColorIndex & fg = Color::Black, const ColorIndex & bg = Color::Transparent);

        int                 bg(void) const;
        int                 fg(void) const;

        Color               bgcolor(void) const;
        Color               fgcolor(void) const;

        void                setbg(const ColorIndex &);
        void                setfg(const ColorIndex &);
    };

} // SWE
#endif

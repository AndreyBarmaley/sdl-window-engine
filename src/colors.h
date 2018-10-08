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

#include "sharedvector.h"
#include "types.h"
#include "tools.h"

class Color;
class ColorIndex;

class ARGB : public packint4
{
public:
    ARGB() : packint4(SDL_ALPHA_OPAQUE, 0, 0, 0) {}
    ARGB(int a, int r, int g, int b) : packint4(a, r, g, b) {}
    ARGB(int val) : packint4(val) {}
    ARGB(const std::string &);

    int	a(void) const { return val1(); }
    int	r(void) const { return val2(); }
    int	g(void) const { return val3(); }
    int	b(void) const { return val4(); }

    void setA(int v) { set1(v); }
    void setR(int v) { set2(v); }
    void setG(int v) { set3(v); }
    void setB(int v) { set4(v); }

    void setARGB(int v) { setvalue(v); }
    int  getARGB(void) const { return value(); }

    bool hasAlpha(void) const { return a() != SDL_ALPHA_OPAQUE; }
};

class Color : public ARGB
{
public:
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
#ifdef OLDENGINE
    Color(const SDL_Color & col) : ARGB(col.unused, col.r, col.g, col.b) {}
#else
    Color(const SDL_Color & col) : ARGB(col.a, col.r, col.g, col.b) {}
#endif
    Color(const std::string & str) : ARGB(str) {}
    Color(int color_t, int a = SDL_ALPHA_OPAQUE);

    SDL_Color		toSDLColor(void) const;
    ColorIndex		toColorIndex(void) const;
    std::string		toString(void) const;
    std::string		toHexString(void) const;

    bool isTransparent(void) const { return *this == transparent(); }

    static Color	transparent(void) { return Color(0, 0, 0, SDL_ALPHA_TRANSPARENT); }
    static Color	colorKey(void) { return Color(254, 0, 254); }
};

class ColorIndex
{
    u32			val;
    bool		isValid(void) const;

public:
    ColorIndex(int color_t = Color::Black);

    u32			operator() (void) const { return val; }
    ColorIndex &	operator=  (int v) { val = v; return *this; }
    u32			index(void) const { return val; }

    bool                operator<  (const ColorIndex & col) const { return index() < col.index(); }
    bool                operator>  (const ColorIndex & col) const { return index() > col.index(); }
    bool		operator== (const ColorIndex & col) const { return index() == col.index(); }
    bool		operator!= (const ColorIndex & col) const { return index() != col.index(); }

    bool		isTransparent(void) const { return Color::Transparent == val; }

    Color		toColor(void) const { return Color(val); }
};

struct FBColors : packshort
{
    FBColors(const ColorIndex & fg = Color::Black, const ColorIndex & bg = Color::Transparent) : packshort(bg(), fg()) {}

    int                 bg(void) const { return val1(); }
    int                 fg(void) const { return val2(); }

    Color               bgcolor(void) const { return Color(bg()); }
    Color               fgcolor(void) const { return Color(fg()); }

    void                setbg(const ColorIndex & col) { set1(col()); }
    void                setfg(const ColorIndex & col) { set2(col()); }
};

#endif

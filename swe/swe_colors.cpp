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

#include <array>
#include <sstream>
#include <algorithm>
#include <iomanip>

#include "swe_types.h"
#include "swe_cstring.h"
#include "swe_colors.h"

namespace SWE
{
    struct colname_t
    {
	int         color;
	const char* name;
    };

    std::array<colname_t, Color::Transparent + 1> _colors = { {
        { 0x00000000, "black" }, { 0x00000080, "navy" }, { 0x0000008b, "darkblue" }, { 0x000000cd, "mediumblue" }, { 0x000000ff, "blue" }, { 0x00006400, "darkgreen" }, { 0x00008000, "green" }, { 0x00008080, "teal" }, { 0x00008b8b, "darkcyan" }, { 0x0000bfff, "deepskyblue" },
        { 0x0000ced1, "darkturquoise" }, { 0x0000fa9a, "mediumspringgreen" }, { 0x0000ff00, "lime" }, { 0x0000ff7f, "springgreen" }, { 0x0000fffe, "aqua" }, { 0x0000ffff, "cyan" }, { 0x00191970, "midnightblue" }, { 0x001e90ff, "dodgerblue" }, { 0x0020b2aa, "lightseagreen" }, { 0x00228b22, "forestgreen" },
        { 0x002e8b57, "seagreen" }, { 0x002f4f4f, "darkslategray" }, { 0x0032cd32, "limegreen" }, { 0x003cb371, "mediumseagreen" }, { 0x0040e0d0, "turquoise" }, { 0x004169e1, "royalblue" }, { 0x004682b4, "steelblue" }, { 0x00483d8b, "darkslateblue" }, { 0x0048d1cc, "mediumturquoise" }, { 0x004b0082, "indigo" },
        { 0x00556b2f, "darkolivegreen" }, { 0x005f9ea0, "cadetblue" }, { 0x006495ed, "cornflowerblue" }, { 0x0066cdaa, "mediumaquamarine" }, { 0x00696969, "dimgray" }, { 0x006a5acd, "slateblue" }, { 0x006b8e23, "olivedrab" }, { 0x00708090, "slategray" }, { 0x00778899, "lightslategray" }, { 0x007b68ee, "mediumslateblue" },
        { 0x007cfc00, "lawngreen" }, { 0x007fff00, "chartreuse" }, { 0x007fffd4, "aquamarine" }, { 0x00800000, "maroon" }, { 0x00800080, "purple" }, { 0x00808000, "olive" }, { 0x00808080, "gray" }, { 0x0087ceeb, "skyblue" }, { 0x0087cefa, "lightskyblue" }, { 0x008a2be2, "blueviolet" },
        { 0x008b0000, "darkred" }, { 0x008b008b, "darkmagenta" }, { 0x008b4513, "saddlebrown" }, { 0x008fbc8f, "darkseagreen" }, { 0x0090ee90, "lightgreen" }, { 0x009370db, "mediumpurple" }, { 0x009400d3, "darkviolet" }, { 0x0098fb98, "palegreen" }, { 0x009932cc, "darkorchid" }, { 0x009966cc, "amethyst" },
        { 0x009acd32, "yellowgreen" }, { 0x00a0522d, "sienna" }, { 0x00a52a2a, "brown" }, { 0x00a9a9a9, "darkgray" }, { 0x00add8e6, "lightblue" }, { 0x00adff2f, "greenyellow" }, { 0x00afeeee, "paleturquoise" }, { 0x00b0c4de, "lightsteelblue" }, { 0x00b0e0e6, "powderblue" }, { 0x00b22222, "firebrick" },
        { 0x00b8860b, "darkgoldenrod" }, { 0x00ba55d3, "mediumorchid" }, { 0x00bc8f8f, "rosybrown" }, { 0x00bdb76b, "darkkhaki" }, { 0x00c0c0c0, "silver" }, { 0x00c71585, "mediumvioletred" }, { 0x00cd5c5c, "indianred" }, { 0x00cd853f, "peru" }, { 0x00d2691e, "chocolate" }, { 0x00d2b48c, "tan" },
        { 0x00d3d3d3, "lightgrey" }, { 0x00d8bfd8, "thistle" }, { 0x00da70d6, "orchid" }, { 0x00daa520, "goldenrod" }, { 0x00db7093, "palevioletred" }, { 0x00dc143c, "crimson" }, { 0x00dcdcdc, "gainsboro" }, { 0x00dda0dd, "plum" }, { 0x00deb887, "burlywood" }, { 0x00e0ffff, "lightcyan" },
        { 0x00e6e6fa, "lavender" }, { 0x00e9967a, "darksalmon" }, { 0x00ee82ee, "violet" }, { 0x00eee8aa, "palegoldenrod" }, { 0x00f08080, "lightcoral" }, { 0x00f0e68c, "khaki" }, { 0x00f0f8ff, "aliceblue", }, {0x00f0fff0, "honeydew" }, { 0x00f0ffff, "azure" }, { 0x00f4a460, "sandybrown" },
        { 0x00f5deb3, "wheat" }, { 0x00f5f5dc, "beige" }, { 0x00f5f5f5, "whitesmoke" }, { 0x00f5fffa, "mintcream" }, { 0x00f8f8ff, "ghostwhite" }, { 0x00fa8072, "salmon" }, { 0x00faebd7, "antiquewhite" }, { 0x00faf0e6, "linen" }, { 0x00fafad2, "lightgoldenrodyellow" }, { 0x00fdf5e6, "oldlace" },
        { 0x00ff0000, "red" }, { 0x00ff00ff, "fuchsia" }, { 0x00ff01ff, "magenta" }, { 0x00ff1493, "deeppink" }, { 0x00ff4500, "orangered" }, { 0x00ff6347, "tomato" }, { 0x00ff69b4, "hotpink" }, { 0x00ff7f50, "coral" }, { 0x00ff8c00, "darkorange" }, { 0x00ffa07a, "lightsalmon" },
        { 0x00ffa500, "orange" }, { 0x00ffb6c1, "lightpink" }, { 0x00ffc0cb, "pink" }, { 0x00ffd700, "gold" }, { 0x00ffdab9, "peachpuff" }, { 0x00ffdead, "navajowhite" }, { 0x00ffe4b5, "moccasin" }, { 0x00ffe4c4, "bisque" }, { 0x00ffe4e1, "mistyrose" }, { 0x00ffebcd, "blanchedalmond" },
        { 0x00ffefd5, "papayawhip" }, { 0x00fff0f5, "lavenderblush" }, { 0x00fff5ee, "seashell" }, { 0x00fff8dc, "cornsilk" }, { 0x00fffacd, "lemonchiffon" }, { 0x00fffaf0, "floralwhite" }, { 0x00fffafa, "snow" }, { 0x00ffff00, "yellow" }, { 0x00ffffe0, "lightyellow" }, { 0x00fffff0, "ivory" },
        { 0x00ffffff, "white" }, { -1, "transparent" }
    } };

    std::vector<ARGB> others_argb;

    ARGB colors2argb(int colors)
    {
        // transparent: -1
        int opaque = SDL_ALPHA_OPAQUE;
        return colors < 0 ? Color::transparent() : ARGB((opaque << 24) | (0x00FFFFFF & colors));
    }

    int argb2colors(const ARGB & argb)
    {
        // transparent: -1
        return argb != Color::transparent() ? 0x00FFFFFF & argb.value() : -1;
    }

    int names2index(const std::string & str)
    {
        auto it = std::find_if(_colors.begin(), _colors.end(),
			    [&](const colname_t & item){ return String::compareInSensitive(str, item.name); });
        return it != _colors.end() ? std::distance(_colors.begin(), it) : -1;
    }

    int colors2index(int color)
    {
        auto it = std::find_if(_colors.begin(), _colors.end(),
			    [=](const colname_t & item){ return item.color == color; });
        return it != _colors.end() ? std::distance(_colors.begin(), it) : -1;
    }

    ARGB index2argb(int index)
    {
        if(index == Color::Transparent)
    	    return Color::transparent();

        if(0 <= index && index < static_cast<int>(_colors.size()))
            return ARGB(colors2argb(_colors[index].color));

	index -= _colors.size();
        if(0 <= index && index < static_cast<int>(others_argb.size()))
            return others_argb[index];

        ERROR("unknown color index: " << index);
        return Color::transparent();
    }

    int argb2index(const ARGB & argb)
    {
        int colors = argb2colors(argb);
        int index = colors2index(colors);

        if(0 > index)
        {
            auto it = std::find(others_argb.begin(), others_argb.end(), argb);

            if(it != others_argb.end())
                index = Color::Transparent + std::distance(others_argb.begin(), it) + 1;
            else
            {
                others_argb.push_back(argb);
                index = Color::Transparent + others_argb.size();
            }
        }

        return index;
    }

    ARGB::ARGB(const std::string & str) : packint4(SDL_ALPHA_OPAQUE, 0, 0, 0)
    {
        if(str.size())
        {
            ARGB argb;

            if(str[0] == '0' && str[1] == 'x')
                argb = colors2argb(String::toInt(str));
            else

                // "#RRGGBB"
                if(str[0] == '#')
                {
                    std::ostringstream os;
                    os << "0x" << str.substr(1, str.size() - 1);
                    argb = colors2argb(String::toInt(os.str()));
                }
                else
                {
                    int index = names2index(str);
                    argb = index2argb(index);
                }

            *this = argb;
        }
    }

    int ARGB::a(void) const
    {
        return val1();
    }

    int ARGB::r(void) const
    {
        return val2();
    }

    int ARGB::g(void) const
    {
        return val3();
    }

    int ARGB::b(void) const
    {
        return val4();
    }

    void ARGB::setA(int v)
    {
        set1(v);
    }

    void ARGB::setR(int v)
    {
        set2(v);
    }

    void ARGB::setG(int v)
    {
        set3(v);
    }

    void ARGB::setB(int v)
    {
        set4(v);
    }

    void ARGB::setARGB(int v)
    {
        setvalue(v);
    }

    int  ARGB::getARGB(void) const
    {
        return value();
    }

    bool ARGB::hasAlpha(void) const
    {
        return a() != SDL_ALPHA_OPAQUE;
    }

    Color::Color(int color_t, int a) : ARGB(index2argb(color_t))
    {
        if(a != SDL_ALPHA_OPAQUE) setA(a);
    }

    SDL_Color Color::toSDLColor(void) const
    {
        SDL_Color res;
        res.b = b();
        res.g = g();
        res.r = r();
#ifdef SWE_SDL12
        res.unused = a();
#else
        res.a = a();
#endif
        return res;
    }

    ColorIndex Color::toColorIndex(void) const
    {
        return ColorIndex(argb2index(*this));
    }

    std::string Color::toHexString(void) const
    {
        return String::hex(value());
    }

    std::string Color::toString(void) const
    {
        ColorIndex col = toColorIndex();

        if(col.index() < _colors.size())
            return _colors[col.index()].name;
        else if(hasAlpha())
            return toHexString();

        std::ostringstream os;
        os << "#" << std::hex << std::uppercase <<
           std::setw(2) << std::setfill('0') << r() <<
           std::setw(2) << std::setfill('0') << g() <<
           std::setw(2) << std::setfill('0') << b();
        return os.str();
    }

    const char* Color::name(color_t col)
    {
        return _colors[col].name;
    }

    bool Color::isColor(color_t col) const
    {
        return col == argb2index(*this);
    }

    bool Color::isTransparent(void) const
    {
        return *this == transparent();
    }

    Color Color::transparent(void)
    {
        return Color(0, 0, 0, SDL_ALPHA_TRANSPARENT);
    }

    Color Color::colorKey(void)
    {
        return Color(254, 0, 254);
    }

    /* ColorIndex */
    ColorIndex::ColorIndex(int color_t) : val(color_t)
    {
        if(! isValid())
        {
            ERROR("unknown color index: " << val);
            val = Color::Black;
        }
    }

    bool ColorIndex::isValid(void) const
    {
        return Color::Black <= val && val <= Color::Transparent + others_argb.size();
    }

    u32 ColorIndex::operator()(void) const
    {
        return val;
    }

    ColorIndex & ColorIndex::operator= (int v)
    {
        val = v;
        return *this;
    }

    u32 ColorIndex::index(void) const
    {
        return val;
    }

    bool ColorIndex::operator< (const ColorIndex & col) const
    {
        return index() < col.index();
    }

    bool ColorIndex::operator> (const ColorIndex & col) const
    {
        return index() > col.index();
    }

    bool ColorIndex::operator== (const ColorIndex & col) const
    {
        return index() == col.index();
    }

    bool ColorIndex::operator!= (const ColorIndex & col) const
    {
        return index() != col.index();
    }

    bool ColorIndex::isTransparent(void) const
    {
        return Color::Transparent == val;
    }

    Color ColorIndex::toColor(void) const
    {
        return Color(val);
    }

    /* FBColors */
    FBColors::FBColors(const ColorIndex & fg, const ColorIndex & bg) : packshort(bg(), fg())
    {
    }

    int FBColors::bg(void) const
    {
        return val1();
    }

    int FBColors::fg(void) const
    {
        return val2();
    }

    Color FBColors::bgcolor(void) const
    {
        return Color(bg());
    }

    Color FBColors::fgcolor(void) const
    {
        return Color(fg());
    }

    void FBColors::setbg(const ColorIndex & col)
    {
        set1(col());
    }

    void FBColors::setfg(const ColorIndex & col)
    {
        set2(col());
    }
}

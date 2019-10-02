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

#include <sstream>
#include <algorithm>
#include <iterator>
#include <iomanip>

#include "types.h"
#include "cstring.h"
#include "colors.h"

namespace
{
    const int _colors[] = {
        0x00000000 /* Black */, 0x00000080 /* Navy */, 0x0000008B /* DarkBlue */, 0x000000CD /* MediumBlue */, 0x000000FF /* Blue */, 0x00006400 /* DarkGreen */, 0x00008000 /* Green */, 0x00008080 /* Teal */, 0x00008B8B /* DarkCyan */, 0x0000BFFF /* DeepSkyBlue */,
        0x0000CED1 /* DarkTurquoise */, 0x0000FA9A /* MediumSpringGreen */, 0x0000FF00 /* Lime */, 0x0000FF7F /* SpringGreen */, 0x0000FFFE /* Aqua */, 0x0000FFFF /* Cyan */, 0x00191970 /* MidnightBlue */, 0x001E90FF /* DodgerBlue */, 0x0020B2AA /* LightSeaGreen */, 0x00228B22 /* ForestGreen */,
        0x002E8B57 /* SeaGreen */, 0x002F4F4F /* DarkSlateGray */, 0x0032CD32 /* LimeGreen */, 0x003CB371 /* MediumSeaGreen */, 0x0040E0D0 /* Turquoise */, 0x004169E1 /* RoyalBlue */, 0x004682B4 /*SteelBlue */, 0x00483D8B /* DarkSlateBlue */, 0x0048D1CC /* MediumTurquoise */, 0x004B0082 /* Indigo */,
        0x00556B2F /* DarkOliveGreen */, 0x005F9EA0 /* CadetBlue */, 0x006495ED /* CornflowerBlue */, 0x0066CDAA /* MediumAquamarine */, 0x00696969 /* DimGray */, 0x006A5ACD /* SlateBlue */, 0x006B8E23 /* OliveDrab */, 0x00708090 /* SlateGray */, 0x00778899 /* LightSlateGray */, 0x007B68EE /* MediumSlateBlue */,
        0x007CFC00 /* LawnGreen */, 0x007FFF00 /* Chartreuse */, 0x007FFFD4 /* Aquamarine */, 0x00800000 /* Maroon */, 0x00800080 /* Purple */, 0x00808000 /* Olive */, 0x00808080 /* Gray */, 0x0087CEEB /* SkyBlue */, 0x0087CEFA /* LightSkyBlue */, 0x008A2BE2 /* BlueViolet */,
        0x008B0000 /* DarkRed */, 0x008B008B /* DarkMagenta */, 0x008B4513 /* SaddleBrown */, 0x008FBC8F /* DarkSeaGreen */, 0x0090EE90 /* LightGreen */, 0x009370DB /* MediumPurple */, 0x009400D3 /* DarkViolet */, 0x0098FB98 /* PaleGreen */, 0x009932CC /* DarkOrchid */, 0x009966CC /* Amethyst */,
        0x009ACD32 /* YellowGreen */, 0x00A0522D /* Sienna */, 0x00A52A2A /* Brown */, 0x00A9A9A9 /* DarkGray */, 0x00ADD8E6 /* LightBlue */, 0x00ADFF2F /* GreenYellow */, 0x00AFEEEE /* PaleTurquoise */, 0x00B0C4DE /* LightSteelBlue */, 0x00B0E0E6 /* PowderBlue */, 0x00B22222 /* FireBrick */,
        0x00B8860B /* DarkGoldenrod */, 0x00BA55D3 /* MediumOrchid */, 0x00BC8F8F /* RosyBrown */, 0x00BDB76B /* DarkKhaki */, 0x00C0C0C0 /* Silver */, 0x00C71585 /* MediumVioletRed */, 0x00CD5C5C /* IndianRed */, 0x00CD853F /* Peru */, 0x00D2691E /* Chocolate */, 0x00D2B48C /* Tan */,
        0x00D3D3D3 /* LightGrey */, 0x00D8BFD8 /* Thistle */, 0x00DA70D6 /* Orchid */, 0x00DAA520 /* Goldenrod */, 0x00DB7093 /* PaleVioletRed */, 0x00DC143C /* Crimson */, 0x00DCDCDC /* Gainsboro */, 0x00DDA0DD /* Plum */, 0x00DEB887 /* BurlyWood */, 0x00E0FFFF /* LightCyan */,
        0x00E6E6FA /* Lavender */, 0x00E9967A /* DarkSalmon */, 0x00EE82EE /* Violet */, 0x00EEE8AA /* PaleGoldenrod */, 0x00F08080 /* LightCoral */, 0x00F0E68C /* Khaki */, 0x00F0F8FF /* AliceBlue*/, 0x00F0FFF0 /* Honeydew */, 0x00F0FFFF /* Azure */, 0x00F4A460 /* SandyBrown */,
        0x00F5DEB3 /* Wheat */, 0x00F5F5DC /* Beige */, 0x00F5F5F5 /* WhiteSmoke */, 0x00F5FFFA /* MintCream */, 0x00F8F8FF /* GhostWhite */, 0x00FA8072 /* Salmon */, 0x00FAEBD7 /* AntiqueWhite */,0x00FAF0E6 /* Linen */, 0x00FAFAD2 /* LightGoldenrodYellow */, 0x00FDF5E6 /* OldLace */,
        0x00FF0000 /* Red */, 0x00FF00FF /* Fuchsia */, 0x00FF01FF /* Magenta */, 0x00FF1493 /* DeepPink */, 0x00FF4500 /* OrangeRed */, 0x00FF6347 /* Tomato */, 0x00FF69B4 /* HotPink */, 0x00FF7F50 /* Coral */, 0x00FF8C00 /* DarkOrange */, 0x00FFA07A /* LightSalmon */,
        0x00FFA500 /* Orange */, 0x00FFB6C1 /* LightPink */, 0x00FFC0CB /* Pink */, 0x00FFD700 /* Gold */, 0x00FFDAB9 /* PeachPuff */, 0x00FFDEAD /* NavajoWhite */, 0x00FFE4B5 /* Moccasin */, 0x00FFE4C4 /* Bisque */, 0x00FFE4E1 /* MistyRose */, 0x00FFEBCD /* BlanchedAlmond */,
        0x00FFEFD5 /* PapayaWhip */, 0x00FFF0F5 /* LavenderBlush */, 0x00FFF5EE /* Seashell */, 0x00FFF8DC /* Cornsilk */, 0x00FFFACD /* LemonChiffon */, 0x00FFFAF0 /* FloralWhite */, 0x00FFFAFA /*Snow */, 0x00FFFF00 /* Yellow */, 0x00FFFFE0 /* LightYellow */, 0x00FFFFF0 /* Ivory */,
        0x00FFFFFF /* White */, -1 /* Transparent */ };

    const char* _names[] = {
	"Black", "Navy", "DarkBlue", "MediumBlue", "Blue", "DarkGreen", "Green", "Teal", "DarkCyan", "DeepSkyBlue",
        "DarkTurquoise", "MediumSpringGreen", "Lime", "SpringGreen", "Aqua", "Cyan", "MidnightBlue", "DodgerBlue", "LightSeaGreen", "ForestGreen",
        "SeaGreen", "DarkSlateGray", "LimeGreen", "MediumSeaGreen", "Turquoise", "RoyalBlue", "SteelBlue", "DarkSlateBlue", "MediumTurquoise", "Indigo",
        "DarkOliveGreen", "CadetBlue", "CornflowerBlue", "MediumAquamarine", "DimGray", "SlateBlue", "OliveDrab", "SlateGray", "LightSlateGray", "MediumSlateBlue",
        "LawnGreen", "Chartreuse", "Aquamarine", "Maroon", "Purple", "Olive", "Gray", "SkyBlue", "LightSkyBlue", "BlueViolet",
        "DarkRed", "DarkMagenta", "SaddleBrown", "DarkSeaGreen", "LightGreen", "MediumPurple", "DarkViolet", "PaleGreen", "DarkOrchid", "Amethyst",
        "YellowGreen", "Sienna", "Brown", "DarkGray", "LightBlue", "GreenYellow", "PaleTurquoise", "LightSteelBlue", "PowderBlue", "FireBrick",
        "DarkGoldenrod", "MediumOrchid", "RosyBrown", "DarkKhaki", "Silver", "MediumVioletRed", "IndianRed", "Peru", "Chocolate", "Tan",
        "LightGrey", "Thistle", "Orchid", "Goldenrod", "PaleVioletRed", "Crimson", "Gainsboro", "Plum", "BurlyWood", "LightCyan",
        "Lavender", "DarkSalmon", "Violet", "PaleGoldenrod", "LightCoral", "Khaki", "AliceBlue", "Honeydew", "Azure", "SandyBrown",
        "Wheat", "Beige", "WhiteSmoke", "MintCream", "GhostWhite", "Salmon", "AntiqueWhite", "Linen", "LightGoldenrodYellow", "OldLace",
        "Red", "Fuchsia", "Magenta", "DeepPink", "OrangeRed", "Tomato", "HotPink", "Coral", "DarkOrange", "LightSalmon",
        "Orange", "LightPink", "Pink", "Gold", "PeachPuff", "NavajoWhite", "Moccasin", "Bisque", "MistyRose", "BlanchedAlmond",
        "PapayaWhip", "LavenderBlush", "Seashell", "Cornsilk", "LemonChiffon", "FloralWhite", "Snow", "Yellow", "LightYellow", "Ivory",
        "White", "Transparent" };

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
	auto _names_end = std::end(_names);
	auto _names_its = std::begin(_names);

	while(_names_its != _names_end)
	{
	    if(String::compareInSensitive(*_names_its, str)) break;
	    _names_its++;
	}

	return _names_its != _names_end ? std::distance(std::begin(_names), _names_its) : -1;
    }

    int colors2index(int color)
    {
	auto _colors_end = std::end(_colors);
	auto _colors_its = std::find(std::begin(_colors), _colors_end, color);

	return _colors_its != _colors_end ? std::distance(std::begin(_colors), _colors_its) : -1;
    }

    ARGB index2argb(int index)
    {
	int argbsz = others_argb.size();

	if(Color::Black <= index && index < Color::Transparent)
	    return ARGB(colors2argb(_colors[index]));
	else
	if(index > Color::Transparent && argbsz  >= index - Color::Transparent)
	    return others_argb[index - Color::Transparent - 1];
	else
	if(index != Color::Transparent)
	{
	    ERROR("unknown color index: " << index);
	}

	// default: transparent
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
}

ARGB::ARGB(const std::string & str) : packint4(SDL_ALPHA_OPAQUE, 0, 0, 0)
{
    if(str.size())
    {
	ARGB argb;

	if(str[0] == '0' && str[1] == 'x')
	{
	    argb = colors2argb(String::toInt(str));
	}
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
#ifdef OLDENGINE
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

    if(Color::Black <= col.index() && col.index() <= Color::Transparent)
	return _names[col.index()];
    else
    if(hasAlpha())
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
    return _names[col];
}

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

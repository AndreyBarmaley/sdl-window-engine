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

#include "swe_display.h"
#include "swe_wingui.h"

namespace SWE
{
    /* WindowToolTipArea */
    WindowToolTipArea::WindowToolTipArea(Window* win) : Window(win)
    {
	resetState(FlagModality);
    }

    WindowToolTipArea::WindowToolTipArea(const Size & sz, Window* win) : Window(sz, win)
    {
	resetState(FlagModality);
    }

    WindowToolTipArea::WindowToolTipArea(const Point & pos, const Size & sz, Window* win) : Window(pos, sz, win)
    {
	resetState(FlagModality);
    }

    void WindowToolTipArea::setToolTip(const std::string & str)
    {
        renderToolTip(str, systemFont(), Color::Black, Color::Wheat, Color::MidnightBlue);
    }

    void WindowToolTipArea::setToolTip(const std::string & str, int width)
    {
	setToolTip(systemFont().splitStringWidth(str, width));
    }

    void WindowToolTipArea::setToolTip(const StringList & list)
    {
        renderToolTip(list, systemFont(), Color::Black, Color::Wheat, Color::MidnightBlue);
    }

    void WindowToolTipArea::renderToolTip(const std::string & str, const FontRender & frs, const Color & fncolor, const Color & bgcolor, const Color & rtcolor)
    {
        Texture text = Display::renderText(frs, str, fncolor);
        tooltip = Display::renderRect(rtcolor, bgcolor, text.size() + Size(6, 6));
        Display::renderTexture(text, text.rect(), tooltip, SWE::Rect(Point(3, 3), text.size()));
    }

    void WindowToolTipArea::renderToolTip(const StringList & list, const FontRender & frs, const Color & fncolor, const Color & bgcolor, const Color & rtcolor)
    {
	int max = list.maxStringWidth();
	int offy = 0;
        tooltip = Display::renderRect(rtcolor, bgcolor, SWE::Size(frs.symbolAdvance(0x20) * max, frs.lineSkipHeight() * list.size()) + Size(6, 6));

	for(auto it = list.begin(); it != list.end(); ++it)
	{
    	    Texture text = Display::renderText(frs, *it, fncolor);
    	    Display::renderTexture(text, text.rect(), tooltip, SWE::Rect(Point(3, offy + 3), text.size()));
	    offy += frs.lineSkipHeight();
	}
    }

    const Texture & WindowToolTipArea::tooltipTexture(void) const
    {
        return tooltip;
    }

    /* WindowToolTipIcon */
    WindowToolTipIcon::WindowToolTipIcon(Window* win) : WindowToolTipArea(win)
    {
	resetState(FlagModality);
    }

    WindowToolTipIcon::WindowToolTipIcon(const Point & pos, const Texture & tx, const std::string & str, Window* win)
	: WindowToolTipArea(pos, tx.size(), win), icon(tx)
    {
	resetState(FlagModality);
	setToolTip(str);
	setVisible(true);
    }

    void WindowToolTipIcon::setTexture(const Texture & tx)
    {
	setSize(tx.size());
	icon = tx;
    }

    void WindowToolTipIcon::renderWindow(void)
    {
	renderTexture(icon, Point(0, 0));
    }
}

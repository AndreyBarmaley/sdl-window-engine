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

#ifndef _SWE_GUI_SCROLL_
#define _SWE_GUI_SCROLL_

#include "swe_wingui.h"

/// @brief пространство SWE
namespace SWE
{
    class ListWidget;

    // ScrollBar emit signals: Signal::ScrollBarMoved
    class ScrollBar : public Window
    {
	TexturePos		txcur;

    protected:
        void                    signalReceive(int, const SignalMember*) override;
        void                    windowMoveEvent(const Point &) override;
        void                    windowResizeEvent(const Size &) override;
        bool                    scrollUpEvent(void) override;
        bool                    scrollDownEvent(void) override;
        bool                    mousePressEvent(const ButtonEvent &) override;
        bool                    mouseReleaseEvent(const ButtonEvent &) override;
        bool                    mouseMotionEvent(const Point &, u32 buttons) override;

    public:
        ScrollBar(bool, ListWidget &);
        ScrollBar(const Point &, const Size &, bool vertical, ListWidget &);

	bool			isAreaPoint(const Point &) const override;
        bool                    isVerticalOrientation(void) const;

	const TexturePos &      textureCursor(void) const;
	void			setTextureCursor(const Texture &);

	virtual Rect		scrollArea(void) const;
	void			renderWindow(void) override;

	const char*             className(void) const override { return "SWE::ScrollBar"; }
#ifdef SWE_WITH_JSON
        JsonObject              toJson(void) const override
        {
            JsonObject res = Window::toJson();
            res.addObject("cursor", txcur.toJson());
            return res;
        }
#endif
	static int		transformToListTopRow(const ScrollBar &, const ListWidget &);
	static Point		transformToScrollCursor(const ListWidget &, const ScrollBar &);
	static Point		transformToScrollCursor(const Point &, const ScrollBar &);
    };


} // SWE
#endif

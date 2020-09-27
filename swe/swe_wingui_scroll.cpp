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

#include <algorithm>
#include <iterator>

#include "swe_wingui_list.h"
#include "swe_wingui_scroll.h"

namespace SWE
{
    int ScrollBar::transformToListTopRow(const ScrollBar & bar, const ListWidget & list)
    {
        int visible = list.visibleItems();

        if(0 < list.count() && visible < list.count())
        {
	    Rect scrollArea = bar.scrollArea();
	    Rect cursorArea = bar.txcur.area();
            int scrollCursorPos = bar.isVerticalOrientation() ? cursorArea.y - scrollArea.y : cursorArea.x - scrollArea.x;
            int scrollCursorSize = bar.isVerticalOrientation() ? cursorArea.h : cursorArea.w;
            int scrollAreaSize = bar.isVerticalOrientation() ? scrollArea.h : scrollArea.w;

            if(scrollAreaSize > scrollCursorSize)
            {
	        int res = (list.count() - visible) * scrollCursorPos / (scrollAreaSize - scrollCursorSize);
		if(res > list.count() - visible)
		    res = list.count() - visible;
		return res;
	    }
        }

        return -1;
    }

    Point ScrollBar::transformToScrollCursor(const ListWidget & list, const ScrollBar & bar)
    {
        int visible = list.visibleItems();
        Rect scrollArea = bar.scrollArea();
	Size cursorSize = bar.txcur.size();
        Point res = scrollArea.toPoint() + (scrollArea.toSize() - cursorSize) / 2;

        if(0 < list.count() && visible < list.count())
        {
            if(bar.isVerticalOrientation())
	    {
        	res.y = scrollArea.y + list.rowTop() * (scrollArea.h - cursorSize.h) / (list.count() - visible);

		if(res.y > scrollArea.y + scrollArea.h - cursorSize.h)
            	    res.y = scrollArea.y + scrollArea.h - cursorSize.h;
		else
		if(res.y < scrollArea.y)
		    res.y = scrollArea.y;
	    }
            else
	    {
                res.x = scrollArea.x + list.rowTop() * (scrollArea.w - cursorSize.w) / (list.count() - visible);

		if(res.x > scrollArea.x + scrollArea.w - cursorSize.w)
            	    res.x = scrollArea.x + scrollArea.w - cursorSize.w;
		else
		if(res.x < scrollArea.x)
		    res.x = scrollArea.x;
	    }
        }

        return res;
    }

    Point ScrollBar::transformToScrollCursor(const Point & pos, const ScrollBar & bar)
    {
        Rect scrollArea = bar.scrollArea();
	Size cursorSize = bar.txcur.size();
        Point res = bar.txcur.position();

        if(bar.isVerticalOrientation())
	{
	    res.y = pos.y;

	    if(res.y > scrollArea.y + scrollArea.h - cursorSize.h)
            	res.y = scrollArea.y + scrollArea.h - cursorSize.h;
	    else
	    if(res.y < scrollArea.y) res.y = scrollArea.y;
        }
	else
	{
	    res.x = pos.x;

	    if(res.x > scrollArea.x + scrollArea.w - cursorSize.w)
            	res.x = scrollArea.x + scrollArea.w - cursorSize.w;
	    else
	    if(res.x < scrollArea.x) res.x = scrollArea.x;
	}

	return res;
    }

    /* ScrollBar */
    ScrollBar::ScrollBar(bool vertical, ListWidget & parent) : Window(& parent)
    {
        if(vertical) setState(FlagVertical);
        setModality(false);

        signalSubscribe(parent, Signal::ListWidgetChanged);
        signalSubscribe(parent, Signal::ListWidgetScrolled);
        signalSubscribe(*this, Signal::ScrollBarMoved, parent);
    }

    ScrollBar::ScrollBar(const Point & pos, const Size & sz, bool vertical, ListWidget & parent) : Window(& parent)
    {
        setSize(sz);
        setPosition(pos);

        if(vertical) setState(FlagVertical);
        setModality(false);

        signalSubscribe(parent, Signal::ListWidgetChanged);
        signalSubscribe(parent, Signal::ListWidgetScrolled);
        signalSubscribe(*this, Signal::ScrollBarMoved, parent);
    }

    bool ScrollBar::isVerticalOrientation(void) const
    {
        return checkState(FlagVertical);
    }

    bool ScrollBar::isAreaPoint(const Point & pt) const
    {
	return (scrollArea() + position()) & pt;
    }

    Rect ScrollBar::scrollArea(void) const
    {
        return rect();
    }

    const TexturePos & ScrollBar::textureCursor(void) const
    {
	return txcur;
    }

    void ScrollBar::setTextureCursor(const Texture & tx)
    {
	txcur.setTexture(tx);
    }

    void ScrollBar::renderWindow(void)
    {
	if(txcur.isValid()) renderTexture(txcur, txcur.position());
    }

    void ScrollBar::signalReceive(int sig, const SignalMember* sm)
    {
        switch(sig)
        {
            case Signal::ListWidgetChanged:
            case Signal::ListWidgetScrolled:
            {
                auto listWidget = dynamic_cast<const ListWidget*>(sm);
                if(listWidget)
		{
		    Point curpos = transformToScrollCursor(*listWidget, *this);
		    txcur.setPosition(curpos);
		    setDirty(true);
		}
            }
            break;

            default:
                break;
        }
    }

    void ScrollBar::windowMoveEvent(const Point &)
    {
        auto listWidget = dynamic_cast<const ListWidget*>(parent());
        if(listWidget)
	{
	    Point curpos = transformToScrollCursor(*listWidget, *this);
	    txcur.setPosition(curpos);
	    setDirty(true);
	}
    }

    void ScrollBar::windowResizeEvent(const Size &)
    {
        auto listWidget = dynamic_cast<const ListWidget*>(parent());
        if(listWidget)
	{
	    Point curpos = transformToScrollCursor(*listWidget, *this);
	    txcur.setPosition(curpos);
	    setDirty(true);
	}
    }

    bool ScrollBar::scrollUpEvent(void)
    {
        auto listWidget = dynamic_cast<ListWidget*>(parent());
        return listWidget ? listWidget->scrollUp(1) : false;
    }

    bool ScrollBar::scrollDownEvent(void)
    {
        auto listWidget = dynamic_cast<ListWidget*>(parent());
        return listWidget ? listWidget->scrollDown(1) : false;
    }

    bool ScrollBar::mousePressEvent(const ButtonEvent & be)
    {
        if(be.isButtonLeft() && (txcur.area() & be.position()))
        {
            setState(FlagPressed);
            return true;
        }
        else if(be.isButtonLeft() && (scrollArea() & be.position()))
        {
	    Point curpos = transformToScrollCursor(be.position(), *this);
	    txcur.setPosition(curpos);
            signalEmit(Signal::ScrollBarMoved);
            setDirty(true);

            return true;
        }

        return false;
    }

    bool ScrollBar::mouseReleaseEvent(const ButtonEvent & be)
    {
        if(be.isButtonLeft() && (txcur.area() & be.position()))
        {
            resetState(FlagPressed);
            auto listWidget = dynamic_cast<const ListWidget*>(parent());
            if(listWidget)
	    {
		Point curpos = transformToScrollCursor(*listWidget, *this);
		txcur.setPosition(curpos);
	    }
            return true;
        }
        return false;
    }

    bool ScrollBar::mouseMotionEvent(const Point & pos1, u32 buttons)
    {
        if((buttons & ButtonLeft) &&
           checkState(FlagPressed))
        {
	    Point curpos = transformToScrollCursor(pos1, *this);
	    txcur.setPosition(curpos);
            signalEmit(Signal::ScrollBarMoved);
            setDirty(true);

            return true;
        }

        return false;
    }
}

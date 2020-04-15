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
#include <sstream>

#include "display.h"
#include "display_scene.h"
#include "systems.h"
#include "inputs_keys.h"
#include "window.h"

Window::Window(Window* win) : prnt(NULL), result(0)
{
    resetState(FlagVisible);
    setState(FlagModality);

    setParent(win);
    DisplayScene::addItem(*this);

    pushEventAction(Signal::WindowCreated, this, NULL);
}

Window::Window(const Point & pos, const Size & sz, Window* win) : prnt(NULL), result(0)
{
    resetState(FlagVisible);
    setState(FlagModality);

    setSize(sz);
    setPosition(pos);
    setParent(win);
    DisplayScene::addItem(*this);

    pushEventAction(Signal::WindowCreated, this, NULL);
}

Window::Window(Window && win) noexcept
{
    gfxpos = win.gfxpos;
    prnt = win.prnt;
    state = win.state;
    result = win.result;
}

Window::Window(const Window & win) : prnt(NULL), result(0)
{
    gfxpos = win.gfxpos;
    state = win.state;
    result = win.result;

    setSize(win.size());
    setParent(win.prnt);
    DisplayScene::addItem(*this);

    pushEventAction(Signal::WindowCreated, this, NULL);
}

Window & Window::operator= (const Window & win)
{
    if(this != & win)
    {
	destroy();

	gfxpos = win.gfxpos;
	state = win.state;
	result = win.result;

	setSize(win.size());
	setParent(win.prnt);
	DisplayScene::addItem(*this);
    }

    return *this;
}

Window::~Window()
{
    destroy();
}

void Window::destroy(void)
{
    setVisible(false);

    auto childs = DisplayScene::findChilds(*this);
    std::for_each(childs.begin(), childs.end(), std::mem_fun(&Window::destroy));

    DisplayScene::removeItem(*this);

    prnt = NULL;
    result = 0;
}

void Window::setParent(Window* win)
{
    prnt = win;
}

void Window::setSize(const Size & sz)
{
    if(! sz.isEmpty())
    {
	gfxpos.setSize(sz);

	if(isVisible())
	{
	    bool focus = isAreaPoint(Display::mouseCursorPosition());

	    if(focus && ! isFocused())
	    {
		setState(FlagFocused);
		mouseFocusEvent();
	    }

	    if(!focus && isFocused())
	    {
		resetState(FlagFocused);
		mouseLeaveEvent();
	    }
	}

	windowResizeEvent(sz);

	if(isVisible())
	    DisplayScene::setDirty(true);
    }
}

void Window::setPosition(const Point & pos)
{
    Point diff = position();

    if(parent())
        gfxpos.setPoint(parent()->position() + pos);
    else
    // absolute position
        gfxpos.setPoint(pos);

    auto childs = DisplayScene::findChilds(*this);
    for(auto it = childs.begin(); it != childs.end(); ++it)
	(*it)->setPosition((*it)->position() - diff);

    if(isVisible())
    {
	bool focus = isAreaPoint(Display::mouseCursorPosition());

	if(focus && ! isFocused())
	{
	    setState(FlagFocused);
	    mouseFocusEvent();
	}

	if(!focus && isFocused())
	{
	    resetState(FlagFocused);
	    mouseLeaveEvent();
	}
    }

    windowMoveEvent(pos);

    if(isVisible())
	DisplayScene::setDirty(true);
}

void Window::setLayerTop(void)
{
    bool f = isVisible();

    DisplayScene::removeItem(*this);
    DisplayScene::addItem(*this);

    setVisible(f);

    auto childs = DisplayScene::findChilds(*this);
    std::for_each(childs.begin(), childs.end(), std::mem_fun(&Window::setLayerTop));
}

bool Window::isID(const WindowId & wid) const
{
    return wid == id();
}

bool Window::isAreaPoint(const Point & pt) const
{
    return area() & pt;
}

bool Window::isFocused(void) const
{
    return checkState(FlagVisible) && checkState(FlagModality | FlagFocused);
}

bool Window::isVisible(void) const
{
    return checkState(FlagVisible);
}

void Window::setVisible(bool f)
{
    if((checkState(FlagVisible) && !f) ||
	(! checkState(FlagVisible) && f))
    {
	setState(FlagVisible, f);

	// set for all childrens
	auto childs = DisplayScene::findChilds(*this);
	std::for_each(childs.begin(), childs.end(), std::bind2nd(std::mem_fun(&Window::setVisible), f));

	if(f)
	{
	    bool focus = isAreaPoint(Display::mouseCursorPosition());

	    if(focus && ! isFocused())
	    {
		setState(FlagFocused);
		mouseFocusEvent();
	    }
	}
	else
	    resetState(FlagFocused);

	DisplayScene::setDirty(true);
	windowVisibleEvent(f);
    }
}

void Window::redraw(void)
{
    if(isVisible())
    {
	//bool redraw = force;
	renderWindow();

	auto childs = DisplayScene::findChilds(*this);

	// redraw childs: order background
	for(auto it = childs.begin(); it != childs.end(); ++it)
	    if((*it)->isVisible() && (*it)->checkState(FlagLayoutBackground)) (*it)->redraw();
	// redraw childs: order normal
	for(auto it = childs.begin(); it != childs.end(); ++it)
	    if((*it)->isVisible() && ! (*it)->checkState(FlagLayoutBackground) && ! (*it)->checkState(FlagLayoutForeground)) (*it)->redraw();
	// redraw childs: order foreground
	for(auto it = childs.begin(); it != childs.end(); ++it)
	    if((*it)->isVisible() && (*it)->checkState(FlagLayoutForeground)) (*it)->redraw();
    }
}

int Window::exec(void)
{
    redraw();

    while(isVisible() && Display::handleEvents())
    {
        Display::redraw();
	Tools::delay(1);
    }

    return resultCode();
}

WindowId Window::id(void) const
{
    return reinterpret_cast<WindowId>(this);
}

const Point & Window::position(void) const
{
    return gfxpos;
}

const Size & Window::size(void) const
{
    return gfxpos;
}

const Rect & Window::area(void) const
{
    return gfxpos;
}

Rect Window::rect(void) const
{
    return Rect(0, 0, gfxpos.w, gfxpos.h);
}

int Window::width(void) const
{
    return gfxpos.w;
}

int Window::height(void) const
{
    return gfxpos.h;
}

const Window* Window::parent(void) const
{
    return prnt;
}

Window* Window::parent(void)
{
    return prnt;
}

int Window::resultCode(void) const
{
    return result;
}

void Window::setResultCode(int code)
{
    result = code;
}

bool Window::checkState(size_t v) const
{
    return state.check(v);
}

void Window::setState(size_t v, bool f)
{
    state.set(v, f);
}

void Window::resetState(size_t v)
{
    state.reset(v);
}

void Window::switchedState(size_t v)
{
    state.switched(v);
}

void Window::pushEventAction(int code, Window* dstWindow, void* data)
{
    DisplayScene::pushEvent((dstWindow ? dstWindow : this), code, data);
}

Rect transformSize(const Rect & trans, const Size & sz)
{
    Rect res = trans;
    if(res.x < 0) res.x = 0;
    if(res.y < 0) res.y = 0;
    if(res.x + res.w > sz.w) res.w = sz.w - res.x;
    if(res.y + res.h > sz.h) res.h = sz.h - res.y;
    return res;
}

Rect transformRect(const Rect & trans, const Rect & pos)
{
    Rect res = transformSize(trans, pos);
    res.x += pos.x;
    res.y += pos.y;
    return res;
}

void Window::renderClear(const Color & col) const
{
    Display::renderColor(col, Display::texture(), gfxpos);
}

void Window::renderColor(const Color & col, const Rect & dstrt) const
{
    if(rect() & dstrt)
	Display::renderColor(col, Display::texture(), transformRect(dstrt, gfxpos));
}

void Window::renderRect(const Color & col, const Rect & dstrt) const
{
    if(rect() & dstrt)
	Display::renderRect(col, Display::texture(), transformRect(dstrt, gfxpos));
}

void Window::renderLine(const Color & col, const Point & pt1, const Point & pt2) const
{
    if((rect() & pt1) && (rect() & pt2))
    {
	Display::renderLine(col, Display::texture(), pt1 + position(), pt2 + position());
    }
}

void Window::renderPoint(const Color & col, const Point & dstpt) const
{
    if(rect() & dstpt)
    {
	Display::renderPoint(col, Display::texture(), dstpt + position());
    }
}

void Window::renderSurface(const Surface & sf, const Point & dstpt) const
{
    if(rect() & Rect(dstpt, sf.size()))
    {
	Rect dstrt = transformRect(Rect(dstpt, sf.size()), gfxpos);
	Display::renderSurface(sf, transformSize(sf.rect(), dstrt), Display::texture(), dstrt);
    }
}

void Window::renderSurface(const Surface & sf, const Rect & srcrt, const Rect & dstrt, bool fixedsize) const
{
    if(rect() & Rect(dstrt.toPoint(), srcrt.toSize()))
    {
	Rect dstrt2 = transformRect(dstrt, gfxpos);
	Display::renderSurface(sf, (fixedsize ? transformSize(srcrt, dstrt2) : srcrt), Display::texture(), dstrt2);
    }
}

void Window::renderTexture(const TexturePos & sp) const
{
    renderTexture(sp.texture(), sp.position());
}

void Window::renderTexture(const Texture & tx, const Point & dstpt) const
{
    if(rect() & Rect(dstpt, tx.size()))
    {
	Rect dstrt = transformRect(Rect(dstpt, tx.size()), gfxpos);
	Display::renderTexture(tx, transformSize(tx.rect(), dstrt), Display::texture(), dstrt);
    }
}

void Window::renderTexture(const Texture & tx, const Rect & srcrt, const Rect & dstrt, bool fixedsize) const
{
    if(rect() & Rect(dstrt.toPoint(), srcrt.toSize()))
    {
	Rect dstrt2 = transformRect(dstrt, gfxpos);
	Display::renderTexture(tx, (fixedsize ? transformSize(srcrt, dstrt2) : srcrt), Display::texture(), dstrt2);
    }
}

Rect Window::renderText(const FontRender & fs, const UnicodeString & ustr, const Color & col, const Point & dpt, int halign, int valign, bool horizontal) const
{
    return Display::renderText(fs, ustr, col, Display::texture(), dpt + position(), halign, valign, horizontal);
}

std::string Window::toString(void) const
{
    std::string ids;

    auto childs = DisplayScene::findChilds(*this);
    for(auto it = childs.begin(); it != childs.end(); ++it)
	ids.append(String::hex((*it)->id())).append(",");

    return StringFormat("id(%1), parent(%2), pos(%3, %4), size(%5, %6), visible(%7), state(%8), childs: [%9]").arg(String::hex(id())).
	    arg(parent() ? String::hex(parent()->id()) : "null").arg(gfxpos.x).arg(gfxpos.y).
	    arg(width()).arg(height()).arg(isVisible() ? 1 : 0).arg(String::hex(state())).arg(ids);
}

CenteredWindow::CenteredWindow(const Size & sz, Window & win)
    : Window(Point(win.size() - sz) / 2, sz, & win)
{
}

DisplayWindow::DisplayWindow(const Color & col) : Window(Point(0, 0), Display::size(), NULL), backcolor(col)
{
    setState(FlagLayoutBackground);
    renderWindow();
    setVisible(true);
}

void DisplayWindow::renderWindow(void)
{
    renderClear(backcolor);
}

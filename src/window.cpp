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
#include <cstring>
#include <sstream>

#include "display.h"
#include "systems.h"
#include "inputs_keys.h"
#include "window.h"

namespace Display
{
    void		handleEvents(void);
    void                redraw(void);
    extern Window*	mainWindow;
    extern Texture	displayTexture;
    extern bool		displayDirty;
}

Window* WindowChilds::findWindow(const WindowId & id)
{
    iterator it = std::find_if(begin(), end(), std::bind2nd(std::mem_fun(&Window::isID), id));
    return it != end() ? *it : NULL;
}

const Window* WindowChilds::findWindowConst(const WindowId & id) const
{
    const_iterator it = std::find_if(begin(), end(), std::bind2nd(std::mem_fun(&Window::isID), id));
    return it != end() ? *it : NULL;
}

Window::Window(Window* win) : prnt(NULL), childs(NULL), result(0)
{
    state.reset(FlagVisible);
    state.set(FlagModality);

    setParent(win);
}

Window::Window(const Point & pos, const Size & sz, Window* win) : prnt(NULL), childs(NULL), result(0)
{
    state.reset(FlagVisible);
    state.set(FlagModality);

    setSize(sz);
    setPosition(pos);
    setParent(win);
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

	if(win.childs)
	{
	    childs = new WindowChilds();
	    childs->assign(win.childs->begin(), win.childs->end());
	}
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

    if(childs)
    std::for_each(childs->begin(), childs->end(), std::mem_fun(&Window::destroy));

    if(parent() && parent()->childs)
        parent()->childs->remove(this);
    else
    if(Display::mainWindow == this)
	Display::mainWindow = NULL;

    if(childs)
    {
	childs->clear();
	delete childs;
    }

    prnt = NULL;
    result = 0;
}

void Window::setParent(Window* win)
{
    if(parent() && parent()->childs)
        parent()->childs->remove(this);

    prnt = win;

    if(! win)
    {
	if(! Display::mainWindow)
	    Display::mainWindow = this;
	else
	    prnt = Display::mainWindow;
    }

    if(parent())
    {
	if(! parent()->childs)
	    parent()->childs = new WindowChilds();
	parent()->childs->push_back(this);
    }
}

void Window::setSize(const Size & sz)
{
    if(! sz.isEmpty())
    {
	gfxpos.setSize(sz);
	windowResizeEvent(sz);
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

    if(childs)
    for(WindowChilds::iterator it = childs->begin(); it != childs->end(); ++it)
	(*it)->setPosition((*it)->position() - diff);

    if(isVisible())
    {
	bool focus = area() & Display::mouseCursorPosition();

	if(focus && !state.check(FlagOnMouse))
	{
	    state.set(FlagOnMouse);
	    mouseFocusEvent();
	}

	if(!focus && state.check(FlagOnMouse))
	{
	    state.reset(FlagOnMouse);
	    mouseLeaveEvent();
	}
    }

    windowMoveEvent(pos);
}

bool Window::isID(const WindowId & wid) const
{
    return wid == id();
}

bool Window::isVisible(void) const
{
    return state.check(FlagVisible);
}

void Window::setVisible(bool f)
{
    if((state.check(FlagVisible) && !f) ||
	(!state.check(FlagVisible) && f))
    {
	state.set(FlagVisible, f);

	// set for all childrens
	if(childs)
	std::for_each(childs->begin(), childs->end(), std::bind2nd(std::mem_fun(&Window::setVisible), f));

	if(f)
	{
	    bool focus = area() & Display::mouseCursorPosition();

	    if(focus && !state.check(FlagOnMouse))
	    {
		state.set(FlagOnMouse);
		mouseFocusEvent();
	    }
	}
	else
	    state.reset(FlagOnMouse);

	Display::displayDirty = true;
    }
}

void Window::redraw(void)
{
    if(isVisible())
    {
	//bool redraw = force;
	renderWindow();

	if(childs)
	{
	    // redraw childs: order background
	    for(WindowChilds::iterator it = childs->begin(); it != childs->end(); ++it)
		if((*it)->checkState(FlagOrderBackground)) (*it)->redraw();
	    // redraw childs: order normal
	    for(WindowChilds::iterator it = childs->begin(); it != childs->end(); ++it)
		if(! (*it)->checkState(FlagOrderBackground | FlagOrderForeground)) (*it)->redraw();
	    // redraw childs: order foreground
	    for(WindowChilds::iterator it = childs->begin(); it != childs->end(); ++it)
		if((*it)->checkState(FlagOrderForeground)) (*it)->redraw();
	}
    }
}

int Window::exec(void)
{
    redraw();

    while(isVisible())
    {
        Display::handleEvents();
        Display::redraw();
	Tools::delay(1);
    }

    return result;
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

bool Window::checkState(int v) const
{
    return state.check(v);
}

void Window::setState(int v)
{
    state.set(v);
}

void Window::resetState(int v)
{
    state.reset(v);
}

bool Window::findChild(const Window* win) const
{
    if(childs)
    {
	if(childs->end() != std::find(childs->begin(), childs->end(), win))
	    return true;

	if(childs->end() != std::find_if(childs->begin(), childs->end(), std::bind2nd(std::mem_fun(&Window::findChild), win)))
    	    return true;
    }

    return false;
}

bool Window::keyPressHandle(int key)
{
    if(childs && childs->rend() != std::find_if(childs->rbegin(), childs->rend(), std::bind2nd(std::mem_fun(&Window::keyPressHandle), key)))
	return true;

    if(isVisible())
    {
	if(state.check(FlagModality))
	{
	    keyPressEvent(key);
	    return true;
	}
	else
	// focus
	if(area() & Display::mouseCursorPosition() || state.check(FlagKeyHandle))
	    return keyPressEvent(key);
    }

    return false;
}

bool Window::keyReleaseHandle(int key)
{
    if(childs && childs->rend() != std::find_if(childs->rbegin(), childs->rend(), std::bind2nd(std::mem_fun(&Window::keyReleaseHandle), key)))
	return true;

    if(isVisible())
    {
	if(state.check(FlagModality))
	{
	    keyReleaseEvent(key);
	    return true;
	}
	else
	// focus
	if(area() & Display::mouseCursorPosition() || state.check(FlagKeyHandle))
	    return keyReleaseEvent(key);
    }

    return false;
}

bool Window::textInputHandle(const std::string & str)
{
    if(childs && childs->rend() != std::find_if(childs->rbegin(), childs->rend(), std::bind2nd(std::mem_fun(&Window::textInputEvent), str)))
	return true;

    return isVisible() && (textInputEvent(str) || state.check(FlagModality));
}

bool Window::mousePressHandle(const ButtonEvent & coord)
{
    if(childs && childs->rend() != std::find_if(childs->rbegin(), childs->rend(), std::bind2nd(std::mem_fun(&Window::mousePressHandle), coord)))
	return true;

    if(isVisible())
    {
	bool focus = area() & coord.position();

	if(focus && !state.check(FlagOnMouse))
	{
	    state.set(FlagOnMouse);
	    mouseFocusEvent();
	}

	if(!focus && state.check(FlagOnMouse))
	{
	    state.reset(FlagOnMouse);
	    mouseLeaveEvent();
	}

	return (state.check(FlagOnMouse) &&
		mousePressEvent(ButtonEvent(coord.button(), coord.position() - position()))) ||
		state.check(FlagModality);
    }

    return false;
}

bool Window::mouseReleaseHandle(const ButtonEvent & coord)
{
    if(childs && childs->rend() != std::find_if(childs->rbegin(), childs->rend(), std::bind2nd(std::mem_fun(&Window::mouseReleaseHandle), coord)))
	return true;

    if(isVisible())
    {
	bool focus = area() & coord.position();

	if(focus && !state.check(FlagOnMouse))
	{
	    state.set(FlagOnMouse);
	    mouseFocusEvent();
	}

	if(!focus && state.check(FlagOnMouse))
	{
	    state.reset(FlagOnMouse);
	    mouseLeaveEvent();
	}

	return (state.check(FlagOnMouse) &&
		mouseReleaseEvent(ButtonEvent(coord.button(), coord.position() - position()))) ||
		state.check(FlagModality);
    }

    return false;
}

bool Window::mouseClickHandle(const ButtonsEvent & st)
{
    if(childs && childs->rend() != std::find_if(childs->rbegin(), childs->rend(), std::bind2nd(std::mem_fun(&Window::mouseClickHandle), st)))
	return true;

    if(isVisible())
    {
	bool focus = (area() & st.press().position()) && (area() & st.release().position());

	if(focus && !state.check(FlagOnMouse))
	{
	    state.set(FlagOnMouse);
	    mouseFocusEvent();
	}

	if(!focus && state.check(FlagOnMouse))
	{
	    state.reset(FlagOnMouse);
	    mouseLeaveEvent();
	}

	return (state.check(FlagOnMouse) &&
		mouseClickEvent(ButtonsEvent(st.press().button(), st.press().position() - position(),
						st.release().position() - position()))) ||
		state.check(FlagModality);
    }

    return false;
}

bool Window::mouseMotionHandle(const Point & pos, u32 buttons)
{
    if(childs)
    for(auto it = childs->rbegin(); it != childs->rend(); ++it)
	if((*it)->mouseMotionHandle(pos, buttons)) return true;

    if(isVisible())
    {
	bool focus = area() & pos;

	if(focus && !state.check(FlagOnMouse))
	{
	    state.set(FlagOnMouse);
	    mouseFocusEvent();
	}

	if(!focus && state.check(FlagOnMouse))
	{
	    state.reset(FlagOnMouse);
	    mouseLeaveEvent();
	}

	return (state.check(FlagOnMouse) &&
		mouseMotionEvent(pos - position(), buttons)) ||
		state.check(FlagModality);
    }

    return false;
}

bool Window::scrollUpHandle(const Point & pos)
{
    if(childs && childs->rend() != std::find_if(childs->rbegin(), childs->rend(), std::bind2nd(std::mem_fun(&Window::scrollUpHandle), pos)))
	return true;

    if(isVisible())
    {
	bool focus = area() & pos;

	return (focus && scrollUpEvent(pos)) ||
		state.check(FlagModality);
    }

    return false;
}

bool Window::scrollDownHandle(const Point & pos)
{
    if(childs && childs->rend() != std::find_if(childs->rbegin(), childs->rend(), std::bind2nd(std::mem_fun(&Window::scrollDownHandle), pos)))
	return true;

    if(isVisible())
    {
	bool focus = area() & pos;

	return (focus && scrollDownEvent(pos)) ||
		state.check(FlagModality);
    }

    return false;
}

bool Window::scrollLeftHandle(const Point & pos)
{
    if(childs && childs->rend() != std::find_if(childs->rbegin(), childs->rend(), std::bind2nd(std::mem_fun(&Window::scrollLeftHandle), pos)))
	return true;

    if(isVisible())
    {
	bool focus = area() & pos;

	return (focus && scrollLeftEvent(pos)) ||
		state.check(FlagModality);
    }

    return false;
}

bool Window::scrollRightHandle(const Point & pos)
{
    if(childs && childs->rend() != std::find_if(childs->rbegin(), childs->rend(), std::bind2nd(std::mem_fun(&Window::scrollRightHandle), pos)))
	return true;

    if(isVisible())
    {
	bool focus = area() & pos;

	return (focus && scrollRightEvent(pos)) ||
		state.check(FlagModality);
    }

    return false;
}

#ifdef OLDENGINE
bool Window::userHandle(const UserEvent & ev)
#else
bool Window::userHandle(const SDL_UserEvent & ev)
#endif
{
    if(isVisible())
    {
	// receiver exists
	if(ev.data1)
	{
	    Window* win = reinterpret_cast<Window*>(ev.data1);

	    if(win && Display::mainWindow &&
    		(Display::mainWindow == win || Display::mainWindow->findChild(win)))
		return win->userEvent(ev.code, ev.data2);
	    else
		ERROR("invalid win");
	}
	else
	// broadcast: for all
	{
	    if(childs)
		std::for_each(childs->begin(), childs->end(), std::bind2nd(std::mem_fun(&Window::userHandle), ev));

	    userEvent(ev.code, ev.data2);
	    return true;
	}
    }

    return false;
}

void Window::renderPresentHandle(u32 tick)
{
    if(childs)
    std::for_each(childs->rbegin(), childs->rend(), std::bind2nd(std::mem_fun(&Window::renderPresentHandle), tick));

    renderPresentEvent(tick);
}

void Window::displayResizeHandle(const Size & sz, bool sdl)
{
    if(childs)
    for(auto it = childs->rbegin(); it != childs->rend(); ++it)
	(*it)->displayResizeHandle(sz, sdl);

    displayResizeEvent(sz, sdl);
}

void Window::displayFocusHandle(bool gain)
{
    if(childs)
    std::for_each(childs->rbegin(), childs->rend(), std::bind2nd(std::mem_fun(&Window::displayFocusHandle), gain));

    displayFocusEvent(gain);
}

void Window::displayResizeEvent(const Size & sz, bool sdl)
{
    setSize(size());
    renderWindow();
}

void Window::tickHandle(u32 tick)
{
    if(childs)
    std::for_each(childs->rbegin(), childs->rend(), std::bind2nd(std::mem_fun(&Window::tickHandle), tick));

    tickEvent(tick);
}

void Window::pushEventActionBroadcast(int code, void* data)
{
    SDL_Event event;
    std::memset(&event, 0, sizeof(event));

    event.type = SDL_USEREVENT;
    event.user.code = code;
    event.user.data1 = NULL;
    event.user.data2 = data;

    if(0 > SDL_PushEvent(&event))
        ERROR(SDL_GetError());
}

void Window::pushEventAction(int code, Window* dstWindow, void* data)
{
    SDL_Event event;
    std::memset(&event, 0, sizeof(event));

    event.type = SDL_USEREVENT;
    event.user.code = code;
    event.user.data1 = dstWindow ? dstWindow : this;
    event.user.data2 = data;

    if(0 > SDL_PushEvent(&event))
        ERROR(SDL_GetError());
}

void Window::renderClear(const Color & col)
{
    Display::renderColor(col, Display::texture(), gfxpos);
}

Rect transformRect(const Rect & trans, const Rect & pos)
{
    Rect res = trans;
    if(res.x < 0) res.x = 0;
    if(res.y < 0) res.y = 0;
    if(res.x + res.w > pos.w) res.w = pos.w - res.x;
    if(res.y + res.h > pos.h) res.h = pos.h - res.y;
    res.x += pos.x;
    res.y += pos.y;
    return res;
}

void Window::renderColor(const Color & col, const Rect & dstrt)
{
    if(rect() & dstrt)
    {
	Rect rt = transformRect(dstrt, gfxpos);
	Display::renderColor(col, Display::texture(), rt);
    }
}

void Window::renderRect(const Color & col, const Rect & dstrt)
{
    if(rect() & dstrt)
    {
	Rect rt = transformRect(dstrt, gfxpos);
	Display::renderRect(col, Display::texture(), rt);
    }
}

void Window::renderLine(const Color & col, const Point & pt1, const Point & pt2)
{
    if((rect() & pt1) && (rect() & pt2))
    {
	Display::renderLine(col, Display::texture(), pt1 + position(), pt2 + position());
    }
}

void Window::renderPoint(const Color & col, const Point & dstpt)
{
    if(rect() & dstpt)
    {
	Display::renderPoint(col, Display::texture(), dstpt + position());
    }
}

void Window::renderSurface(const Surface & sf, const Point & dstpt)
{
    Rect dstrt(dstpt, sf.size());
    if(rect() & dstrt)
    {
	Rect rt = transformRect(dstrt, gfxpos);
	Display::renderSurface(sf, sf.rect(), Display::texture(), rt);
    }
}

void Window::renderSurface(const Surface & sf, const Rect & srcrt, const Rect & dstrt)
{
    if(rect() & dstrt)
    {
	Rect rt = transformRect(dstrt, gfxpos);
	Display::renderSurface(sf, srcrt, Display::texture(), rt);
    }
}

void Window::renderTexture(const TexturePos & sp)
{
    renderTexture(sp.texture(), sp.position());
}

void Window::renderTexture(const Texture & tx, const Point & dstpt)
{
    Rect dstrt(dstpt, tx.size());
    if(rect() & dstrt)
    {
	Rect rt = transformRect(dstrt, gfxpos);
	Display::renderTexture(tx, tx.rect(), Display::texture(), rt);
    }
}

void Window::renderTexture(const Texture & tx, const Rect & srcrt, const Rect & dstrt)
{
    if(rect() & dstrt)
    {
	Rect rt = transformRect(dstrt, gfxpos);
	Display::renderTexture(tx, srcrt, Display::texture(), rt);
    }
}

Rect Window::renderText(const FontRender & fs, const UnicodeString & ustr, const Color & col, const Point & dpt, int halign, int valign, bool horizontal)
{
    return Display::renderText(fs, ustr, col, Display::texture(), dpt + position(), halign, valign, horizontal);
}

std::string Window::toString(void) const
{
    std::ostringstream os;
    os <<
	"id" << "(" << String::hex(id()) << "), " <<
	"childs" << "(" << (childs ? childs->size() : 0) << "), ";
    return os.str();
}

CenteredWindow::CenteredWindow(const Size & sz, Window & win)
    : Window(Point(win.size() - sz) / 2, sz, & win)
{
}

DisplayWindow::DisplayWindow(const Color & col) : Window(Point(0, 0), Display::size(), NULL), backcolor(col)
{
    setState(FlagOrderBackground);
    setVisible(true);
}

void DisplayWindow::renderWindow(void)
{
    renderClear(backcolor);
}

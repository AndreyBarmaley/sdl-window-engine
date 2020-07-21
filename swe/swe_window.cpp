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

#include "swe_display.h"
#include "swe_display_scene.h"
#include "swe_systems.h"
#include "swe_inputs_keys.h"
#include "swe_window.h"

#ifdef WITH_JSON
#include "swe_json_ext.h"
#endif

namespace SWE
{
    void WindowProtect::setState(Window* win, int st, bool f)
    {
	if(win) win->setState(st, f);
    }

    /* Window */
    Window::Window(Window* win) : prnt(win), result(0)
    {
        resetState(FlagVisible);
        setState(FlagModality);
        //setParent(win);
        DisplayScene::addItem(*this);
        pushEventAction(Signal::WindowCreated, this, NULL);
    }

    Window::Window(const Size & sz, Window* win) : prnt(win), result(0)
    {
        resetState(FlagVisible);
        setState(FlagModality);
        setSize(sz);
        //setParent(win);
        DisplayScene::addItem(*this);
        pushEventAction(Signal::WindowCreated, this, NULL);
    }

    Window::Window(const Point & pos, const Size & sz, Window* win) : prnt(win), result(0)
    {
        resetState(FlagVisible);
        setState(FlagModality);
        setSize(sz);
	// set position depends from parent
        setPosition(pos);
        //setParent(win);
        DisplayScene::addItem(*this);
        pushEventAction(Signal::WindowCreated, this, NULL);
    }

/*
    Window::Window(Window && win) noexcept
    {
        gfxpos = win.gfxpos;
        prnt = win.prnt;
        state = win.state;
        result = win.result;
    }
*/
    Window::Window(const Window & win) : prnt(NULL), result(0)
    {
        //gfxpos = win.gfxpos;
        state = win.state;
        result = win.result;
        setPosition(win.position());
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
            //gfxpos = win.gfxpos;
            state = win.state;
            result = win.result;
    	    setPosition(win.position());
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
        std::for_each(childs.begin(), childs.end(), [](Window* win){ win->destroy(); });
        DisplayScene::removeItem(*this);
        prnt = NULL;
        result = 0;
    }

    void Window::setParent(Window* win)
    {
        prnt = win;
    }

    void Window::setSize(int sw, int sh)
    {
	setSize(Size(sw, sh));
    }

    void Window::setSize(const Size & sz)
    {
        if(! sz.isEmpty())
        {
            gfxpos.setSize(sz);

            if(isVisible() && (isAreaPoint(Display::mouseCursorPosition()) || isFocused()))
            {
    		pushEventAction(Signal::WindowCheckFocus, this, NULL);
            }

            windowResizeEvent(sz);

            if(isVisible())
                DisplayScene::setDirty(true);
        }
    }

    void Window::setPosition(int px, int py)
    {
	setPosition(Point(px, py));
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

        if(isVisible() && (isAreaPoint(Display::mouseCursorPosition()) || isFocused()))
        {
    	    pushEventAction(Signal::WindowCheckFocus, this, NULL);
        }

        windowMoveEvent(pos);

        if(isVisible())
            DisplayScene::setDirty(true);
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
        return isVisible() && (isModality() || checkState(FlagFocused));
    }

    bool Window::isHidden(void) const
    {
        return checkState(FlagLayoutHidden);
    }

    bool Window::isVisible(void) const
    {
        return checkState(FlagVisible);
    }

    bool Window::isModality(void) const
    {
        return checkState(FlagModality);
    }

    void Window::setModality(bool f)
    {
	setState(FlagModality, f);
    }

    bool Window::keyHandle(const KeySym & key, bool press)
    {
        return press ?
        	keyPressEvent(key) : keyReleaseEvent(key);
    }

    bool Window::mouseClickHandle(const ButtonsEvent & be)
    {
        Point relpos1 = be.press().position() - position();
        Point relpos2 = be.release().position() - position();

        return mouseClickEvent(ButtonsEvent(be.press().button(), relpos1, relpos2));
    }

    bool Window::mouseButtonHandle(const ButtonEvent & be, bool press)
    {
        Point relpos = be.position() - position();

        return press ?
                mousePressEvent(ButtonEvent(be.button(), relpos)) :
                mouseReleaseEvent(ButtonEvent(be.button(), relpos));
    }

    bool Window::mouseMotionHandle(const Point & abspos, u32 buttons)
    {
	Point relpos = abspos - position();
	return mouseMotionEvent(relpos, buttons);
    }

    bool Window::scrollHandle(bool isup)
    {
        return isup ?
            scrollUpEvent() : scrollDownEvent();
    }

    void Window::focusHandle(bool focus)
    {
        if(focus && ! checkState(FlagFocused))
        {
            setState(FlagFocused);
            mouseFocusEvent();
        }
        else if(! focus && checkState(FlagFocused))
        {
	    if(isModality() && isVisible()) return;
            resetState(FlagFocused);
            mouseLeaveEvent();
        }
    }

    Texture & Window::targetTexture(void)
    {
	return Display::texture();
    }

    void Window::setDirty(bool f)
    {
	if(f) DisplayScene::setDirty(f);
    }

    void Window::setHidden(bool f)
    {
        if((checkState(FlagLayoutHidden) && !f) ||
           (! checkState(FlagLayoutHidden) && f))
        {
    	    // set for all childrens (if for parent applies)
    	    auto childs = DisplayScene::findChilds(*this);
    	    std::for_each(childs.begin(), childs.end(), [=](Window* win){ win->setHidden(f); });

            setState(FlagLayoutHidden, f);
            if(f) resetState(FlagFocused);
	}
    }

    void Window::setVisible(bool f)
    {
        // set for all childrens
        auto childs = DisplayScene::findChilds(*this);
        std::for_each(childs.begin(), childs.end(), [=](Window* win){ win->setVisible(f); });

        if((checkState(FlagVisible) && !f) ||
           (! checkState(FlagVisible) && f))
        {
            setState(FlagVisible, f);

            if(f)
            {
		// modality upper move
		if(isModality()) DisplayScene::moveTopLayer(*this);
            }

    	    if((!f && isFocused()) || (f && isAreaPoint(Display::mouseCursorPosition())))
    		pushEventAction(Signal::WindowCheckFocus, this, NULL);

            setDirty(true);
            windowVisibleEvent(f);
        }
    }

    void Window::redraw(void)
    {
        if(isVisible() && ! isHidden())
        {
	    renderBackground();

            //bool redraw = force;
            auto childs = DisplayScene::findChilds(*this);

            // redraw childs: order background
            for(auto it = childs.begin(); it != childs.end(); ++it)
	    {
		// modality is redraw on displayscene::redraw
		if((*it)->isModality()) continue;
                if((*it)->isVisible() && (*it)->checkState(FlagLayoutBackground))(*it)->redraw();
	    }

            renderWindow();

            // redraw childs: order normal
            for(auto it = childs.begin(); it != childs.end(); ++it)
	    {
		// modality is redraw on displayscene::redraw
		if((*it)->isModality()) continue;
                if((*it)->isVisible() && !(*it)->checkState(FlagLayoutBackground) && !(*it)->checkState(FlagLayoutForeground))(*it)->redraw();
	    }

            // redraw childs: order foreground
            for(auto it = childs.begin(); it != childs.end(); ++it)
	    {
		// modality is redraw on displayscene::redraw
		if((*it)->isModality()) continue;
                if((*it)->isVisible() && (*it)->checkState(FlagLayoutForeground))(*it)->redraw();
	    }

	    renderForeground();
        }
    }

    int Window::exec(void)
    {
        redraw();
	DisplayScene::handleWhileVisible(*this);
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

    void Window::setState(size_t v)
    {
        state.set(v, true);
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

    Rect transformRect(const Rect & trans, const Rect & local)
    {
	Rect crop;

	if(Rect::intersection(trans, local, & crop))
	    return crop;

        return trans;
    }

    void Window::renderClear(const Color & col)
    {
        Display::renderColor(col, targetTexture(), gfxpos);
    }

    void Window::renderColor(const Color & col, const Rect & dstrt)
    {
	Rect drt = transformRect(dstrt + gfxpos.toPoint(), gfxpos);
        if(gfxpos & drt)
            Display::renderColor(col, targetTexture(), drt);
    }

    void Window::renderRect(const Color & col, const Rect & dstrt)
    {
	Rect drt = transformRect(dstrt + gfxpos.toPoint(), gfxpos);
        if(gfxpos & drt)
            Display::renderRect(col, targetTexture(), drt);
    }

    void Window::renderLine(const Color & col, const Point & pt1, const Point & pt2)
    {
	// render horizontal crop
	if(pt1.y == pt2.y)
	{
	    Rect srt = Rect(pt1, pt2);
	    Rect drt = transformRect(srt + gfxpos.toPoint(), gfxpos);

    	    if(gfxpos & drt)
    		Display::renderLine(col, targetTexture(), drt.topLeft(), drt.topRight());
	}
	else
	// render vertical crop
	if(pt1.x == pt2.x)
	{
	    Rect srt = Rect(pt1, pt2);
	    Rect drt = transformRect(srt + gfxpos.toPoint(), gfxpos);

    	    if(gfxpos & drt)
    		Display::renderLine(col, targetTexture(), drt.topLeft(), drt.bottomLeft());
	}
	else
        if((rect() & pt1) && (rect() & pt2))
            Display::renderLine(col, targetTexture(), pt1 + position(), pt2 + position());
    }

    void Window::renderPoint(const Color & col, const Point & dpt)
    {
        if(rect() & dpt)
            Display::renderPoint(col, targetTexture(), dpt + position());
    }

    void Window::renderSurface(const Surface & sf, const Point & dpt, int flip)
    {
        renderSurface(sf, sf.rect(), dpt, flip);
    }

    void Window::renderSurface(const Surface & sf, const Rect & srcrt, const Point & dpt, int flip)
    {
	Rect srt = transformRect(srcrt, sf.rect());
	Rect drt = transformRect(Rect(dpt, srt.toSize()) + gfxpos.toPoint(), gfxpos);

        if(gfxpos & drt)
        {
	    // crop
	    if(dpt.x < 0) { srt.x += std::abs(dpt.x); srt.w -= std::abs(dpt.x); }
	    if(dpt.y < 0) { srt.y += std::abs(dpt.y); srt.h -= std::abs(dpt.y); }
	    if(srt.w > drt.w) { srt.w = drt.w; }
	    if(srt.h > drt.h) { srt.h = drt.h; }

            Display::renderSurface(sf, srt, targetTexture(), drt, flip);
	}
    }

    void Window::renderTexture(const TexturePos & sp)
    {
        renderTexture(sp.texture(), sp.texture().rect(), sp.position());
    }

    void Window::renderTexture(const Texture & tx, const Point & dpt, int flip)
    {
        renderTexture(tx, tx.rect(), dpt, flip);
    }

    void Window::renderTexture(const Texture & tx, const Rect & srcrt, const Point & dpt, int flip)
    {
	Rect srt = transformRect(srcrt, tx.rect());
	Rect drt = transformRect(Rect(dpt, srt.toSize()) + gfxpos.toPoint(), gfxpos);

        if(gfxpos & drt)
        {
	    // crop
	    if(dpt.x < 0) { srt.x += std::abs(dpt.x); srt.w -= std::abs(dpt.x); }
	    if(dpt.y < 0) { srt.y += std::abs(dpt.y); srt.h -= std::abs(dpt.y); }
	    if(srt.w > drt.w) { srt.w = drt.w; }
	    if(srt.h > drt.h) { srt.h = drt.h; }

            Display::renderTexture(tx, srt, targetTexture(), drt, flip);
	}
    }

    Rect Window::renderText(const FontRender & fs, const UnicodeString & ustr, const Color & col, const Point & dpt, int halign, int valign, bool horizontal)
    {
        return Display::renderTextFixed(fs, ustr, col, targetTexture(), dpt, gfxpos, halign, valign, horizontal);
    }

#ifdef WITH_JSON
    JsonObject Window::toJson(void) const
    {
	JsonObject res = SignalMember::toJson();
        if(size().isEmpty())
        {
            res.addBoolean("valid", false);
            return res;
        }
	res.addArray("position", JsonPack::point(position()));
	res.addArray("size", JsonPack::size(size()));
	res.addString("state", String::hex(state.value()));
	res.addInteger("result", result);
	res.addBoolean("visible", isVisible());
	res.addBoolean("modality", isModality());
	res.addBoolean("hidden", isHidden());
	res.addString("self", String::pointer(this));
	res.addString("parent", String::pointer(parent()));
        res.addBoolean("valid", true);

	return res;
    }
#endif

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

    void Window::dumpState(void) const
    {
        VERBOSE("self: " << String::pointer(this) <<
            ", class: " << className() <<
            ", parent: " << String::pointer(parent()) <<
            ", visible: " << String::Bool(checkState(FlagVisible)) <<
            ", modality: " << String::Bool(checkState(FlagModality)) <<
            ", hidden: " << String::Bool(checkState(FlagLayoutHidden)) <<
            ", focus: " << String::Bool(checkState(FlagFocused)) <<
            ", area: " << gfxpos.toString() <<
            ", state: " << String::hex(state.value()));
    }

    /* CenteredWindow */
    CenteredWindow::CenteredWindow(const Size & sz, Window* win) : Window(sz, win)
    {
        if(parent())
	{
	    Size offset = (parent()->size() - sz) / 2;
	    setPosition(parent()->position() - offset);
	}
    }

    /* DisplayWindow */
    DisplayWindow::DisplayWindow(const Color & col) : Window(Display::size(), NULL), backcolor(col)
    {
        setState(FlagLayoutBackground);
        renderWindow();
        setVisible(true);
    }

#ifdef WITH_JSON
    JsonObject DisplayWindow::toJson(void) const
    {
	JsonObject res = Window::toJson();
	res.addString("backcolor", backcolor.toString());
	return res;
    }
#endif

    void DisplayWindow::renderWindow(void)
    {
        renderClear(backcolor);
    }

    /* TargetWindow */
    TargetWindow::TargetWindow(const Size & wsz) : Window(wsz, NULL)
    {
	setModality(false);
        DisplayScene::removeItem(*this);
	setTexture(Display::createTexture(wsz));
    }
}

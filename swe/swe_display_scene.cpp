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

#include "swe_engine.h"
#include "swe_wingui.h"
#include "swe_cstring.h"
#include "swe_display.h"
#include "swe_inputs_keys.h"
#include "swe_display_scene.h"

namespace SWE
{
    class DisplayToolTip : public Window
    {
        Point                   mousePos;
        u32                     mouseIdle;
        const WindowToolTipArea* owner;

    public:
        DisplayToolTip(const WindowToolTipArea* win) : Window(nullptr), mouseIdle(0), owner(win)
        {
    	    setSize(owner->tooltipTexture().size());
            setState(FlagLayoutForeground);
    	    resetState(FlagModality);
        }

        bool isOwner(const Window & win) const
        {
            return owner == & win;
        }

	void renderWindow(void) override
	{
    	    renderTexture(owner->tooltipTexture(), Point(0, 0));
	}

	const char* className(void) const override
	{
	    return "SWE::DisplayToolTip";
	}

#ifdef SWE_WITH_JSON
	JsonObject toJson(void) const override
	{
    	    JsonObject res = Window::toJson();
    	    res.addArray("mousePos", JsonPack::point(mousePos));
    	    res.addInteger("mouseIdle", mouseIdle);
    	    res.addInteger("timeout", owner->tooltipTimeout());
    	    res.addString("owner", String::pointer(owner));
    	    return res;
	}
#endif

        void resetVisibleTimeout(void)
        {
            setVisible(false);
            mouseIdle = Tools::ticks() - Display::timeStart();
        }

    protected:
        Point fixPosition(const Point & mousepos, const Size & winsz, const Size & displaysz)
        {
            const int offset = 2;
            int posx = mousepos.x + winsz.w + offset > displaysz.w ? mousepos.x - winsz.w - offset : mousepos.x + offset;
            int posy = mousepos.y - winsz.h - offset;
	    if(posx < 0) posx = 0;
	    if(posy < 0) posy = 0;
            return Point(posx, posy);
        }

        void tickEvent(u32 ms) override
        {
            const Point & cursorPos = Display::mouseCursorPosition();

            if(mousePos != cursorPos)
            {
                mousePos = cursorPos;
                mouseIdle = ms;

                if(isVisible())
                    setVisible(false);
            }
            else if(! isVisible() &&
                    ms - mouseIdle > owner->tooltipTimeout())
            {
                setPosition(fixPosition(mousePos, size(), Display::size()));
                setVisible(true);
            }
        }
    };

/*
    struct CompWindowPtr
    {
	bool isParentChild(const Window* parent, const Window* child) const
	{
	    while(child->parent())
	    {
    		child = child->parent();
		if(child == parent) return true;
    	    }
	    return false;
	}

	bool operator() (const Window* lhs, const Window* rhs) const
	{
	    if(nullptr == lhs->parent() && rhs->parent())
		return true;
	    // background vs normal
	    if(lhs->checkState(FlagLayoutBackground) && !rhs->checkState(FlagLayoutBackground))
		return true;
	    // parent vs child
	    if(isParentChild(lhs, rhs))
		return true;
	    // normal vs modality
	    if(!lhs->isModality() && rhs->isModality())
		return true;
	    // normal vs foreground
	    if(! lhs->checkState(FlagLayoutForeground) && rhs->checkState(FlagLayoutForeground))
		return true;

	    return false;
	}
    };
*/
    std::list<BaseObject*>
			sceneObjects;
    std::list<Window*>	sceneItems;
    bool		sceneDirty = true;
    DisplayToolTip*	sceneToolTip = nullptr;

    Texture		cursorTexture;
    Point		cursorOffset;

    bool		markTopWidget = false;
#ifdef SWE_SDL12
    // SDL12 small queue events, resize it, see pushEvent
    std::list<UserEvent> events;
#endif
}

const std::list<SWE::Window*> & SWE::DisplayScene::items(void)
{
    return sceneItems;
}

SWE::Window* SWE::DisplayScene::rootWindow(void)
{
    for(auto it = sceneItems.begin(); it != sceneItems.end(); ++it)
        if((*it)->parent() == nullptr) return *it;

    return nullptr;
}

SWE::Window* SWE::DisplayScene::focusedWindow(void)
{
    for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
    {
        if((*it)->isVisible() && !(*it)->isHidden())
	{
	    if((*it)->isModality())
		return *it;

            if((*it)->isAreaPoint(Display::mouseCursorPosition()))
		return *it;
	}
    }

    return nullptr;
}

void SWE::DisplayScene::resetCursor(void)
{
    cursorTexture.reset();
    cursorOffset = Point(0, 0);
    setDirty(true);
}

void SWE::DisplayScene::setCursor(const Texture & tx, const Point & off)
{
    cursorTexture = tx;
    cursorOffset = off;
    setDirty(true);
}

void SWE::DisplayScene::setDirty(bool f)
{
    sceneDirty = f;
}

bool SWE::DisplayScene::isDirty(void)
{
    return sceneDirty;
}

std::list<SWE::Window*> SWE::DisplayScene::findParents(const Window & self)
{
    std::list<Window*> parents;
    auto win = & self;

    while(win->parent())
    {
        win = win->parent();

        if(parents.end() != std::find(parents.begin(), parents.end(), win))
        {
            ERROR("loop detected");
            break;
        }

        parents.push_back(const_cast<Window*>(win));
    }

    return parents;
}

std::list<SWE::Window*> SWE::DisplayScene::findChilds(const Window & win)
{
    std::list<Window*> childs;

    for(auto & child : sceneItems)
        if(child->parent() == & win)
	    childs.push_back(child);

    return childs;
}

/*
#include <bitset>
bool SWE::DisplayScene::windowVisible(const Window & win)
{
    const Size & dsz = Display::size();
    static std::vector<bool> points(dsz.w * dsz.h, false);
    Rect hidden;

    auto itw = std::find(sceneItems.begin(), sceneItems.end(), & win);
    if(itw == sceneItems.end())
        return false;

    points.resize(win.width() * win.height(), false);
    for(int pos = 0; pos < points.size(); ++pos)
        points[pos] = true;

    for(auto it = std::next(itw); it != sceneItems.end(); ++it)
    {
        if(! Rect::intersection(win.area(), (*it)->area(), & hidden))
            continue;

        for(int rx = 0; rx < hidden.w; ++rx)
        {
            for(int ry = 0; ry < hidden.h; ++ry)
            {
                int offset = (hidden.y + ry - win.position().y) * win.width() + hidden.x + rx - win.position().x;
                if(offset < points.size())
                    points[offset] = false;
            }
        }
    }

    return std::any_of(points.begin(), points.end(), [](const auto & v){ return v; });
}
*/

void SWE::DisplayScene::addObject(BaseObject & obj)
{
    /* back: top level */
    sceneObjects.push_back(& obj);
}

void SWE::DisplayScene::removeObject(const BaseObject & obj)
{
    sceneObjects.remove(const_cast<BaseObject*>(& obj));
}

void SWE::DisplayScene::addItem(Window & win)
{
    /* back: top level */
    sceneItems.push_back(& win);
}

void SWE::DisplayScene::removeItem(const Window & win)
{
    sceneItems.remove(const_cast<Window*>(& win));

    // fixed tooltip used pointer
    if(sceneToolTip && sceneToolTip->isOwner(win))
    {
        delete sceneToolTip;
        sceneToolTip = nullptr;
    }
}

void SWE::DisplayScene::moveTopLayer(const Window & win)
{
    // move top sorting
    removeItem(win);
    addItem(const_cast<Window &>(win));

    auto childs = findChilds(win);
    for(auto it = childs.begin(); it != childs.end(); ++it)
	moveTopLayer(**it);
}

void SWE::DisplayScene::sceneDestroy(void)
{
    if(sceneToolTip)
    {
        delete sceneToolTip;
        sceneToolTip = nullptr;
    }

    if(cursorTexture.isValid())
        cursorTexture.reset();

    for(auto & win : sceneItems)
        win->setVisible(false);

    sceneItems.clear();
}

/// @details рендер всех видимых объектов на сцене, необходимость отрисовки задается через метод DisplayScene::setDirty, генерирует событие Window::renderPresentEvent на каждый объект Window
void SWE::DisplayScene::sceneRedraw(bool force)
{
    if(isDirty() || force)
    {
	u32 renderStart = Tools::ticks();

	for(auto & win : sceneItems)
	{
	    if(nullptr == win->parent() || win->isModality())
		win->redraw();
	}

#if defined(SWE_DEBUG_SCENE) || defined(SWE_DEBUG)
	// mark top focused
	if(markTopWidget)
	{
	    for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
	    {
		if((*it)->isVisible() && !(*it)->isHidden() &&
		    (*it)->isAreaPoint(Display::mouseCursorPosition()))
		{
    		    Display::renderTexture(Display::renderRect(Color::Red, Color::Transparent, (*it)->size()), (*it)->position());
		    if((*it)->parent())
    			Display::renderTexture(Display::renderRect(Color::Blue, Color::Transparent, (*it)->parent()->size()), (*it)->parent()->position());
		    (*it)->dumpState();
		    break;
		}
	    }
	}
#endif

	if(sceneToolTip)
	    sceneToolTip->redraw();

	if(cursorTexture.isValid())
    	    Display::renderTexture(cursorTexture, Display::mouseCursorPosition() + cursorOffset);

	// render present
        Display::renderPresent();

        renderPresentHandle(Tools::ticks() - renderStart);
        setDirty(false);
    }
    else
    {
        Tools::delay(1);
    }
}

void SWE::DisplayScene::handleWhileVisible(const Window & win)
{
    sceneRedraw(true);

    while(win.isVisible())
    {
	Display::handleEvents();

	tickHandle(Tools::ticks() - Display::timeStart());
        sceneRedraw();
    }
}

/// @details состоит из 3 основных частей, обработка системных событий Display::handleEvents, генерация события ObjectEvent::tickEvent, рендера сцены DisplayScene::sceneRedraw
void SWE::DisplayScene::handleEvents(u32 interval)
{
    sceneRedraw(true);

    u32 start = Tools::ticks();

    while(true)
    {
        Display::handleEvents();

	tickHandle(Tools::ticks() - Display::timeStart());
        sceneRedraw();

        if(0 == interval || start + interval < Tools::ticks())
            break;
    }
}

void SWE::DisplayScene::pushEvent(const ObjectEvent* dst, int code, void* data)
{
    SDL_Event event;
    std::memset(&event, 0, sizeof(event));

    event.type = SDL_USEREVENT;
    event.user.code = code;
    event.user.data1 = const_cast<ObjectEvent*>(dst);
    event.user.data2 = data;

    if(0 > SDL_PushEvent(&event))
    {
#ifdef SWE_SDL12
        // backup sdl12 event
	events.emplace_back(UserEvent(event.user.code, event.user.data1, event.user.data2));
#else
        ERROR("win: " << String::pointer(dst) << ", " << "code: " << String::hex(code) << ", " << "error: " << SDL_GetError());
#endif
    }
}

////////////// handle events
// enum EventFilter { KeyHandle, MouseButtonHandle, MouseTrackingHandle, SystemTickHandle };

bool SWE::DisplayScene::keyHandle(const KeySym & key, bool press)
{
#if defined(SWE_DEBUG_SCENE) || defined(SWE_DEBUG)
    if(press && key.keymod().isLeftShift() && key.isKeyCode(Key::F8))
    {
        markTopWidget = !markTopWidget;
	sceneDirty = true;
    }

    if(press && key.keymod().isLeftShift() && key.isKeyCode(Key::F9))
    {
	dumpScene();
    }

#ifdef SWE_WITH_JSON
    if(press && key.keymod().isLeftShift() && key.isKeyCode(Key::F10))
    {
	if(Systems::saveString2File(toJson().toString(), "scene.json"))
	    VERBOSE("DisplayScene saved to scene.json");
    }
#endif
#endif

    // focused window
    Window* topWin = windowsFocusHandle();

    if(topWin && topWin->keyHandle(key, press))
	return true;

    // reverse order
    for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
    {
        if(topWin && *it == topWin)
        {
            // stop layer
            if(topWin->isModality())
                return true;

            continue;
        }

        if((*it)->isVisible())
        {

            // first modality stop event
            if((*it)->isModality())
	        return (*it)->keyHandle(key, press);

            // other win only FlagKeyHandle
            if((*it)->checkState(FlagKeyHandle) && (*it)->keyHandle(key, press))
                return true;
        }
    }

    return false;
}

bool SWE::DisplayScene::textInputHandle(const std::string & str)
{
    // focused window
    Window* topWin = windowsFocusHandle();

    if(topWin && topWin->textInputEvent(str))
	return true;

    // reverse order
    for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
    {
        if(topWin && *it == topWin)
        {
            // stop layer
            if(topWin->isModality())
                return true;

            continue;
        }

        if((*it)->isVisible())
        {
            // first modality stop event
            if((*it)->isModality())
	        return (*it)->textInputEvent(str);

            // other win only FlagKeyHandle
            if((*it)->checkState(FlagKeyHandle) && (*it)->textInputEvent(str))
                return true;
        }
    }

    return false;
}

SWE::Window* SWE::DisplayScene::windowsFocusHandle(void)
{
    Window* topWin = focusedWindow();

    // global reset focus
    for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
	if(*it != topWin) (*it)->focusHandle(false);

    if(topWin)
        topWin->focusHandle(true);

    return topWin;
}

bool SWE::DisplayScene::mouseButtonHandle(const ButtonEvent & st, bool press)
{
    Window* topWin = windowsFocusHandle();

    // mouse manip: hide tooltip
    if(sceneToolTip && sceneToolTip->isVisible())
        sceneToolTip->resetVisibleTimeout();

    if(topWin)
    {
        if(topWin->mouseButtonHandle(st, press) || topWin->isModality())
	    return true;
    }

    for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
    {
        if((*it)->isVisible() && !(*it)->isHidden())
        {
	    if(topWin == *it) continue;

            if((*it)->isModality())
            {
		(*it)->mouseButtonHandle(st, press);
                return true;
            }
            else
	    if((*it)->isAreaPoint(st.position()))
	    {
		if((*it)->mouseButtonHandle(st, press))
            	    return true;
	    }
        }
    }

    return false;
}

bool SWE::DisplayScene::mouseClickHandle(const ButtonsEvent & st)
{
    Window* topWin = windowsFocusHandle();

    if(topWin)
    {
        if(topWin->mouseClickHandle(st) || topWin->isModality())
	    return true;
    }

    for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
    {
        if((*it)->isVisible() && !(*it)->isHidden())
        {
	    if(topWin == *it) continue;
            bool focus = (*it)->isAreaPoint(st.press().position()) && (*it)->isAreaPoint(st.release().position());

            if((*it)->isModality())
            {
                (*it)->mouseClickHandle(st);
                return true;
            }
            else
	    if(focus)
	    {
        	if((*it)->mouseClickHandle(st))
            	    return true;
	    }
        }
    }

    return false;
}

bool SWE::DisplayScene::mouseMotionHandle(const Point & pos, u32 buttons)
{
    if(cursorTexture.isValid())
        setDirty(true);

    Window* topWin = windowsFocusHandle();

    // mouse manip: hide tooltip
    if(sceneToolTip && sceneToolTip->isVisible())
        sceneToolTip->resetVisibleTimeout();

    for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
    {
        // global mouse tracking event
        if((*it)->checkState(FlagMouseTracking))
            (*it)->mouseTrackingEvent(pos, buttons);
    }

    if(topWin)
    {
        if(topWin->mouseMotionHandle(pos, buttons) || topWin->isModality())
	    return true;
    }

    for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
    {
        if((*it)->isVisible() && !(*it)->isHidden())
        {
	    if(topWin == *it) continue;

            if((*it)->isModality())
            {
                (*it)->mouseMotionHandle(pos, buttons);
                return true;
            }
            else
            if((*it)->mouseMotionHandle(pos, buttons))
            	return true;
        }
    }

    return false;
}

bool SWE::DisplayScene::scrollHandle(bool isup)
{
    Window* topWin = windowsFocusHandle();

    if(topWin)
    {
        if(topWin->scrollHandle(isup) || topWin->isModality())
	    return true;
    }

    for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
    {
        if((*it)->isVisible() && !(*it)->isHidden())
        {
	    if(topWin == *it) continue;

            if((*it)->isModality())
            {
		(*it)->scrollHandle(isup);
                return true;
            }
            else
	    if((*it)->isAreaPoint(Display::mouseCursorPosition()))
	    {
		if((*it)->scrollHandle(isup))
            	    return true;
	    }
        }
    }

    return false;
}

#ifdef SWE_SDL12
bool SWE::DisplayScene::userHandle(const UserEvent & ev)
#else
bool SWE::DisplayScene::userHandle(const SDL_UserEvent & ev)
#endif
{
    // receiver exists
    if(ev.data1)
    {
        if(std::any_of(sceneItems.begin(), sceneItems.end(), [&](const Window* win){ return win == ev.data1; }))
        {
            if(ev.code == Signal::WindowCreated)
            {
        	Window* win = static_cast<Window*>(ev.data1);
                win->windowCreateEvent();
                return true;
            }
	    else
            if(ev.code == Signal::WindowCheckFocus)
            {
		// rescan focused
		windowsFocusHandle();
		return true;
	    }

    	    ObjectEvent* dst = static_cast<ObjectEvent*>(ev.data1);
            return  dst->userEvent(ev.code, ev.data2);
        }
        else
        if(std::any_of(sceneObjects.begin(), sceneObjects.end(), [&](const BaseObject* obj){ return obj == ev.data1; }))
        {
    	    ObjectEvent* dst = static_cast<ObjectEvent*>(ev.data1);
            return  dst->userEvent(ev.code, ev.data2);
	}
	else
        {
            // skip signal, temp win
            if(ev.code == WindowCreated)
                return true;
            else
	    if(ev.code == WindowCheckFocus)
                return true;

            DEBUG("skip signal: " << String::hex(ev.code) << ", win not found: " << String::hex(reinterpret_cast<std::uintptr_t>(ev.data1)));
            return true;
        }
    }
    else
        // broadcast: for all
    {
        // fixed events: not signal (first and break)
        switch(ev.code)
        {
            case Signal::SceneDirty:
                    setDirty(true);
                    return true;

            case Signal::GestureFingerUp:
            case Signal::GestureFingerDown:
            case Signal::GestureFingerLeft:
            case Signal::GestureFingerRight:
            case Signal::FingerMoveUp:
            case Signal::FingerMoveDown:
            case Signal::FingerMoveLeft:
            case Signal::FingerMoveRight:
            {
                for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
                    if(!(*it)->isHidden() && (*it)->userEvent(ev.code, ev.data2)) return true;

                return false;
            }

            default:
                break;
        }

        // other signal for alls windows
        for(auto & item : sceneItems)
            if(! item->isHidden()) item->userEvent(ev.code, ev.data2);

        // other signal for alls objects
        for(auto & object : sceneObjects)
            if(object->isValidObject()) object->userEvent(ev.code, ev.data2);

        return true;
    }

    return false;
}

void SWE::DisplayScene::tickHandle(u32 ms)
{
    for(auto & win : sceneItems)
    {
        if(! win->isHidden() && ! win->checkState(FlagSystemTickSkip)) win->tickEvent(ms);
        if(sceneToolTip) static_cast<Window*>(sceneToolTip)->tickEvent(ms);
    }

    for(auto & object : sceneObjects)
        if(object->isValidObject()) object->tickEvent(ms);

    // find top focused
    auto it = sceneItems.rbegin();
    for(; it != sceneItems.rend(); ++it)
    {
	if((*it)->isVisible() && !(*it)->isHidden() &&
		(*it)->isModality()) break;

	if((*it)->isVisible() && !(*it)->isHidden() &&
		(*it)->isAreaPoint(Display::mouseCursorPosition())) break;
    }

    if(it != sceneItems.rend() &&
	// maybe modality break
	(*it)->isAreaPoint(Display::mouseCursorPosition()))
    {
	auto win = dynamic_cast<WindowToolTipArea*>(*it);
        if(win && win->tooltipTexture().isValid())
        {
            if(sceneToolTip && ! sceneToolTip->isOwner(*win))
            {
                delete sceneToolTip;
                sceneToolTip = nullptr;
            }

            if(! sceneToolTip)
            {
                sceneToolTip = new DisplayToolTip(win);
                sceneItems.remove(sceneToolTip);
            }
        }
        else if(sceneToolTip)
        {
            delete sceneToolTip;
            sceneToolTip = nullptr;
        }
    }
    else if(sceneToolTip)
    {
        delete sceneToolTip;
        sceneToolTip = nullptr;
    }

#ifdef SWE_SDL12
    // push event if present
    while(events.size())
    {
	userHandle(events.front());
	events.pop_front();
    }
#endif
}

void SWE::DisplayScene::renderPresentHandle(u32 ms)
{
    for(auto & win : sceneItems)
        win->renderPresentEvent(ms);
}

void SWE::DisplayScene::textureInvalidHandle(void)
{
    for(auto & win : sceneItems)
        win->textureInvalidEvent();
}

void SWE::DisplayScene::displayResizeHandle(const Size & sz)
{
    for(auto & win : sceneItems)
    {
    	if(win->isVisible() && !win->isHidden())
    	    win->displayResizeEvent(sz);
    }
}

void SWE::DisplayScene::displayFocusHandle(bool gain)
{
    for(auto & win : sceneItems)
    {
        if(win->isVisible() && !win->isHidden())
            win->displayFocusEvent(gain);
    }
}

#ifdef SWE_WITH_JSON

SWE::JsonObject SWE::DisplayScene::toJson(void)
{
    JsonObject scene;

    scene.addInteger("version", Engine::version());
    scene.addArray("windowSize", JsonPack::size(Display::size()));
    scene.addArray("renderSize", JsonPack::size(Display::device()));
    scene.addBoolean("fullscreeen", Display::isFullscreenWindow());
    scene.addBoolean("maximized", Display::isMaximizedWindow());

    JsonArray windows;
    for(auto & win : sceneItems)
    {
	windows.addObject(win->toJson());
    }
    scene.addArray("windows", windows);

    scene.addObject("cursorTexture", cursorTexture.toJson());
    scene.addArray("cursorOffset", JsonPack::point(cursorOffset));

    if(sceneToolTip)
	scene.addObject("sceneToolTip", sceneToolTip->toJson());

    return scene;
}
#endif

void SWE::DisplayScene::dumpScene(void)
{
    VERBOSE("items: " << sceneItems.size());
    VERBOSE("TOP LAYER");

    for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
    {
	(*it)->dumpState();
    }
}

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
        DisplayToolTip(const WindowToolTipArea* win) : Window(NULL), mouseIdle(0), owner(win)
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

#ifdef WITH_JSON
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

    protected:
        Point fixPosition(const Point & mousepos, const Size & winsz, const Size & displaysz)
        {
            const int offset = 2;
            int posx = mousepos.x + winsz.w + offset > displaysz.w ? mousepos.x - winsz.w - offset : mousepos.x + offset;
            int posy = mousepos.y - winsz.h - offset;
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
	    if(NULL == lhs->parent() && rhs->parent())
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
    std::list<Window*>	sceneItems;
    bool		sceneDirty = false;
    DisplayToolTip*	sceneToolTip = NULL;

    Texture		cursorTexture;
    Point		cursorOffset;

    bool		markTopWidget = false;
#ifdef OLDENGINE
    // SDL12 small queue events, resize it, see pushEvent
    std::list<SDL_Event> events;
#endif
}


SWE::Window* SWE::DisplayScene::rootWindow(void)
{
    for(auto it = sceneItems.begin(); it != sceneItems.end(); ++it)
        if((*it)->parent() == NULL) return *it;

    return NULL;
}

SWE::Window* SWE::DisplayScene::focusedWindow(void)
{
    for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
    {
        if((*it)->isVisible() && !(*it)->isHidden() &&
                (*it)->isAreaPoint(Display::mouseCursorPosition()))
	{
	    (*it)->focusHandle(true);
	    return *it;
	}
    }

    return NULL;
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

std::list<SWE::Window*> SWE::DisplayScene::findChilds(const Window & self)
{
    std::list<Window*> childs;

    for(auto it = sceneItems.begin(); it != sceneItems.end(); ++it)
    {
        if((*it)->parent() == & self) childs.push_back(*it);
    }

    return childs;
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
        sceneToolTip = NULL;
    }
}

void SWE::DisplayScene::moveTopLayer(const Window & win)
{
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
        sceneToolTip = NULL;
    }

    if(cursorTexture.isValid())
        cursorTexture.reset();

    for(auto it = sceneItems.begin(); it != sceneItems.end(); ++it)
    {
        (*it)->setVisible(false);

        if((*it)->checkState(FlagAllocated))
        {
            delete *it;
            *it = NULL;
        }
    }

    sceneItems.clear();
}

void SWE::DisplayScene::sceneRedraw(void)
{
    u32 tickCurrent = Tools::ticks();

    if(isDirty())
    {
	// redraw only root items
	for(auto it = sceneItems.begin(); it != sceneItems.end(); ++it)
    	    if((*it)->parent() == NULL)(*it)->redraw();

#ifdef SWE_DEBUG
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

	if(sceneToolTip) sceneToolTip->redraw();

	if(cursorTexture.isValid())
    	    Display::renderTexture(cursorTexture, Display::mouseCursorPosition() + cursorOffset);

	// render present
        Display::renderPresent();
        renderPresentHandle(Tools::ticks() - tickCurrent);

        setDirty(false);
    }
}

void SWE::DisplayScene::handleWhileVisible(const Window & win)
{
    while(win.isVisible() && Display::handleEvents())
    {
        DisplayScene::sceneRedraw();
        Tools::delay(1);
    }
}

void SWE::DisplayScene::pushEvent(const Window* dst, int code, void* data)
{
    SDL_Event event;
    std::memset(&event, 0, sizeof(event));
    event.type = SDL_USEREVENT;
    event.user.code = code;
    event.user.data1 = const_cast<Window*>(dst);
    event.user.data2 = data;

    if(0 > SDL_PushEvent(&event))
    {
#ifdef OLDENGINE
	events.emplace_back(event);
#else
        ERROR("win: " << String::pointer(dst) << ", " << "code: " << String::hex(code) << ", " << "error: " << SDL_GetError());
#endif
    }
}

////////////// handle events
// enum EventFilter { KeyHandle, MouseButtonHandle, MouseTrackingHandle, SystemTickHandle };

bool SWE::DisplayScene::keyHandle(const KeySym & key, bool press)
{
#ifdef SWE_DEBUG
    if(press && key.isLeftShift() && key.isKeyCode(Key::F8))
    {
        markTopWidget = !markTopWidget;
	sceneDirty = true;
    }

    if(press && key.isLeftShift() && key.isKeyCode(Key::F9))
    {
	dumpScene();
    }

    if(press && key.isLeftShift() && key.isKeyCode(Key::F10))
    {
	if(Systems::saveString2File(toJson().toString(), "scene.json"))
	    VERBOSE("DisplayScene saved to scene.json");
    }
#endif

    for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
    {
        if((*it)->isVisible())
        {
            if((*it)->isModality())
            {
		(*it)->keyHandle(key, press);
                return true;
            }
            else
            if((*it)->checkState(FlagKeyHandle) || (*it)->isFocused())
	    {
		if((*it)->keyHandle(key, press))
		    return true;
	    }
        }
    }

    return false;
}

bool SWE::DisplayScene::textInputHandle(const std::string & str)
{
    for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
    {
        if((*it)->isVisible())
        {
            if((*it)->isModality())
            {
                (*it)->textInputEvent(str);
                return true;
            }
            else
            if((*it)->checkState(FlagKeyHandle) || (*it)->isFocused())
	    {
        	if((*it)->textInputEvent(str))
		    return true;
	    }
        }
    }

    return false;
}

bool SWE::DisplayScene::mouseButtonHandle(const ButtonEvent & st, bool press)
{
    Window* topWin = focusedWindow();

    // global reset focus
    for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
	if(*it != topWin) (*it)->focusHandle(false);

    if(topWin && topWin->mouseButtonHandle(st, press))
	return true;

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
    Window* topWin = focusedWindow();

    // global reset focus
    for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
	if(*it != topWin) (*it)->focusHandle(false);

    if(topWin && topWin->mouseClickHandle(st))
	return true;

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

    Window* topWin = focusedWindow();

    // global reset focus
    for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
    {
	if(*it != topWin) (*it)->focusHandle(false);

        // global mouse tracking event
        if((*it)->checkState(FlagMouseTracking))
            (*it)->mouseTrackingEvent(pos, buttons);
    }

    if(topWin && topWin->mouseMotionHandle(pos, buttons))
	return true;

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
	    if((*it)->isAreaPoint(pos))
	    {
                if((*it)->mouseMotionHandle(pos, buttons))
            	    return true;
            }
        }
    }

    return false;
}

bool SWE::DisplayScene::scrollHandle(bool isup)
{
    Window* topWin = focusedWindow();

    // global reset focus
    for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
	if(*it != topWin) (*it)->focusHandle(false);

    if(topWin && topWin->scrollHandle(isup))
	return true;

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

#ifdef OLDENGINE
bool SWE::DisplayScene::userHandle(const UserEvent & ev)
#else
bool SWE::DisplayScene::userHandle(const SDL_UserEvent & ev)
#endif
{
    // receiver exists
    if(ev.data1)
    {
        if(sceneItems.end() != std::find(sceneItems.begin(), sceneItems.end(), ev.data1))
        {
            // dst always Window type
            Window* win = static_cast<Window*>(ev.data1);

            if(ev.code == Signal::WindowCreated)
            {
                win->windowCreateEvent();
                return true;
            }

            return  win->userEvent(ev.code, ev.data2);
        }
        else
        {
            // skip signal, temp win
            if(ev.code == WindowCreated)
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

        // other signal for alls
        for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
            if(! (*it)->isHidden())(*it)->userEvent(ev.code, ev.data2);

        return true;
    }

    return false;
}

void SWE::DisplayScene::tickHandle(u32 ms)
{
    for(auto it = sceneItems.begin(); it != sceneItems.end(); ++it)
    {
        if(! (*it)->isHidden() && ! (*it)->checkState(FlagSystemTickSkip)) (*it)->tickEvent(ms);
        if(sceneToolTip) static_cast<Window*>(sceneToolTip)->tickEvent(ms);
    }

    // find top focused
    auto it = sceneItems.rbegin();
    for(; it != sceneItems.rend(); ++it)
    {
	if((*it)->isVisible() && !(*it)->isHidden() &&
		(*it)->isAreaPoint(Display::mouseCursorPosition())) break;
    }

    if(it != sceneItems.rend())
    {
	auto win = dynamic_cast<WindowToolTipArea*>(*it);
        if(win && win->tooltipTexture().isValid())
        {
            if(sceneToolTip && ! sceneToolTip->isOwner(*win))
            {
                delete sceneToolTip;
                sceneToolTip = NULL;
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
            sceneToolTip = NULL;
        }
    }
    else if(sceneToolTip)
    {
        delete sceneToolTip;
        sceneToolTip = NULL;
    }

#ifdef OLDENGINE
    // push event if present
    while(events.size() &&
	0 == SDL_PushEvent(& events.front()))
    {
	events.pop_front();
    }
#endif
}

void SWE::DisplayScene::renderPresentHandle(u32 ms)
{
    std::for_each(sceneItems.begin(), sceneItems.end(), std::bind2nd(std::mem_fun(&Window::renderPresentEvent), ms));
}

void SWE::DisplayScene::textureInvalidHandle(void)
{
    std::for_each(sceneItems.begin(), sceneItems.end(), std::mem_fun(&Window::textureInvalidEvent));
}

void SWE::DisplayScene::displayResizeHandle(const Size & sz, bool sdlmode)
{
    for(auto it = sceneItems.begin(); it != sceneItems.end(); ++it)
    {
        if((*it)->isVisible() && !(*it)->isHidden())
            (*it)->displayResizeEvent(sz, sdlmode);
    }
}

void SWE::DisplayScene::displayFocusHandle(bool gain)
{
    for(auto it = sceneItems.begin(); it != sceneItems.end(); ++it)
    {
        if((*it)->isVisible() && !(*it)->isHidden())
            (*it)->displayFocusEvent(gain);
    }
}

void SWE::DisplayScene::handleEvents(u32 interval)
{
    u32 start = Tools::ticks();

    while(true)
    {
        if(Display::handleEvents())
            sceneRedraw();

        if(0 == interval || start + interval < Tools::ticks())
            break;

        Tools::delay(1);
    }
}

#ifdef WITH_JSON

SWE::JsonObject SWE::DisplayScene::toJson(void)
{
    JsonObject res;
    res.addInteger("version", Engine::version());

    JsonObject scene;
    scene.addBoolean("dirty", sceneDirty);


    JsonArray windows;
    for(auto it = sceneItems.begin(); it != sceneItems.end(); ++it)
	windows.addObject((*it)->toJson());
    scene.addArray("windows", windows);

    scene.addObject("cursorTexture", cursorTexture.toJson());
    scene.addArray("cursorOffset", JsonPack::point(cursorOffset));

    if(sceneToolTip)
	scene.addObject("sceneToolTip", sceneToolTip->toJson());

    res.addObject("scene", scene);

    return res;
}

void SWE::DisplayScene::dumpScene(void)
{
    VERBOSE("items: " << sceneItems.size());
    VERBOSE("TOP LAYER");

    std::for_each(sceneItems.rbegin(), sceneItems.rend(), std::mem_fun(&Window::dumpState));
}
#endif

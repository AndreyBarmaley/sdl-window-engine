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

#include "window.h"
#include "display.h"
#include "window_gui.h"
#include "inputs_keys.h"
#include "display_scene.h"

namespace SWE
{
    class WindowToolTip : public WindowTexture
    {
        Point                       mousePos;
        u32                         mouseTimeout;
        u32                         mouseIdle;
        const Window*		owner;

    public:
        WindowToolTip(const Texture & tx, const Window* win) : WindowTexture(tx, NULL), mouseTimeout(400), mouseIdle(0), owner(win)
        {
            resetState(FlagModality);
            setState(FlagLayoutForeground);
        }

        bool isOwner(const Window* win) const
        {
            return owner == win;
        }

    protected:
        Point fixPosition(const Point & mousepos, const Size & winsz, const Size & displaysz)
        {
            const int offset = 2;
            int posx = mousepos.x + winsz.w + offset > displaysz.w ? mousepos.x - winsz.w - offset : mousepos.x + offset;
            int posy = mousepos.y - winsz.h - offset;
            return Point(posx, posy);
        }

        void tickEvent(u32 ms)
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
                    ms - mouseIdle > mouseTimeout)
            {
                setPosition(fixPosition(mousePos, size(), Display::size()));
                setVisible(true);
            }
        }
    };

    std::list<Window*>	sceneItems;
    bool		sceneDirty = false;

    WindowToolTip*	sceneToolTip = NULL;

    Texture		cursorTexture;
    Point		cursorOffset;
}

SWE::Window* SWE::DisplayScene::rootWindow(void)
{
    for(auto it = sceneItems.begin(); it != sceneItems.end(); ++it)
        if(*it && (*it)->parent() == NULL) return *it;

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
        if(*it && (*it)->parent() == & self) childs.push_back(*it);
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
        if(*it)
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
    // redraw only root items
    for(auto it = sceneItems.begin(); it != sceneItems.end(); ++it)
        if(*it && (*it)->parent() == NULL)(*it)->redraw();

    if(sceneToolTip) sceneToolTip->redraw();

    if(cursorTexture.isValid())
        Display::renderTexture(cursorTexture, Display::mouseCursorPosition() + cursorOffset);
}

void SWE::DisplayScene::dumpScene(void)
{
    for(auto it = sceneItems.begin(); it != sceneItems.end(); ++it)
        if(*it) DEBUG((*it)->toString());
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
        ERROR("win: " << String::pointer(dst) << ", " << "code: " << String::hex(code) << ", " << "error: " << SDL_GetError());
}

////////////// handle events
enum EventFilter { KeyHandle, MouseButtonHandle, MouseTrackingHandle, SystemTickHandle };

bool SWE::DisplayScene::keyPressHandle(const KeySym & key)
{
    for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
    {
        if(*it && (*it)->isVisible())
        {
            if((*it)->checkState(FlagModality))
            {
                (*it)->keyPressEvent(key);
                return true;
            }
            else

                // focus
                if(((*it)->isFocused() || (*it)->checkState(FlagKeyHandle)) &&
                   (*it)->keyPressEvent(key)) return true;
        }
    }

    return false;
}

bool SWE::DisplayScene::keyReleaseHandle(const KeySym & key)
{
    for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
    {
        if(*it && (*it)->isVisible())
        {
            if((*it)->checkState(FlagModality))
            {
                (*it)->keyReleaseEvent(key);
                return true;
            }
            else

                // focus
                if(((*it)->isFocused() || (*it)->checkState(FlagKeyHandle)) &&
                   (*it)->keyReleaseEvent(key)) return true;
        }
    }

    return false;
}

bool SWE::DisplayScene::textInputHandle(const std::string & str)
{
    for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
    {
        if(*it && (*it)->isVisible())
        {
            if((*it)->checkState(FlagModality))
            {
                (*it)->textInputEvent(str);
                return true;
            }
            else

                // focus
                if(((*it)->isFocused() || (*it)->checkState(FlagKeyHandle)) &&
                   (*it)->textInputEvent(str)) return true;
        }
    }

    return false;
}

bool SWE::DisplayScene::mousePressHandle(const ButtonEvent & st)
{
    for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
    {
        if(*it && (*it)->isVisible())
        {
            bool focus = (*it)->isAreaPoint(st.position());

            if(focus && !(*it)->isFocused())
            {
                (*it)->setState(FlagFocused);
                (*it)->mouseFocusEvent();
            }
            else if(! focus && (*it)->isFocused())
            {
                (*it)->resetState(FlagFocused);
                (*it)->mouseLeaveEvent();
            }

            if((*it)->checkState(FlagModality))
            {
                (*it)->mousePressEvent(ButtonEvent(st.button(), st.position() - (*it)->position()));
                return true;
            }
            else if((*it)->isFocused() &&
                    (*it)->mousePressEvent(ButtonEvent(st.button(), st.position() - (*it)->position())))
                return true;
        }
    }

    return false;
}

bool SWE::DisplayScene::mouseReleaseHandle(const ButtonEvent & st)
{
    for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
    {
        if(*it && (*it)->isVisible())
        {
            bool focus = (*it)->isAreaPoint(st.position());

            if(focus && !(*it)->isFocused())
            {
                (*it)->setState(FlagFocused);
                (*it)->mouseFocusEvent();
            }
            else if(! focus && (*it)->isFocused())
            {
                (*it)->resetState(FlagFocused);
                (*it)->mouseLeaveEvent();
            }

            if((*it)->checkState(FlagModality))
            {
                (*it)->mouseReleaseEvent(ButtonEvent(st.button(), st.position() - (*it)->position()));
                return true;
            }
            else if((*it)->isFocused() &&
                    (*it)->mouseReleaseEvent(ButtonEvent(st.button(), st.position() - (*it)->position())))
                return true;
        }
    }

    return false;
}

bool SWE::DisplayScene::mouseClickHandle(const ButtonsEvent & st)
{
    for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
    {
        if(*it && (*it)->isVisible())
        {
            bool focus = (*it)->isAreaPoint(st.press().position()) && (*it)->isAreaPoint(st.release().position());

            if(focus && !(*it)->isFocused())
            {
                (*it)->setState(FlagFocused);
                (*it)->mouseFocusEvent();
            }
            else if(! focus && (*it)->isFocused())
            {
                (*it)->resetState(FlagFocused);
                (*it)->mouseLeaveEvent();
            }

            if((*it)->checkState(FlagModality))
            {
                (*it)->mouseClickEvent(ButtonsEvent(st.press().button(),
                                                    st.press().position() - (*it)->position(),
                                                    st.release().position() - (*it)->position()));
                return true;
            }
            else if((*it)->isFocused() &&
                    (*it)->mouseClickEvent(ButtonsEvent(st.press().button(), st.press().position() - (*it)->position(),
                                           st.release().position() - (*it)->position())))
                return true;
        }
    }

    return false;
}

void SWE::DisplayScene::mouseMotionHandle(const Point & pos, u32 buttons)
{
    for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
    {
        if(*it && (*it)->isVisible())
        {
            bool focus = (*it)->isAreaPoint(pos);

            if(focus && !(*it)->isFocused())
            {
                (*it)->setState(FlagFocused);
                (*it)->mouseFocusEvent();
            }
            else if(! focus && (*it)->isFocused())
            {
                (*it)->resetState(FlagFocused);
                (*it)->mouseLeaveEvent();
            }

            // global mouse tracking event
            if((*it)->checkState(FlagMouseTracking))
                (*it)->mouseTrackingEvent(pos, buttons);

            // local mouse motion event
            if((*it)->checkState(FlagModality))
            {
                (*it)->mouseMotionEvent(pos - (*it)->position(), buttons);
                break;
            }
            else if((*it)->isFocused() &&
                    (*it)->mouseMotionEvent(pos - (*it)->position(), buttons))
                break;
        }
    }

    if(cursorTexture.isValid())
        setDirty(true);
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
                    if(*it && (*it)->parent() != NULL && (*it)->userEvent(ev.code, ev.data2)) return true;

                for(auto it = sceneItems.begin(); it != sceneItems.end(); ++it)
                    if(*it && (*it)->parent() == NULL && (*it)->userEvent(ev.code, ev.data2)) return true;

                return false;
            }

            default:
                break;
        }

        // other signal for alls
        for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
            if(*it && (*it)->parent() != NULL)(*it)->userEvent(ev.code, ev.data2);

        for(auto it = sceneItems.begin(); it != sceneItems.end(); ++it)
            if(*it && (*it)->parent() == NULL)(*it)->userEvent(ev.code, ev.data2);

        return true;
    }

    return false;
}

bool SWE::DisplayScene::scrollUpHandle(const Point & pos)
{
    for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
    {
        if(*it && (*it)->isVisible())
        {
            bool focus = (*it)->isAreaPoint(pos);

            if(focus && !(*it)->isFocused())
            {
                (*it)->setState(FlagFocused);
                (*it)->mouseFocusEvent();
            }
            else if(! focus && (*it)->isFocused())
            {
                (*it)->resetState(FlagFocused);
                (*it)->mouseLeaveEvent();
            }

            if((*it)->checkState(FlagModality))
            {
                (*it)->scrollUpEvent(pos);
                return true;
            }
            else if((*it)->isFocused() && (*it)->scrollUpEvent(pos))
                return true;
        }
    }

    return false;
}

bool SWE::DisplayScene::scrollDownHandle(const Point & pos)
{
    for(auto it = sceneItems.rbegin(); it != sceneItems.rend(); ++it)
    {
        if(*it && (*it)->isVisible())
        {
            bool focus = (*it)->isAreaPoint(pos);

            if(focus && !(*it)->isFocused())
            {
                (*it)->setState(FlagFocused);
                (*it)->mouseFocusEvent();
            }
            else if(! focus && (*it)->isFocused())
            {
                (*it)->resetState(FlagFocused);
                (*it)->mouseLeaveEvent();
            }

            if((*it)->checkState(FlagModality))
            {
                (*it)->scrollDownEvent(pos);
                return true;
            }
            else if((*it)->isFocused() && (*it)->scrollDownEvent(pos))
                return true;
        }
    }

    return false;
}

void SWE::DisplayScene::tickHandle(u32 ms)
{
    for(auto it = sceneItems.begin(); it != sceneItems.end(); ++it)
    {
        if(*it) (*it)->tickEvent(ms);

        if(sceneToolTip) static_cast<Window*>(sceneToolTip)->tickEvent(ms);
    }

    // find top focused
    auto it = sceneItems.rbegin();
    for(; it != sceneItems.rend(); ++it)
        if((*it) && (*it)->isFocused()) break;

    if(it != sceneItems.rend())
    {
        const Texture* tx = (*it)->tooltipTexture();

        if(tx)
        {
            if(sceneToolTip && ! sceneToolTip->isOwner(*it))
            {
                delete sceneToolTip;
                sceneToolTip = NULL;
            }

            if(! sceneToolTip)
            {
                sceneToolTip = new WindowToolTip(*tx, *it);
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
}

void SWE::DisplayScene::renderPresentHandle(u32 ms)
{
    for(auto it = sceneItems.begin(); it != sceneItems.end(); ++it)
    {
        if(*it)(*it)->renderPresentEvent(ms);
    }
}

void SWE::DisplayScene::textureInvalidHandle(void)
{
    for(auto it = sceneItems.begin(); it != sceneItems.end(); ++it)
    {
        if(*it)(*it)->textureInvalidEvent();
    }
}

void SWE::DisplayScene::displayResizeHandle(const Size & sz, bool sdlmode)
{
    for(auto it = sceneItems.begin(); it != sceneItems.end(); ++it)
    {
        if(*it && (*it)->isVisible())
            (*it)->displayResizeEvent(sz, sdlmode);
    }
}

void SWE::DisplayScene::displayFocusHandle(bool gain)
{
    for(auto it = sceneItems.begin(); it != sceneItems.end(); ++it)
    {
        if(*it && (*it)->isVisible())
            (*it)->displayFocusEvent(gain);
    }
}

void SWE::DisplayScene::handleEvents(u32 interval)
{
    u32 start = Tools::ticks();

    while(true)
    {
        if(Display::handleEvents())
            Display::redraw();

        if(0 == interval || start + interval < Tools::ticks())
            break;

        Tools::delay(1);
    }
}
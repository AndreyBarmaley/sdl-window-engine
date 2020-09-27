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
#include "swe_display_scene.h"
#include "swe_inputs_keys.h"
#include "swe_wingui.h"
#include "swe_wingui_button.h"

namespace SWE
{
    /* WindowButton */
    WindowButton::WindowButton(Window* win) : WindowToolTipArea(win), hotkey(Key::NONE)
    {
        setKeyHandle(true);
        signalSubscribe(*this, Signal::ButtonTimerComplete);
    }

    WindowButton::WindowButton(const Size & sz, Window* win) : WindowToolTipArea(sz, win), hotkey(Key::NONE)
    {
        setKeyHandle(true);
        signalSubscribe(*this, Signal::ButtonTimerComplete);
    }

    WindowButton::WindowButton(const Point & pos, const Size & sz, Window* win) : WindowToolTipArea(pos, sz, win), hotkey(Key::NONE)
    {
        setKeyHandle(true);
        signalSubscribe(*this, Signal::ButtonTimerComplete);
    }

    WindowButton::WindowButton(const WindowButton & win) : WindowToolTipArea(win), hotkey(win.hotkey)
    {
        signalSubscribe(*this, Signal::ButtonTimerComplete);
    }

    u32 WindowButton::renderButtonComplete(u32 tick, void* ptr)
    {
        if(ptr)
        {
            // ptr always WindowButton type
            WindowButton* win = static_cast<WindowButton*>(ptr);
            win->signalEmit(Signal::ButtonTimerComplete);
        }

        return 0;
    }

    void WindowButton::setInformed(bool f)
    {
        setState(FlagInformed, f);
    }

    void WindowButton::renderDisabled(void)
    {
        auto tx = textureDisabled();

        if(tx && tx->isValid() && ! size().isEmpty())
            renderTexture(*tx, SWE::Point(0, 0));
    }

    void WindowButton::renderPressed(void)
    {
        auto tx = texturePressed();

        if(tx && tx->isValid() && ! size().isEmpty())
            renderTexture(*tx, SWE::Point(0, 0));
    }

    void WindowButton::renderFocused(void)
    {
        auto tx = textureFocused();

        if(tx && tx->isValid() && ! size().isEmpty())
            renderTexture(*tx, SWE::Point(0, 0));
    }

    void WindowButton::renderInformed(void)
    {
        auto tx = textureInformed();

        if(tx && tx->isValid() && ! size().isEmpty())
            renderTexture(*tx, SWE::Point(0, 0));
    }

    void WindowButton::renderReleased(void)
    {
        auto tx = textureReleased();

        if(tx && tx->isValid() && ! size().isEmpty())
            renderTexture(*tx, SWE::Point(0, 0));
    }

    void WindowButton::renderWindow(void)
    {
	if(isDisabled())
            renderDisabled();
        else
	if(isPressed())
            renderPressed();
        else
	if(isFocused() && textureFocused() && textureFocused()->isValid())
            renderFocused();
        else
	if(checkState(FlagInformed) && textureInformed() && textureInformed()->isValid())
            renderInformed();
        else
            renderReleased();
    }

    void WindowButton::setPressed(bool f)
    {
        if(f && ! isPressed())
        {
            setState(FlagPressed);
            signalEmit(Signal::ButtonPressed);
        }
        else if(!f && isPressed())
        {
            resetState(FlagPressed);
            signalEmit(Signal::ButtonReleased);

            if(isAreaPoint(Display::mouseCursorPosition()))
                setClickedComplete();
        }

        setDirty(true);
    }

    void WindowButton::setReleased(void)
    {
        resetState(FlagPressed);
        signalEmit(Signal::ButtonReleased);
        setDirty(true);
    }

    void WindowButton::setClickedComplete(void)
    {
        signalEmit(Signal::ButtonClicked);

        if(action() && parent())
        {
            //SignalMember::signalEmit(*parent(), action());
            pushEventAction(action(), parent(), this);
        }
    }

    void WindowButton::setClicked(void)
    {
        if(! checkState(FlagDisabled) &&
           ! checkState(FlagPressed))
        {
            setPressed(true);
            SDL_AddTimer(100, renderButtonComplete, this);
        }
    }

    void WindowButton::setDisabled(bool f)
    {
        setState(FlagDisabled, f);
        setDirty(true);
    }

    void WindowButton::setAction(int action)
    {
        setResultCode(action);
    }

    bool WindowButton::isDisabled(void) const
    {
        return checkState(FlagDisabled);
    }

    bool WindowButton::isPressed(void) const
    {
        return checkState(FlagPressed);
    }

    bool WindowButton::isReleased(void) const
    {
        return ! isPressed();
    }

    bool WindowButton::isAction(int val) const
    {
        return action() == val;
    }

    int WindowButton::action(void) const
    {
        return resultCode();
    }

    bool WindowButton::mousePressEvent(const ButtonEvent & st)
    {
        if(! checkState(FlagDisabled) && st.isButtonLeft() && ! checkState(FlagPressed))
        {
            setPressed(true);
            return true;
        }

        return false;
    }

    bool WindowButton::mouseReleaseEvent(const ButtonEvent & st)
    {
        if(! checkState(FlagDisabled) && st.isButtonLeft() && checkState(FlagPressed))
        {
            setPressed(false);
            return true;
        }

        return false;
    }

    void WindowButton::mouseFocusEvent(void)
    {
        setDirty(true);
    }

    void WindowButton::mouseLeaveEvent(void)
    {
        if(! checkState(FlagDisabled) && checkState(FlagPressed))
            setPressed(false);
        else
            setDirty(true);
    }

    bool WindowButton::keyPressEvent(const KeySym & key)
    {
        if(hotkey && hotkey == key.keycode() && ! isDisabled() && isReleased())
        {
            setPressed(true);
            return true;
        }

        return false;
    }

    bool WindowButton::keyReleaseEvent(const KeySym & key)
    {
        if(hotkey == key.keycode() && ! isDisabled() && isPressed())
        {
            setReleased();
            setClickedComplete();
            return true;
        }

        return false;
    }

    void WindowButton::signalReceive(int sig, const SignalMember* sm)
    {
        if(sig == Signal::ButtonTimerComplete && sm == this)
        {
            setReleased();
            setClickedComplete();
        }
    }

    void WindowButton::setHotKey(const std::string & str)
    {
        if(str.size())
            hotkey = Key::toKey(str);
    }

    void WindowButton::setHotKey(int key)
    {
        hotkey = key;
    }

    bool WindowButton::isHotKey(int val) const
    {
        return hotKey() == val;
    }

    int WindowButton::hotKey(void) const
    {
        return hotkey;
    }

    std::string WindowButton::toString(void) const
    {
        return StringFormat("%1, disabled(%2), pressed(%3), released(%4)").arg(Window::toString()).
               arg(isDisabled() ? 1 : 0).arg(isPressed() ? 1 : 0).arg(isReleased() ? 1 : 0);
    }

    /* TextureButton */
    TextureButton::TextureButton(const Point & pos, const Texture & tx1, const Texture & tx2, int action, Window* win)
        : WindowButton(win)
    {
        setPosition(pos);
        setAction(action);
        setSprites(tx1, tx2);
    }

    const Texture* TextureButton::textureDisabled(void) const
    {
        return texturePressed();
    }

    const Texture* TextureButton::texturePressed(void) const
    {
        return &txPress;
    }

    const Texture* TextureButton::textureReleased(void) const
    {
        return &txRelease;
    }

    void TextureButton::setSprites(const Texture & tx1, const Texture & tx2)
    {
        txRelease = tx1;
        txPress = tx2;
        setSize(tx1.size() < tx2.size() ? tx2.size() : tx1.size());

        if(isVisible())
            DisplayScene::setDirty(true);
    }

    /* WindowCheckBox */
    bool WindowCheckBox::mousePressEvent(const ButtonEvent & st)
    {
        if(! checkState(FlagDisabled) && st.isButtonLeft())
        {
            setPressed(! checkState(FlagPressed));
            return true;
        }

        return false;
    }

    void WindowCheckBox::mouseLeaveEvent(void)
    {
        // skip WindowButton::mouseLeaveEvent
    }

    bool WindowCheckBox::mouseReleaseEvent(const ButtonEvent & st)
    {
        return true;
    }

}

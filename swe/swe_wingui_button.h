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

#ifndef _SWE_GUI_BUTTON_
#define _SWE_GUI_BUTTON_

#include "swe_wingui.h"
// WindowButton emit signals: Signal::ButtonPressed, Signal::ButtonReleased, Signal::ButtonClicked

/// @brief пространство SWE
namespace SWE
{
    struct KeySym;

    /* WindowButton */
    class WindowButton : public WindowToolTipArea
    {
        int         hotkey;

    protected:
        virtual const Texture* 	textureFocused(void) const  { return nullptr; }
        virtual const Texture* 	textureDisabled(void) const { return texturePressed(); }
        virtual const Texture* 	texturePressed(void) const  { return textureReleased(); }
        virtual const Texture* 	textureReleased(void) const { return nullptr; }
        virtual const Texture* 	textureInformed(void) const { return nullptr; }

        bool        		mousePressEvent(const ButtonEvent &) override;
        bool        		mouseReleaseEvent(const ButtonEvent &) override;
        void        		mouseLeaveEvent(void) override;
        void        		mouseFocusEvent(void) override;
        bool			keyPressEvent(const KeySym &) override;
        bool			keyReleaseEvent(const KeySym &) override;
        void			signalReceive(int, const SignalMember*) override;

        void			setClickedComplete(void);
        void			setReleased(void);

        static u32		renderButtonComplete(u32 tick, void* ptr);

        virtual void		renderFocused(void);
        virtual void		renderDisabled(void);
        virtual void		renderPressed(void);
        virtual void		renderReleased(void);
        virtual void		renderInformed(void);

    public:
        WindowButton(Window*);
        WindowButton(const Size &, Window*);
        WindowButton(const Point &, const Size &, Window*);
        WindowButton(const WindowButton &);

        void        		renderWindow(void) override;

        void        		setAction(int);
        void       		setDisabled(bool);
        void			setPressed(bool);
        void			setClicked(void);
        void			setInformed(bool);
        void       		setHotKey(int);
        void       		setHotKey(const std::string &);

        bool        		isAction(int) const;
        bool        		isDisabled(void) const;
        bool       		isPressed(void) const;
        bool        		isReleased(void) const;
        bool        		isHotKey(int) const;

        int         		action(void) const;
        int         		hotKey(void) const;

        std::string		toString(void) const override;

	const char*     	className(void) const override { return "SWE::WindowButton"; }
#ifdef SWE_WITH_JSON
        JsonObject		toJson(void) const override
	{
	    JsonObject res = WindowToolTipArea::toJson();
	    res.addInteger("hotkey", hotkey);
	    return res;
	}
#endif
    };

    /* WindowCheckBox */
    class WindowCheckBox : public WindowButton
    {
    protected:
        void 			mouseLeaveEvent(void) override;
        bool 			mousePressEvent(const ButtonEvent &) override;
        bool 			mouseReleaseEvent(const ButtonEvent &) override;

    public:
        WindowCheckBox(Window* win) : WindowButton(win) {}
        WindowCheckBox(const Size & sz, Window* win) : WindowButton(sz, win) {}
        WindowCheckBox(const Point & pos, const Size & sz, Window* win) : WindowButton(pos, sz, win) {}

        void			setChecked(bool f)
        {
            setPressed(f);
        }
        bool			isChecked(void) const
        {
            return isPressed();
        }

	const char*     	className(void) const override { return "SWE::WindowCheckBox"; }
    };

    /* TextureButton */
    class TextureButton : public WindowButton
    {
    protected:
        Texture     txRelease, txPress;

        const Texture* textureDisabled(void) const override;
        const Texture* texturePressed(void) const override;
        const Texture* textureReleased(void) const override;


    public:
        TextureButton(Window* win) : WindowButton(win) {}
        TextureButton(const Size & sz, Window* win) : WindowButton(sz, win) {}
        TextureButton(const Point & pos, const Size & sz, Window* win) : WindowButton(pos, sz, win) {}
        TextureButton(const Point &, const Texture & release, const Texture & press, int action, Window*);

        void			setSprites(const Texture & release, const Texture & press);

	const char*     	className(void) const override { return "SWE::TextureButton"; }
#ifdef SWE_WITH_JSON
        JsonObject		toJson(void) const override
	{
	    JsonObject res = WindowButton::toJson();
	    res.addObject("press", txPress.toJson());
	    res.addObject("release", txRelease.toJson());
	    return res;
	}
#endif
    };

} // SWE
#endif

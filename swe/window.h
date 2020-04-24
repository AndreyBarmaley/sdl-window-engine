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

#ifndef _SWE_WINDOW_
#define _SWE_WINDOW_

#include "types.h"
#include "tools.h"
#include "rect.h"
#include "surface.h"
#include "fontset.h"
#include "events.h"

namespace SWE
{

    typedef std::uintptr_t	WindowId;
    struct KeySym;

    enum { FlagVisible = 0x80000000, FlagModality = 0x40000000, FlagFocused = 0x20000000, FlagAllocated = 0x10000000,

           FlagKeyHandle = 0x08000000, FlagMouseTracking = 0x04000000,
           FlagLayoutBackground = 0x02000000, FlagLayoutForeground = 0x01000000,

           FlagButtonPressed = 0x00800000, FlagButtonCentered = 0x00400000, FlagButtonDisabled = 0x00200000, FlagButtonFocused = 0x00100000,
           FlagButtonInformed = 0x00080000, FlagSelected = 0x00040000, FlagWrap = 0x00020000, FlagVertical = 0x00010000,
           FlagFreeMask = 0x0000FFFF
         };

    // KeyHandle, MouseButtonHandle, MouseTrackingHandle, SystemTickHandle

    class Window : public SignalMember
    {
        friend class DisplayScene;

    protected:
        Rect            gfxpos;
        Window*         prnt;
        BitFlags	state;
        int             result;

        void		destroy(void);
        void		redraw(void);
        void		setSize(int, int);
        void		setPosition(int, int);

        // SignalMember
        // virtual void	signalReceive(int, const SignalMember*) {}

        // Window
        virtual void	textureInvalidEvent(void) {}
        virtual void	windowMoveEvent(const Point &) {}
        virtual void	windowResizeEvent(const Size &) {}
        virtual void	windowVisibleEvent(bool) {}
        virtual void	windowCreateEvent(void) {}
        virtual bool	keyPressEvent(const KeySym &) { return false; }
        virtual bool	keyReleaseEvent(const KeySym &) { return false; }
        virtual bool	textInputEvent(const std::string &) { return false; }
        virtual bool	mousePressEvent(const ButtonEvent &) { return false; }
        virtual bool	mouseReleaseEvent(const ButtonEvent &) { return false; }
        virtual bool	mouseClickEvent(const ButtonsEvent &) { return false; }
        virtual void	mouseFocusEvent(void) {}
        virtual void	mouseLeaveEvent(void) {}
        virtual void	mouseTrackingEvent(const Point &, u32 buttons) {}
        virtual bool	mouseMotionEvent(const Point &, u32 buttons) { return false; }
        virtual bool	userEvent(int, void*) { return false; }
        virtual bool	scrollUpEvent(const Point &) { return false; }
        virtual bool	scrollDownEvent(const Point &) { return false; }
        virtual void	tickEvent(u32 ms) {}
        virtual void	renderPresentEvent(u32 ms) {}
        virtual void	displayResizeEvent(const Size &, bool) {}
        virtual void	displayFocusEvent(bool gain) {}

        virtual const Texture* tooltipTexture(void) const
        {
            return NULL;
        }

    public:
        Window(Window*);
        Window(const Point &, const Size &, Window*);
        virtual ~Window();

        Window(Window &&) noexcept;
        Window(const Window &);
        Window 	&	operator= (const Window &);

        bool		isID(const WindowId &) const;
        bool		isVisible(void) const;
        bool		isFocused(void) const;
        virtual bool	isAreaPoint(const Point &) const;
        WindowId	id(void) const;
        const Point &	position(void) const;
        const Size &	size(void) const;
        const Rect &	area(void) const;
        const Window*	parent(void) const;
        Window*		parent(void);
        int		width(void) const;
        int		height(void) const;
        int		resultCode(void) const;
        Rect		rect(void) const;

        void		setParent(Window*);
        void		setVisible(bool);
        void		setModality(bool);
        void		setResultCode(int);
        virtual void	setSize(const Size &);
        virtual void	setPosition(const Point &);
        void		setLayerTop(void);

        bool		checkState(size_t) const;
        void		setState(size_t, bool f = true);
        void		resetState(size_t);
        void		switchedState(size_t);

        int		exec(void);
        void		pushEventAction(int, Window*, void*);

        void            renderSurface(const Surface &, const Point &) const;
        void            renderSurface(const Surface &, const Rect &, const Point &) const;
        void            renderTexture(const Texture &, const Point &) const;
        void            renderTexture(const Texture &, const Rect &, const Point &) const;
        void            renderTexture(const TexturePos &) const;

        Rect		renderText(const FontRender &, const UnicodeString &, const Color &, const Point &, int halign = AlignLeft, int valign = AlignTop, bool horizontal = true) const;

        virtual void    renderClear(const Color &) const;
        void            renderColor(const Color &, const Rect &) const;
        void            renderRect(const Color &, const Rect &) const;
        void            renderLine(const Color &, const Point &, const Point &) const;
        void            renderPoint(const Color &, const Point &) const;

        virtual void	renderWindow(void) = 0;

        virtual std::string toString(void) const;
    };

    class DisplayWindow : public Window
    {
        Color	backcolor;

    public:
        DisplayWindow(const Color & back = Color(Color::Black));

        void	renderWindow(void);
    };

    class CenteredWindow : public Window
    {
    public:
        CenteredWindow(const Size &, Window &);
    };

} // SWE
#endif

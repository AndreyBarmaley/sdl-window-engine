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

#include "swe_types.h"
#include "swe_tools.h"
#include "swe_rect.h"
#include "swe_surface.h"
#include "swe_fontset.h"
#include "swe_events.h"
#include "swe_display.h"

/// @brief пространство SWE
namespace SWE
{

    typedef std::uintptr_t	WindowId;
    struct KeySym;

    enum { // Window 0xxxxx0000
	    FlagVisible = 0x80000000, FlagModality = 0x40000000, FlagFocused = 0x20000000, FlagBroken = 0x10000000,

	    FlagKeyHandle = 0x04000000, FlagMouseTracking = 0x02000000, FlagSystemTickSkip = 0x01000000,
	    FlagLayoutHidden = 0x00800000, FlagLayoutBackground = 0x00200000, FlagLayoutForeground = 0x00100000,

	    // GUI 0x0000xxxx
	    FlagPressed = 0x00080000, FlagInformed = 0x00040000, FlagSelected = 0x00020000, FlagDisabled = 0x00010000,
	    FlagWrap = 0x00008000, FlagVertical = 0x00004000,
	    FlagFreeMask = 0x00000FFF
         };

    // MouseButtonHandle, SystemTickHandle
    class Window;

    /// @brief базовый класс графических объектов сцены DisplayScene
    class Window : public SignalMember
    {
        friend class DisplayScene;

	//
	void		focusHandle(bool gain);
	bool		scrollHandle(bool isup);
	bool		mouseMotionHandle(const Point &, u32);
	bool		mouseButtonHandle(const ButtonEvent & st, bool press);
	bool		mouseClickHandle(const ButtonsEvent & st);
	bool		keyHandle(const KeySym & key, bool press);

    protected:
        Rect            gfxpos;
        Window*         prnt;
        BitFlags	state;
        int             result;

        void		destroy(void);
        void		redraw(void);
        void		setSize(int, int);
        void		setPosition(int, int);

        // SignalMember:
        // virtual void	signalReceive(int, const SignalMember*) override {}
	//
	// ObjectEvent:
        // virtual bool	userEvent(int, void*) override { return false; }
        // virtual void	tickEvent(u32 ms) override {}

	// target
	virtual Texture & targetTexture(void);

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
        virtual bool	scrollUpEvent(void) { return false; }
        virtual bool	scrollDownEvent(void) { return false; }
        virtual void	renderPresentEvent(u32 ms) {}
        virtual void	displayResizeEvent(const Size &) {}
        virtual void	displayFocusEvent(bool gain) {}

    protected:
        bool		checkState(size_t) const;
        void		setState(size_t);
        void		resetState(size_t);
        void		switchedState(size_t);

	virtual void	renderBackground(void) {}
	virtual void	renderForeground(void) {}

    public:
        Window(Window*);
        Window(const Size &, Window*);
        Window(const Point &, const Size &, Window*);
        virtual ~Window();

        Window(Window &&) noexcept;
        Window 	&	operator= (Window &&) noexcept;

        Window(const Window &);
        Window 	&	operator= (const Window &);

	// Signal Member:
        // void         signalSubscribe(const SignalMember & sender, int sig);
        // void         signalUnsubscribe(const SignalMember &);
        // void         signalEmit(int);
	//
	// ObjectEvent:
        // void		pushEventAction(int code, const ObjectEvent* dst, void* data);

        bool		isID(const WindowId &) const;
        bool		isVisible(void) const;
        bool		isHidden(void) const;
        bool		isFocused(void) const;
        bool		isModality(void) const;
        bool		isKeyHandle(void) const;
        bool		isMouseTrack(void) const;
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

        void		setState(size_t, bool f);
	void		setDirty(bool);
        void		setParent(Window*);
        void		setVisible(bool);
	void		setHidden(bool);
        void		setModality(bool);
        void		setKeyHandle(bool);
        void		setMouseTrack(bool);
        void		setResultCode(int);
        virtual void	setSize(const Size &);
        virtual void	setPosition(const Point &);

        int		exec(void);

        /// @brief функция запрета обработки tickEvent
        void            disableTickEvent(bool);


        void            renderSurface(const Surface &, const Point &, int flip = FlipNone);
        void            renderSurface(const Surface &, const Rect &, const Point &, int flip = FlipNone);
        void            renderTexture(const Texture &, const Point &, int flip = FlipNone);
        void            renderTexture(const Texture &, const Rect &, const Point &, int flip = FlipNone);
        void            renderTexture(const TexturePos &);

        Rect		renderText(const FontRender &, const UnicodeString &, const Color &, const Point &,
				    const AlignType & hAlign = AlignLeft, const AlignType & vAlign = AlignTop, bool isHorizontal = true,
				    const CharRender & render = RenderDefault, int style = StyleDefault, const CharHinting & hinting = HintingDefault);

        virtual void    renderClear(const Color &);
        void            renderColor(const Color &, const Rect &);
        void            renderRect(const Color &, const Rect &);
        void            renderLine(const Color &, const Point &, const Point &);
        void            renderPoint(const Color &, const Point &);

        virtual void	renderWindow(void) = 0;

	const char*	className(void) const override { return "SWE::Window"; }
#ifdef SWE_WITH_JSON
	JsonObject 	toJson(void) const override;
#endif
	void		dumpState(void) const;
        virtual std::string toString(void) const;
    };

    class DisplayWindow : public Window
    {
        Color	backcolor;

    public:
        DisplayWindow(const Color & back = Color(Color::Black));

        void		renderBackground(void) override;
        void		renderWindow(void) override {}

	const char*	className(void) const override { return "SWE::DisplayWindow"; }
#ifdef SWE_WITH_JSON
	JsonObject 	toJson(void) const override;
#endif
    };

    class CenteredWindow : public Window
    {
    public:
        CenteredWindow(const Size &, Window*);
	const char*	className(void) const override { return "SWE::CenteredWindow"; }
    };

    class TargetWindow : public Window, public Texture
    {
    protected:
	    Texture &	targetTexture(void) { return *this; }

    public:
	    TargetWindow(const Size &);
    };
} // SWE
#endif

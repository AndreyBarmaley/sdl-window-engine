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

#include <list>

#include "types.h"
#include "tools.h"
#include "rect.h"
#include "surface.h"
#include "display.h"
#include "fontset.h"
#include "events.h"

namespace Display
{
    void                handleMouseButton(const SDL_MouseButtonEvent &);
    void                handleMouseMotion(const SDL_MouseMotionEvent &);
    void		handleKeyboard(const SDL_KeyboardEvent &);
    void		handleUserEvent(const SDL_UserEvent &);
    void		handleScrollEvent(int, const Point &);
    void		handleFocusEvent(bool);
    void		handleEvents(void);
    bool		resizeWindow(const Size &, bool);
    void		redraw(void);
#ifdef OLDENGINE
    void                handleMouseWheel(int, int);
#else
    void                handleMouseWheel(const SDL_MouseWheelEvent &);
    void		handleTextInput(const SDL_TextInputEvent &);
    void		handleFingerTap(const SDL_TouchFingerEvent &);
    void		handleFingerMotion(const SDL_TouchFingerEvent &);
#endif
}

class Window;
class WindowAction;
typedef std::uintptr_t	WindowId;

struct WindowChilds : std::list<Window*>
{
    Window*		findWindow(const WindowId &);
    const Window*	findWindowConst(const WindowId &) const;
};

enum { FlagVisible = 0x80000000, FlagKeyHandle = 0x40000000, FlagModality = 0x20000000, FlagOnMouse = 0x10000000,
	FlagButtonPressed = 0x08000000, FlagButtonCentered = 0x04000000, FlagButtonDisabled = 0x02000000, FlagButtonFocused = 0x01000000,
	FlagSelected = 0x00800000, FlagOrderBackground = 0x00400000, FlagOrderForeground = 0x00200000, FlagVertical = 0x00100000,
	FlagWrap = 0x00080000, FlagTermCursor = 0x00040000, FlagDebug = 0x00010000 };

class Window : public SignalMember
{
    friend void         Display::handleMouseButton(const SDL_MouseButtonEvent &);
    friend void         Display::handleMouseMotion(const SDL_MouseMotionEvent &);
    friend void		Display::handleKeyboard(const SDL_KeyboardEvent &);
    friend void		Display::handleUserEvent(const SDL_UserEvent &);
    friend void		Display::handleScrollEvent(int, const Point &);
    friend void		Display::handleFocusEvent(bool);
    friend void		Display::redraw(void);
    friend void		Display::handleEvents(void);
    friend bool		Display::resizeWindow(const Size &, bool);
#ifdef OLDENGINE
    friend void         Display::handleMouseWheel(int, int);
#else
    friend void         Display::handleMouseWheel(const SDL_MouseWheelEvent &);
    friend void		Display::handleTextInput(const SDL_TextInputEvent &);
    friend void		Display::handleFingerTap(const SDL_TouchFingerEvent &);
    friend void		Display::handleFingerMotion(const SDL_TouchFingerEvent &);
#endif

    bool		keyPressHandle(int);
    bool		keyReleaseHandle(int);
    bool		textInputHandle(const std::string &);
    bool		mousePressHandle(const ButtonEvent &);
    bool		mouseReleaseHandle(const ButtonEvent &);
    bool		mouseClickHandle(const ButtonsEvent &);
    bool		mouseMotionHandle(const Point &, u32 buttons);
#ifdef OLDENGINE
    bool		userHandle(const UserEvent &);
#else
    bool		userHandle(const SDL_UserEvent &);
#endif
    bool		scrollUpHandle(const Point &);
    bool		scrollDownHandle(const Point &);
    bool		scrollLeftHandle(const Point &);
    bool		scrollRightHandle(const Point &);
    void		tickHandle(u32);
    void		renderPresentHandle(u32);
    void		displayResizeHandle(const Size &, bool);
    void		displayFocusHandle(bool);

protected:
    Rect            	gfxpos;
    Window*         	prnt;
    WindowChilds*	childs;
    BitFlags		state;
    int             	result;

    void		redraw(void);

    virtual void	windowMoveEvent(const Point &) {}
    virtual void	windowResizeEvent(const Size &) {}
    virtual bool	keyPressEvent(int) { return false; }
    virtual bool	keyReleaseEvent(int) { return false; }
    virtual bool	textInputEvent(const std::string &) { return false; }
    virtual bool	mousePressEvent(const ButtonEvent &) { return false; }
    virtual bool	mouseReleaseEvent(const ButtonEvent &) { return false; }
    virtual bool	mouseClickEvent(const ButtonsEvent &) { return false; }
    virtual void	mouseFocusEvent(void) {}
    virtual void	mouseLeaveEvent(void) {}
    virtual bool	mouseMotionEvent(const Point &, u32 buttons) { return false; }
    virtual bool	userEvent(int, void*) { return false; }
    virtual bool	scrollUpEvent(const Point &) { return false; }
    virtual bool	scrollDownEvent(const Point &) { return false; }
    virtual bool	scrollLeftEvent(const Point &) { return false; }
    virtual bool	scrollRightEvent(const Point &) { return false; }
    virtual void	tickEvent(u32 ms) {}
    virtual void	renderPresentEvent(u32 ms) {}
    virtual void	displayResizeEvent(const Size &, bool);
    virtual void	displayFocusEvent(bool gain) {}

    bool		findChild(const Window*) const;

public:
    Window(Window*);
    Window(const Point &, const Size &, Window*);
    //Window(const Window &);
    virtual ~Window();

    Window &		operator= (const Window &);

    bool		isID(const WindowId &) const;
    bool		isVisible(void) const;
    WindowId		id(void) const;
    const Point	&	position(void) const;
    const Size &	size(void) const;
    const Rect &	area(void) const;
    const Window*	parent(void) const;
    Window*		parent(void);
    int			width(void) const;
    int			height(void) const;
    int			resultCode(void) const;
    Rect		rect(void) const;

    void		setParent(Window*);
    void		setVisible(bool);
    void		setResultCode(int);
    void		setSize(const Size &);
    void		setPosition(const Point &);

    bool		checkState(int) const;
    void		setState(int);
    void		resetState(int);

    int			exec(void);
    void		destroy(void);
    void		pushEventAction(int, Window*, void*);
    static void		pushEventActionBroadcast(int, void*);

    void                renderSurface(const Surface &, const Point &);
    void		renderSurface(const Surface &, const Rect &, const Rect &);
    void                renderTexture(const Texture &, const Point &);
    void		renderTexture(const Texture &, const Rect &, const Rect &);
    void                renderTexture(const TexturePos &);

    Rect		renderText(const FontRender &, const UnicodeString &, const Color &, const Point &, int halign = AlignLeft, int valign = AlignTop, bool horizontal = true);

    void                renderClear(const Color &);
    void                renderColor(const Color &, const Rect &);
    void                renderRect(const Color &, const Rect &);
    void                renderLine(const Color &, const Point &, const Point &);
    void                renderPoint(const Color &, const Point &);

    virtual void	renderWindow(void) = 0;

    std::string		toString(void) const;
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

#endif

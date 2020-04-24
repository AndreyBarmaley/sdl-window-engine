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

#ifndef _SWE_EVENTS_
#define _SWE_EVENTS_

#include "rect.h"

namespace SWE
{
    void	clearAllSignals(void);

    struct SignalMember
    {
        SignalMember() {}
        virtual ~SignalMember();

        static void	signalSubscribe(const SignalMember &, int, SignalMember &);
        static void	signalEmit(const SignalMember &, int);
        static bool	signalSubscribed(const SignalMember &, int = 0);

    protected:
        void		signalSubscribe(const SignalMember &, int);
        void		signalUnsubscribe(const SignalMember &);

        void		signalEmit(int);
        virtual void	signalReceive(int, const SignalMember*) {}
    };

#ifdef OLDENGINE
    enum { ButtonNone, ButtonLeft = SDL_BUTTON(SDL_BUTTON_LEFT), ButtonRight = SDL_BUTTON(SDL_BUTTON_RIGHT), ButtonMiddle = SDL_BUTTON(SDL_BUTTON_MIDDLE),
           ButtonX1 = SDL_BUTTON(SDL_BUTTON_WHEELUP), ButtonX2 = SDL_BUTTON(SDL_BUTTON_WHEELDOWN), FingerTap = SDL_BUTTON(ButtonLeft)
         };
#else
    enum { ButtonNone, ButtonLeft = SDL_BUTTON(SDL_BUTTON_LEFT), ButtonRight = SDL_BUTTON(SDL_BUTTON_RIGHT), ButtonMiddle = SDL_BUTTON(SDL_BUTTON_MIDDLE),
           ButtonX1 = SDL_BUTTON(SDL_BUTTON_X1), ButtonX2 = SDL_BUTTON(SDL_BUTTON_X2), FingerTap = SDL_BUTTON(ButtonLeft)
         };
#endif

    class MouseButton
    {
    protected:
        int              btn;

    public:
        MouseButton(int type = ButtonNone) : btn(type) {}

        bool 		isButton(int type) const 	{ return btn == type; }
        bool 		isButtonLeft(void) const 	{ return btn == ButtonLeft; }
        bool 		isButtonRight(void) const 	{ return btn == ButtonRight; }
        bool 		isButtonMiddle(void) const 	{ return btn == ButtonMiddle; }
        bool 		isButtonX1(void) const 		{ return btn == ButtonX1; }
        bool 		isButtonX2(void) const 		{ return btn == ButtonX2; }
        int  		button(void) const 		{ return btn; }
    };

    class ButtonEvent : public MouseButton
    {
    protected:
        Point           coord;

    public:
        ButtonEvent(int type = ButtonNone) : MouseButton(type) {}
        ButtonEvent(int type, const Point & pos) : MouseButton(type), coord(pos) {}

        void 		setPosition(const Point & pos) { coord = pos; }
        const Point &	position(void) const { return coord; }
    };

    class ButtonsEvent
    {
    protected:
        ButtonEvent     coord1;
        ButtonEvent     coord2;

    public:
        ButtonsEvent(int type = ButtonNone) : coord1(type), coord2(type) {}
        ButtonsEvent(int type, const Point & pos1, const Point & pos2) : coord1(type, pos1), coord2(type, pos2) {}

        bool		isClick(const Rect & rt) const {
        			return (rt & coord1.position()) && (rt & coord2.position()); }

        void		setPress(const Point & pos) 	{ coord1.setPosition(pos); }
        void		setRelease(const Point & pos) 	{ coord2.setPosition(pos); }

        const ButtonEvent & press(void) const 		{ return coord1; }
        const ButtonEvent & release(void) const 	{ return coord2; }

        bool		isButtonLeft(void) const 	{ return press().isButtonLeft(); }
        bool		isButtonRight(void) const 	{ return press().isButtonRight(); }
        bool		isButtonMiddle(void) const 	{ return press().isButtonMiddle(); }
        bool		isButtonX1(void) const 		{ return press().isButtonX1(); }
        bool		isButtonX2(void) const 		{ return press().isButtonX2(); }
    };

    struct UserEvent
    {
        int                 code;
        void*               data1;
        void*               data2;

        UserEvent() : code(0), data1(NULL), data2(NULL) {}
        UserEvent(int v, void* p1, void* p2) : code(v), data1(p1), data2(p2) {}
        UserEvent(const SDL_UserEvent & ev) : code(ev.code), data1(ev.data1), data2(ev.data2) {}

        bool operator== (const UserEvent & ev) const
        {
            return code == ev.code && data1 == ev.data1 && data2 == ev.data2;
        }
    };

    enum Signal
    {
        EventNone = 0x01000000,
        WindowCreated, WindowScrolledPrev, WindowScrolledNext, WindowPageUp, WindowPageDown,
        ListboxChangedList, ListboxSelectedItem, ListBoxClickItem,
        ScrollBarScrolledPrev, ScrollBarScrolledNext, ScrollBarMovedCursor,
        ButtonPressed, ButtonReleased, ButtonClicked, ButtonSetFocus, ButtonResetFocus, ButtonTimerComplete,
        GestureFingerUp, GestureFingerDown, GestureFingerLeft, GestureFingerRight,
        FingerMoveUp, FingerMoveDown, FingerMoveLeft, FingerMoveRight,
        LuaUnrefAction
    };

} // SWE
#endif

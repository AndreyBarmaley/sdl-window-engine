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

#include "swe_rect.h"
#include "swe_object.h"

/// @brief пространство SWE
namespace SWE
{
    /// @protected
    void	clearAllSignals(void);

    /// @brief класс объектов сцены DisplayScene, с возможностью отправки/получения сигналов
/*!
    @details
    Алгоритм использования:
        - подписка на событие
        @code
            void signalSubscribe(const SignalMember & sender, int signal, SignalMember & receiver);
        @endcode
        - отправка signal со стороны отправителя
        @code
            void signalEmit(int signal);
        @endcode
        - получение через виртуальную функцию
        @code
            void signalReceive(int signal, const SignalMember* sender) override;
        @endcode
        особенность данного метода - получение signal происходит в конце этой же итерации основного цикла обработки событий DisplayScene,
        а также нет возможности отправки данных.

    @see ObjectEvent
*/
    class SignalMember : public ObjectEvent
    {
    public:
        virtual ~SignalMember();

	/// @brief функция подписки на сигнал
	/// @param sender отправитель сигнала
	/// @param sig тип сигнала
	/// @param receiver получатель сигнала
        static void	signalSubscribe(const SignalMember & sender, int sig, SignalMember & receiver);

	/// @brief функция отписки от сигнала
	/// @param receiver получатель сигнала
        static void	signalUnsubscribe(const SignalMember & receiver);

	/// @brief функция отправки сигнала с подстановкой отправителя
	/// @param sender отправитель сигнала
	/// @param sig тип сигнала
        static void	signalEmit(const SignalMember & sender, int sig);

	/// @brief функция проверки на подписку сигнала
	/// @param sig тип сигнала, если равно нулю - проверяется на любой подписанный сигнал
        static bool	signalSubscribed(const SignalMember & sender, int sig = 0 /* all signals */);

	const char*	className(void) const override { return "SWE::SignalMember"; }

    protected:
	/// @brief функция подписки на сигнал
	/// @param sender отправитель сигнала
	/// @param sig тип сигнала
        void		signalSubscribe(const SignalMember & sender, int sig);


	/// @brief фунция отправки сигнала
	/// @param sig тип сигнала
        void		signalEmit(int sig);

	/// @brief метод получатель, вызывается при signalEmit со стороны отправителя
        virtual void	signalReceive(int sig, const SignalMember* sender) {}
    };

    /// @brief базовый класс не графических объектов сцены DisplayScene
    class BaseObject : public SignalMember
    {
    public:
	BaseObject();
	virtual ~BaseObject();

	/// @brief функция состояния объекта, используется для авторизации получения событий ObjectEvent::tickEvent и ObjectEvent::userEvent
	virtual bool	isValidObject(void) const { return false; }
    };

#ifdef SWE_SDL12
    /// @enum SWE::ButtonType
    /// @brief перечисление тип кнопки мышки
    enum ButtonType
    {
        ButtonNone, ButtonLeft = SDL_BUTTON(SDL_BUTTON_LEFT), ButtonRight = SDL_BUTTON(SDL_BUTTON_RIGHT), ButtonMiddle = SDL_BUTTON(SDL_BUTTON_MIDDLE),
           ButtonX1 = SDL_BUTTON(SDL_BUTTON_WHEELUP), ButtonX2 = SDL_BUTTON(SDL_BUTTON_WHEELDOWN), FingerTap = SDL_BUTTON(ButtonLeft)
    };
#else
    /// @enum SWE::ButtonType
    /// @brief перечисление тип кнопки мышки
    enum ButtonType
    {
        ButtonNone, ButtonLeft = SDL_BUTTON(SDL_BUTTON_LEFT), ButtonRight = SDL_BUTTON(SDL_BUTTON_RIGHT), ButtonMiddle = SDL_BUTTON(SDL_BUTTON_MIDDLE),
           ButtonX1 = SDL_BUTTON(SDL_BUTTON_X1), ButtonX2 = SDL_BUTTON(SDL_BUTTON_X2), FingerTap = SDL_BUTTON(ButtonLeft)
    };
#endif

    /// @brief базовый класс кнопки мышки
    class MouseButton
    {
    protected:
        ButtonType      btn;

    public:
        MouseButton(ButtonType type = ButtonNone) : btn(type) {}

        bool 		isButton(ButtonType type) const { return btn == type; }
        bool 		isButtonLeft(void) const 	{ return btn == ButtonLeft; }
        bool 		isButtonRight(void) const 	{ return btn == ButtonRight; }
        bool 		isButtonMiddle(void) const 	{ return btn == ButtonMiddle; }
        bool 		isButtonX1(void) const 		{ return btn == ButtonX1; }
        bool 		isButtonX2(void) const 		{ return btn == ButtonX2; }
        const ButtonType & button(void) const 		{ return btn; }
    };

    /// @brief event класс кнопки мыши
    class ButtonEvent : public MouseButton
    {
    protected:
        Point           coord;

    public:
        ButtonEvent(ButtonType type = ButtonNone) : MouseButton(type) {}
        ButtonEvent(ButtonType type, const Point & pos) : MouseButton(type), coord(pos) {}

        void 		setPosition(const Point & pos) { coord = pos; }
        const Point &	position(void) const { return coord; }
    };

    /// @brief event класс кнопки мыши (с состояниями press, release)
    class ButtonsEvent
    {
    protected:
        ButtonEvent     coord1;
        ButtonEvent     coord2;

    public:
        ButtonsEvent(ButtonType type = ButtonNone) : coord1(type), coord2(type) {}
        ButtonsEvent(ButtonType type, const Point & pos1, const Point & pos2) : coord1(type, pos1), coord2(type, pos2) {}

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

    /// @brief event класс пользовательских данных
    struct UserEvent
    {
        int                 code;
        void*               data1;
        void*               data2;

        UserEvent() : code(0), data1(nullptr), data2(nullptr) {}
        UserEvent(int v, void* p1, void* p2) : code(v), data1(p1), data2(p2) {}
        UserEvent(const SDL_UserEvent & ev) : code(ev.code), data1(ev.data1), data2(ev.data2) {}

        bool operator== (const UserEvent & ev) const
        {
            return code == ev.code && data1 == ev.data1 && data2 == ev.data2;
        }
    };

    /// @enum SWE::Signal
    /// @brief перечисление системных сигналов
    enum Signal
    {
        EventNone = 0x01000000,
        SceneDirty,
        WindowCreated, WindowCheckFocus, WindowScrolledPrev, WindowScrolledNext, WindowPageUp, WindowPageDown,
	ListWidgetScrolled, ListWidgetChanged, ListWidgetRemoveItem, ListWidgetTakeItem,
        ScrollBarMoved,
	LineEditTextChanged, LineEditCursorChanged, LineEditReturnPressed,

        ButtonPressed, ButtonReleased, ButtonClicked, ButtonSetFocus, ButtonResetFocus, ButtonTimerComplete,
        GestureFingerUp, GestureFingerDown, GestureFingerLeft, GestureFingerRight,
        FingerMoveUp, FingerMoveDown, FingerMoveLeft, FingerMoveRight,
        LuaUnrefAction
    };

} // SWE
#endif

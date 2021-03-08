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

#ifndef _SWE_DISPLAY_SCENE_
#define _SWE_DISPLAY_SCENE_

#include <list>
#include "swe_events.h"

#ifdef SWE_WITH_JSON
#include "swe_json_ext.h"
#endif

/// @brief пространство SWE
namespace SWE
{
    class Window;
    class Texture;
    struct KeySym;
    class BaseObject;

/*! 
    @details
    @section sec1 Основной цикл обработки сообщений сцены

    Цикл обработки запускается от любого объекта класса Window,
    @code
	int 	Window::exec(void);
    @endcode
    при этом допустимы вложенные вызовы, [Window::exec()] -> [Window::exec()] -> [Window::exec()]

    Цикл обработки определен в DisplayScene::handleEvents, и состоит из трех из 3 основных частей:
	- обработка SDL_Event событий Display::handleEvents и рассылка по адресатам
	- генерация события ObjectEvent::tickEvent
	- рендер сцены DisplayScene::sceneRedraw

    Базовое взаимодействиe между объектами сцены DisplayScene:
    1. использование методов класса ObjectEvent
        - отправка event
        @code
            void pushEventAction(int code, ObjectEvent* dst, void* data);
        @endcode
        - получение через виртуальную функцию
        @code
            bool userEvent(int code, void* data) override;
        @endcode
        особенность данного метода - используется внутренние механизмы SDL_PushEvent/SDL_PollEvent,
        здесь есть возможность отправки данных через указатель и получение event происходит
        на следующую итерацию основного цикла обработки событий DisplayScene.

    2. использование методов класса SignalMember
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
        особенность данного метода - получение signal происходит на этойже итерации в конце основного цикла обработки событий DisplayScene,
        а также нет возможности отправки данных.
*/
    class DisplayScene
    {
    public:
        static Window*          rootWindow(void);
	static Window*          focusedWindow(void);
	/// @private
	static Window*          windowsFocusHandle(void);

	/// @private
        static bool             keyHandle(const KeySym &, bool);
	/// @private
        static bool             textInputHandle(const std::string &);
	/// @private
        static bool             mouseButtonHandle(const ButtonEvent &, bool);
	/// @private
        static bool             mouseClickHandle(const ButtonsEvent &);
	/// @private
        static bool             mouseMotionHandle(const Point &, u32 buttons);
#ifdef SWE_SDL12
	/// @private
        static bool             userHandle(const UserEvent &);
#else
	/// @private
        static bool             userHandle(const SDL_UserEvent &);
#endif
	/// @private
        static bool             scrollHandle(bool);
	/// @private
        static void             tickHandle(u32);
	/// @private
        static void             renderPresentHandle(u32);
	/// @private
        static void             textureInvalidHandle(void);
	/// @private
        static void             displayResizeHandle(const Size &);
	/// @private
        static void             displayFocusHandle(bool);

	static const std::list<Window*> & items(void);
        static std::list<Window*>  findChilds(const Window &);
        static std::list<Window*>  findParents(const Window &);

	static void		addObject(BaseObject &);
	static void		removeObject(const BaseObject &);

	/// @brief функция добавления объекта Window на сцену
	/// @warning при создании объекта Window вызывается автоматически из конструктора Window
        static void		addItem(Window &);
	/// @brief функция удаления объекта Window из сцены
	/// @warning при удалении объекта Window вызывается автоматически из деструктора Window
        static void		removeItem(const Window &);
	/// @brief функция отправки сообщения на сцену
        static void		pushEvent(const ObjectEvent* dst, int code, void* data);
	static void		moveTopLayer(const Window &);

	/// @brief функция рендера сцены DisplayScene
        static void		sceneRedraw(bool force = false);
	/// @private
        static void		sceneDestroy(void);
        static void		handleWhileVisible(const Window &);
	/// @brief функция установки спрайта графического курсора
        static void		setCursor(const Texture &, const Point &);
	/// @brief функция отмены установки спрайта графического курсора
        static void		resetCursor(void);

	/// @private
        static bool		isDirty(void);
	/// @brief фунция манипулирования флагом необходимости отрисовки сцены
        static void		setDirty(bool);

	/// @brief функция главного цикла сцены DisplayScene
        static void		handleEvents(u32 interval = 0);

#ifdef SWE_WITH_JSON
	static JsonObject	toJson(void);
#endif
	/// @brief функция дампа в консоль всех Window обектов текущей сцены
        static void		dumpScene(void);
    };

} // SWE
#endif

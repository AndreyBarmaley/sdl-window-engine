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

#ifndef _SWE_OBJECT_
#define _SWE_OBJECT_

#include "swe_types.h"

#ifdef SWE_WITH_JSON
#include "swe_json_ext.h"
#endif

/// @brief пространство SWE
namespace SWE
{
    class DisplayScene;

    /// @brief родительский класс объектов сцены DisplayScene
    class ObjectClass
    {
    public:
	virtual ~ObjectClass() {}

	/// @brief идентификацинная метка класса
	virtual const char* className(void) const
	{
	    return "SWE::ObjectClass";
	}

#ifdef SWE_WITH_JSON
	/// @brief упаковка класса в json
	virtual JsonObject toJson(void) const
	{
	    JsonObject res;
	    res.addString("className", className());
	    return res;
	}
#endif
    };

    /// @brief класс объектов сцены DisplayScene, с возможностью отправки/получения сообщений
/*! 
    @details
    Использование:
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

    @see SignalMember
*/

    class ObjectEvent : public ObjectClass
    {
    protected:
	friend class DisplayScene;

	/// @brief метод получатель, вызывается при получении сообщения, отправленного через pushEventAction
	/// @param code тип сообщения
	/// @param data указатель на данные
	/// @return не используется
        virtual bool    userEvent(int code, void* data) { return false; }

	/// @brief метод получатель, вызывается один раз за каждую итерацию главного цикла сцены DisplayScene, но перед отрисовкой всей сцены
	/// @param ms продолжительность работы программы от начала запуска в миллисекундах
	/// @see DisplayScene::handleEvents
        virtual void    tickEvent(u32 ms) {}

    public:
	virtual ~ObjectEvent() {}

	/// @brief функция отправки сообщения с данными для объектов сцены DisplayScene
	/// @param code тип сообщения
	/// @param data указатель на данные
	/// @param dst получатель сообщения, для отправки сообщения типа broadcast используйте nullptr 
	void		pushEventAction(int code, ObjectEvent* dst, void* data);

	const char*	className(void) const override
	{
	    return "SWE::ObjectEvent";
	}
    };
}

#endif

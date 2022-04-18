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

#include <ctime>

#include "swe_engine.h"
#include "swe_events.h"
#include "swe_music.h"
#include "swe_display.h"

namespace SWE
{
    namespace Engine
    {
	int		engineDebug = 0;
        void		quit(void);
    }

    namespace Display
    {
#ifdef __MINGW32__
	extern bool fingerEventEmulation;
#endif
	void		closeWindow(void);
    }
}

#ifdef BOOST_STACKTRACE_USE_ADDR2LINE
#include "boost/stacktrace.hpp"
#endif

void SWE::Engine::except(const char* func, const char* message)
{
    if(func && message) COUT(String::time() << ": [EXCEPTION]\t" << func << ":  " << message);

#ifdef BOOST_STACKTRACE_USE_ADDR2LINE

    if(!message || strcmp(message, "SDL_QUIT"))
        LogWrapper() << boost::stacktrace::stacktrace();

#endif
#ifdef ANDROID
    exit(0);
#else
    throw exception();
#endif
}

bool SWE::Engine::init(bool debug)
{
    engineDebug = debug;
    int init_flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER;
    std::srand((u32) std::time(0));
    DEBUG("SDL Window Engine: " << "version: " << version());
    const SDL_version* sdlver1 = nullptr;

#ifdef SWE_SDL12
    sdlver1 = SDL_Linked_Version();
    DEBUG("usage " << "SDL" << ", " << "version: " << static_cast<int>(sdlver1->major) << "." << static_cast<int>(sdlver1->minor) << "." << static_cast<int>(sdlver1->patch));
#else
    SDL_version sdlver2;
    SDL_GetVersion(&sdlver2);
    DEBUG("usage " << "SDL" << ", " << "version: " << static_cast<int>(sdlver2.major) << "." << static_cast<int>(sdlver2.minor) << "." << static_cast<int>(sdlver2.patch) <<
              ", revision: " << SDL_GetRevision());
#endif

    if(SDL_Init(init_flags) != 0)
    {
        ERROR("SDL_Init" << ": " << SDL_GetError());
        return false;
    }

#ifndef SWE_DISABLE_NETWORK
    sdlver1 = SDLNet_Linked_Version();

    if(sdlver1)
        DEBUG("usage " << "SDL_net" << ", " << "version: " << static_cast<int>(sdlver1->major) << "." << static_cast<int>(sdlver1->minor) << "." << static_cast<int>(sdlver1->patch));

    if(SDLNet_Init() < 0)
    {
        ERROR("SDLNet_Init" << ": " << SDL_GetError());
        return false;
    }
#endif // SWE_DISABLE_NETWORK

#ifndef SWE_DISABLE_IMAGE
    sdlver1 = IMG_Linked_Version();

    if(sdlver1)
        DEBUG("usage " << "SDL_image" << ", " << "version: " << static_cast<int>(sdlver1->major) << "." << static_cast<int>(sdlver1->minor) << "." << static_cast<int>(sdlver1->patch));

    if(IMG_Init(IMG_INIT_PNG) == 0)
    {
        ERROR("IMG_Init" << ": " << SDL_GetError());
        return false;
    }
#endif // SWE_DISABLE_IMAGE

#ifndef SWE_DISABLE_TTF
    sdlver1 = TTF_Linked_Version();

    if(sdlver1)
        DEBUG("usage " << "SDL_ttf" << ", " << "version: " << static_cast<int>(sdlver1->major) << "." << static_cast<int>(sdlver1->minor) << "." << static_cast<int>(sdlver1->patch));

    if(TTF_Init() != 0)
    {
        ERROR("TTF_Init" << ": " << SDL_GetError());
	return false;
    }
#endif // SWE_DISABLE_TTF

#ifndef SWE_DISABLE_AUDIO
#if (SDL_VERSIONNUM(SDL_MIXER_MAJOR_VERSION, SDL_MIXER_MINOR_VERSION, SDL_MIXER_PATCHLEVEL) > SDL_VERSIONNUM(1,2,8))
    sdlver1 = Mix_Linked_Version();

    if(sdlver1)
        DEBUG("usage " << "SDL_mixer" << ", " << "version: " << static_cast<int>(sdlver1->major) << "." << static_cast<int>(sdlver1->minor) << "." << static_cast<int>(sdlver1->patch));

#if (SDL_VERSIONNUM(SDL_MIXER_MAJOR_VERSION, SDL_MIXER_MINOR_VERSION, SDL_MIXER_PATCHLEVEL) < SDL_VERSIONNUM(2,0,4))
    int formats = MIX_INIT_MP3|MIX_INIT_OGG|MIX_INIT_MOD;
#else
    int formats = MIX_INIT_MP3|MIX_INIT_OGG|MIX_INIT_MOD|MIX_INIT_MID;
#endif
    int res = Mix_Init(formats);

    if(res != formats)
        ERROR("MIX_Init" << ": " << Mix_GetError());
#endif // SDL_VERSION

    if(0 > Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024))
        ERROR("MIX_OpenAudio" << ": " << Mix_GetError());

#endif // SWE_DISABLE_AUDIO

#ifdef __MINGW32__
    Display::fingerEventEmulation = true;
#endif

#ifdef SWE_SDL12
    SDL_EnableKeyRepeat(400, 75);
#else
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
#endif

    return true;
}

int SWE::Engine::version(void)
{
    return SWE_VERSION;
}

void SWE::Engine::setDebugMode(int f)
{
    engineDebug = f;
}

int SWE::Engine::debugMode(void)
{
    return engineDebug;
}

void SWE::Engine::quit(void)
{
    clearAllSignals();
    Display::closeWindow();
#ifndef SWE_DISABLE_AUDIO
    Music::reset();
    Mix_CloseAudio();
#if (SDL_VERSIONNUM(SDL_MIXER_MAJOR_VERSION, SDL_MIXER_MINOR_VERSION, SDL_MIXER_PATCHLEVEL) > SDL_VERSIONNUM(1,2,8))
    Mix_Quit();
#endif
#endif
#ifndef SWE_DISABLE_IMAGE
    IMG_Quit();
#endif
#ifndef SWE_DISABLE_NETWORK
    SDLNet_Quit();
#endif
#ifndef SWE_DISABLE_TTF
    TTF_Quit();
#endif
    SDL_Quit();
}


/*!
@mainpage Библиотека графической сцены (SDL Window Engine)
Что это?

    - библиотека графической сцены на базе SDL1.2 или SDL2

А попроще?

    - просто создаем объекты Window, и при этом, не вдаваясь во все внутренние подробности библиотеки SDL, наша сцена DisplayScene автоматически
      обрабатывает все нужные события и доносит их до получателей

Что такое Window?

    - Window это базовый графической примитив, который можно поместить на сцену DisplayScene.
      он не содержит каких либо текстур спрайтов, это простейшая невидимая область Rect на сцене DisplayScene

    @code
        class Window : public SignalMember
        {
            Rect            gfxpos;
            Window*         parent;
            BitFlags        state;
            int             result;
        };
    @endcode


Как начать?
    - инициализируем графическую сцену

    @code
        SWE::Engine::init();
        SWE::Display::init(std::string title, SWE::Size geometry);
    @endcode

    - создаем базовый объект Window на всю доступную область, это первый элемент сцены, все последующие элементы будут потомками от него

    @code
        class MainWindow : public SWE::Window
        {
        protected:
            bool mouseMotionEvent(const SWE::Point & pos, u32 buttons) override
            {
                VERBOSE("mouse coord: " << pos.toString());
                return true;
            }

            bool keyPressEvent(const SWE::KeySym & key)
            {
                if(key.keycode() == Key::ESCAPE)
                {
                    setVisible(false);
                    return true;
                }
                return false;
            }

        public:
            MainWindow() : SWE::Window(SWE::Display::size(), nullptr)
            {
                setVisible(true);
            }

            void renderWindow(void) override
            {
                renderClear(SWE::Color::Gray);
            }
        };
    @endcode

    - переопределяем нужные виртуальные функции

    @code
        // SWE::ObjectEvent
        bool userEvent(int event, void* data) override;
        void tickEvent(u32 ms) override;

        // SWE::SignalMember
        void signalReceive(int signal, const SWE::SignalMember* sender) override;

        // SWE::Window
        void windowCreateEvent(void);
        bool keyPressEvent(const SWE::KeySym & ks);
        bool keyReleaseEvent(const SWE::KeySym & ks);
        bool textInputEvent(const std::string & str);
        bool mousePressEvent(const SWE::ButtonEvent & be);
        bool mouseReleaseEvent(const SWE::ButtonEvent & be);
        bool mouseClickEvent(const SWE::ButtonsEvent & be);
        void mouseFocusEvent(void);
        void mouseLeaveEvent(void);
        void mouseTrackingEvent(const SWE::Point & pos, u32 buttons);
        bool mouseMotionEvent(const SWE::Point & pos, u32 buttons);
        bool scrollUpEvent(void);
        bool scrollDownEvent(void);
    @endcode

    - минимальные требования для получения events это иметь размерность и определить видимость

    @code
    void Window::setSize(const SWE::Size &);
    void Window::setVisible(true);
    @endcode

    - Для того чтобы его увидеть на сцене не забываем про основной код отрисовки объекта

    @code
        void renderWindow(void) override
        {
            // renderClear
            // renderColor
            // renderSurface
            // renderTexture
            // renderText
            // renderRect
            // renderLine
            // renderPoint
        }
    @endcode

    - создаем множество других объектов, с родителем MainWindow

    @code
        class RedWindow : public SWE::Window
        {
        protected:
            void mouseFocusEvent(void) override
            {
                VERBOSE("focused!");
            }

        public:
            RedWindow(const SWE::Point & pos, const SWE::Size & wsz, SWE::Window & parent) : SWE::Window(wsz, parent)
            {
                setPosition(pos);
                setModality(false);
                setVisible(true);
            }

            void renderWindow(void) override
            {
                renderClear(SWE::Color::Red);
            }
        };

        class HaosWindow : public SWE::Window
        {
            SWE::Texture sprite;

        protected:
            void tickEvent(u32 ms) override
            {
                auto dsz = SWE::Display::size();
                setPosition(SWE::Tools::rand(0, dsz.w - width()), SWE::Tools::rand(0, dsz.h - height()));
            }

        public:
            HaosWindow(const std::string & file, SWE::Window & parent) : SWE::Window(& parent)
            {
                sprite = SWE::Display::createTexture(file);

                setSize(sprite.size());
                setModality(false);
                setVisible(true);
            }

            void renderWindow(void) override
            {
                renderTexture(sprite, SWE::Point(0, 0));
            }
        };
    @endcode

    - стартует сцена очень просто

    @code
        // первое графическое окно
        MainWindow1 win1;

        // выполняется до момента вызова win1.setVisible(false)
        int result = win1.exec();

        // в зависимости от результата запускаем другие
        if(2 == result)
        {
            // выполняется до момента вызова Window::setVisible(false) внутри класса
            MainWindow2().exec();
        }
        else
        {
            MainWindow3().exec();
        }
    @endcode

    - для большего ознакомления см examples

    1. examples\\test_fps - тест fps рендера 5000 спрайтов
    2. examples\\test_gui - пример построения простейших элементов GUI
    3. examples\\test_term_gui - пример графического терминала, возможности
    4. examples\\test_term_cmd - пример графического терминала, файловый браузер
*/

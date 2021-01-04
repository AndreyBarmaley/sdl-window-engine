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

#ifndef _SWE_KEYS_
#define _SWE_KEYS_

#include <string>
#include "swe_types.h"

/// @brief пространство SWE
namespace SWE
{

#ifdef SWE_SDL12
    typedef SDL_keysym SDL_Keysym;
#endif

    struct KeyName
    {
        int key;
        const char* name;

        bool iskey(int v) const;
        bool isname(const std::string &) const;

        KeyName() : key(0), name(nullptr) {}
        KeyName(int k, const char* v) : key(k), name(v) {}
    };

    /// @brief пространство Key
    namespace Key
    {
        /// @brief перечисление тип клавиатурных символов
        enum
        {
            NONE = SDLK_UNKNOWN,
            BACKSPACE = SDLK_BACKSPACE,
            TAB = SDLK_TAB,
            CLEAR = SDLK_CLEAR,
            RETURN = SDLK_RETURN,
            PAUSE = SDLK_PAUSE,
            ESCAPE = SDLK_ESCAPE,
            SPACE = SDLK_SPACE,
            EXCLAIM = SDLK_EXCLAIM,
            QUOTEDBL = SDLK_QUOTEDBL,
            HASH = SDLK_HASH,
            DOLLAR = SDLK_DOLLAR,
            AMPERSAND = SDLK_AMPERSAND,
            QUOTE = SDLK_QUOTE,
            LEFTPAREN = SDLK_LEFTPAREN,
            RIGHTPAREN = SDLK_RIGHTPAREN,
            ASTERISK = SDLK_ASTERISK,
            PLUS = SDLK_PLUS,
            COMMA = SDLK_COMMA,
            MINUS = SDLK_MINUS,
            PERIOD = SDLK_PERIOD,
            SLASH = SDLK_SLASH,
            K0 = SDLK_0,
            K1 = SDLK_1,
            K2 = SDLK_2,
            K3 = SDLK_3,
            K4 = SDLK_4,
            K5 = SDLK_5,
            K6 = SDLK_6,
            K7 = SDLK_7,
            K8 = SDLK_8,
            K9 = SDLK_9,
            COLON = SDLK_COLON,
            SEMICOLON = SDLK_SEMICOLON,
            LESS = SDLK_LESS,
            EQUALS = SDLK_EQUALS,
            GREATER = SDLK_GREATER,
            QUESTION = SDLK_QUESTION,
            AT = SDLK_AT,
            LEFTBRACKET = SDLK_LEFTBRACKET,
            BACKSLASH = SDLK_BACKSLASH,
            RIGHTBRACKET = SDLK_RIGHTBRACKET,
            CARET = SDLK_CARET,
            UNDERSCORE = SDLK_UNDERSCORE,
            BACKQUOTE = SDLK_BACKQUOTE,
            a = SDLK_a,
            b = SDLK_b,
            c = SDLK_c,
            d = SDLK_d,
            e = SDLK_e,
            f = SDLK_f,
            g = SDLK_g,
            h = SDLK_h,
            i = SDLK_i,
            j = SDLK_j,
            k = SDLK_k,
            l = SDLK_l,
            m = SDLK_m,
            n = SDLK_n,
            o = SDLK_o,
            p = SDLK_p,
            q = SDLK_q,
            r = SDLK_r,
            s = SDLK_s,
            t = SDLK_t,
            u = SDLK_u,
            v = SDLK_v,
            w = SDLK_w,
            x = SDLK_x,
            y = SDLK_y,
            z = SDLK_z,
            DELETE = SDLK_DELETE,
#ifdef SWE_SDL12
            KP_K0 = SDLK_KP0,
            KP_K1 = SDLK_KP1,
            KP_K2 = SDLK_KP2,
            KP_K3 = SDLK_KP3,
            KP_K4 = SDLK_KP4,
            KP_K5 = SDLK_KP5,
            KP_K6 = SDLK_KP6,
            KP_K7 = SDLK_KP7,
            KP_K8 = SDLK_KP8,
            KP_K9 = SDLK_KP9,
#else
            KP_K0 = SDLK_KP_0,
            KP_K1 = SDLK_KP_1,
            KP_K2 = SDLK_KP_2,
            KP_K3 = SDLK_KP_3,
            KP_K4 = SDLK_KP_4,
            KP_K5 = SDLK_KP_5,
            KP_K6 = SDLK_KP_6,
            KP_K7 = SDLK_KP_7,
            KP_K8 = SDLK_KP_8,
            KP_K9 = SDLK_KP_9,
#endif
            KP_PERIOD = SDLK_KP_PERIOD,
            KP_DIVIDE = SDLK_KP_DIVIDE,
            KP_MULTIPLY = SDLK_KP_MULTIPLY,
            KP_MINUS = SDLK_KP_MINUS,
            KP_PLUS = SDLK_KP_PLUS,
            KP_ENTER = SDLK_KP_ENTER,
            KP_EQUALS = SDLK_KP_EQUALS,
            UP = SDLK_UP,
            DOWN = SDLK_DOWN,
            RIGHT = SDLK_RIGHT,
            LEFT = SDLK_LEFT,
            INSERT = SDLK_INSERT,
            HOME = SDLK_HOME,
            END = SDLK_END,
            PAGEUP = SDLK_PAGEUP,
            PAGEDOWN = SDLK_PAGEDOWN,
            F1 = SDLK_F1,
            F2 = SDLK_F2,
            F3 = SDLK_F3,
            F4 = SDLK_F4,
            F5 = SDLK_F5,
            F6 = SDLK_F6,
            F7 = SDLK_F7,
            F8 = SDLK_F8,
            F9 = SDLK_F9,
            F10 = SDLK_F10,
            F11 = SDLK_F11,
            F12 = SDLK_F12,
            F13 = SDLK_F13,
            F14 = SDLK_F14,
            F15 = SDLK_F15,
            CAPSLOCK = SDLK_CAPSLOCK,
#ifdef SWE_SDL12
            SCROLLOCK = SDLK_SCROLLOCK,
#else
            SCROLLOCK = SDLK_SCROLLLOCK,
#endif
            RSHIFT = SDLK_RSHIFT,
            LSHIFT = SDLK_LSHIFT,
            RCTRL = SDLK_RCTRL,
            LCTRL = SDLK_LCTRL,
            RALT = SDLK_RALT,
            LALT = SDLK_LALT,
            MODE = SDLK_MODE,
            HELP = SDLK_HELP,
#ifdef SWE_SDL12
            PRINT = SDLK_PRINT,
#else
            PRINT = SDLK_PRINTSCREEN,
#endif
            SYSREQ = SDLK_SYSREQ,
            MENU = SDLK_MENU,
            POWER = SDLK_POWER,
            UNDO = SDLK_UNDO,

            AC_BACK = 0x4000010e,

            LAST
        };

        int		toKey(const std::string &);
        const char*	toName(int);
	bool		isPressed(int);

        const std::initializer_list<KeyName> & allKeys(void);
    }

    struct KeyMod
    {
	u16	mod;

        KeyMod() : mod(SDL_GetModState()) {}
        KeyMod(u16 val) : mod(val) {}

        bool isCtrl(void) const { return mod & KMOD_CTRL; }
        bool isLeftCtrl(void) const { return mod & KMOD_LCTRL; }
        bool isRightCtrl(void) const { return mod & KMOD_RCTRL; }
        bool isShift(void) const { return mod & KMOD_SHIFT; }
        bool isLeftShift(void) const { return mod & KMOD_LSHIFT; }
        bool isRightShift(void) const { return mod & KMOD_RSHIFT; }
        bool isAlt(void) const { return mod & KMOD_ALT; }
        bool isLeftAlt(void) const { return mod & KMOD_LALT; }
        bool isRightAlt(void) const { return mod & KMOD_RALT; }

#ifndef SWE_SDL12
        bool isGui(void) const { return mod & KMOD_GUI; }
        bool isLGui(void) const { return mod & KMOD_LGUI; }
        bool isRGui(void) const { return mod & KMOD_RGUI; }
#endif

        bool isNum(void) const { return mod & KMOD_NUM; }
        bool isCaps(void) const { return mod & KMOD_CAPS; }
        bool isMode(void) const { return mod & KMOD_MODE; }
    };

    struct KeySym : std::pair<KeyMod, int>
    {
        KeySym(int sym = SDLK_UNKNOWN, int mod = SDL_GetModState()) : std::pair<KeyMod, int>(KeyMod(mod), sym) {}
        KeySym(const SDL_Keysym & sdl) : std::pair<KeyMod, int>(KeyMod(sdl.mod), sdl.sym) {}

        const KeyMod & keymod(void) const
        {
            return first;
        }

        const int & keycode(void) const
        {
            return second;
        }

        int keychar(void) const;

#ifdef SWE_SDL12
        int scancode(void) const
        {
            return 0;
        }
#else
        int scancode(void) const
        {
            return SDL_GetScancodeFromKey(keycode());
        }
#endif
        const char* keyname(void) const
        {
            return Key::toName(keycode());
        }

	bool isKeyCode(int code) const
	{
	    return code == second;
	}
    };

} // SWE
#endif

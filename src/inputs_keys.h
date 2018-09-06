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
#include "types.h"

#ifdef OLDENGINE
typedef int SDL_Keycode;
#endif

namespace Key
{
    enum
    {
        NONE = 0,

        ESCAPE, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, PRINT, INSERT, HOME, END, DELETE, PAGEUP, PAGEDOWN, LEFT, RIGHT, UP, DOWN, RETURN,
	BACKQUOTE, K1, K2, K3, K4, K5, K6, K7, K8, K9, K0, MINUS, EQUALS, BACKSPACE,
	TAB, LEFTBRACKET, RIGHTBRACKET, BACKSLASH, SEMICOLON, QUOTE, COMMA, PERIOD, SLASH, SPACE,
        a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z,

	CTRL		= KMOD_CTRL << 16,
	ALT		= KMOD_ALT << 16,
	SHIFT		= KMOD_SHIFT << 16,

	TILDE = BACKQUOTE | SHIFT, EXCLAIM = K1 | SHIFT, AT = K2 | SHIFT, HASH = K3 | SHIFT, DOLLAR = K4 | SHIFT, PERCENT = K5 | SHIFT, CARET = K6 | SHIFT,
	AMPERSAND = K7 | SHIFT, ASTERISK = K8 | SHIFT, LEFTPAREN = K9 | SHIFT, RIGHTPAREN = K0 | SHIFT, UNDERSCORE = MINUS | SHIFT, PLUS = EQUALS | SHIFT,
	LEFTCURLY = LEFTBRACKET | SHIFT, RIGHTCURLY = RIGHTBRACKET | SHIFT, VERTLINE = BACKSLASH | SHIFT, COLON = SEMICOLON | SHIFT, QUOTEDBL = QUOTE | SHIFT,
	LESS = COMMA | SHIFT, GREATER = PERIOD | SHIFT, QUESTION = SLASH | SHIFT,

	A = a | SHIFT, B = b | SHIFT, C = c | SHIFT, D = d | SHIFT, E = e | SHIFT, F = f | SHIFT, G = g | SHIFT, H = h | SHIFT, I = i | SHIFT,
	J = j | SHIFT, K = k | SHIFT, L = l | SHIFT, M = m | SHIFT, N = n | SHIFT, O = o | SHIFT, P = p | SHIFT, Q = q | SHIFT, R = r | SHIFT,
	S = s | SHIFT, T = t | SHIFT, U = u | SHIFT, V = v | SHIFT, W = w | SHIFT, X = x | SHIFT, Y = y | SHIFT, Z = z | SHIFT,

	AC_SEARCH,
	AC_HOME,
	AC_BACK,
	AC_FORWARD,
	AC_STOP,
	AC_REFRESH,
	AC_BOOKMARKS,

	ANY,
	LAST
    };

    SDL_Keycode toSDLKeyCode(int key);
    int		toKey(const SDL_Keycode &, int mod);
    int		toKey(int ch);
    int		toKey(const std::string &);
    int		toChar(int key);

    int		lower(int key);
    int		upper(int key);
}

#endif

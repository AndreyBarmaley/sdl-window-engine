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

#include <iterator>
#include <algorithm>

#include "swe_systems.h"
#include "swe_inputs_keys.h"

namespace SWE
{
    std::initializer_list<KeyName> allkeys =
    {
        { Key::NONE, "NONE" },
        { Key::BACKSPACE, "BACKSPACE" },
        { Key::TAB, "TAB" },
        { Key::CLEAR, "CLEAR" },
        { Key::RETURN, "RETURN" },
        { Key::PAUSE, "PAUSE" },
        { Key::ESCAPE, "ESCAPE" },
        { Key::SPACE, "SPACE" },
        { Key::EXCLAIM, "EXCLAIM" },
        { Key::QUOTEDBL, "QUOTEDBL" },
        { Key::HASH, "HASH" },
        { Key::DOLLAR, "DOLLAR" },
        { Key::AMPERSAND, "AMPERSAND" },
        { Key::QUOTE, "QUOTE" },
        { Key::LEFTPAREN, "LEFTPAREN" },
        { Key::RIGHTPAREN, "RIGHTPAREN" },
        { Key::ASTERISK, "ASTERISK" },
        { Key::PLUS, "PLUS" },
        { Key::COMMA, "COMMA" },
        { Key::MINUS, "MINUS" },
        { Key::PERIOD, "PERIOD" },
        { Key::SLASH, "SLASH" },
        { Key::K0, "0" },
        { Key::K1, "1" },
        { Key::K2, "2" },
        { Key::K3, "3" },
        { Key::K4, "4" },
        { Key::K5, "5" },
        { Key::K6, "6" },
        { Key::K7, "7" },
        { Key::K8, "8" },
        { Key::K9, "9" },
        { Key::COLON, "COLON" },
        { Key::SEMICOLON, "SEMICOLON" },
        { Key::LESS, "LESS" },
        { Key::EQUALS, "EQUALS" },
        { Key::GREATER, "GREATER" },
        { Key::QUESTION, "QUESTION" },
        { Key::AT, "AT" },
        { Key::LEFTBRACKET, "LEFTBRACKET" },
        { Key::BACKSLASH, "BACKSLASH" },
        { Key::RIGHTBRACKET, "RIGHTBRACKET" },
        { Key::CARET, "CARET" },
        { Key::UNDERSCORE, "UNDERSCORE" },
        { Key::BACKQUOTE, "BACKQUOTE" },
        { Key::a, "A" },
        { Key::b, "B" },
        { Key::c, "C" },
        { Key::d, "D" },
        { Key::e, "E" },
        { Key::f, "F" },
        { Key::g, "G" },
        { Key::h, "H" },
        { Key::i, "I" },
        { Key::j, "J" },
        { Key::k, "K" },
        { Key::l, "L" },
        { Key::m, "M" },
        { Key::n, "N" },
        { Key::o, "O" },
        { Key::p, "P" },
        { Key::q, "Q" },
        { Key::r, "R" },
        { Key::s, "S" },
        { Key::t, "T" },
        { Key::u, "U" },
        { Key::v, "V" },
        { Key::w, "W" },
        { Key::x, "X" },
        { Key::y, "Y" },
        { Key::z, "Z" },
        { Key::DELETE, "DELETE" },
        { Key::KP_K0, "KP_0" },
        { Key::KP_K1, "KP_1" },
        { Key::KP_K2, "KP_2" },
        { Key::KP_K3, "KP_3" },
        { Key::KP_K4, "KP_4" },
        { Key::KP_K5, "KP_5" },
        { Key::KP_K6, "KP_6" },
        { Key::KP_K7, "KP_7" },
        { Key::KP_K8, "KP_8" },
        { Key::KP_K9, "KP_9" },
        { Key::KP_PERIOD, "KP_PERIOD" },
        { Key::KP_DIVIDE, "KP_DIVIDE" },
        { Key::KP_MULTIPLY, "KP_MULTIPLY" },
        { Key::KP_MINUS, "KP_MINUS" },
        { Key::KP_PLUS, "KP_PLUS" },
        { Key::KP_ENTER, "KP_ENTER" },
        { Key::KP_EQUALS, "KP_EQUALS" },
        { Key::UP, "UP" },
        { Key::DOWN, "DOWN" },
        { Key::RIGHT, "RIGHT" },
        { Key::LEFT, "LEFT" },
        { Key::INSERT, "INSERT" },
        { Key::HOME, "HOME" },
        { Key::END, "END" },
        { Key::PAGEUP, "PAGEUP" },
        { Key::PAGEDOWN, "PAGEDOWN" },
        { Key::F1, "F1" },
        { Key::F2, "F2" },
        { Key::F3, "F3" },
        { Key::F4, "F4" },
        { Key::F5, "F5" },
        { Key::F6, "F6" },
        { Key::F7, "F7" },
        { Key::F8, "F8" },
        { Key::F9, "F9" },
        { Key::F10, "F10" },
        { Key::F11, "F11" },
        { Key::F12, "F12" },
        { Key::F13, "F13" },
        { Key::F14, "F14" },
        { Key::F15, "F15" },
        { Key::CAPSLOCK, "CAPSLOCK" },
        { Key::SCROLLOCK, "SCROLLOCK" },
        { Key::RSHIFT, "RSHIFT" },
        { Key::LSHIFT, "LSHIFT" },
        { Key::RCTRL, "RCTRL" },
        { Key::LCTRL, "LCTRL" },
        { Key::RALT, "RALT" },
        { Key::LALT, "LALT" },
        { Key::MODE, "MODE" },
        { Key::HELP, "HELP" },
        { Key::PRINT, "PRINT" },
        { Key::SYSREQ, "SYSREQ" },
        { Key::MENU, "MENU" },
        { Key::POWER, "POWER" },
        { Key::UNDO, "UNDO" }
    };

    bool KeyName::iskey(int v) const
    {
        return key == v;
    }

    bool KeyName::isname(const std::string & v) const
    {
        return name && 0 == v.compare(name);
    }

    const std::initializer_list<KeyName> & Key::allKeys(void)
    {
        return allkeys;
    }

    /*
    int Key::toKey(int ch)
    {
        switch(ch)
        {
            case '`': return BACKQUOTE;
            case '!': return EXCLAIM;
            case '@': return AT;
            case '$': return DOLLAR;
            case '^': return CARET;
            case '&': return AMPERSAND;
            case '*': return ASTERISK;
            case '(': return LEFTPAREN;
            case ')': return RIGHTPAREN;
            case '_': return UNDERSCORE;
            case '-': return MINUS;
            case '+': return PLUS;
            case '=': return EQUALS;
            case '[': return LEFTBRACKET;
            case ']': return RIGHTBRACKET;
            case '\\': return BACKSLASH;
            case ':': return COLON;
            case ';': return SEMICOLON;
            case '"': return QUOTEDBL;
            case '\'': return QUOTE;
            case '<': return LESS;
            case ',': return COMMA;
            case '>': return GREATER;
            case '.': return PERIOD;
            case '?': return QUESTION;
            case '/': return SLASH;

            case '1': return K1;
            case '2': return K2;
            case '3': return K3;
            case '4': return K4;
            case '5': return K5;
            case '6': return K6;
            case '7': return K7;
            case '8': return K8;
            case '9': return K9;
            case '0': return K0;

            case 'a': return a;
            case 'b': return b;
            case 'c': return c;
            case 'd': return d;
            case 'e': return e;
            case 'f': return f;
            case 'g': return g;
            case 'h': return h;
            case 'i': return i;
            case 'j': return j;
            case 'k': return k;
            case 'l': return l;
            case 'm': return m;
            case 'n': return n;
            case 'o': return o;
            case 'p': return p;
            case 'q': return q;
            case 'r': return r;
            case 's': return s;
            case 't': return t;
            case 'u': return u;
            case 'v': return v;
            case 'w': return w;
            case 'x': return x;
            case 'y': return y;
            case 'z': return z;

            case 0x20: return SPACE;

            default: break;
        }

        return NONE;
    }
    */

    int Key::toKey(const std::string & name)
    {
        auto it = std::find_if(allkeys.begin(), allkeys.end(), [&](const KeyName & kn){ return kn.isname(name); });
        if(it != allkeys.end()) return (*it).key;

        FIXME("key not found: " << name);
        return Key::NONE;
    }

    const char* Key::toName(int key)
    {
        auto it = std::find_if(allkeys.begin(), allkeys.end(), [=](const KeyName & kn){ return kn.iskey(key); });
        if(it != allkeys.end()) return (*it).name;

        FIXME("key not found: " << key);
        return nullptr;
    }

    bool Key::isPressed(int key)
    {
	int numkeys = 0;
#ifdef SWE_SDL12
	u8* keystate = SDL_GetKeyState(& numkeys);
	return key < numkeys ? keystate[key] : false;
#else
	const u8* keystate = SDL_GetKeyboardState(& numkeys);
	SDL_Scancode scan = SDL_GetScancodeFromKey(key);
	return scan < numkeys ? keystate[scan] : false;
#endif
    }

    int KeySym::keychar(void) const
    {
        if(keymod().isCaps() || keymod().isShift())
        {
            switch(keycode())
            {
                case Key::BACKQUOTE:
                    return '~';

                case Key::BACKSLASH:
                    return '|';

                case Key::LEFTBRACKET:
                    return '{';

                case Key::RIGHTBRACKET:
                    return '}';

                case Key::K0:
		    return ')';
                case Key::K1:
		    return '!';
                case Key::K2:
		    return '@';
                case Key::K3:
		    return '#';
                case Key::K4:
		    return '$';
                case Key::K5:
		    return '%';
                case Key::K6:
		    return '^';
                case Key::K7:
		    return '&';
                case Key::K8:
		    return '*';
                case Key::K9:
		    return '(';
        	case Key::MINUS:
            	    return '_';
        	case Key::EQUALS:
            	    return '+';

                case Key::a:
                    return 'A';

                case Key::b:
                    return 'B';

                case Key::c:
                    return 'C';

                case Key::d:
                    return 'D';

                case Key::e:
                    return 'E';

                case Key::f:
                    return 'F';

                case Key::g:
                    return 'G';

                case Key::h:
                    return 'H';

                case Key::i:
                    return 'I';

                case Key::j:
                    return 'J';

                case Key::k:
                    return 'K';

                case Key::l:
                    return 'L';

                case Key::m:
                    return 'M';

                case Key::n:
                    return 'N';

                case Key::o:
                    return 'O';

                case Key::p:
                    return 'P';

                case Key::q:
                    return 'Q';

                case Key::r:
                    return 'R';

                case Key::s:
                    return 'S';

                case Key::t:
                    return 'T';

                case Key::u:
                    return 'U';

                case Key::v:
                    return 'V';

                case Key::w:
                    return 'W';

                case Key::x:
                    return 'X';

                case Key::y:
                    return 'Y';

                case Key::z:
                    return 'Z';

                default:
                    break;
            }
        }

        switch(keycode())
        {
            case Key::BACKQUOTE:
                return '`';

            case Key::EXCLAIM:
                return '!';

            case Key::AT:
                return '@';

            case Key::DOLLAR:
                return '$';

            case Key::CARET:
                return '^';

            case Key::AMPERSAND:
                return '&';

            case Key::ASTERISK:
                return '*';

            case Key::LEFTPAREN:
                return '(';

            case Key::RIGHTPAREN:
                return ')';

            case Key::UNDERSCORE:
                return '_';

            case Key::MINUS:
                return '-';

            case Key::PLUS:
                return '+';

            case Key::EQUALS:
                return '=';

            case Key::LEFTBRACKET:
                return '[';

            case Key::RIGHTBRACKET:
                return ']';

            case Key::BACKSLASH:
                return '\\';

            case Key::COLON:
                return ':';

            case Key::SEMICOLON:
                return ';';

            case Key::QUOTEDBL:
                return '"';

            case Key::QUOTE:
                return '\'';

            case Key::LESS:
                return '<';

            case Key::COMMA:
                return ',';

            case Key::GREATER:
                return '>';

            case Key::PERIOD:
                return '.';

            case Key::QUESTION:
                return '?';

            case Key::SLASH:
                return '/';

            case Key::K1:
                return '1';

            case Key::K2:
                return '2';

            case Key::K3:
                return '3';

            case Key::K4:
                return '4';

            case Key::K5:
                return '5';

            case Key::K6:
                return '6';

            case Key::K7:
                return '7';

            case Key::K8:
                return '8';

            case Key::K9:
                return '9';

            case Key::K0:
                return '0';

            case Key::a:
                return 'a';

            case Key::b:
                return 'b';

            case Key::c:
                return 'c';

            case Key::d:
                return 'd';

            case Key::e:
                return 'e';

            case Key::f:
                return 'f';

            case Key::g:
                return 'g';

            case Key::h:
                return 'h';

            case Key::i:
                return 'i';

            case Key::j:
                return 'j';

            case Key::k:
                return 'k';

            case Key::l:
                return 'l';

            case Key::m:
                return 'm';

            case Key::n:
                return 'n';

            case Key::o:
                return 'o';

            case Key::p:
                return 'p';

            case Key::q:
                return 'q';

            case Key::r:
                return 'r';

            case Key::s:
                return 's';

            case Key::t:
                return 't';

            case Key::u:
                return 'u';

            case Key::v:
                return 'v';

            case Key::w:
                return 'w';

            case Key::x:
                return 'x';

            case Key::y:
                return 'y';

            case Key::z:
                return 'z';

            case Key::SPACE:
                return 0x20;

            default:
                break;
        }

        // skip warning
        switch(keycode())
        {
            // left ctrl
            case 0x400000e0:
            // right ctrl
            case 0x400000e4:
            // left alt
            case 0x400000e2:
            // right alt
            case 0x400000e6:
            // left shift
            case 0x400000e1:
            // right shift
            case 0x400000e5:
            // capslock
            case 0x40000000:
                return 0;

            default: break;
        }

        FIXME("unknown keycode: " << String::hex(keycode()) << ", mod: " << String::hex(keymod().mod, 4));
        return 0;
    }
}

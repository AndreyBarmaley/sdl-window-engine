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

#include <algorithm>

#include "types.h"
#include "systems.h"
#include "inputs_keys.h"

SDL_Keycode Key::toSDLKeyCode(int key)
{
    switch(key)
    {
        case BACKSPACE:    return SDLK_BACKSPACE;
        case RETURN:       return SDLK_RETURN;
        case ESCAPE:       return SDLK_ESCAPE;
        case SPACE:        return SDLK_SPACE;
        case EXCLAIM:      return SDLK_EXCLAIM;
        case BACKQUOTE:    return SDLK_BACKQUOTE;
        case QUOTEDBL:     return SDLK_QUOTEDBL;
        case HASH:         return SDLK_HASH;
        case DOLLAR:       return SDLK_DOLLAR;
        case AMPERSAND:    return SDLK_AMPERSAND;
        case QUOTE:        return SDLK_QUOTE;
        case LEFTPAREN:    return SDLK_LEFTPAREN;
        case RIGHTPAREN:   return SDLK_RIGHTPAREN;
        case ASTERISK:     return SDLK_ASTERISK;
        case PLUS:         return SDLK_PLUS;
        case COMMA:        return SDLK_COMMA;
        case MINUS:        return SDLK_MINUS;
        case PERIOD:       return SDLK_PERIOD;
        case SLASH:        return SDLK_SLASH;
        case COLON:        return SDLK_COLON;
        case SEMICOLON:    return SDLK_SEMICOLON;
        case LESS:         return SDLK_LESS;
        case EQUALS:       return SDLK_EQUALS;
        case GREATER:      return SDLK_GREATER;
        case QUESTION:     return SDLK_QUESTION;
        case AT:           return SDLK_AT;
#ifndef OLDENGINE
        case PERCENT:      return SDLK_PERCENT;
#endif
        case LEFTBRACKET:  return SDLK_LEFTBRACKET;
        case BACKSLASH:    return SDLK_BACKSLASH;
        case RIGHTBRACKET: return SDLK_RIGHTBRACKET;
        case CARET:        return SDLK_CARET;
        case UNDERSCORE:   return SDLK_UNDERSCORE;
        case TAB:          return SDLK_TAB;
        case INSERT:       return SDLK_INSERT;
        case HOME:         return SDLK_HOME;
        case END:          return SDLK_END;
        case DELETE:       return SDLK_DELETE;
        case PAGEUP:       return SDLK_PAGEUP;
        case PAGEDOWN:     return SDLK_PAGEDOWN;
        case F1:           return SDLK_F1;
        case F2:           return SDLK_F2;
        case F3:           return SDLK_F3;
        case F4:           return SDLK_F4;
        case F5:           return SDLK_F5;
        case F6:           return SDLK_F6;
        case F7:           return SDLK_F7;
        case F8:           return SDLK_F8;
        case F9:           return SDLK_F9;
        case F10:          return SDLK_F10;
        case F11:          return SDLK_F11;
        case F12:          return SDLK_F12;
        case LEFT:         return SDLK_LEFT;
        case RIGHT:        return SDLK_RIGHT;
        case UP:           return SDLK_UP;
        case DOWN:         return SDLK_DOWN;

        case K0:           return SDLK_0;
        case K1:           return SDLK_1;
        case K2:           return SDLK_2;
        case K3:           return SDLK_3;
        case K4:           return SDLK_4;
        case K5:           return SDLK_5;
        case K6:           return SDLK_6;
        case K7:           return SDLK_7;
        case K8:           return SDLK_8;
        case K9:           return SDLK_9;

        case a:            return SDLK_a;
        case b:            return SDLK_b;
        case c:            return SDLK_c;
        case d:            return SDLK_d;
        case e:            return SDLK_e;
        case f:            return SDLK_f;
        case g:            return SDLK_g;
        case h:            return SDLK_h;
        case i:            return SDLK_i;
        case j:            return SDLK_j;
        case k:            return SDLK_k;
        case l:            return SDLK_l;
        case m:            return SDLK_m;
        case n:            return SDLK_n;
        case o:            return SDLK_o;
        case p:            return SDLK_p;
        case q:            return SDLK_q;
        case r:            return SDLK_r;
        case s:            return SDLK_s;
        case t:            return SDLK_t;
        case u:            return SDLK_u;
        case v:            return SDLK_v;
        case w:            return SDLK_w;
        case x:            return SDLK_x;
        case y:            return SDLK_y;
        case z:            return SDLK_z;

#ifdef OLDENGINE
        case PRINT:        return SDLK_PRINT;
#else
        case PRINT:        return SDLK_PRINTSCREEN;
#endif

/*
        case KP0:          return SDLK_KP_0;
        case KP1:          return SDLK_KP_1;
        case KP2:          return SDLK_KP_2;
        case KP3:          return SDLK_KP_3;
        case KP4:          return SDLK_KP_4;
        case KP5:          return SDLK_KP_5;
        case KP6:          return SDLK_KP_6;
        case KP7:          return SDLK_KP_7;
        case KP8:          return SDLK_KP_8;
        case KP9:          return SDLK_KP_9;
        case KP_PERIOD:    return SDLK_KP_PERIOD;
        case KP_DIVIDE:    return SDLK_KP_DIVIDE;
        case KP_MULTIPLY:  return SDLK_KP_MULTIPLY;
        case KP_MINUS:     return SDLK_KP_MINUS;
        case KP_PLUS:      return SDLK_KP_PLUS;
        case KP_ENTER:     return SDLK_KP_ENTER;
        case KP_EQUALS:    return SDLK_KP_EQUALS;
*/
#ifndef OLDENGINE
        case AC_SEARCH:    return SDLK_AC_SEARCH;
        case AC_HOME:      return SDLK_AC_HOME;
        case AC_BACK:      return SDLK_AC_BACK;
        case AC_FORWARD:   return SDLK_AC_FORWARD;
        case AC_STOP:      return SDLK_AC_STOP;
        case AC_REFRESH:   return SDLK_AC_REFRESH;
        case AC_BOOKMARKS: return SDLK_AC_BOOKMARKS;
#endif
    }

    return SDLK_UNKNOWN;
}

int Key::toKey(const SDL_Keycode & sym, int mod)
{
    int res = SDLK_UNKNOWN;

    switch(sym)
    {
        case SDLK_BACKSPACE:	res = BACKSPACE; break;
        case SDLK_RETURN:	res = RETURN; break;
        case SDLK_ESCAPE:	res = ESCAPE; break;
        case SDLK_SPACE:	res = SPACE; break;
        case SDLK_EXCLAIM:	res = EXCLAIM; break;
        case SDLK_BACKQUOTE:	res = BACKQUOTE; break;
        case SDLK_QUOTEDBL:	res = QUOTEDBL; break;
        case SDLK_HASH:		res = HASH; break;
        case SDLK_DOLLAR:	res = DOLLAR; break;
        case SDLK_AMPERSAND:	res = AMPERSAND; break;
        case SDLK_QUOTE:	res = QUOTE; break;
        case SDLK_LEFTPAREN:	res = LEFTPAREN; break;
        case SDLK_RIGHTPAREN:	res = RIGHTPAREN; break;
        case SDLK_ASTERISK:	res = ASTERISK; break;
        case SDLK_PLUS:		res = PLUS; break;
        case SDLK_COMMA:	res = COMMA; break;
        case SDLK_MINUS:	res = MINUS; break;
        case SDLK_PERIOD:	res = PERIOD; break;
        case SDLK_SLASH:	res = SLASH; break;
        case SDLK_COLON:	res = COLON; break;
        case SDLK_SEMICOLON:	res = SEMICOLON; break;
        case SDLK_LESS:		res = LESS; break;
        case SDLK_EQUALS:	res = EQUALS; break;
        case SDLK_GREATER:	res = GREATER; break;
        case SDLK_QUESTION:	res = QUESTION; break;
        case SDLK_AT:		res = AT; break;
#ifndef OLDENGINE
        case SDLK_PERCENT:	res = PERCENT; break;
#endif
        case SDLK_LEFTBRACKET:	res = LEFTBRACKET; break;
        case SDLK_BACKSLASH:	res = BACKSLASH; break;
        case SDLK_RIGHTBRACKET:	res = RIGHTBRACKET; break;
        case SDLK_CARET:	res = CARET; break;
        case SDLK_UNDERSCORE:	res = UNDERSCORE; break;
        case SDLK_TAB:		res = TAB; break;
        case SDLK_INSERT:	res = INSERT; break;
        case SDLK_HOME:		res = HOME; break;
        case SDLK_END:		res = END; break;
        case SDLK_DELETE:	res = DELETE; break;
        case SDLK_PAGEUP:	res = PAGEUP; break;
        case SDLK_PAGEDOWN:	res = PAGEDOWN; break;
        case SDLK_F1:		res = F1; break;
        case SDLK_F2:		res = F2; break;
        case SDLK_F3:		res = F3; break;
        case SDLK_F4:		res = F4; break;
        case SDLK_F5:		res = F5; break;
        case SDLK_F6:		res = F6; break;
        case SDLK_F7:		res = F7; break;
        case SDLK_F8:		res = F8; break;
        case SDLK_F9:		res = F9; break;
        case SDLK_F10:		res = F10; break;
        case SDLK_F11:		res = F11; break;
        case SDLK_F12:		res = F12; break;
        case SDLK_LEFT:		res = LEFT; break;
        case SDLK_RIGHT:	res = RIGHT; break;
        case SDLK_UP:		res = UP; break;
        case SDLK_DOWN:		res = DOWN; break;

        case SDLK_0:		res = K0; break;
        case SDLK_1:		res = K1; break;
        case SDLK_2:		res = K2; break;
        case SDLK_3:		res = K3; break;
        case SDLK_4:		res = K4; break;
        case SDLK_5:		res = K5; break;
        case SDLK_6:		res = K6; break;
        case SDLK_7:		res = K7; break;
        case SDLK_8:		res = K8; break;
        case SDLK_9:		res = K9; break;

        case SDLK_a:		res = a; break;
        case SDLK_b:		res = b; break;
        case SDLK_c:		res = c; break;
        case SDLK_d:		res = d; break;
        case SDLK_e:		res = e; break;
        case SDLK_f:		res = f; break;
        case SDLK_g:		res = g; break;
        case SDLK_h:		res = h; break;
        case SDLK_i:		res = i; break;
        case SDLK_j:		res = j; break;
        case SDLK_k:		res = k; break;
        case SDLK_l:		res = l; break;
        case SDLK_m:		res = m; break;
        case SDLK_n:		res = n; break;
        case SDLK_o:		res = o; break;
        case SDLK_p:		res = p; break;
        case SDLK_q:		res = q; break;
        case SDLK_r:		res = r; break;
        case SDLK_s:		res = s; break;
        case SDLK_t:		res = t; break;
        case SDLK_u:		res = u; break;
        case SDLK_v:		res = v; break;
        case SDLK_w:		res = w; break;
        case SDLK_x:		res = x; break;
        case SDLK_y:		res = y; break;
        case SDLK_z:		res = z; break;

#ifdef OLDENGINE
        case SDLK_PRINT:	res = PRINT; break;
#else
        case SDLK_PRINTSCREEN:	res = PRINT; break;
#endif

/*
        case SDLK_KP_0:		res = KP0; break;
        case SDLK_KP_1:		res = KP1; break;
        case SDLK_KP_2:		res = KP2; break;
        case SDLK_KP_3:		res = KP3; break;
        case SDLK_KP_4:		res = KP4; break;
        case SDLK_KP_5:		res = KP5; break;
        case SDLK_KP_6:		res = KP6; break;
        case SDLK_KP_7:		res = KP7; break;
        case SDLK_KP_8:		res = KP8; break;
        case SDLK_KP_9:		res = KP9; break;
        case SDLK_KP_PERIOD:	res = KP_PERIOD; break;
        case SDLK_KP_DIVIDE:	res = KP_DIVIDE; break;
        case SDLK_KP_MULTIPLY:	res = KP_MULTIPLY; break;
        case SDLK_KP_MINUS:	res = KP_MINUS; break;
        case SDLK_KP_PLUS:	res = KP_PLUS; break;
        case SDLK_KP_ENTER:	res = KP_ENTER; break;
        case SDLK_KP_EQUALS:	res = KP_EQUALS; break;
*/
#ifndef OLDENGINE
        case SDLK_AC_SEARCH:    res = AC_SEARCH; break;
        case SDLK_AC_HOME:      res = AC_HOME; break;
        case SDLK_AC_BACK:      res = AC_BACK; break;
        case SDLK_AC_FORWARD:   res = AC_FORWARD; break;
        case SDLK_AC_STOP:      res = AC_STOP; break;
        case SDLK_AC_REFRESH:   res = AC_REFRESH; break;
        case SDLK_AC_BOOKMARKS: res = AC_BOOKMARKS; break;
#endif
	default: break;
    }


    if(mod)
    {
	if(KMOD_CTRL & mod) res |= CTRL;
	if(KMOD_ALT & mod) res |= ALT;
	if(KMOD_SHIFT & mod) res |= SHIFT;
    }

    return res;
}

int Key::toChar(int key)
{
    switch(key)
    {
	case BACKQUOTE:		return '`';
        case TILDE:		return '~';
	case EXCLAIM:		return '!';
	case AT:		return '@';
	case DOLLAR:		return '$';
	case PERCENT:		return '%';
	case CARET:		return '^';
	case AMPERSAND:		return '&';
	case ASTERISK:		return '*';
	case LEFTPAREN:		return '(';
	case RIGHTPAREN:	return ')';
	case UNDERSCORE:	return '_';
	case MINUS:		return '-';
	case PLUS:		return '+';
	case EQUALS:		return '=';
	case LEFTCURLY:		return '{';
	case LEFTBRACKET:	return '[';
	case RIGHTCURLY:	return '}';
	case RIGHTBRACKET:	return ']';
	case VERTLINE:		return '|';
	case BACKSLASH:		return '\\';
	case COLON:		return ':';
	case SEMICOLON:		return ';';
	case QUOTEDBL:		return '"';
	case QUOTE:		return '\'';
	case LESS:		return '<';
	case COMMA:		return ',';
	case GREATER:		return '>';
	case PERIOD:		return '.';
	case QUESTION:		return '?';
	case SLASH:		return '/';

        case K1:		return '1';
        case K2:		return '2';
        case K3:		return '3';
        case K4:		return '4';
        case K5:		return '5';
        case K6:		return '6';
        case K7:		return '7';
        case K8:		return '8';
        case K9:		return '9';
        case K0:		return '0';

        case a:            return 'a';
        case b:            return 'b';
        case c:            return 'c';
        case d:            return 'd';
        case e:            return 'e';
        case f:            return 'f';
        case g:            return 'g';
        case h:            return 'h';
        case i:            return 'i';
        case j:            return 'j';
        case k:            return 'k';
        case l:            return 'l';
        case m:            return 'm';
        case n:            return 'n';
        case o:            return 'o';
        case p:            return 'p';
        case q:            return 'q';
        case r:            return 'r';
        case s:            return 's';
        case t:            return 't';
        case u:            return 'u';
        case v:            return 'v';
        case w:            return 'w';
        case x:            return 'x';
        case y:            return 'y';
        case z:            return 'z';

        case A:            return 'A';
        case B:            return 'B';
        case C:            return 'C';
        case D:            return 'D';
        case E:            return 'E';
        case F:            return 'F';
        case G:            return 'G';
        case H:            return 'H';
        case I:            return 'I';
        case J:            return 'J';
        case K:            return 'K';
        case L:            return 'L';
        case M:            return 'M';
        case N:            return 'N';
        case O:            return 'O';
        case P:            return 'P';
        case Q:            return 'Q';
        case R:            return 'R';
        case S:            return 'S';
        case T:            return 'T';
        case U:            return 'U';
        case V:            return 'V';
        case W:            return 'W';
        case X:            return 'X';
        case Y:            return 'Y';
        case Z:            return 'Z';

	case SPACE:	   return 0x20;

	default: break;
    }

    return 0;
}

int Key::toKey(const std::string & name)
{
#ifdef OLDENGINE
    if(1 == name.size())
	return toKey(name[0]);

    const char* _names1[] = { "NONE", "ESCAPE", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "PRINT", "INSERT", "HOME", "END",
	"DELETE", "PAGEUP", "PAGEDOWN", "LEFT", "RIGHT", "UP", "DOWN", "RETURN",
	"BACKQUOTE", "K1", "K2", "K3", "K4", "K5", "K6", "K7", "K8", "K9", "K0", "MINUS", "EQUALS", "BACKSPACE", "TAB", "LEFTBRACKET", "RIGHTBRACKET",
	"BACKSLASH", "SEMICOLON", "QUOTE", "COMMA", "PERIOD", "SLASH", "SPACE" };

    const char* _names2[] = { "TILDE", "EXCLAIM", "AT", "HASH", "DOLLAR", "PERCENT", "CARET", "AMPERSAND", "ASTERISK", "LEFTPAREN", "RIGHTPAREN",
	"UNDERSCORE", "PLUS", "LEFTCURLY", "RIGHTCURLY", "VERTLINE", "COLON", "QUOTEDBL", "LESS", "GREATER", "QUESTION" };

    const char** _names1_end = ARRAY_COUNT_END(_names1);
    const char** _names1_its = std::find(_names1, _names1_end, name);

    if(_names1_its != _names1_end)
	return NONE + (_names1_its - _names1);

    const char** _names2_end = ARRAY_COUNT_END(_names2);
    const char** _names2_its = std::find(_names2, _names2_end, name);

    if(_names2_its != _names2_end)
	return TILDE + (_names2_its - _names2);

    ERROR("key not found: " << name);
    return 0;
#else
    return toKey(SDL_GetKeyFromName(name.c_str()), 0);
#endif
}

int Key::toKey(int ch)
{
    switch(ch)
    {
        case '`': return BACKQUOTE;
        case '~': return TILDE;
        case '!': return EXCLAIM;
        case '@': return AT;
        case '$': return DOLLAR;
        case '%': return PERCENT;
        case '^': return CARET;
        case '&': return AMPERSAND;
        case '*': return ASTERISK;
        case '(': return LEFTPAREN;
        case ')': return RIGHTPAREN;
        case '_': return UNDERSCORE;
        case '-': return MINUS;
        case '+': return PLUS;
        case '=': return EQUALS;
        case '{': return LEFTCURLY;
        case '[': return LEFTBRACKET;
        case '}': return RIGHTCURLY;
        case ']': return RIGHTBRACKET;
        case '|': return VERTLINE;
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

        case 'A': return A;
        case 'B': return B;
        case 'C': return C;
        case 'D': return D;
        case 'E': return E;
        case 'F': return F;
        case 'G': return G;
        case 'H': return H;
        case 'I': return I;
        case 'J': return J;
        case 'K': return K;
        case 'L': return L;
        case 'M': return M;
        case 'N': return N;
        case 'O': return O;
        case 'P': return P;
        case 'Q': return Q;
        case 'R': return R;
        case 'S': return S;
        case 'T': return T;
        case 'U': return U;
        case 'V': return V;
        case 'W': return W;
        case 'X': return X;
        case 'Y': return Y;
        case 'Z': return Z;

        case 0x20: return SPACE;

        default: break;
    }

    return NONE;
}

int Key::lower(int key)
{
    switch(key)
    {
        case A:            return a;
        case B:            return b;
        case C:            return c;
        case D:            return d;
        case E:            return e;
        case F:            return f;
        case G:            return g;
        case H:            return h;
        case I:            return i;
        case J:            return j;
        case K:            return k;
        case L:            return l;
        case M:            return m;
        case N:            return n;
        case O:            return o;
        case P:            return p;
        case Q:            return q;
        case R:            return r;
        case S:            return s;
        case T:            return t;
        case U:            return u;
        case V:            return v;
        case W:            return w;
        case X:            return x;
        case Y:            return y;
        case Z:            return z;
	default: break;
    }

    return key;
}

int Key::upper(int key)
{
    switch(key)
    {
        case a:            return A;
        case b:            return B;
        case c:            return C;
        case d:            return D;
        case e:            return E;
        case f:            return F;
        case g:            return G;
        case h:            return H;
        case i:            return I;
        case j:            return J;
        case k:            return K;
        case l:            return L;
        case m:            return M;
        case n:            return N;
        case o:            return O;
        case p:            return P;
        case q:            return Q;
        case r:            return R;
        case s:            return S;
        case t:            return T;
        case u:            return U;
        case v:            return V;
        case w:            return W;
        case x:            return X;
        case y:            return Y;
        case z:            return Z;
	default: break;
    }

    return key;
}


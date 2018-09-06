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

#ifndef _SWE_TERMWIN_GUI_
#define _SWE_TERMWIN_GUI_

#include "termwin.h"

namespace TermGUI
{

enum { ColorBackground, ColorBorderLine, ColorHeaderText, ColorBodyText,
	ColorButtonBackground, ColorButtonBracket, ColorButtonFirstText, ColorButtonBodyText,
	ColorSelectedButtonBackground, ColorSelectedButtonBracket, ColorSelectedButtonFirstText, ColorSelectedButtonBodyText,
	ColorInputFieldBackground, ColorInputFieldText, ColorInputCursor,
	ColorListSelectedBackground, ColorListSelectedText,
	/* CHANGE SIZE ARRAY vals[]!!! */
	ColorUnknown };

class DefaultTheme
{
protected:
    ColorIndex vals[17];
    line_t line;

public:
    DefaultTheme(const line_t & = LineAscii);

    const ColorIndex & get(int) const;

    void setLine(const line_t & lt) { line = lt; }
    void set(int, const ColorIndex &);
    void reset(void);

    line_t typeLine(void) const { return line; }

    const ColorIndex & background(void) const 			{ return get(ColorBackground); }
    const ColorIndex & borderLine(void) const 			{ return get(ColorBorderLine); }
    const ColorIndex & headerText(void) const 			{ return get(ColorHeaderText); }
    const ColorIndex & bodyText(void) const 			{ return get(ColorBodyText); }
    const ColorIndex & buttonBackground(void) const 		{ return get(ColorButtonBackground); }
    const ColorIndex & buttonBracket(void) const 		{ return get(ColorButtonBracket); }
    const ColorIndex & buttonFirstText(void) const 		{ return get(ColorButtonFirstText); }
    const ColorIndex & buttonBodyText(void) const 		{ return get(ColorButtonBodyText); }
    const ColorIndex & selectedButtonBackground(void) const 	{ return get(ColorSelectedButtonBackground); }
    const ColorIndex & selectedButtonBracket(void) const 	{ return get(ColorSelectedButtonBracket); }
    const ColorIndex & selectedButtonFirstText(void) const 	{ return get(ColorSelectedButtonFirstText); }
    const ColorIndex & selectedButtonBodyText(void) const 	{ return get(ColorSelectedButtonBodyText); }
    const ColorIndex & inputFieldBackground(void) const 	{ return get(ColorInputFieldBackground); }
    const ColorIndex & inputFieldText(void) const 		{ return get(ColorInputFieldText); }
    const ColorIndex & inputCursor(void) const 			{ return get(ColorInputCursor); }
    const ColorIndex & listSelectedBackground(void) const 	{ return get(ColorListSelectedBackground); }
    const ColorIndex & listSelectedText(void) const 		{ return get(ColorListSelectedText); }

    static DefaultTheme & defaults(const line_t & = LineAscii);
};

enum buttons_t { ButtonUnknown, ButtonOk = 1 << 1, ButtonYes = 1 << 2, ButtonNo = 1 << 3, ButtonCancel = 1 << 4 };

class MessageBoxButton : protected packint2
{
    enum { ButtonSelected = 0x00008000, DisableHotkeys = 0x00004000 };

public:
    MessageBoxButton() : packint2(ButtonUnknown, 0) {}
    MessageBoxButton(int val, bool sel = false) : packint2(val, sel ? ButtonSelected : 0) {}

    int                 button(void) const { return val1(); }
    virtual size_t      width(void) const;
    virtual UCString    label(const DefaultTheme &) const;
    virtual int         hotkey(void) const;

    void                setSelected(bool f) { if(f) set2(val2() | ButtonSelected); else set2(val2() & ~ButtonSelected); }
    bool                isSelected(void) const { return val2() & ButtonSelected; }

    bool                isValid(void) const { return 0 != val1(); }

    void		disableHotkey(void) { set2(val2() | DisableHotkeys); }
    bool		hotkeyDisabled(void) const { return val2() & DisableHotkeys; }
};

class MessageBoxButtons
{
    MessageBoxButton    btn1;
    MessageBoxButton    btn2;
    Rect		pos1;
    Rect		pos2;

public:
    MessageBoxButtons(int buttons);

    size_t              width(void) const;
    UCString            labels(const DefaultTheme &)  const;
    int                 selected(void) const;

    bool                keyPressEvent(int key);

    const MessageBoxButton & button1(void) const { return btn1; }
    const MessageBoxButton & button2(void) const { return btn2; }

    void		disableHotkeys(void);
};

class EmptyAreaBox : public TermWindow
{
protected:
    MessageBoxButtons   buttons;

    bool                keyPressEvent(int);

public:
    EmptyAreaBox(int buttons, TermWindow &, const DefaultTheme* = NULL);
    EmptyAreaBox(int buttons, const FontRender &, Window &, const DefaultTheme* = NULL);
    EmptyAreaBox(size_t cols, size_t rows, int buttons, TermWindow &, const DefaultTheme* = NULL);
    EmptyAreaBox(size_t cols, size_t rows, int buttons, const FontRender &, Window &, const DefaultTheme* = NULL);

    virtual ~EmptyAreaBox() {}

    void                renderBody(const UnicodeString & header);
    void                renderButtons(void);

    void		setAreaSize(size_t cols, size_t rows);

    const DefaultTheme & theme(void) const;

private:
    const DefaultTheme* themedef;
};

class MessageBox : public EmptyAreaBox
{
protected:
    bool                keyPressEvent(int);
    bool                mouseClickEvent(const ButtonsEvent &);

    void		renderBody(const UnicodeString &, const UCString &);

public:
    MessageBox(const UnicodeString &, const UnicodeString &, int buttons, TermWindow &, const DefaultTheme* = NULL);
    MessageBox(const UnicodeString &, const UnicodeString &, int buttons, const FontRender &, Window &, const DefaultTheme* = NULL);

    MessageBox(const UnicodeString &, const UCString &, int buttons, TermWindow &, const DefaultTheme* = NULL);
    MessageBox(const UnicodeString &, const UCString &, int buttons, const FontRender &, Window &, const DefaultTheme* = NULL);

    void                renderWindow(void);
};

class InputBox : public EmptyAreaBox
{
    std::string		strLine;
    u32			tickLast;

protected:
    bool                keyPressEvent(int);
    void		tickEvent(u32 ms);

public:
    InputBox(const UnicodeString &, size_t cols, const std::string & def, TermWindow &, const DefaultTheme* = NULL);
    InputBox(const UnicodeString &, size_t cols, const std::string & def, const FontRender &, Window &, const DefaultTheme* = NULL);

    void                renderWindow(void);
    const std::string &	result(void) const { return strLine; }
};

class ListBox : public EmptyAreaBox
{
    UnicodeString	header;
    UCStringList	content;
    std::string		strLine;
    int			selected;

protected:
    bool                keyPressEvent(int);

public:
    ListBox(const UnicodeString &, const UCStringList &, size_t rows, TermWindow &, const DefaultTheme* = NULL);
    ListBox(const UnicodeString &, const UCStringList &, size_t rows, const FontRender &, Window &, const DefaultTheme* = NULL);

    void                renderWindow(void);
    const std::string &	result(void) const { return strLine; }
};

} // namespace TermGUI

#endif

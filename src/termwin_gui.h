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

#ifndef DISABLE_TERMGUI

#ifndef _SWE_TERMWIN_GUI_
#define _SWE_TERMWIN_GUI_

#include <memory>
#include "termwin.h"

namespace TermGUI
{

enum { ColorBackground, ColorBorderLine, ColorHeaderText, ColorBodyText,
	ColorButtonBackground, ColorButtonBracket, ColorButtonFirstText, ColorButtonBodyText,
	ColorFocusedButtonBackground, ColorFocusedButtonBracket, ColorFocusedButtonFirstText, ColorFocusedButtonBodyText,
	ColorInputFieldBackground, ColorFocusedInputFieldBackground, ColorInputFieldText, ColorInputCursor,
	ColorListSelectedBackground, ColorListSelectedText,
	ColorUnknown };

class ThemeColors
{
protected:
    ColorIndex vals[ColorUnknown + 1];
    line_t line;

public:
    ThemeColors(const line_t & = LineAscii);

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
    const ColorIndex & focusedButtonBackground(void) const 	{ return get(ColorFocusedButtonBackground); }
    const ColorIndex & focusedButtonBracket(void) const 	{ return get(ColorFocusedButtonBracket); }
    const ColorIndex & focusedButtonFirstText(void) const 	{ return get(ColorFocusedButtonFirstText); }
    const ColorIndex & focusedButtonBodyText(void) const 	{ return get(ColorFocusedButtonBodyText); }
    const ColorIndex & inputFieldBackground(void) const 	{ return get(ColorInputFieldBackground); }
    const ColorIndex & focusedInputFieldBackground(void) const 	{ return get(ColorFocusedInputFieldBackground); }
    const ColorIndex & inputFieldText(void) const 		{ return get(ColorInputFieldText); }
    const ColorIndex & inputCursor(void) const 			{ return get(ColorInputCursor); }
    const ColorIndex & listSelectedBackground(void) const 	{ return get(ColorListSelectedBackground); }
    const ColorIndex & listSelectedText(void) const 		{ return get(ColorListSelectedText); }

    static ThemeColors & defaults(const line_t & = LineAscii);
};

enum buttons_t { ButtonUnknown, ButtonOk = 1 << 1, ButtonYes = 1 << 2, ButtonNo = 1 << 3, ButtonCancel = 1 << 4 };
std::vector<buttons_t> buttonsAll(int);

class CurrentTheme
{
    const ThemeColors* themedef;

public:
    CurrentTheme(const ThemeColors* = NULL);

    void	setTheme(const ThemeColors &);
    const ThemeColors & theme(void) const;
};

class LabelAction : public TermWindow, public CurrentTheme
{
    int         hotkey;
    std::string content;

protected:
    bool        keyPressEvent(const KeySym &) override;
    void        mouseFocusEvent(void) override;
    void        mouseLeaveEvent(void) override;
    bool        mouseClickEvent(const ButtonsEvent &) override;

    bool	setLabelHotKey(const std::string &);
    void        renderLabel(const ThemeColors &);
    void	clickAction(void);

public:
    LabelAction(const std::string &, int action, const TermPos &, TermWindow &, const ThemeColors* = NULL);
    LabelAction(TermWindow &, const ThemeColors* = NULL);

    virtual void setLabel(const std::string &);
    const std::string & label(void) const;
    bool	isLabel(const std::string &) const;

    void	setAction(int);
    int		action(void) const;
    bool	isAction(int) const;

    void	setHotKey(int);
    void	setHotKey(const std::string &);
    int		hotKey(void) const;
    bool	isHotKey(int) const;

    void	setHotKeyDisabled(bool);
    bool	isHotKeyDisabled(void) const;

    void	setDisabled(bool);
    bool	isDisabled(void) const;
    
    void        setSelected(bool);
    bool        isSelected(void) const;

    void        renderWindow(void) override;
};

class TextButton : public LabelAction
{
public:
    TextButton(const std::string &, int action, const TermPos &, TermWindow &, const ThemeColors* = NULL);
    TextButton(const buttons_t &, TermWindow &, const ThemeColors* = NULL);
    TextButton(TermWindow & term, const ThemeColors* theme = NULL) : LabelAction(term, theme) {}

    void	setLabel(const std::string &) override;
    void	setLabel(const buttons_t &);
    void        renderWindow(void) override;

};

struct LabelActionPtr : std::shared_ptr<LabelAction>
{
    LabelActionPtr(LabelAction* ptr) : std::shared_ptr<LabelAction>(ptr) {}

    LabelActionPtr(TermWindow & term, const ThemeColors* theme = NULL)
	: std::shared_ptr<LabelAction>(std::make_shared<LabelAction>(term, theme)) {}

    LabelActionPtr(const std::string & str, int action, const TermPos & pos, TermWindow & term, const ThemeColors* theme = NULL)
	: std::shared_ptr<LabelAction>(std::make_shared<LabelAction>(str, action, pos, term, theme)) {}
};

class LabelActionGroup : protected std::list<LabelActionPtr>
{
public:
    LabelActionGroup() {}

    LabelAction*	addLabel(const std::string &, int action, const TermPos &, TermWindow &, const ThemeColors* = NULL);
    LabelAction*	addLabel(TermWindow &, const ThemeColors* = NULL);
    LabelAction*	addLabel(LabelAction*);

    bool		findLabel(const LabelAction*) const;
    LabelAction*	findLabel(const std::string &) const;
    LabelAction*	findAction(int) const;
    LabelAction*	findHotKey(int) const;
    LabelAction*	findSelected(void) const;
    LabelAction*	findIndex(size_t) const;

    bool		setSelected(const LabelAction*);
    void		resetSelected(const LabelAction* exclude = NULL);
    void		nextSelected(void);
    void		prevSelected(void);
    void		firstSelected(void);
    void		lastSelected(void);
    bool		isFirstSelected(void) const;
    bool		isLastSelected(void) const;

    int			index(LabelAction*) const;
    size_t		count(void) const { return size(); }
    size_t		rows(void) const;
    size_t		cols(void) const;

    void		renderWindow(void);
};

class ButtonsGroup : public LabelActionGroup
{
public:
    ButtonsGroup(int buttons, TermWindow &, const ThemeColors* = NULL);
};

class HeaderAreaBox : public TermWindow, public CurrentTheme
{
    UCString		header;

protected:
    bool                keyPressEvent(const KeySym &) override;
    virtual void	renderWindowLine(const line_t &);

    void		initHeaderAreaBox(const TermSize &);

public:
    HeaderAreaBox(const UnicodeString &, const TermSize &, TermWindow &, const ThemeColors* = NULL);
    HeaderAreaBox(const UCString &, const TermSize &, TermWindow &, const ThemeColors* = NULL);
    //
    HeaderAreaBox(const UnicodeString &, TermWindow &, const ThemeColors* = NULL);
    HeaderAreaBox(const UCString &, TermWindow &, const ThemeColors* = NULL);
    HeaderAreaBox(const UnicodeString &, const FontRender &, Window &, const ThemeColors* = NULL);
    HeaderAreaBox(const UCString &, const FontRender &, Window &, const ThemeColors* = NULL);
    virtual ~HeaderAreaBox() {}

    void		renderWindow(void) override;
};

class ButtonsAreaBox : public HeaderAreaBox
{
protected:
    ButtonsGroup	buttonsGroup;

protected:
    void		signalReceive(int, const SignalMember*) override;
    void		renderWindowLine(const line_t &) override;
    bool                keyPressEvent(const KeySym &) override;

    void		setButtonsPosition(void);
    void		setHotKeyDisabled(bool);
    void		setButtonsSubscribe(int);
    int			maxColumns(void) const;
    void		initButtonsAreaBox(const TermSize &);

public:
    ButtonsAreaBox(const UnicodeString &, const TermSize &, int buttons, TermWindow &, const ThemeColors* = NULL);
    ButtonsAreaBox(const UCString &, const TermSize &, int buttons, TermWindow &, const ThemeColors* = NULL);
    //
    ButtonsAreaBox(const UnicodeString &, int buttons, TermWindow &, const ThemeColors* = NULL);
    ButtonsAreaBox(const UCString &, int buttons, TermWindow &, const ThemeColors* = NULL);
    ButtonsAreaBox(const UnicodeString &, int buttons, const FontRender &, Window &, const ThemeColors* = NULL);
    ButtonsAreaBox(const UCString &, int buttons, const FontRender &, Window &, const ThemeColors* = NULL);
};

class MessageBox : public ButtonsAreaBox
{
    UCStringList	content;

protected:
    void		renderWindowLine(const line_t &) override;
    void		initMessageBox(const UCString &);

public:
    MessageBox(const UnicodeString &, const UnicodeString &, int buttons, TermWindow &, const ThemeColors* = NULL);
    MessageBox(const UCString &, const UCString &, int buttons, TermWindow &, const ThemeColors* = NULL);

    MessageBox(const UnicodeString &, const UnicodeString &, int buttons, const FontRender &, Window &, const ThemeColors* = NULL);
    MessageBox(const UCString &, const UCString &, int buttons, const FontRender &, Window &, const ThemeColors* = NULL);
};

class InputBox : public ButtonsAreaBox
{
    std::string		sResult;
    u32			tickLast;

protected:
    bool                keyPressEvent(const KeySym &) override;
    void		tickEvent(u32 ms) override;
    void		renderWindowLine(const line_t &) override;

    void		setInputFocused(bool);
    bool		checkInputFocused(void) const;
    void		setFlagTermCursor(bool);
    bool		checkFlagTermCursor(void) const;

public:
    InputBox(const UnicodeString &, size_t cols, const std::string & def, TermWindow &, const ThemeColors* = NULL);
    InputBox(const UnicodeString &, size_t cols, const std::string & def, const FontRender &, Window &, const ThemeColors* = NULL);

    const std::string &	result(void) const { return sResult; }
};

class ListBox : public ButtonsAreaBox
{
    UCStringList	content;
    std::string		sResult;
    int			selected;
    int			skipped;

protected:
    bool                keyPressEvent(const KeySym &) override;
    bool		mouseClickEvent(const ButtonsEvent &) override;
    bool		scrollUpEvent(const Point &) override;
    bool		scrollDownEvent(const Point &) override;
    void		windowVisibleEvent(bool) override;
    void		renderWindowLine(const line_t &) override;

    bool		scrollUpContent(void);
    bool		scrollDownContent(void);

public:
    ListBox(const UnicodeString &, const UCStringList &, size_t rows, TermWindow &, const ThemeColors* = NULL);
    ListBox(const UnicodeString &, const UCStringList &, size_t rows, const FontRender &, Window &, const ThemeColors* = NULL);

    const std::string &	result(void) const { return sResult; }
};

} // namespace TermGUI

#endif

#endif

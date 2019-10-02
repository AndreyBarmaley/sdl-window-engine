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

#include <algorithm> 

#include "engine.h"
#include "termwin_gui.h"

namespace TermGUI
{

std::vector<buttons_t> buttonsAll(int buttons)
{
    std::vector<buttons_t> v;
    v.reserve(6);

    if(buttons & ButtonOk)
	v.push_back(ButtonOk);
    if(buttons & ButtonYes)
	v.push_back(ButtonYes);
    if(buttons & ButtonNo)
	v.push_back(ButtonNo);
    if(buttons & ButtonCancel)
	v.push_back(ButtonCancel);

    return v;
}

/* ThemeColors */
ThemeColors::ThemeColors(const line_t & lt) : line(lt)
{
    reset();
}

ThemeColors & ThemeColors::defaults(const line_t & lt)
{
    static ThemeColors _defcols(lt);
    return _defcols;
}

const ColorIndex & ThemeColors::get(int col) const
{
    return 0 <= col && col < ColorUnknown ? vals[col] : vals[ColorUnknown];
}

void ThemeColors::set(int col, const ColorIndex & val)
{
    if(0 <= col && col < ColorUnknown)
	vals[col] = val;
}

void ThemeColors::reset(void)
{
    set(ColorBackground, Color::RoyalBlue);
    set(ColorBorderLine, Color::Yellow);
    set(ColorHeaderText, Color::Red);
    set(ColorBodyText, Color::LightCyan);
    set(ColorButtonBackground, Color::RoyalBlue);
    set(ColorButtonBracket, Color::Silver);
    set(ColorButtonFirstText, Color::Red);
    set(ColorButtonBodyText, Color::Silver);
    set(ColorFocusedButtonBackground, Color::Yellow);
    set(ColorFocusedButtonBracket, Color::Silver);
    set(ColorFocusedButtonFirstText, Color::Red);
    set(ColorFocusedButtonBodyText, Color::Blue);
    set(ColorInputFieldBackground, Color::LightYellow);
    set(ColorFocusedInputFieldBackground, Color::Yellow);
    set(ColorInputFieldText, Color::Navy);
    set(ColorInputCursor, Color::Navy);
    set(ColorListSelectedBackground, Color::Yellow);
    set(ColorListSelectedText, Color::Navy);
    set(ColorUnknown, Color::Transparent);
}

/* CurrentTheme */
CurrentTheme::CurrentTheme(const ThemeColors* theme) : themedef(theme)
{
    if(! themedef)
    {
	const line_t line = LineThin; // FIXME TermWindow::fontRender == & systemFont() ? LineAscii : theme().typeLine();                
	themedef = & ThemeColors::defaults(line);
    }
}

void CurrentTheme::setTheme(const ThemeColors & theme)
{
    themedef = & theme;
}

const ThemeColors & CurrentTheme::theme(void) const
{
    return themedef ? *themedef : ThemeColors::defaults();
}

/* LabelAction */
LabelAction::LabelAction(const std::string & str, int action, const TermPos & pos, TermWindow & term, const ThemeColors* theme)
    : TermWindow(term), CurrentTheme(theme), hotkey(Key::NONE)
{
    resetState(FlagModality);
    setState(FlagKeyHandle);
    setAction(action);
    setPosition(term.sym2gfx(pos));
    setLabel(str);

    setVisible(true);
}

LabelAction::LabelAction(TermWindow & term, const ThemeColors* theme) : TermWindow(term), CurrentTheme(theme), hotkey(Key::NONE)
{
    resetState(FlagModality);
    setState(FlagKeyHandle);

    setVisible(false);
}

bool LabelAction::setLabelHotKey(const std::string & str)
{
    bool blockPresent = false;

    auto it1 = std::find(str.begin(), str.end(), '<');
    if(it1 != str.end())
    {
	auto it2 = std::find(it1 + 1, str.end(), '>');
	if(it2 != str.end())
	{
	    blockPresent = true;
	    setHotKey(std::string(it1 + 1, it2));
	}
    }

    content = str;
    return blockPresent;
}

void LabelAction::setLabel(const std::string & str)
{
    bool blockPresent = setLabelHotKey(str);
    setTermSize(TermSize(str.size() - (blockPresent ? 2 : 0), 1));
    renderWindow();
}

bool LabelAction::isLabel(const std::string & str) const
{
    return content == str;
}

const std::string & LabelAction::label(void) const
{
    return content;
}

void LabelAction::setAction(int act)
{
    setResultCode(act);
}

int LabelAction::action(void) const
{
    return resultCode();
}

bool LabelAction::isAction(int act) const
{
    return action() == act;
}

void LabelAction::setHotKey(const std::string & str)
{
    hotkey = Key::toKey(str);
}

void LabelAction::setHotKey(int key)
{
    hotkey = key;
}

void LabelAction::setHotKeyDisabled(bool f)
{
    setUnused8(f ? 1 : 0);
}

bool LabelAction::isHotKeyDisabled(void) const
{
    return unused8();
}

int LabelAction::hotKey(void) const
{
    return isHotKeyDisabled() ? Key::NONE : hotkey;
}

bool LabelAction::isHotKey(int key) const
{
    return hotKey() == key;
}   
    
void LabelAction::setSelected(bool f)
{
    if(! isDisabled())
    {
	if(f)
	{
    	    setState(FlagSelected);
    	    signalEmit(Signal::ButtonSetFocus);
	}
	else
	{
    	    resetState(FlagSelected);
    	    signalEmit(Signal::ButtonResetFocus);
	}

	renderWindow();
    }
}

bool LabelAction::isSelected(void) const
{
    return checkState(FlagSelected);
}

void LabelAction::setDisabled(bool f)
{
    if(f)
    {
        setState(FlagButtonDisabled);
    }
    else
    {
        resetState(FlagButtonDisabled);
    }

    renderWindow();
}

bool LabelAction::isDisabled(void) const
{
    return checkState(FlagButtonDisabled);
}

void LabelAction::renderLabel(const ThemeColors & defcols)
{
    const ColorIndex & blck = isSelected() ? defcols.focusedButtonFirstText() : defcols.buttonFirstText();
    const ColorIndex & body = isSelected() ? defcols.focusedButtonBodyText() : defcols.buttonBodyText();

    for(auto it = label().begin(); it != label().end(); ++it)
    {
        if(*it == '<')
        {
            *this << set::fgcolor(hotKey() ? blck : body);
            continue;
        }
        else
        if(*it == '>')
        {
            *this << set::fgcolor(body);
            continue;
        }

        *this << *it;
    }
}

void LabelAction::renderWindow(void)
{
    const ThemeColors & defcols = theme();
    const ColorIndex & body = isSelected() ? defcols.focusedButtonBodyText() : defcols.buttonBodyText();
    const ColorIndex & back = isSelected() ? defcols.focusedButtonBackground() : defcols.buttonBackground();

    *this << reset::defaults() << fill::defaults(body, back);

    renderLabel(defcols);

    *this << set::flush();
}

bool LabelAction::keyPressEvent(const KeySym & key)
{
    if(Key::NONE != hotKey() && hotKey() == key.keycode())
    {
        if(isSelected())
	    clickAction();
        else
            setSelected(true);
        
        return true;
    }
    else
    if(isSelected() && key.keycode() == Key::RETURN)
    {
	clickAction();
        return true;
    }

    return false;
}

void LabelAction::mouseFocusEvent(void)
{
    setSelected(true);
}

void LabelAction::mouseLeaveEvent(void)
{
    setSelected(false);
}

bool LabelAction::mouseClickEvent(const ButtonsEvent & be)
{
    if(! isDisabled())
	clickAction();

    return true;
}

void LabelAction::clickAction(void)
{
    setSelected(false);

    signalEmit(Signal::ButtonClicked);

    if(action() && parent())
    	pushEventAction(action(), parent(), this);
}

/* LabelActionGroup */
LabelAction* LabelActionGroup::addLabel(const std::string & label, int action, const TermPos & pos, TermWindow & term, const ThemeColors* theme)
{
    push_back(LabelActionPtr(label, action, pos, term, theme));
    return back().get();
}

LabelAction* LabelActionGroup::addLabel(TermWindow & term, const ThemeColors* theme)
{
    push_back(LabelActionPtr(term, theme));
    return back().get();
}

LabelAction* LabelActionGroup::addLabel(LabelAction* ptr)
{
    push_back(LabelActionPtr(ptr));
    return back().get();
}

bool LabelActionGroup::findLabel(const LabelAction* lb) const
{
    if(lb)
    {
	for(auto it = begin(); it != end(); ++it)
	    if((*it).get() == lb) return true;
    }

    return false;
}

LabelAction* LabelActionGroup::findLabel(const std::string & str) const
{
    for(auto it = begin(); it != end(); ++it)
	if((*it).get() && (*it).get()->isLabel(str)) return (*it).get();
    return NULL;
}

LabelAction* LabelActionGroup::findAction(int action) const
{
    for(auto it = begin(); it != end(); ++it)
	if((*it).get() && (*it).get()->isAction(action)) return (*it).get();
    return NULL;
}

LabelAction* LabelActionGroup::findHotKey(int key) const
{
    for(auto it = begin(); it != end(); ++it)
	if((*it).get() && (*it).get()->isHotKey(key)) return (*it).get();
    return NULL;
}

LabelAction* LabelActionGroup::findSelected(void) const
{
    for(auto it = begin(); it != end(); ++it)
	if((*it).get() && (*it).get()->isSelected()) return (*it).get();
    return NULL;
}

LabelAction* LabelActionGroup::findIndex(size_t dist) const
{
    auto it = std::next(begin(), dist);
    return it != end() ? (*it).get() : NULL;
}

int LabelActionGroup::index(LabelAction *ptr) const
{
    if(ptr)
	for(auto it = begin(); it != end(); ++it)
	    if((*it).get() == ptr) return std::distance(begin(), it);
    return -1;
}

bool LabelActionGroup::setSelected(const LabelAction* lb)
{
    if(lb)
    {
	for(auto it = begin(); it != end(); ++it)
	    if((*it).get() == lb)
	{
	    resetSelected();
	    (*it).get()->setSelected(true);
	    return true;
	}
    }

    return false;
}

void LabelActionGroup::nextSelected(void)
{
    if(1 < size())
    {
	auto it = begin();

	for(; it != end(); ++it)
	    if((*it).get() && (*it).get()->isSelected()) break;

	if(it != end())
	{
	    it++;

	    if(it == end())
		it = begin();

	    resetSelected();
	    (*it).get()->setSelected(true);
	}
    }
}

void LabelActionGroup::prevSelected(void)
{
    if(1 < size())
    {
	auto it = begin();

	for(; it != end(); ++it)
	    if((*it).get() && (*it).get()->isSelected()) break;

	if(it != end())
	{
	    if(it == begin())
		it = end();

	    it--;

	    resetSelected();
	    (*it).get()->setSelected(true);
	}
    }
}

void LabelActionGroup::firstSelected(void)
{
    if(size())
    {
	auto & ptr = front();

	if(ptr.get() && ! ptr.get()->isSelected())
	{
	    resetSelected();
	    ptr.get()->setSelected(true);
	}
    }
}

bool LabelActionGroup::isFirstSelected(void) const
{
    if(size())
    {
	auto & ptr = front();
	return ptr.get() == findSelected();
    }
    return false;
}

void LabelActionGroup::lastSelected(void)
{
    if(size())
    {
	auto & ptr = back();

	if(ptr.get() && ! ptr.get()->isSelected())
	{
	    resetSelected();
	    ptr.get()->setSelected(true);
	}
    }
}

bool LabelActionGroup::isLastSelected(void) const
{
    if(size())
    {
	auto & ptr = back();
	return ptr.get() == findSelected();
    }
    return false;
}

void LabelActionGroup::resetSelected(const LabelAction* exclude)
{
    for(auto it = begin(); it != end(); ++it)
	if((*it).get() && (!exclude || exclude != (*it).get())) (*it).get()->setSelected(false);
}

void LabelActionGroup::renderWindow(void)
{
    for(auto it = begin(); it != end(); ++it)
	if((*it).get()) (*it).get()->renderWindow();
}

size_t LabelActionGroup::rows(void) const
{
    size_t res = 0;
    for(auto it = begin(); it != end(); ++it)
	if((*it).get()) res += (*it).get()->rows();
    return res;
}

size_t LabelActionGroup::cols(void) const
{
    size_t res = 0;
    for(auto it = begin(); it != end(); ++it)
	if((*it).get()) res += (*it).get()->cols();
    return res;
}

/* TextButton */
TextButton::TextButton(const std::string & str, int action, const TermPos & pos, TermWindow & term, const ThemeColors* theme) : LabelAction(term, theme)
{
    setAction(action);
    setPosition(term.sym2gfx(pos));
    setLabel(str);
    setVisible(true);
}

TextButton::TextButton(const buttons_t & type, TermWindow & term, const ThemeColors* theme) : LabelAction(term, theme)
{
    setAction(type);
    setLabel(type);
}

void TextButton::setLabel(const std::string & str)
{
    bool blockPresent = setLabelHotKey(str);
    setTermSize(TermSize(label().size() + (blockPresent ? 0 : 2), 1));
    renderWindow();
}

void TextButton::setLabel(const buttons_t & type)
{
    const char* str = "UnknownButton";

    switch(type)
    {
	case ButtonOk:	   str = _("<O>k"); break;
	case ButtonYes:    str = _("<Y>es"); break;
	case ButtonNo:     str = _("<N>o"); break;
	case ButtonCancel: str = _("<C>ancel"); break;
	default: break;
    }

    setLabel(str);
}

void TextButton::renderWindow(void)
{
    if(isVisible())
    {
	const ThemeColors & defcols = theme();
	const ColorIndex & brkt = isSelected() ? defcols.focusedButtonBracket() : defcols.buttonBracket();
	const ColorIndex & body = isSelected() ? defcols.focusedButtonBodyText() : defcols.buttonBodyText();
	const ColorIndex & back = isSelected() ? defcols.focusedButtonBackground() : defcols.buttonBackground();

	*this << reset::defaults() <<
    	    fill::defaults(body, back) << UnicodeColor('[', FBColors(brkt, defcols.buttonBackground()));

	renderLabel(defcols);

	*this << UnicodeColor(']', FBColors(brkt, defcols.buttonBackground())) <<
            set::flush();
    }
}

#define ButtonSelected 0x80000000

/* ButtonsGroup */
ButtonsGroup::ButtonsGroup(int buttons, TermWindow & term, const ThemeColors* defcols)
{
    std::vector<buttons_t> v = buttonsAll(buttons);

    for(auto it = v.begin(); it != v.end(); ++it)
	addLabel(new TextButton(*it, term, defcols));

    if(buttons & ButtonSelected) lastSelected();
}

/* HeaderAreaBox */
HeaderAreaBox::HeaderAreaBox(const UnicodeString & str, const TermSize & tsz, TermWindow & term, const ThemeColors* defcols)
    : TermWindow(term), CurrentTheme(defcols), header(str, theme().headerText())
{
    initHeaderAreaBox(tsz);

    renderWindow();
    setVisible(true);
}

HeaderAreaBox::HeaderAreaBox(const UCString & str, const TermSize & tsz, TermWindow & term, const ThemeColors* defcols)
    : TermWindow(term), CurrentTheme(defcols), header(str)
{
    initHeaderAreaBox(tsz);

    renderWindow();
    setVisible(true);
}

HeaderAreaBox::HeaderAreaBox(const UnicodeString & str, TermWindow & term, const ThemeColors* defcols)
    : TermWindow(term), CurrentTheme(defcols), header(str, theme().headerText())
{
}

HeaderAreaBox::HeaderAreaBox(const UCString & str, TermWindow & term, const ThemeColors* defcols)
    : TermWindow(term), CurrentTheme(defcols), header(str)
{
}

HeaderAreaBox::HeaderAreaBox(const UnicodeString & str, const FontRender & frs, Window & win, const ThemeColors* defcols)
    : TermWindow(frs, win), CurrentTheme(defcols), header(str)
{
}

HeaderAreaBox::HeaderAreaBox(const UCString & str, const FontRender & frs, Window & win, const ThemeColors* defcols)
    : TermWindow(frs, win), CurrentTheme(defcols), header(str)
{
}

void HeaderAreaBox::initHeaderAreaBox(const TermSize & tsz)
{
    setTermSize(tsz + TermSize(2, 2)); // border reserved
    if(parent()) setPosition((parent()->size() - size()) / 2);

    *this << set::colors(theme().borderLine(), theme().background()) <<
	fill::defaults(theme().borderLine(), theme().background());
}

void HeaderAreaBox::renderWindow(void)
{
    const line_t & line = TermWindow::fontRender == & systemFont() ? LineAscii : theme().typeLine();
    renderWindowLine(line);
    *this << set::flush();
}

void HeaderAreaBox::renderWindowLine(const line_t & line)
{
    *this << set::colors(theme().borderLine(), theme().background());

    // border section
    *this << draw::rect(0, 0, cols(), rows(), line);

    if(header.size())
	*this << cursor::set((cols() - header.size() - 2) / 2, 0) <<
    	    acs::rtee(line) << header << set::fgcolor(theme().borderLine()) << acs::ltee(line);
}

bool HeaderAreaBox::keyPressEvent(const KeySym & key)
{
    if(key.keycode() == Key::ESCAPE)
    {
	setVisible(false);
	return true;
    }

    return false;
}

/* ButtonsAreaBox */
ButtonsAreaBox::ButtonsAreaBox(const UnicodeString & str, int buttons, TermWindow & term, const ThemeColors* defcols)
    : HeaderAreaBox(str, term, defcols), buttonsGroup(buttons, *this, defcols)
{
}

ButtonsAreaBox::ButtonsAreaBox(const UCString & str, int buttons, TermWindow & term, const ThemeColors* defcols)
    : HeaderAreaBox(str, term, defcols), buttonsGroup(buttons, *this, defcols)
{
}

ButtonsAreaBox::ButtonsAreaBox(const UnicodeString & str, int buttons, const FontRender & frs, Window & win, const ThemeColors* defcols)
    : HeaderAreaBox(str, frs, win, defcols), buttonsGroup(buttons, *this, defcols)
{
}

ButtonsAreaBox::ButtonsAreaBox(const UCString & str, int buttons, const FontRender & frs, Window & win, const ThemeColors* defcols)
    : HeaderAreaBox(str, frs, win, defcols), buttonsGroup(buttons, *this, defcols)
{
}

ButtonsAreaBox::ButtonsAreaBox(const UnicodeString & str, const TermSize & tsz, int buttons, TermWindow & term, const ThemeColors* defcols)
    : HeaderAreaBox(str, term, defcols), buttonsGroup(buttons, *this, defcols)
{
    initButtonsAreaBox(tsz);
    renderWindow();
}

ButtonsAreaBox::ButtonsAreaBox(const UCString & str, const TermSize & tsz, int buttons, TermWindow & term, const ThemeColors* defcols)
    : HeaderAreaBox(str, term, defcols), buttonsGroup(buttons, *this, defcols)
{
    initButtonsAreaBox(tsz);
    renderWindow();
}

void ButtonsAreaBox::initButtonsAreaBox(const TermSize & tsz)
{
    int cols = std::max(tsz.cols(), maxColumns()) + 2 /* 2 space */;
    initHeaderAreaBox(TermSize(cols, tsz.rows() + 2)) /* 2 button section */;

    setButtonsPosition();
    setButtonsSubscribe(Signal::ButtonClicked);
    setButtonsSubscribe(Signal::ButtonSetFocus);
    signalSubscribe(*this, Signal::ButtonResetFocus);
}

void ButtonsAreaBox::signalReceive(int sig, const SignalMember* sm)
{
    switch(sig)
    {
	case Signal::ButtonResetFocus:
	    buttonsGroup.resetSelected();
	break;

	case Signal::ButtonClicked:
	{
	    const TextButton* tb = dynamic_cast<const TextButton*>(sm);
	    if(tb) setResultCode(tb->action());
	    setVisible(false);
	}
	break;

	case Signal::ButtonSetFocus:
	{
	    const TextButton* tb = dynamic_cast<const TextButton*>(sm);
	    if(tb) buttonsGroup.resetSelected(tb);
	}
	break;

	default: break;
    }
}

void ButtonsAreaBox::setHotKeyDisabled(bool f)
{
    for(size_t it = 0; it < buttonsGroup.count(); ++it)
    {
	LabelAction* btn = buttonsGroup.findIndex(it);
	if(btn) btn->setHotKeyDisabled(f);
    }
}

void ButtonsAreaBox::setButtonsPosition(void)
{
    size_t width = buttonsGroup.cols() + (buttonsGroup.count() - 1);
    Point pos = sym2gfx(TermPos((cols() - width) / 2, rows() - 2)) - position();
 
    for(size_t it = 0; it < buttonsGroup.count(); ++it)
    {
        LabelAction* btn = buttonsGroup.findIndex(it);
        if(btn)
        {
	    btn->setPosition(pos);
            btn->setVisible(true);
	    pos += sym2gfx(TermSize(btn->cols() + 1, 0));
	}
    }
}

void ButtonsAreaBox::setButtonsSubscribe(int sig)
{
    for(size_t it = 0; it < buttonsGroup.count(); ++it)
    {
	LabelAction* btn = buttonsGroup.findIndex(it);
	if(btn) signalSubscribe(*btn, sig);
    }
}

void ButtonsAreaBox::renderWindowLine(const line_t & line)
{
    HeaderAreaBox::renderWindowLine(line);

    // bottom section
    *this << cursor::set(0, rows() - 3) <<
    	acs::ltee(line) << draw::hline(cols() - 2, acs::hline(line)) << acs::rtee(line);

    // buttons section
    buttonsGroup.renderWindow();
}

bool ButtonsAreaBox::keyPressEvent(const KeySym & key)
{
    switch(key.keycode())
    {
        case Key::LEFT:
	    if(buttonsGroup.findSelected())
	    {
		buttonsGroup.prevSelected();
		renderWindow();
		return true;
	    }
	    break;

        case Key::RIGHT:
	    if(buttonsGroup.findSelected())
	    {
		buttonsGroup.nextSelected();
		renderWindow();
		return true;
	    }
	    break;

	case Key::ESCAPE:
	    setVisible(false);
	    return true;

        default: break;
    }

    return false;
}

int ButtonsAreaBox::maxColumns(void) const
{
    return buttonsGroup.cols() + (buttonsGroup.count() - 1); 
}

/* MessageBox */
MessageBox::MessageBox(const UnicodeString & header, const UnicodeString & message, int buttons, TermWindow & term, const ThemeColors* defcols)
    : ButtonsAreaBox(header, buttons | ButtonSelected, term, defcols)
{
    initMessageBox(UCString(message, theme().bodyText()));

    renderWindow();
    setVisible(true);
}

MessageBox::MessageBox(const UCString & header, const UCString & message, int buttons, TermWindow & term, const ThemeColors* defcols)
    : ButtonsAreaBox(header, buttons | ButtonSelected, term, defcols)
{
    initMessageBox(message);

    renderWindow();
    setVisible(true);
}

MessageBox::MessageBox(const UnicodeString & header, const UnicodeString & message, int buttons, const FontRender & frs, Window & win, const ThemeColors* defcols)
    : ButtonsAreaBox(header, buttons | ButtonSelected, frs, win, defcols)
{
    initMessageBox(UCString(message, theme().bodyText()));

    renderWindow();
    setVisible(true);
}

MessageBox::MessageBox(const UCString & header, const UCString & message, int buttons, const FontRender & frs, Window & win, const ThemeColors* defcols)
    : ButtonsAreaBox(header, buttons | ButtonSelected, frs, win, defcols)
{
    initMessageBox(message);

    renderWindow();
    setVisible(true);
}

void MessageBox::initMessageBox(const UCString & message)
{
    content = message.split('\n');
    initButtonsAreaBox(TermSize(content.maxStringWidth(), content.size()));
}

void MessageBox::renderWindowLine(const line_t & line)
{
    ButtonsAreaBox::renderWindowLine(line);

    for(auto it = content.begin(); it != content.end(); ++it)
        *this << cursor::set((cols() - (*it).size()) / 2, 1 + std::distance(content.begin(), it)) << *it;
}

/* InputBox */
InputBox::InputBox(const UnicodeString & header, size_t cols, const std::string & defstr, TermWindow & term, const ThemeColors* defcols)
    : ButtonsAreaBox(header, ButtonYes | ButtonNo, term, defcols), sResult(defstr), tickLast(0)
{
    setFlagTermCursor(true);
    setInputFocused(true);
    setHotKeyDisabled(true);
    initButtonsAreaBox(TermSize(std::max(header.size(), cols), 1));

    renderWindow();
    setVisible(true);
}

InputBox::InputBox(const UnicodeString & header, size_t cols, const std::string & defstr, const FontRender & frs, Window & win, const ThemeColors* defcols)
    : ButtonsAreaBox(header, ButtonYes | ButtonNo, frs, win, defcols), sResult(defstr), tickLast(0)
{
    setFlagTermCursor(true);
    setInputFocused(true);
    setHotKeyDisabled(true);
    initButtonsAreaBox(TermSize(std::max(header.size(), cols), 1));

    renderWindow();
    setVisible(true);
}

void InputBox::setInputFocused(bool f)
{
    if(f)
	setUnused8(unused8() | 0x02);
    else
	setUnused8(unused8() & (~ 0x02));
}

bool InputBox::checkInputFocused(void) const
{
    return unused8() & 0x02;
}

void InputBox::setFlagTermCursor(bool f)
{
    if(f)
	setUnused8(unused8() | 0x01);
    else
	setUnused8(unused8() & (~ 0x01));
}

bool InputBox::checkFlagTermCursor(void) const
{
    return unused8() & 0x01;
}

void InputBox::tickEvent(u32 ms)
{
    if(checkInputFocused() &&
	(tickLast == 0 || (ms - tickLast) > 500))
    {
	setFlagTermCursor(! checkFlagTermCursor());
	renderWindow();
	tickLast = ms;
    }
}

bool InputBox::keyPressEvent(const KeySym & key)
{
    if(ButtonsAreaBox::keyPressEvent(key))
        return true;

    if(! checkInputFocused())
    {
	if(key.keycode() == Key::TAB)
	{
	    if(buttonsGroup.isFirstSelected())
		buttonsGroup.lastSelected();
	    else
	    if(buttonsGroup.isLastSelected())
	    {
		buttonsGroup.resetSelected();
		setInputFocused(true);
	    }

	    return true;
	}

    	return false;
    }

    switch(key.keycode())
    {
        case Key::RETURN:
	    setResultCode(ButtonYes);
	    setVisible(false);
            return true;

        case Key::ESCAPE:
	    sResult.clear();
	    setResultCode(ButtonNo);
            setVisible(false);
            return true;

        case Key::TAB:
	    setInputFocused(false);
	    buttonsGroup.firstSelected();
	    renderWindow();
	    return true;

        case Key::BACKSPACE:
	    if(0 < sResult.size())
	    {
		sResult.erase(sResult.end() - 1);
		renderWindow();
		return true;
	    }
	    break;

        default:
	{
	    int ch = key.keychar();
	    if(ch)
    	    {
		sResult.append(1, ch);
		renderWindow();
		return true;
	    }
	    break;
	}
    }

    return false;
}

void InputBox::renderWindowLine(const line_t & line)
{
    ButtonsAreaBox::renderWindowLine(line);
    size_t iw = cols() - 2;

    // render input
    *this << cursor::set(1, 1) <<
	set::colors(theme().inputFieldText(), (checkInputFocused() ? theme().focusedInputFieldBackground() : theme().inputFieldBackground())) <<
	fill::space(cols() - 2) << cursor::set(1, 1);

    if(sResult.size() < iw)
	*this << sResult;
    else
	*this << sResult.substr(sResult.size() - iw + 1, iw - 1);

    if(checkInputFocused())
	*this << set::fgcolor(theme().inputCursor()) << "_";

    *this << set::flush();
}

/* ListBox */
ListBox::ListBox(const UnicodeString & header, const UCStringList & ucsl, size_t rows, TermWindow & term, const ThemeColors* defcols)
    : ButtonsAreaBox(header, ButtonYes | ButtonNo | ButtonSelected, term, defcols), content(ucsl), selected(-1), skipped(0)
{
    initButtonsAreaBox(TermSize(std::max(header.size(), content.maxStringWidth()), rows));

    renderWindow();
    setVisible(true);
}

ListBox::ListBox(const UnicodeString & header, const UCStringList & ucsl, size_t rows, const FontRender & frs, Window & win, const ThemeColors* defcols)
    : ButtonsAreaBox(header, ButtonYes | ButtonNo | ButtonSelected, frs, win, defcols), content(ucsl), selected(-1), skipped(0)
{
    initButtonsAreaBox(TermSize(std::max(header.size(), content.maxStringWidth()), rows));

    renderWindow();
    setVisible(true);
}

void ListBox::renderWindowLine(const line_t & line)
{
    ButtonsAreaBox::renderWindowLine(line);

    int max = rows() - 4;
    skipped = (max <= selected ? (selected - max + 1) : 0);
    auto it = std::next(content.begin(), skipped);

    for(; it != content.end(); ++it)
    {
	int dist = std::distance(content.begin(), it) - skipped;
	if(dist >= max) break;

	*this << cursor::set(1, 1 + dist);

	if(dist + skipped == selected)
	{
	    *this << cursor::set(1, 1 + dist) <<
		set::colors(theme().listSelectedText(), theme().listSelectedBackground()) <<
		cursor::set(1, 1 + dist) << (*it).toUnicodeString() << fill::space(cols() - 2 - (*it).size());
	}
	else
	{
	    *this << cursor::set(1, 1 + dist) <<
		set::colors(theme().bodyText(), theme().background()) <<
		cursor::set(1, 1 + dist) << *it << fill::space(cols() - 2 - (*it).size());
	}
    }
}

void ListBox::windowVisibleEvent(bool show)
{
    if( ! show)
    {
	sResult.clear();

	if(ButtonYes == resultCode())
	{
	    auto it = std::next(content.begin(), selected);
	    if(it != content.end())
		sResult = (*it).toString();
	}
    }
}

bool ListBox::mouseClickEvent(const ButtonsEvent & be)
{
    Rect rt = sym2gfx(TermRect(1, 1, cols() - 2, rows() - 4));
    Point cur = be.press().position() + position();

    if(rt & cur)
    {
	signalEmit(Signal::ButtonResetFocus);
	int index = skipped + (cur.y - rt.y) / frs()->lineSkipHeight();

	// double click: close event
	if(index == selected)
	{
	    setResultCode(ButtonYes);
	    setVisible(false);
	}
	else
	{
	    selected = index;
	    renderWindow();
	}
    }

    return false;
}

bool ListBox::scrollUpContent(void)
{
    if(0 < selected)
    {
	selected--;
	signalEmit(Signal::ButtonResetFocus);
	renderWindow();
    	return true;
    }

    return false;
}

bool ListBox::scrollDownContent(void)
{
    if(selected < static_cast<int>(content.size() - 1))
    {
	selected++;
	signalEmit(Signal::ButtonResetFocus);
	renderWindow();
        return true;
    }

    return false;
}

bool ListBox::scrollUpEvent(const Point & cur)
{
    Rect rt = sym2gfx(TermRect(1, 1, cols() - 2, rows() - 4));

    if(rt & cur)
	return scrollUpContent();

    return false;
}

bool ListBox::scrollDownEvent(const Point & cur)
{
    Rect rt = sym2gfx(TermRect(1, 1, cols() - 2, rows() - 4));

    if(rt & cur)
	return scrollDownContent();

    return false;
}

bool ListBox::keyPressEvent(const KeySym & key)
{
    if(ButtonsAreaBox::keyPressEvent(key))
        return true;

    switch(key.keycode())
    {
        case Key::UP:
	    return scrollUpContent();

        case Key::DOWN:
	    return scrollDownContent();

        default:
	    break;
    }

    return false;
}

} // namespace TermGUI

#endif

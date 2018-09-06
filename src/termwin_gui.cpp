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

#include "engine.h"
#include "termwin_gui.h"

namespace TermGUI
{

DefaultTheme::DefaultTheme(const line_t & lt) : line(lt)
{
    reset();
}

DefaultTheme & DefaultTheme::defaults(const line_t & lt)
{
    static DefaultTheme _defcols(lt);
    return _defcols;
}

const ColorIndex & DefaultTheme::get(int col) const
{
    return 0 <= col && col < ColorUnknown ? vals[col] : vals[ColorUnknown];
}

void DefaultTheme::set(int col, const ColorIndex & val)
{
    if(0 <= col && col < ColorUnknown)
	vals[col] = val;
}

void DefaultTheme::reset(void)
{
    set(ColorBackground, Color::RoyalBlue);
    set(ColorBorderLine, Color::Yellow);
    set(ColorHeaderText, Color::Red);
    set(ColorBodyText, Color::LightCyan);
    set(ColorButtonBackground, Color::RoyalBlue);
    set(ColorButtonBracket, Color::Silver);
    set(ColorButtonFirstText, Color::Red);
    set(ColorButtonBodyText, Color::Silver);
    set(ColorSelectedButtonBackground, Color::Yellow);
    set(ColorSelectedButtonBracket, Color::Silver);
    set(ColorSelectedButtonFirstText, Color::Red);
    set(ColorSelectedButtonBodyText, Color::Blue);
    set(ColorInputFieldBackground, Color::Khaki);
    set(ColorInputFieldText, Color::Navy);
    set(ColorInputCursor, Color::Navy);
    set(ColorListSelectedBackground, Color::Yellow);
    set(ColorListSelectedText, Color::Navy);
    set(ColorUnknown, Color::Transparent);
}

/* MessageBoxButton */
int MessageBoxButton::hotkey(void) const
{
    switch(button())
    {
        case ButtonOk:  return Key::O;
        case ButtonYes: return Key::Y;
        case ButtonNo:  return Key::N;
        case ButtonCancel: return Key::C;

        default: break;
    }

    return Key::NONE;
}

size_t MessageBoxButton::width(void) const
{
    switch(button())
    {
        case ButtonOk:  return 4;
        case ButtonYes: return 5;
        case ButtonNo:  return 4;
        case ButtonCancel: return 8;

        default: break;
    }

    return 0;
}

UCString MessageBoxButton::label(const DefaultTheme & defcols) const
{
    ColorIndex first = isSelected() ? defcols.selectedButtonFirstText() : defcols.buttonFirstText();
    ColorIndex bracket = isSelected() ? defcols.selectedButtonBracket() : defcols.buttonBracket(); 
    ColorIndex body = isSelected() ? defcols.selectedButtonBodyText() : defcols.buttonBodyText();
    ColorIndex back = isSelected() ? defcols.selectedButtonBackground() : defcols.buttonBackground();

    if(hotkeyDisabled()) first = body;

    UCString res;

    res << UnicodeColor('[', bracket);

    switch(button())
    {
        case ButtonOk:
            res << UCString("O", FBColors(first, back)) << UCString("k", FBColors(body, back));
            break;

        case ButtonYes:
            res << UCString("Y", FBColors(first, back)) << UCString("es", FBColors(body, back));
            break;

        case ButtonNo:
            res << UCString("N", FBColors(first, back)) << UCString("o", FBColors(body, back));
            break;

        case ButtonCancel:
            res << UCString("C", FBColors(first, back)) << UCString("ancel", FBColors(body, back));
            break;

        default:
            res << UCString("Unknown", Color::Gray);
            break;
    }

    res << UnicodeColor(']', bracket);

    return res;
}

/* MessageBoxButtons */
MessageBoxButtons::MessageBoxButtons(int buttons)
{
    if(buttons & ButtonYes)
        btn1 = MessageBoxButton(ButtonYes);
    else
    if(buttons & ButtonOk)
        btn1 = MessageBoxButton(ButtonOk);

    if(buttons & ButtonNo)
        btn2 = MessageBoxButton(ButtonNo, true);
    else
    if(buttons & ButtonCancel)
        btn2 = MessageBoxButton(ButtonCancel, true);

    if(btn1.isValid() && ! btn2.isValid())
        btn1.setSelected(true);
}

int MessageBoxButtons::selected(void) const
{
    return btn1.isSelected() ? btn1.button() : btn2.button();
}

size_t MessageBoxButtons::width(void) const
{
    if(btn1.isValid() && btn2.isValid())
        return btn1.width() + btn2.width() + 1;

    if(btn1.isValid())
        return btn1.width();

    if(btn2.isValid())
        return btn2.width();

    return 0;
}

void MessageBoxButtons::disableHotkeys(void)
{
    btn1.disableHotkey();
    btn2.disableHotkey();
}

UCString MessageBoxButtons::labels(const DefaultTheme & defcols) const
{
    UCString res;

    if(btn1.isValid() && btn2.isValid())
        res << btn1.label(defcols) << UnicodeString(" ") << btn2.label(defcols);
    else
    {
        if(btn1.isValid())
            return btn1.label(defcols);

        if(btn2.isValid())
            return btn2.label(defcols);
    }

    return res;
}

bool MessageBoxButtons::keyPressEvent(int key)
{
    if(btn1.isValid() && ! btn1.isSelected() &&
	! btn1.hotkeyDisabled() && (key == btn1.hotkey() || key == Key::lower(btn1.hotkey())))
    {
	if(btn2.isSelected()) btn2.setSelected(false);
        btn1.setSelected(true);
        return true;
    }
    else
    if(btn2.isValid() && ! btn2.isSelected() &&
	! btn2.hotkeyDisabled() && (key == btn2.hotkey() || key == Key::lower(btn2.hotkey())))
    {
	if(btn1.isSelected()) btn1.setSelected(false);
        btn2.setSelected(true);
        return true;
    }
    else
    if(key == Key::LEFT)
    {
        if(btn1.isValid() && btn2.isValid())
        {
            if(btn2.isSelected()) btn2.setSelected(false);
            btn1.setSelected(true);
            return true;
        }
    }
    if(key == Key::RIGHT)
    {
        if(btn1.isValid() && btn2.isValid())
        {
            if(btn1.isSelected()) btn1.setSelected(false);
            btn2.setSelected(true);
            return true;
        }
    }

    return false;
}

/* EmptyAreaBox */
EmptyAreaBox::EmptyAreaBox(int btns, TermWindow & term, const DefaultTheme* defcols)
    : TermWindow(term), buttons(btns), themedef(defcols)
{
}

EmptyAreaBox::EmptyAreaBox(int btns, const FontRender & frs, Window & win, const DefaultTheme* defcols)
    : TermWindow(frs, win), buttons(btns), themedef(defcols)
{
}

EmptyAreaBox::EmptyAreaBox(size_t cols, size_t rows, int btns, TermWindow & term, const DefaultTheme* defcols)
    : TermWindow(term), buttons(btns), themedef(defcols)
{
    setAreaSize(cols, rows);
}

EmptyAreaBox::EmptyAreaBox(size_t cols, size_t rows, int btns, const FontRender & frs, Window & win, const DefaultTheme* defcols)
    : TermWindow(frs, win), buttons(btns), themedef(defcols)
{
    setAreaSize(cols, rows);
}

void EmptyAreaBox::setAreaSize(size_t cols, size_t rows)
{
    Size boxsz;

    boxsz.w = std::max(cols, buttons.width()) + 4;
    boxsz.h = 4 + rows;

    if(fontRender)
    {
	Size gfsz = fontRender->size() * boxsz;

	setPosition((parent()->size() - gfsz) / 2);
	setSize(gfsz);
    }
}

const DefaultTheme & EmptyAreaBox::theme(void) const
{
    if(! themedef)
    {
	const line_t line = LineThin; //TermWindow::fontRender == & systemFont() ? LineAscii : theme().typeLine();
	return DefaultTheme::defaults(line);
    }

    return *themedef;
}

void EmptyAreaBox::renderBody(const UnicodeString & header)
{
    const line_t & line = TermWindow::fontRender == & systemFont() ? LineAscii : theme().typeLine();

    *this << set::colors(theme().borderLine(), theme().background()) <<
    // border section
	draw::rect(0, 0, cols(), rows(), line) <<
    // bottom section
	cursor::set(0, rows() - 3) <<
    	acs::ltee(line) << draw::hline(cols() - 2, acs::hline(line)) << acs::rtee(line);

    // header
    if(header.size())
    {
	*this << cursor::set((cols() - header.size() - 2) / 2, 0) <<
    	    acs::rtee(line) << set::fgcolor(theme().headerText()) << header << set::fgcolor(theme().borderLine()) << acs::ltee(line);
    }
}

void EmptyAreaBox::renderButtons(void)
{
    *this << set::colors(theme().bodyText(), theme().background()) <<
	cursor::set((cols() - buttons.width()) / 2, rows() - 2) <<
        buttons.labels(theme());
}

bool EmptyAreaBox::keyPressEvent(int key)
{
    return buttons.keyPressEvent(key);
}

/* MessageBox */
MessageBox::MessageBox(const UnicodeString & header, const UnicodeString & content, int btns, TermWindow & term, const DefaultTheme* defcols)
    : EmptyAreaBox(btns, term, defcols)
{
    renderBody(header, UCString(content, theme().bodyText()));
    renderWindow();
    setVisible(true);
}

MessageBox::MessageBox(const UnicodeString & header, const UnicodeString & content, int btns, const FontRender & frs, Window & win, const DefaultTheme* defcols)
    : EmptyAreaBox(btns, frs, win, defcols)
{
    renderBody(header, UCString(content, theme().bodyText()));
    renderWindow();
    setVisible(true);
}

MessageBox::MessageBox(const UnicodeString & header, const UCString & content, int btns, TermWindow & term, const DefaultTheme* defcols)
    : EmptyAreaBox(btns, term)
{
    renderBody(header, content);
    renderWindow();
    setVisible(true);
}

MessageBox::MessageBox(const UnicodeString & header, const UCString & content, int btns, const FontRender & frs, Window & win, const DefaultTheme* defcols)
    : EmptyAreaBox(btns, frs, win)
{
    renderBody(header, content);
    renderWindow();
    setVisible(true);
}

void MessageBox::renderBody(const UnicodeString & header, const UCString & content)
{
    auto ucsl = content.split('\n');
    setAreaSize(ucsl.maxStringWidth(), ucsl.size());

    *this << reset::defaults() <<
        fill::defaults(theme().bodyText(), theme().background());

    EmptyAreaBox::renderBody(header);

    for(auto it = ucsl.begin(); it != ucsl.end(); ++it)
    {
        *this << cursor::set((cols() - (*it).size()) / 2, 1 + std::distance(ucsl.begin(), it)) << *it;
    }
}

void MessageBox::renderWindow(void)
{
    EmptyAreaBox::renderButtons();
    *this << set::dirty() << set::flush();
}

bool MessageBox::keyPressEvent(int key)
{
    if(EmptyAreaBox::keyPressEvent(key))
    {
        renderWindow();
        return true;
    }

    switch(key)
    {
        case Key::RETURN:
    	    setResultCode(buttons.selected());
            setVisible(false);
            return true;

        case Key::ESCAPE:
            setVisible(false);
            return true;

        default: break;
    }

    return false;
}

bool MessageBox::mouseClickEvent(const ButtonsEvent & ev)
{
    return false;
}

InputBox::InputBox(const UnicodeString & header, size_t cols, const std::string & defstr, TermWindow & term, const DefaultTheme* defcols)
    : EmptyAreaBox(ButtonYes | ButtonNo, term, defcols), strLine(defstr), tickLast(0)
{
    setAreaSize(std::max(header.size() + 4, cols), 1);

    *this << reset::defaults() <<
        fill::defaults(theme().bodyText(), theme().background());

    buttons.disableHotkeys();
    EmptyAreaBox::renderBody(header);

    renderWindow();
    setVisible(true);
}

InputBox::InputBox(const UnicodeString & header, size_t cols, const std::string & defstr, const FontRender & frs, Window & win, const DefaultTheme* defcols)
    : EmptyAreaBox(ButtonYes | ButtonNo, frs, win, defcols), strLine(defstr), tickLast(0)
{
    setAreaSize(std::max(header.size() + 4, cols), 1);

    *this << reset::defaults() <<
        fill::defaults(theme().bodyText(), theme().background());

    buttons.disableHotkeys();
    EmptyAreaBox::renderBody(header);

    renderWindow();
    setVisible(true);
}

void InputBox::renderWindow(void)
{
    int iw = cols() - 2;

    // render input
    *this << cursor::set(1, 1) <<
	set::colors(theme().inputFieldText(), theme().inputFieldBackground()) << fill::space(cols() - 2) <<
	cursor::set(1, 1);

    if(strLine.size() < iw)
	*this << strLine;
    else
	*this << strLine.substr(strLine.size() - iw + 1, iw - 1);

    if(state.check(FlagTermCursor))
	*this << set::fgcolor(theme().inputCursor()) << "_";

    EmptyAreaBox::renderButtons();
    *this << set::dirty() << set::flush();
}

void InputBox::tickEvent(u32 ms)
{
    if(tickLast == 0 || (ms - tickLast) > 500)
    {
	state.switched(FlagTermCursor);
	renderWindow();
	tickLast = ms;
    }
}

bool InputBox::keyPressEvent(int key)
{
    if(EmptyAreaBox::keyPressEvent(key))
    {
        renderWindow();
        return true;
    }

    switch(key)
    {
        case Key::RETURN:
	    if(ButtonYes == buttons.selected())
        	setResultCode(ButtonYes);
            setVisible(false);
            return true;

        case Key::ESCAPE:
	    strLine.clear();
            setVisible(false);
            return true;

        case Key::BACKSPACE:
	    if(0 < strLine.size())
	    {
		strLine.erase(strLine.end() - 1);
		renderWindow();
		return true;
	    }
	    break;

        default:
	{
	    int ch = Key::toChar(key);
	    if(ch)
    	    {
		strLine.append(1, ch);
		renderWindow();
		return true;
	    }
	    break;
	}
    }

    return false;
}

ListBox::ListBox(const UnicodeString & hdr, const UCStringList & ucsl, size_t rows, TermWindow & term, const DefaultTheme* defcols)
    : EmptyAreaBox(ButtonYes | ButtonNo, term, defcols), header(hdr), content(ucsl), selected(-1)
{
    setAreaSize(std::max(header.size(), content.maxStringWidth()), rows);

    *this << reset::defaults() <<
        fill::defaults(theme().bodyText(), theme().background());

    renderWindow();
    setVisible(true);
}

ListBox::ListBox(const UnicodeString & hdr, const UCStringList & ucsl, size_t rows, const FontRender & frs, Window & win, const DefaultTheme* defcols)
    : EmptyAreaBox(ButtonYes | ButtonNo, frs, win, defcols), header(hdr), content(ucsl), selected(-1)
{
    setAreaSize(std::max(header.size(), content.maxStringWidth()), rows);

    *this << reset::defaults() <<
        fill::defaults(theme().bodyText(), theme().background());


    renderWindow();
    setVisible(true);
}

void ListBox::renderWindow(void)
{
    int max = rows() - 4;
    EmptyAreaBox::renderBody(header);

    auto it = content.begin();
    int skipped = (max <= selected ? (selected - max + 1) : 0);
    std::advance(it, skipped);

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

    EmptyAreaBox::renderButtons();
    *this << set::dirty() << set::flush();
}

bool ListBox::keyPressEvent(int key)
{
    if(EmptyAreaBox::keyPressEvent(key))
    {
        renderWindow();
        return true;
    }

    switch(key)
    {
        case Key::RETURN:
	    if(ButtonYes == buttons.selected())
	    {
        	setResultCode(ButtonYes);

		auto it = content.index(selected);
		if(it != content.end())
		    strLine = (*it).toString();
	    }
    	    setVisible(false);
    	    return true;
    
        case Key::ESCAPE:
            setVisible(false);
            return true;

        case Key::UP:
	    if(0 < selected)
	    {
		selected--;
		renderWindow();
        	return true;
	    }
	    break;

        case Key::DOWN:
	    if(selected < static_cast<int>(content.size() - 1))
	    {
		selected++;
		renderWindow();
        	return true;
	    }
	    break;

        default:
	    break;
    }

    return false;
}

} // namespace TermGUI

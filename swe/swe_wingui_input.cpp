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

#include "swe_display.h"
#include "swe_inputs_keys.h"
#include "swe_wingui_input.h"

namespace SWE
{
    LineEdit::LineEdit(Window* win)
	    : WindowToolTipArea(win), curpos(0)
    {
        setKeyHandle(true);
	setModality(false);
    }

    LineEdit::LineEdit(const Size & sz, Window* win)
	    : WindowToolTipArea(sz, win), curpos(0)
    {
        setKeyHandle(true);
	setModality(false);
    }

    LineEdit::LineEdit(const Point & pos, const Size & sz, Window* win)
	    : WindowToolTipArea(pos, sz, win), curpos(0)
    {
        setKeyHandle(true);
	setModality(false);
    }

    LineEdit::LineEdit(const Point & pos, const Size & sz, const std::string & str, Window* win)
	    : WindowToolTipArea(pos, sz, win), content(str), curpos(str.size())
    {
        setKeyHandle(true);
	setModality(false);
    }

    bool LineEdit::mouseClickEvent(const ButtonsEvent & be)
    {
	setCursorPosition(cursorPositionAt(position() + be.press().position()));
	return true;
    }

    bool LineEdit::keyPressEvent(const KeySym & key)
    {
	if(key.keycode() == Key::RETURN)
	{
	    returnPressed();
	    signalEmit(Signal::LineEditReturnPressed);
	    return true;
	}
	else
	if(key.keycode() == Key::BACKSPACE && content.size())
	{
	    if(0 < curpos)
	    {
    		content.erase(curpos - 1, 1);
		curpos = curpos - 1;
		textChanged(content);
		cursorPositionChanged(curpos + 1, curpos);
		signalEmit(Signal::LineEditTextChanged);
		signalEmit(Signal::LineEditCursorChanged);
	    }
	    return true;
	}
	else
	if(key.keycode() == Key::DELETE)
	{
	    if(0 < curpos)
	    {
    		content.erase(curpos, 1);
		textChanged(content);
		signalEmit(Signal::LineEditTextChanged);
	    }
	    return true;
	}
	else
	if(key.keycode() == Key::LEFT)
	{
            if(0 < curpos)
            {
                curpos = curpos - 1;
                cursorPositionChanged(curpos + 1, curpos);
		signalEmit(Signal::LineEditCursorChanged);
            }
	    return true;
	}
	else
	if(key.keycode() == Key::RIGHT)
	{
            if(curpos < static_cast<int>(content.size()))
            {
                curpos = curpos + 1;
                cursorPositionChanged(curpos - 1, curpos);
		signalEmit(Signal::LineEditCursorChanged);
	    }
	    return true;
	}
	else
	if(key.keycode() == Key::HOME)
	{
	    if(0 < curpos)
	    {
    		int oldpos = curpos;
		curpos = 0;
		cursorPositionChanged(oldpos, curpos);
		signalEmit(Signal::LineEditCursorChanged);
	    }
	    return true;
	}
	else
	if(key.keycode() == Key::END)
	{
            if(curpos < static_cast<int>(content.size()))
            {
                int oldpos = curpos;
                curpos = content.size();
                cursorPositionChanged(oldpos, curpos);
		signalEmit(Signal::LineEditCursorChanged);
            }
	    return true;
	}
#ifndef SWE_SDL12
	//
	else
	if(key.keycode() == Key::INSERT && key.keymod().isShift() &&
    	    SDL_HasClipboardText())
	{
    	    std::string str = SDL_GetClipboardText();
	    textInputEvent(str);
	    return true;
	}
#endif

#ifdef SWE_SDL12
	// text input SDL12: ascii only
	if(0x20 <= key.keychar() && key.keychar() < 255)
	{
	    textInputEvent(std::string(1, key.keychar()));
	    return true;
	}
#endif

	return false;
    }

    bool LineEdit::textInputEvent(const std::string & str)
    {
	if(str.size())
	{
	    content.insert(curpos, str);
	    curpos = curpos + str.size();
	    textChanged(content);
            cursorPositionChanged(curpos - str.size(), curpos);
	    signalEmit(Signal::LineEditTextChanged);
	}
	return true;
    }

    const FontRender & LineEdit::fontRender(void) const
    {
	return systemFont();
    }

    void LineEdit::clear(void)
    {
	if(content.size())
	{
	    content.clear();
	    int oldpos = curpos;
	    curpos = 0;
	    textChanged(content);
    	    cursorPositionChanged(oldpos, curpos);
	    signalEmit(Signal::LineEditTextChanged);
	    signalEmit(Signal::LineEditCursorChanged);
	}
    }

    int LineEdit::cursorPosition(void) const
    {
	return curpos;
    }

    int LineEdit::cursorPositionAt(const Point & pos) const
    {
	if(isAreaPoint(pos))
	{
	    Rect area(position(), Size(0, 0));
	    for(auto it = content.begin(); it != content.end(); ++it)
	    {
		const std::string sym = std::string(1, *it);
		area = area + fontRender().stringSize(sym);
		if(area & pos) return std::distance(content.begin(), it);
	    }
	    return content.size();
	}
	return -1;
    }

    void LineEdit::setCursorPosition(int pos)
    {
	if(0 > pos || pos > static_cast<int>(content.size()))
	    pos = content.size();

	if(curpos != pos)
	{
	    int oldpos = curpos;
	    curpos = pos;
    	    cursorPositionChanged(oldpos, curpos);
	    signalEmit(Signal::LineEditCursorChanged);
	}
    }

    void LineEdit::setText(const std::string & str)
    {
	if(str.size())
	{
	    content.assign(str);
	    int oldpos = curpos;
	    curpos = str.size();
	    textChanged(content);
    	    cursorPositionChanged(oldpos, curpos);
    	    signalEmit(Signal::LineEditTextChanged);
	    signalEmit(Signal::LineEditCursorChanged);
	}
	else
	{
	    clear();
	}
    }

    const std::string & LineEdit::text(void) const
    {
	return content;
    }

    int calcOffset(const SWE::FontRender & frs, const std::string & content, int txwidth, int width)
    {
	int pos = 0;
	int csz = content.size();
	while(pos < csz)
	{
	    auto strsz = frs.stringSize(content.substr(pos));
	    if(strsz.w + txwidth <= width) break;
	    pos = pos + 1;
	}
	return pos;
    }

    void LineEdit::renderLineEdit(const Color & textColor, const Color & cursorColor, int cursorHeight, int cursorOffset)
    {
	const FontRender & frs = fontRender();

	// align bottom
	int offy = height() - frs.lineSkipHeight();
	Rect pos;

	if(0 > curpos || curpos == static_cast<int>(content.size()))
	{
	    int cursorW = frs.symbolAdvance(0x20);

	    if(content.size())
	    {
		auto strsz = frs.stringSize(content);
		// align right
		if(strsz.w + cursorW > width())
		    pos = renderText(frs, content, textColor, Point(width() - cursorW, offy), SWE::AlignRight);
		// align left
		else
		    pos = renderText(frs, content, textColor, Point(0, offy));
	    }
	    if(0 <= curpos)
		renderColor(cursorColor, Rect(pos.x + pos.w, cursorOffset, cursorW, cursorHeight));
	}
	else
	{
	    const std::string sym = std::string(1, content[curpos]);
	    int cursorW = frs.symbolAdvance(content[curpos]);

	    auto strsz = frs.stringSize(content.substr(0, curpos));
	    // align right
	    if(strsz.w + cursorW > width())
	    {
		// cursor
		renderColor(cursorColor, Rect(width() - cursorW, cursorOffset, cursorW, cursorHeight));
		pos = renderText(frs, sym, textColor, Point(width() - cursorW, offy));
		// prefix
		pos = renderText(frs, content.substr(0, curpos), textColor, Point(pos.x, offy), SWE::AlignRight);
	    }
	    // align left
	    else
	    {
		// prefix
		pos = renderText(frs, content.substr(0, curpos), textColor, Point(0, offy));
		// cursor
		renderColor(cursorColor, Rect(pos.x + pos.w, cursorOffset, cursorW, cursorHeight));
		pos = renderText(frs, sym, textColor, Point(pos.x + pos.w, offy));
		// suffix
		pos = renderText(frs, content.substr(curpos + 1), textColor(), Point(pos.x + pos.w, offy));
	    }
	}
    }

    void LineEdit::cursorPositionChanged(int oldpos, int newpos)
    {
	setDirty(true);
    }

    void LineEdit::textChanged(const std::string & text)
    {
	setDirty(true);
    }

    void LineEdit::renderWindow(void)
    {
	renderLineEdit(Color::White, Color::Silver, fontRender().lineSkipHeight(), 0);
    }

}

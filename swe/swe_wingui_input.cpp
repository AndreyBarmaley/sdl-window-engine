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
	resetState(FlagModality);
    }

    LineEdit::LineEdit(const Size & sz, Window* win)
	    : WindowToolTipArea(sz, win), curpos(0)
    {
        resetState(FlagModality);
    }

    LineEdit::LineEdit(const Point & pos, const Size & sz, Window* win)
	    : WindowToolTipArea(pos, sz, win), curpos(0)
    {
        resetState(FlagModality);
    }

    LineEdit::LineEdit(const Point & pos, const Size & sz, const std::string & str, Window* win)
	    : WindowToolTipArea(pos, sz, win), content(str), curpos(str.size())
    {
        resetState(FlagModality);
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
            if(curpos < content.size())
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
            if(curpos < content.size())
            {
                int oldpos = curpos;
                curpos = content.size();
                cursorPositionChanged(oldpos, curpos);
		signalEmit(Signal::LineEditCursorChanged);
            }
	    return true;
	}
#ifndef OLDENGINE
	// 
	else
	if(key.keycode() == Key::INSERT && key.isShift() &&
    	    SDL_HasClipboardText())
	{
    	    std::string str = SDL_GetClipboardText();
	    textInputEvent(str);
	    return true;
	}
#endif

#ifdef OLDENGINE
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

    Color LineEdit::textColor(void) const
    {
	return Color::White;
    }

    Texture LineEdit::cursorTexture(int sym) const
    {
	const FontRender & frs = fontRender();
	return Display::renderRect(Color::Silver, Color::Transparent, Size(frs.symbolAdvance(sym), frs.lineSkipHeight()));
    }

    const FontRender & LineEdit::fontRender(void) const
    {
	return systemFont();
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
	if(0 > pos || pos > content.size())
	    pos = content.size();

	if(curpos != pos)
	{
	    int oldpos = curpos;
	    curpos = pos;
    	    cursorPositionChanged(oldpos, curpos);
	    signalEmit(Signal::LineEditCursorChanged);
	}
    }

    void LineEdit::setKeyHandle(bool f)
    {
	setState(FlagKeyHandle);
    }

    void LineEdit::setText(const std::string & str)
    {
	if(str.size())
	{
	    content = str;
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

    void LineEdit::renderWindow(void)
    {
	const FontRender & frs = fontRender();
	// align bottom
	int offy = height() - frs.lineSkipHeight();
	Rect pos;

	if(0 > curpos || curpos == content.size())
	{
	    if(content.size())
		pos = renderText(frs, content, textColor(), Point(0, offy));
	    if(0 <= curpos)
		renderTexture(cursorTexture(0x20), Point(pos.x + pos.w, 0));
	}
	else
	{
	    pos = renderText(frs, content.substr(0, curpos), textColor(), Point(0, offy));
	    const std::string sym = std::string(1, content[curpos]);
	    renderTexture(cursorTexture(content[curpos]), Point(pos.x + pos.w, 0));
	    pos = renderText(frs, sym, textColor(), Point(pos.x + pos.w, offy));
	    pos = renderText(frs, content.substr(curpos + 1), textColor(), Point(pos.x + pos.w, offy));
	}
    }

    void LineEdit::cursorPositionChanged(int oldpos, int newpos)
    {
	renderWindow();
    }

    void LineEdit::textChanged(const std::string & text)
    {
	renderWindow();
    }
}

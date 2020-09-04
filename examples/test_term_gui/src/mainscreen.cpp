/***************************************************************************
 *   Copyright (C) 2020 by SWE team <sdl.window.engine@gmail.com>          *
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

#include "mainscreen.h"
#include "default_font.h"

using namespace SWE;

DisplayInit::DisplayInit(const std::string & title, const Size & minsz, int fsz) : termsz(minsz)
{
    ttf = BinaryBuf(_default_ttf_h.data, sizeof(_default_ttf_h.data)).zlibUncompress(_default_ttf_h.size);
    frs = FontRenderTTF(ttf, fsz, false);

    auto defsz = termsz * frs.size();

    if(! Display::init(title, defsz, defsz, false, true, true))
	    Engine::except(__FUNCTION__, "display init error");
}

bool DisplayInit::setFontSize(int fsz, const TermSize & termsz)
{
    frs = FontRenderTTF(ttf, fsz, false);
    auto winsz = termsz.toSize() * frs.size();
    return Display::resize(winsz);
}

MainScreen::MainScreen(const std::string & title, int fsz) : DisplayInit(title, Size(80, 25), fsz), fontsz(fsz)
{
    setFontRender(DisplayInit::frs);

    *this << reset::defaults() <<
	fill::defaults(Color::Silver, Color::Black);

    labels.push_back(new TermGUI::TextButton("<T>est Button1", 1025, TermPos(50, 10), *this));
    labels.push_back(new TermGUI::TextButton("T<e>st Button2", 1026, TermPos(50, 12), *this));
    labels.push_back(new TermGUI::TextButton("Te<s>t Button3", 1027, TermPos(50, 14), *this));

    setVisible(true);
}

MainScreen::~MainScreen()
{
    for(auto it = labels.begin(); it != labels.end(); ++it)
	delete *it;
}

MainScreen & MainScreen::init(const std::string & title)
{
    static MainScreen main(title, 14);
    return main;
}

void MainScreen::renderWindow(void)
{
    *this << reset::defaults() <<
        fill::defaults(Color::Silver, Color::Black);

    *this << "Example Strings" << set::rn();

    *this << "simple std::string" << set::rn();
    *this << set::colors(Color::DarkRed, Color::RoyalBlue) << "simple" << reset::colors() << " " <<
	set::colors(Color::Red, Color::Blue) << "color" << reset::colors() << " " <<
	set::colors(Color::IndianRed, Color::DarkBlue) << "string" << reset::colors() << set::rn();

    *this << UnicodeString("simple unicode: Привет Бармалей") << set::rn();
    *this << UnicodeString("color unicode: ") << 
	set::colors(Color::Yellow, Color::Navy) << UnicodeString("Привет") << reset::colors() << " " <<
	set::colors(Color::Moccasin, Color::Maroon) << UnicodeString("Бармалей") << reset::colors() << set::rn();

    UCString ucs(FBColors(Color::Silver, Color::Black));
    ucs << UnicodeColor(L'Б', Color::IndianRed) <<
	UnicodeColor(L'а', Color::Pink) <<
	UnicodeColor(L'р', Color::Lavender) <<
	UnicodeColor(L'м', Color::Amethyst) <<
	UnicodeColor(L'а', Color::LightGreen) <<
	UnicodeColor(L'л', Color::Aquamarine) <<
	UnicodeColor(L'е', Color::CornflowerBlue) <<
	UnicodeColor(L'й', Color::Seashell);

    *this << "test property style: ";
    *this << fill::property(CharsetProperty(true, StyleBold, HintingNormal), TermSize(9, 1)) << cursor::move(MoveLeft, 9) << "StyleBold ";
    *this << fill::property(CharsetProperty(true, StyleItalic, HintingNormal), TermSize(11, 1)) << cursor::move(MoveLeft, 11) << "StyleItalic ";
    *this << fill::property(CharsetProperty(true, StyleUnderLine, HintingNormal), TermSize(14, 1)) << cursor::move(MoveLeft, 14) << "StyleUnderLine" << set::rn();

    *this << "simple UCString: " << ucs << set::rn();
    
    *this << "UCString::parseUnicode: " << UCString::parseUnicode("test", FBColors(Color::Silver)) << set::rn();
    *this << "UCString::parseUnicode: " << UCString::parseUnicode("test [color:red]red", FBColors(Color::Silver)) << set::rn();
    *this << "UCString::parseUnicode: " << UCString::parseUnicode("test [color:red]red[color:default] default", FBColors(Color::Silver)) << set::rn();
    *this << "UCString::parseUnicode: " << UCString::parseUnicode("test [color:red]red[color:default] [color:yellow:red]yellow_red", FBColors(Color::Silver)) << set::rn();

    *this  << set::rn();;
    *this << "examples:" << set::rn();
    *this << "Mouse scroll up/scroll down: changed font size" << set::rn();
    *this << "F1: TermGUI::MessageBox" << set::rn();
    *this << "F2: TermGUI::InputBox" << set::rn();
    *this << "F3: TermGUI::ListBox" << set::rn();
    *this << "ESC: exit program" << set::rn();
    *this << cursor::right();

    // flush
    *this << set::flush();

    std::for_each(labels.begin(), labels.end(), std::mem_fun(&Window::renderWindow));
}

bool MainScreen::keyPressEvent(const KeySym & key)
{
    if(key.keycode() == Key::F1)
    {
	TermGUI::MessageBox msg("Message Box", "Exit from my super program?\nPlease selected....", 
				TermGUI::ButtonOk | TermGUI::ButtonCancel, *this);
	if(TermGUI::ButtonOk == msg.exec())
	{
	    VERBOSE("MessageBox: " << "return ButtonOK");
	}
	else
	{
	    VERBOSE("MessageBox: " << "discard");
	}
	return true;
    }
    else
    if(key.keycode() == Key::F2)
    {
	TermGUI::InputBox msg("Input Value", 20, "", *this);
	if(msg.exec())
	{
	    VERBOSE("InputBox: " << "return " << (msg.result().size() ? msg.result() : "empty"));
	}
	else
	{
	    VERBOSE("InputBox: " << "discard");
	}
	return true;
    }
    else
    if(key.keycode() == Key::F3)
    {
	UnicodeList ul;
	ul << "test 123" << "test 234" << "test 345" << "test 456" << "test 567" << "test 678" << "test 789";
	TermGUI::ListBox msg("Select Value", UCStringList(ul, Color::SpringGreen), 5, *this);
	if(msg.exec())
	{
	    VERBOSE("ListBox: " << "return " << (msg.result().size() ? msg.result() : "empty"));
	}
	else
	{
	    VERBOSE("ListBox: " << "discard");
	}
	return true;
    }
    else
    if(key.keycode() == Key::F4)
    {
	TermGUI::HeaderAreaBox msg("Header Box", TermSize(20, 10), *this);
	msg.exec();
	VERBOSE("HeaderBox: " << "closed");
	return true;
    }
    else
    if(key.keycode() == Key::F5)
    {
	TermGUI::ButtonsAreaBox msg("Buttons Box", TermSize(20, 10), TermGUI::ButtonYes|TermGUI::ButtonNo|TermGUI::ButtonCancel, *this);
	if(int res = msg.exec())
	{
	    VERBOSE("ButtonsBox: " << "return " << res);
	}
	return true;
    }
    else
    if(key.keycode() == Key::ESCAPE)
    {
	setVisible(false);
	return true;
    }

    return false;
}

bool MainScreen::scrollUpEvent(void)
{
    if(9 < fontsz)
    {
	fontsz--;
	setFontSize(fontsz, termSize());
    }
    return true;
}

bool MainScreen::scrollDownEvent(void)
{
    if(24 > fontsz)
    {
	fontsz++;
	setFontSize(fontsz, termSize());
    }
    return true;
}

bool MainScreen::userEvent(int act, void* data)
{
    switch(act)
    {
	case 1025: VERBOSE("LabelAction: " << 1025); return true;
	case 1026: VERBOSE("LabelAction: " << 1026); return true;
	case 1027: VERBOSE("LabelAction: " << 1027); return true;
	default: break;
    }

    return false;
}

void MainScreen::displayResizeEvent(const Size & sz, bool sdl)
{
    if(sdl)
    {
	Size termsz = sz / DisplayInit::frs.size();
	Size gfxsz = (termsz < DisplayInit::termsz ? DisplayInit::termsz : termsz) * DisplayInit::frs.size();

	Display::resize(gfxsz);
	setSize(gfxsz);
    }
    else
    {
	// font size changed event
	Window::displayResizeEvent(sz, sdl);
    }
}

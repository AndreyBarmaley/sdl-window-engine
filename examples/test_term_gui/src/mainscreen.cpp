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

DisplayInit::DisplayInit(const std::string & title, const TermSize & tsz, int fsz, const char* extfont) : ext(extfont)
{
    if(!ext && Systems::isFile("DejaVuSansMono.ttf"))
        ext = "DejaVuSansMono.ttf";

    if(ext && Systems::isFile(ext))
    {
        VERBOSE("use external font: " << ext);
        frs.open(ext, fsz, RenderBlended);
    }
    else
    {
        VERBOSE("use internal font: " << "terminus");
	ttf = BinaryBuf(_default_ttf_h.data, sizeof(_default_ttf_h.data)).zlibUncompress(_default_ttf_h.size);
	frs.load(ttf, fsz, false);
    }

    auto defsz = tsz.toSize() * frs.size();

    if(! Display::init(title, defsz, defsz, false, true, true))
	    Engine::except(__FUNCTION__, "display init error");
}

bool DisplayInit::setFontSize(int fsz, const TermSize & termsz)
{
    if(ext && Systems::isFile(ext))
        frs.open(ext, fsz, RenderBlended);
    else
	frs.load(ttf, fsz, false);

    auto winsz = termsz.toSize() * frs.size();
    return Display::resize(winsz);
}

MainScreen::MainScreen(const std::string & title, int fsz, const char* extfont)
    : DisplayInit(title, minimalTerminalSize(), fsz, extfont), SWE::FullTerminal(DisplayInit::frs), fontsz(fsz), buttons(0, *this)
{
    *this << fill::defaults(defaultColors(), 0x20, defaultProperty()) << cursor::set(0, 0);

    buttons.addLabel(new TermGUI::TextButton("<T>est Button1", 1025, *this));
    buttons.addLabel(new TermGUI::TextButton("T<e>st Button2", 1026, *this));
    buttons.addLabel(new TermGUI::TextButton("Te<s>t Button3", 1027, *this));

    setButtonsPosition();
    setVisible(true);
}

void MainScreen::setButtonsPosition(void)
{
    size_t width = buttons.cols() + (buttons.count() - 1);
    TermPos pos((cols() - width) / 2, rows() - 2);

    for(auto & ptr : buttons)
    {
        if(ptr)
        {
            ptr->setTermPos(pos);
            ptr->setVisible(true);
            pos += TermSize(ptr->cols() + 1, 0);
        }
    }
}

SWE::TermSize MainScreen::minimalTerminalSize(void) const
{
    return SWE::TermSize(80, 25);
}

SWE::CharsetProperty MainScreen::defaultProperty(void) const
{
    return SWE::CharsetProperty(SWE::RenderBlended, SWE::StyleNormal, SWE::HintingNormal);
}

SWE::FBColors MainScreen::defaultColors(void) const
{
    return SWE::FBColors(SWE::Color::Silver, SWE::Color::Black);
}

MainScreen & MainScreen::init(const std::string & title, const char* extfont)
{
    static MainScreen main(title, 14, extfont);
    return main;
}

void MainScreen::renderWindow(void)
{
    *this << reset::defaults() <<
	"SWE::TermWindow Examples: ";

    if(ext)
        *this << "use external font: " << ext;
    else
        *this << "use internal font: " << "terminus";

    *this << set::rn();

    *this << "simple std::string" << set::rn();
    *this << set::colors(Color::DarkRed, Color::RoyalBlue) << "simple" << reset::colors() << " " <<
	set::colors(Color::Red, Color::Blue) << "color" << reset::colors() << " " <<
	set::colors(Color::IndianRed, Color::DarkBlue) << "string" << reset::colors() << set::rn();

    *this << UnicodeString("simple unicode: Привет Бармалей") << set::rn();
    *this << UnicodeString("color unicode: ") << 
	set::colors(Color::Yellow, Color::Navy) << UnicodeString("Привет") << reset::colors() << " " <<
	set::colors(Color::Moccasin, Color::Maroon) << UnicodeString("Бармалей") << reset::colors() << set::rn();

    SWE::CharsetProperty defProperty = defaultProperty();

    *this << "test property render: ";
    *this << set::property(RenderBlended, StyleNormal, HintingNormal) << "RenderBlended" << reset::property() << " ";
    *this << set::property(RenderSolid, StyleNormal, HintingNormal) << "RenderSolid" << reset::property() << set::rn();
    *this << "test property style: ";
    *this << set::property(defProperty.render(), StyleBold, defProperty.hinting()) << "StyleBold" << reset::property() << " ";
    *this << set::property(defProperty.render(), StyleItalic, defProperty.hinting()) << "StyleItalic" << reset::property() << " ";
    *this << set::property(defProperty.render(), StyleUnderLine, defProperty.hinting()) << "StyleUnderLine" << reset::property() << " ";
    *this << set::property(defProperty.render(), StyleStrikeThrough, defProperty.hinting()) << "StyleStrikeThrough" << reset::property() << set::rn();
    *this << "test property hinting: ";
    *this << set::property(defProperty.render(), defProperty.style(), HintingNormal) << "HintingNormal" << reset::property() << " ";
    *this << set::property(defProperty.render(), defProperty.style(), HintingLight) << "HintingLight" << reset::property() << " ";
    *this << set::property(defProperty.render(), defProperty.style(), HintingMono) << "HintingMono" << reset::property() << " ";
    *this << set::property(defProperty.render(), defProperty.style(), HintingNone) << "HintingNone" << reset::property() << set::rn();

    UCString ucs(FBColors(Color::Silver, Color::Black));
    ucs << UnicodeColor(L'Б', Color::IndianRed) <<
	UnicodeColor(L'а', Color::Pink) <<
	UnicodeColor(L'р', Color::Lavender) <<
	UnicodeColor(L'м', Color::Amethyst) <<
	UnicodeColor(L'а', Color::LightGreen) <<
	UnicodeColor(L'л', Color::Aquamarine) <<
	UnicodeColor(L'е', Color::CornflowerBlue) <<
	UnicodeColor(L'й', Color::Seashell);

    *this << "simple UCString: " << ucs << set::rn();
    
    *this << "UCString::parseUnicode: " << set::fgcolor(Color::ForestGreen) << "\"test [color:red]red\"" << reset::fgcolor() << set::rn();
    *this << "result: " << UCString::parseUnicode("test [color:red]red", FBColors(Color::Silver)) << set::rn();

    *this << "UCString::parseUnicode: " << set::fgcolor(Color::ForestGreen) << "\"test [color:red]red[color:default] default\"" << reset::fgcolor() << set::rn();
    *this << "result: " << UCString::parseUnicode("test [color:red]red[color:default] default", FBColors(Color::Silver)) << set::rn();

    *this << "UCString::parseUnicode: " << set::fgcolor(Color::ForestGreen) << "\"test [color:red]red[color:default] [color:yellow:red]yellow_red\"" << reset::fgcolor() << set::rn();
    *this << "result: " << UCString::parseUnicode("test [color:red]red[color:default] [color:yellow:red]yellow_red", FBColors(Color::Silver)) << set::rn();

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
}

bool MainScreen::keyPressEvent(const KeySym & key)
{
    if(key.keycode() == Key::LEFT)
    {
        if(buttons.findSelected())
        {
            buttons.prevSelected();
            return true;
        }
    }
    else
    if(key.keycode() == Key::RIGHT)
    {
	if(buttons.findSelected())
        {
            buttons.nextSelected();
            return true;
        }
    }
    else
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
	auto list = { "test 123", "test 234", "test 345", "test 456", "test 567", "test 678", "test 789" };
	TermGUI::ListBox msg("Select Value", list, 5, *this);
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

bool MainScreen::mouseClickEvent(const SWE::ButtonsEvent & be)
{
    const TermCharset* ch = charset(gfx2sym(be.press().position()));
    if(ch)
    {
        auto & uc = ch->charset();
        auto & prop = ch->property();

	VERBOSE("charset info: " << SWE::StringFormat("unicode: %1, fgcolor: %2, bgcolor: %3, blend: %4, style: %5, hinting: %6").
			arg(SWE::String::hex(uc.unicode(), 4)).arg(uc.fgcolor().toString()).arg(uc.bgcolor().toString()).
			arg(prop.render()).arg(SWE::String::hex(prop.style(), 2)).arg(prop.hinting()));
    }

    return true;
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
	case 1025:
	    keyPressEvent(Key::F1);
	    return true;
	case 1026:
	    keyPressEvent(Key::F2);
	    return true;
	case 1027:
	    keyPressEvent(Key::F3);
	    return true;
	default: break;
    }

    return false;
}

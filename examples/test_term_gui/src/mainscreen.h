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

#ifndef _MAINSCREEN_H_
#define _MAINSCREEN_H_

#include "libswe.h"

class DisplayInit
{
protected:
    SWE::BinaryBuf		ttf;
    SWE::FontRenderTTF		frt;
    const char*                 ext;

public:
    DisplayInit(const std::string &, const SWE::TermSize &, int fsz, const char* extfont);
};

class MainScreen : protected DisplayInit, public SWE::FullTerminal
{
    int				fontsz;
    int				renderContent(const SWE::UnicodeList &, int, const SWE::Point &);
    SWE::TermGUI::ButtonsGroup	buttons;

protected:
    bool			keyPressEvent(const SWE::KeySym &) override;
    bool			scrollUpEvent(void) override;
    bool			scrollDownEvent(void) override;
    bool			userEvent(int act, void* data) override;
    bool			mouseClickEvent(const SWE::ButtonsEvent &) override;
    SWE::CharRender	        defaultProperty(void) const override;
    SWE::FBColors		defaultColors(void) const override;
    SWE::TermSize      		minimalTerminalSize(void) const override;

    void			setButtonsPosition(void);
    bool                        setFontSize(int, const SWE::TermSize &);
    MainScreen(const std::string &, int, const char* extfont);

public:
    static MainScreen & init(const std::string &, const char* extfont);
    void			renderWindow(void);
};

#endif

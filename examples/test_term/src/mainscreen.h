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
    SWE::Size		termsz;
    SWE::BinaryBuf	ttf;
    SWE::FontRenderTTF	frs;

public:
    DisplayInit(const std::string &, const SWE::Size &, int);

    bool setFontSize(int, const SWE::TermSize &);
};

class MainScreen : protected DisplayInit, public SWE::TermWindow
{
    int			fontsz;
    int			renderContent(const SWE::UnicodeList &, int, const SWE::Point &);
    std::vector<SWE::Window*> labels;

protected:
    bool		keyPressEvent(const SWE::KeySym &) override;
    bool		scrollUpEvent(void) override;
    bool		scrollDownEvent(void) override;
    void		displayResizeEvent(const SWE::Size &, bool) override;
    bool		userEvent(int act, void* data) override;

    MainScreen(const std::string &, int);
    ~MainScreen();

public:
    static MainScreen & init(const std::string &);
    void		renderWindow(void);
};

#endif

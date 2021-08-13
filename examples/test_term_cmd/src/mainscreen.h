/***************************************************************************
 *   Copyright (C) 2018 by public.irkutsk@gmail.com                        *
 *                                                                         *
 *   Part of the SWE (SDL Window Engine) examples:                         *
 *   https://github.com/AndreyBarmaley/sdl-window-engine.git               *
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

#ifndef _CNA_MAINSCREEN_
#define _CNA_MAINSCREEN_

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "settings.h"


class FileInfo : protected std::pair<std::string, struct stat>
{
    std::shared_ptr<FileInfo> link;

public:
    FileInfo(const std::string &, const struct stat &);

    const std::string & path(void) const { return first; }
    std::string name(void) const { return Systems::basename(first); }

    bool isName(const std::string & str) const { return str == name(); }
    bool isUpDir(void) const;
    bool isDirectory(void) const;
    bool isFile(void) const;
    bool isLink(void) const;

    bool checkAccess(int) const;
    bool checkMode(int) const;

    size_t fileSize(void) const;
    const FileInfo* linkRef(void) const { return link.get(); }

    std::pair<int, ColorIndex> typeMarker(void) const;
};

class FontRenderInit
{
protected:
    BinaryBuf		ttf;
    FontRenderTTF	frt;

public:
    FontRenderInit(const std::string &, const TermSize &, int);
};

class TermPanel : public TermBase
{
    std::string		currentDirectory;
    TermRect		leftArea;
    TermRect		rightArea;
    std::list<FileInfo>	filesList;
    int			cursorPos;
    int			skipList;
    bool 		focus;

    void		readCurrentDirectory(const std::string &);

    bool		keyPressReturn(void);
    bool		keyPressHome(void);
    bool		keyPressEnd(void);
    bool		keyPressPageUp(void);
    bool		keyPressPageDown(void);
    bool		keyPressLeft(void);
    bool		keyPressRight(void);
    bool		keyPressUp(void);
    bool		keyPressDown(void);

protected:
    bool		keyPressEvent(const KeySym &) override;
    void		windowResizeEvent(const Size &) override;

public:
    TermPanel(TermWindow*);

    void		renderWindow(void) override;

    void		setFocus(bool f) { focus = f; }
    bool		isFocused(void) const { return focus; }
    void                setCharset(int ch, const ColorIndex & fg = Color::Transparent, const ColorIndex & bg = Color::Transparent, const CharProperty* prop = nullptr) override;
    void		renderFlush(void) override;
    const FontRender*   frs(void) const override;
};

class MainScreen : protected FontRenderInit, public FullTerminal
{
    int			fontsz;
    int			renderContent(const UnicodeList &, int, const Point &);
    TermPanel		leftPanel, rightPanel;

protected:
    bool		keyPressEvent(const KeySym &) override;
    bool		scrollUpEvent(void) override;
    bool		scrollDownEvent(void) override;
    bool		userEvent(int act, void* data) override;
    void		renderPresentEvent(u32 ms) override;

    SWE::CharProperty   defaultProperty(void) const override;
    SWE::FBColors       defaultColors(void) const override;
    SWE::TermSize       minimalTerminalSize(void) const override;
    void                terminalResizeEvent(void) override;


    bool                setFontSize(int, const TermSize &);
    void		panelsPositions(void);

    MainScreen(const std::string &, int);

public:
    static MainScreen & init(const std::string &);
    void		renderWindow(void) override;
};

#endif

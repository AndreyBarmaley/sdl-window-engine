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

#include <algorithm>

#include "settings.h"
#include "mainscreen.h"
#include "default_font.h"

FileInfo::FileInfo(const std::string & path, const struct stat & st) : std::pair<std::string, struct stat>(path, st)
{
    if(S_ISLNK(second.st_mode))
    {
	char buf[PATH_MAX];
	bzero(buf, PATH_MAX);

	int res = readlink(path.c_str(), buf, PATH_MAX - 1);
	if(0 < res)
	{
	    struct stat st;

	    if(0 == lstat(buf, & st))
        	link = std::make_shared<FileInfo>(buf, st);
	}
    }
}

bool FileInfo::isUpDir(void) const
{
    return isDirectory() && name() == "..";
}

bool FileInfo::isDirectory(void) const
{
    if(isLink())
    {
	const FileInfo* ref = linkRef();
	return ref ? ref->isDirectory() : false;
    }

    return S_ISDIR(second.st_mode);
}

bool FileInfo::isLink(void) const
{
    return S_ISLNK(second.st_mode);
}

bool FileInfo::isFile(void) const
{
    return S_ISREG(second.st_mode);
}

bool FileInfo::checkAccess(int mode) const
{
    return 0 == access(first.c_str(), mode);
}

bool FileInfo::checkMode(int mode) const
{
    return second.st_mode & mode;
}

size_t FileInfo::fileSize(void) const
{
    return second.st_size;
}

std::pair<int, ColorIndex> FileInfo::typeMarker(void) const
{
    // link
    if(isLink())
    {
	const FileInfo* ref = linkRef();
	if(! ref)
    	    return std::make_pair<int, ColorIndex>('!', Color::IndianRed);

    	return ref->isDirectory() ? std::make_pair<int, ColorIndex>('~', Color::White) :
				    std::make_pair<int, ColorIndex>('@', Color::SlateGray);
    }

    // dir
    if(isDirectory())
	return std::make_pair<int, ColorIndex>('/', Color::White);

    // executable
    if(checkAccess(X_OK))
	return std::make_pair<int, ColorIndex>('*', Color::DeepSkyBlue);

    return std::make_pair<int, ColorIndex>(0x20, Color::Silver);
}

bool SortFileInfoPred(const FileInfo & fn1, const FileInfo & fn2)
{
    if(fn1.isDirectory() && fn2.isDirectory())
	return fn1.name() < fn2.name();

    if(fn1.isDirectory())
	return true;

    if(fn2.isDirectory())
	return false;

    return fn1.name() < fn2.name();
};

/* FontRenderInit */
FontRenderInit::FontRenderInit(const std::string & title, const TermSize & termsz, int fsz)
{
    ttf = BinaryBuf(_default_ttf_h.data, sizeof(_default_ttf_h.data)).zlibUncompress(_default_ttf_h.size);
    frt = FontRenderTTF(ttf, fsz, false);

    const Size defsz = termsz.toSize() * frt.size();

    bool fullscreen = false;
    bool accel = false;
    bool resized = true;

    if(! Display::init(title, defsz, defsz, fullscreen, accel, resized))
	    Engine::except(__FUNCTION__, "display init error");
}

bool FontRenderInit::setFontSize(int fsz, const TermSize & termsz)
{
    frt = FontRenderTTF(ttf, fsz, false);
    const Size winsz = termsz.toSize() * frt.size();
    return Display::resize(winsz);
}

UnicodeString ShrinkFileName(const std::string & name, int smax)
{
    UnicodeString res = name;

    if(res.size() > smax)
    {
	int rem = res.size() - (smax + 1);
	int left = (res.size() - rem) / 2;
	int right = smax - (left + 1);

        return res.substr(0, left).append("~").append(res.substr(res.size() - right));
    }
    else
    if(res.size() < smax)
    {
	// res = name;
	res.append(std::string(smax - res.size(), 0x20));
    }

    return res;
}

/* TermPanel */
TermPanel::TermPanel(TermWindow & term) : TermArea(& term), cursorPos(0), skipList(0), focus(false)
{
    resetState(FlagModality);
    setState(FlagKeyHandle);

    const char* home = Systems::environment("HOME");
    readCurrentDirectory(home ? home : "/");

    *this << reset::defaults() <<
	fill::defaults(Color::Silver, Color::RoyalBlue);
}

void TermPanel::readCurrentDirectory(const std::string & path)
{
    currentDirectory = path;    
    StringList filesNames = Systems::readDir(currentDirectory, false);

    filesList.clear();
    struct stat st;

    for(auto it = filesNames.begin(); it != filesNames.end(); ++it)
    {
	if(0 == (*it).compare(".")) continue;

	std::string path = Systems::concatePath(currentDirectory, *it);

	if(0 > lstat(path.c_str(), & st))
	{
            ERROR("error stat for:" << path);
	}
	else
	    filesList.push_back(FileInfo(path, st));
    }

    filesList.sort(SortFileInfoPred);
}

void TermPanel::renderWindow(void)
{
    const ColorIndex defBackground = Color::Navy;

    *this << reset::defaults() <<
	fill::defaults(Color::Silver, defBackground);

    // rect
    *this << cursor::set(0, 0) << draw::rect(0, 0, cols(), rows(), LineThin);
    // vertical line
    *this << cursor::set(rightArea.posx() - 1, 1) << draw::vline(leftArea.rows(), acs::vline(LineThin));

    auto it = filesList.begin();
    std::advance(it, skipList);
    int cury = 0;

    // left column
    cury = leftArea.posy();
    for(; it != filesList.end(); ++it)
    {
	std::pair<int, ColorIndex> marker = (*it).typeMarker();
	*this << cursor::set(leftArea.posx(), cury);

	if(isFocused() && cursorPos == std::distance(filesList.begin(), it))
	    *this << set::colors(Color::Black, Color::GreenYellow);
	else
	    *this << set::colors(marker.second, defBackground);

	*this << std::string(1, marker.first) << ShrinkFileName((*it).name(), leftArea.cols()) << set::rn();

	if(++cury > leftArea.rows()) break;
    }

    if(it != filesList.end())
    {
	// right column
	cury = rightArea.posy();
	for(++it; it != filesList.end(); ++it)
	{
	    std::pair<int, ColorIndex> marker = (*it).typeMarker();
	    *this << cursor::set(rightArea.posx(), cury);

	    if(isFocused() && cursorPos == std::distance(filesList.begin(), it))
		*this << set::colors(Color::Black, Color::GreenYellow);
	    else
		*this << set::colors(marker.second, defBackground);

	    *this << std::string(1, marker.first) << ShrinkFileName((*it).name(), rightArea.cols()) << set::rn();

	    if(++cury > rightArea.rows()) break;
	}
    }

    // file info area
    *this << set::colors(Color::Silver, defBackground) <<
	cursor::set(0, leftArea.rows() + 1) << 
	acs::ltee(LineThin) << draw::hline(cols() - 2, acs::hline(LineThin)) << acs::rtee(LineThin);

    it = filesList.begin();
    std::advance(it, cursorPos);
    if(it != filesList.end())
    {
	const FileInfo* ref = (*it).linkRef();

	if(ref)
	{
	    *this << cursor::set(2, leftArea.rows() + 2) << ShrinkFileName((*it).isUpDir() ? _("-UPDIR-") : ref->path(), cols() - 4);
	}
	else
	{
	    *this << cursor::set(2, leftArea.rows() + 2) << ShrinkFileName((*it).isUpDir() ? _("-UPDIR-") : (*it).name(), cols() - 4 - 10 - 9);

	    *this << cursor::set(cols() - 2 - 10, leftArea.rows() + 2) <<
		((*it).isDirectory() ? "d" : "-") <<
		((*it).checkMode(S_IRUSR) ? "r" : "-") << ((*it).checkMode(S_IWUSR) ? "w" : "-") << ((*it).checkMode(S_IXUSR) ? "x" : "-") <<
		((*it).checkMode(S_IRGRP) ? "r" : "-") << ((*it).checkMode(S_IWGRP) ? "w" : "-") << ((*it).checkMode(S_IXGRP) ? "x" : "-") <<
		((*it).checkMode(S_IROTH) ? "r" : "-") << ((*it).checkMode(S_IWOTH) ? "w" : "-") << ((*it).checkMode(S_IXOTH) ? "x" : "-");

	    size_t sz = (*it).fileSize();
	    int suffix = 0;

	    if(sz > 9999999)
	    {
		suffix = 'K';
		sz >>= 10;
	    }
	    if(sz > 9999999)
	    {
		suffix = 'M';
		sz >>= 10;
	    }
	    if(sz > 9999999)
	    {
		suffix = 'G';
		sz >>= 10;
	    }
	    std::string numstr = String::number(sz);
	    if(suffix)
		*this << cursor::set(cols() - 2 - 12 - numstr.size(), leftArea.rows() + 2) << numstr << suffix;
	    else
		*this << cursor::set(cols() - 2 - 11 - numstr.size(), leftArea.rows() + 2) << numstr;
	}
    }
    *this << set::flush();
}

bool TermPanel::keyPressReturn(void)
{
    auto it = filesList.begin();
    std::advance(it, cursorPos);

    if(it != filesList.end())
    {
	if((*it).isUpDir())
	{
	    std::string name = Systems::basename(currentDirectory);
	    readCurrentDirectory(Systems::dirname(currentDirectory));

	    // select prev name
	    auto it = std::find_if(filesList.begin(), filesList.end(), std::bind2nd(std::mem_fun_ref(&FileInfo::isName), name));
	    cursorPos = it != filesList.end() ? std::distance(filesList.begin(), it) : 0;

	    //
	    skipList = 0;
	    if(leftArea.rows() * 2 < filesList.size())
	    {
		skipList = cursorPos - leftArea.rows() / 2;
		if(skipList < 0) skipList = 0;
	    }
	    renderWindow();
	}
	else
	if((*it).isDirectory() && (*it).checkAccess(R_OK))
	{
	    readCurrentDirectory((*it).path());
	    cursorPos = 0;
	    skipList = 0;
	    renderWindow();
	}
    }
    return true;
}

bool TermPanel::keyPressHome(void)
{
    cursorPos = 0;
    skipList = 0;
    renderWindow();
    return true;
}

bool TermPanel::keyPressEnd(void)
{
    cursorPos = filesList.size() - 1;
    skipList = filesList.size() - leftArea.rows() * 2;
    renderWindow();
    return true;
}

bool TermPanel::keyPressPageUp(void)
{
    if(0 < cursorPos)
    {
	cursorPos -= leftArea.rows() * 2;
	if(0 > cursorPos) cursorPos = 0;
	if(cursorPos < skipList)
	{
	    skipList = cursorPos - leftArea.rows() * 2 + leftArea.rows() + 1;
	    if(skipList < 0) skipList = 0;
	}
	renderWindow();
    }
    return true;
}
	
bool TermPanel::keyPressPageDown(void)
{
    if(cursorPos < filesList.size() - 1)
    {
	cursorPos += leftArea.rows() * 2;
	if(cursorPos >= filesList.size()) cursorPos = filesList.size() - 1;
	if(cursorPos > skipList + leftArea.rows() * 2 - 1)
	{
	    skipList = cursorPos;
	    if(skipList + leftArea.rows() * 2 >= filesList.size())
		skipList = filesList.size() - leftArea.rows() * 2;
	}
	renderWindow();
    }
    return true;
}

bool TermPanel::keyPressLeft(void)
{
    if(0 < cursorPos)
    {
	cursorPos -= leftArea.rows();
	if(0 > cursorPos) cursorPos = 0;
	if(cursorPos < skipList)
	{
	    skipList = cursorPos - leftArea.rows() * 2 + leftArea.rows() + 1;
	    if(skipList < 0) skipList = 0;
	}
	renderWindow();
    }
    return true;
}

bool TermPanel::keyPressRight(void)
{
    if(cursorPos < filesList.size() - 1)
    {
	cursorPos += leftArea.rows();
	if(cursorPos >= filesList.size()) cursorPos = filesList.size() - 1;
	if(cursorPos > skipList + leftArea.rows() * 2 - 1)
	{
	    skipList = cursorPos - leftArea.rows() * 2 + leftArea.rows();
	    if(skipList + leftArea.rows() * 2 >= filesList.size())
		skipList = filesList.size() - leftArea.rows() * 2;
	}
	renderWindow();
    }
    return true;
}

bool TermPanel::keyPressUp(void)
{
    if(0 < cursorPos)
    {
	cursorPos--;
	if(cursorPos < skipList)
	{
	    skipList = cursorPos - leftArea.rows() * 2 + leftArea.rows() + 1;
	    if(skipList < 0) skipList = 0;
	}
	renderWindow();
    }
    return true;
}

bool TermPanel::keyPressDown(void)
{
    if(cursorPos < filesList.size() - 1)
    {
	cursorPos++;
	if(cursorPos > skipList + leftArea.rows() * 2 - 1)
	{
	    skipList = cursorPos - leftArea.rows() * 2 + leftArea.rows();
	    if(skipList + leftArea.rows() * 2 >= filesList.size())
		skipList = filesList.size() - leftArea.rows() * 2;
	}
	renderWindow();
    }
    return true;
}

bool TermPanel::keyPressEvent(const KeySym & key)
{
    // not focus
    if(! isFocused())
	return false;

    switch(key.keycode())
    {
	case Key::RETURN:	return keyPressReturn();
	case Key::HOME:		return keyPressHome();
	case Key::END:		return keyPressEnd();
	case Key::PAGEUP:	return keyPressPageUp();
	case Key::PAGEDOWN:	return keyPressPageDown();
	case Key::LEFT:		return keyPressLeft();
	case Key::RIGHT:	return keyPressRight();
	case Key::UP:		return keyPressUp();
	case Key::DOWN:		return keyPressDown();

	default: break;
    }

    return false;
}

void TermPanel::windowResizeEvent(const Size & sz)
{
    leftArea = TermRect(1, 1, cols() / 2 - 2, rows() - 4);
    rightArea = TermRect(leftArea.cols() + 3, 1, cols() - leftArea.cols() - 5, rows() - 4);
}

/* MenuBar */
MenuBar::MenuBar(TermWindow & term) : TermArea(& term)
{
    setState(FlagModality);
    setVisible(false);
}

void MenuBar::renderWindow(void)
{
    *this << reset::defaults() <<
	fill::defaults(Color::White, Color::LightSeaGreen);
}

/* MainScreen */
MainScreen::MainScreen(const std::string & title, int fsz)
    : FontRenderInit(title, minimalTerminalSize(), fsz), fontsz(fsz), left(*this), right(*this)//, menu(*this)
{
    setFontRender(FontRenderInit::frt);

    *this << reset::defaults() <<
        fill::defaults(Color::Silver, Color::Black);

    panelsPositions();
    left.setFocus(true);
    right.setFocus(false);

    VERBOSE("you can change the window size and font size, also use mouse scroll.");
    setVisible(true);
}

void MainScreen::panelsPositions(void)
{
    int panw = cols() / 2;

    left.setTermArea(0, 0, panw, rows());
    right.setTermArea(panw, 0, panw, rows());
}

MainScreen & MainScreen::init(const std::string & title)
{
    static MainScreen main(title, 14);
    return main;
}

void MainScreen::renderWindow(void)
{
    left.renderWindow();
    right.renderWindow();
    // flush
    *this << set::flush();
}

bool MainScreen::keyPressEvent(const KeySym & key)
{
    if(key.keycode() == Key::F10 || key.keycode() == Key::ESCAPE)
    {
	TermGUI::MessageBox msg("Message Box", "Exit from my super program?\nPlease selected....", 
				TermGUI::ButtonOk | TermGUI::ButtonCancel, *this);
	if(TermGUI::ButtonOk == msg.exec())
	    setVisible(false);
	return true;
    }
    else
    if(key.keycode() == Key::TAB)
    {
	left.setFocus(! left.isFocused());
    	right.setFocus(! right.isFocused());

	renderWindow();
    }
    else
    if(key.keycode() == Key::ESCAPE)
    {
	// frs()->dumpCache();
    }

    return false;
}

bool MainScreen::scrollUpEvent(void)
{
    if(12 < fontsz)
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
	default: break;
    }

    return false;
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

void MainScreen::terminalResizeEvent(void)
{
	panelsPositions();
}

void MainScreen::renderPresentEvent(u32 ms)
{
    VERBOSE("render " << chars.size() << " tiles, for " << ms << " ms");
}

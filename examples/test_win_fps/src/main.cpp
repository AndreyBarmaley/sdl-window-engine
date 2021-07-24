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

#include <list>
#include "libswe.h"

using namespace SWE;

class ColorRectWindow : public Window
{
    Color	bg;

public:
    ColorRectWindow(const Color & col, const Rect & rt, Window* parent) : Window(rt.toPoint(), rt.toSize(), parent), bg(col)
    {
	setModality(false);
	setVisible(true);
    }

    void setColor(const Color & col)
    {
	bg = col;
    }

    void renderWindow(void)
    {
	renderClear(bg);
        if(isFocused())
            renderRect(SWE::Color::Red, rect());
    }

    static std::pair<Color, Rect> rand(const Size & wsz)
    {
	std::pair<Color, Rect> res;
	res.first = Color(Tools::rand(Color::Black, Color::White));
	res.second = Rect(Tools::rand(0, wsz.w - 1), Tools::rand(0, wsz.h - 1), 32, 32);
	return res;
    }
};

class MainWindow : public DisplayWindow
{
    std::string			fps;
    std::list< std::unique_ptr<ColorRectWindow> >
				wins;

public:
    MainWindow(const Color & col, const std::string & file2, int ncounts) : DisplayWindow(col)
    {
	Display::hardwareCursorHide();
	Display::renderCursor(Display::createTexture(file2));

	for(int it = 0; it < ncounts; ++it)
	{
	    auto pair = ColorRectWindow::rand(size());
	    auto ptr = new ColorRectWindow(pair.first, pair.second, this);
	    wins.emplace_back(ptr);
	}
    }

    ~MainWindow()
    {
	Display::hardwareCursorShow();
    }

    void renderWindow(void)
    {
    }

    void renderForeground(void)
    {
	renderText(systemFont(), fps, Color::Black, Point(width() - 10, height() - 10), AlignRight, AlignBottom);
	//VERBOSE(fps);
    }

    bool keyPressEvent(const KeySym & key)
    {
	switch(key.keycode())
	{
	    case Key::ESCAPE:
		setVisible(false);
		return true;

	    default: break;
	}

	return false;
    }

    void tickEvent(u32 ms)
    {
	wins.pop_front();
	auto pair = ColorRectWindow::rand(size());
	auto ptr = new ColorRectWindow(pair.first, pair.second, this);
	wins.emplace_back(ptr);

	// max performance
	// see SWE::DisplayScene::handleWhileVisible
	setDirty(true);
    }

    void renderPresentEvent(u32 ms)
    {
	fps = SWE::String::number(1000 / static_cast<double>(ms), 2).append(" fps");
    }

/*
    ALL EVENTS:
    void        windowMoveEvent(const Point &) {}
    void        windowResizeEvent(const Size &) {}
    void        windowVisibleEvent(bool) {}
    void        windowCreateEvent(void) {}
    void        windowDestroyEvent(void) {}
    bool        keyDebugEvent(const SDL_KeyboardEvent &) { return false; }
    bool        keyPressEvent(const KeySym &) { return false; }
    bool        keyReleaseEvent(+const KeySym &) { return false; }
    bool        textInputEvent(const std::string &) { return false; }
    bool        mousePressEvent(const ButtonEvent &) { return false; }
    bool        mouseReleaseEvent(const ButtonEvent &) { return false; }
    bool        mouseClickEvent(const ButtonsEvent &) { return false; }
    void        mouseFocusEvent(void) {}
    void        mouseLeaveEvent(void) {}
    bool        mouseMotionEvent(const Point &, u32 buttons) { return false; }
    bool        userEvent(int, void*) { return false; }
    bool        scrollUpEvent(void) { return false; }
    bool        scrollDownEvent(void) { return false; }
    void        tickEvent(u32 ms) {}
    void        renderPresentEvent(u32 ms) {}
    void        displayResizeEvent(const Size &, bool) {}
    void        displayFocusEvent(bool gain) {}
*/
};

class MainClient
{
public:
    MainClient(int argc, char** argv)
    {
    }

    bool exec(void)
    {
	if(! Engine::init())
    	    return EXIT_FAILURE;

	const int ncounts = 1000;
	const std::string title = StringFormat("SWE: TestWinFPS(%1 Windows): %2").arg(ncounts).arg(Engine::version());

	const Size render(800, 600);
	const Size geometry(800, 600);

	if(! Display::init(title, geometry, render, false, true, false))
    	    return false;

	MainWindow(Color::Navy, "cursor16.png", ncounts).exec();

	Engine::quit();
	return true;
    }
};

int main(int argc, char** argv)
{
    Systems::setLocale(LC_ALL, "");

    try
    {
        MainClient client(argc, argv);

        if(! client.exec())
            return EXIT_FAILURE;
    }
    catch(Engine::exception &)
    {
    }

    return EXIT_SUCCESS;
}

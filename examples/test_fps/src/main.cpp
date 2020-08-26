#include <list>
#include "libswe.h"

using namespace SWE;

struct ColorRect : std::pair<Color, Rect>
{
    ColorRect() {}

    static ColorRect rand(const Size & wsz)
    {
	ColorRect res;
	res.first = Color(Tools::rand(Color::Black, Color::White));
	res.second = Rect(Tools::rand(0, wsz.w - 1), Tools::rand(0, wsz.h - 1), 32, 32);
	return res;
    }
};

class MainWindow : public DisplayWindow
{
    std::string		fps;
    std::list<ColorRect>rects;

public:
    MainWindow(const Color & col, const std::string & file2) : DisplayWindow(col)
    {
	Display::hardwareCursorHide();
	Display::renderCursor(Display::createTexture(file2));

	for(int it = 0; it < 5000; ++it)
	    rects.emplace_back(ColorRect::rand(size()));

    }

    ~MainWindow()
    {
	Display::hardwareCursorShow();
    }

    void renderWindow(void)
    {
	for(auto & rtcol : rects)
	    renderColor(rtcol.first, rtcol.second);
    }

    void renderForeground(void)
    {
	renderText(systemFont(), fps, Color::Black, Point(width() - 10, height() - 10), AlignRight, AlignBottom);
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
	rects.pop_front();
	rects.emplace_back(ColorRect::rand(size()));

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

	const std::string title = StringFormat("SWE: TestFPS(5000 Rects): %1").arg(Engine::version());

	const Size render(800, 600);
	const Size geometry(800, 600);

	if(! Display::init(title, geometry, render, false, true, false))
    	    return false;

	MainWindow(Color::Navy, "cursor16.png").exec();

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

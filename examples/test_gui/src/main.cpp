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

#include "listitems.h"

using namespace SWE;

class SimpleScrollBar : public ScrollBar
{
public:
    SimpleScrollBar(const Rect & pos, ListWidget & win) : ScrollBar(true, win)
    {
	setPosition(pos);
	setSize(pos);

	const Size & srt = scrollArea();
	int sz = srt.w < srt.h ? srt.w : srt.h;
	setTextureCursor(Display::renderRect(Color::Lime, Color::Teal, Size(sz, sz)));

	setVisible(true);
    }

    Rect scrollArea(void) const override
    {
	return rect() + Point(1,1) - Size(2,2);
    }

    void renderWindow(void) override
    {
	renderClear(Color::Black);
	renderRect(Color::Tomato, rect());
	ScrollBar::renderWindow();
    }
};

class SimpleListItem : public ListWidgetItem
{
    Texture		colorTexture;

public:
    SimpleListItem(int col, ListWidget & win) : ListWidgetItem(win)
    {
	setSize(Size(200, 25));
	setPosition(Point(0, 0));

	if(col < 0)
	{
	    const Color color(Color::Black);
    	    colorTexture = Display::createTexture(size());
	    Display::renderClear(color, colorTexture);

	    const std::string & colorName = "SWE::ListWidget";
	    const FontRender & frs = FontRenderSystem();

	    Texture text = Display::renderText(frs, colorName, Color::Red);
	    Display::renderTexture(text, text.rect(), colorTexture, Rect((size() - text.size()) / 2, text.size()));
	}
	else
	{
	    Color color = ColorIndex(col).toColor();
    	    colorTexture = Display::createTexture(size());
	    Display::renderClear(color, colorTexture);

	    const std::string & colorName = color.toString();
	    const FontRender & frs = FontRenderSystem();

	    Texture text = Display::renderText(frs, colorName, Color::Black);
	    Display::renderTexture(text, text.rect(), colorTexture, Rect((size() - text.size()) / 2, text.size()));

	    setToolTip(colorName);
	}

	setVisible(true);
    }

    void renderWindow(void)
    {
	renderTexture(colorTexture, Point(0, 0));

	if(isSelected())
	    renderRect(Color::Yellow, rect());
    }
};

class SimpleListBox : public ListWidget
{
    SimpleScrollBar	scrollBar;

public:
    SimpleListBox(const Rect & pos, Window & win) : ListWidget(pos.toPoint(), pos.toSize(), true, & win),
	scrollBar(Rect(pos.w + 1, 0, 20, pos.h), *this)
    {
	//setPosition(pos);
	addItem(new SimpleListItem(-1, *this));

	for(int col = Color::Black + 1; col < Color::Transparent; ++col)
	    addItem(new SimpleListItem(col, *this));

	setVisible(true);
    }

protected:
};

class SimpleTextButton : public WindowButton
{
    Texture tx1;
    Texture tx2;
    Texture tx3;

public:
    SimpleTextButton(const Point & pos, const std::string & text, Window & win) : WindowButton(& win)
    {
        const FontRender & frs = FontRenderSystem();

        Texture textBlack = Display::renderText(frs, text, Color::Black);
        Texture textYellow = Display::renderText(frs, text, Color::Yellow);
        const int space = 5;

        tx1 = Display::renderRect(Color::Black, Color::Silver, textBlack.size() + Size(space, space) * 2, 1);
        tx2 = Display::renderRect(Color::Black, Color::DimGray, textYellow.size() + Size(space, space) * 2, 1);
        tx3 = Display::renderRect(Color::Black, Color::Silver, textYellow.size() + Size(space, space) * 2, 1);

        Display::renderTexture(textBlack, textBlack.rect(), tx1, textBlack.rect() + Point(space, space));
        Display::renderTexture(textYellow, textYellow.rect(), tx2, textYellow.rect() + Point(space, space));
        Display::renderTexture(textYellow, textYellow.rect(), tx3, textYellow.rect() + Point(space, space));

        setSize(tx1.size());
	setPosition(pos);
    }

protected:
    const Texture* textureFocused(void) const
    {
        return &tx3;
    }

    const Texture* textureDisabled(void) const
    {
        return &tx2;
    }

    const Texture* texturePressed(void) const
    {
        return &tx2;
    }

    const Texture* textureReleased(void) const
    {
        return &tx1;
    }
};

class SimpleCheckBox : public WindowCheckBox
{
    Texture tx1;
    Texture tx2;

public:
    SimpleCheckBox(const Point & pos, const std::string & text, Window & win) : WindowCheckBox(& win)
    {
        const FontRender & frs = FontRenderSystem();

        Texture textBlack = Display::renderText(frs, text, Color::Black);
        Texture textYellow = Display::renderText(frs, text, Color::Yellow);
        const int space = 5;

        tx1 = Display::renderRect(Color::Black, Color::Silver, textBlack.size() + Size(space, space) * 2, 1);
        tx2 = Display::renderRect(Color::Black, Color::DimGray, textYellow.size() + Size(space, space) * 2, 1);

        Display::renderTexture(textBlack, textBlack.rect(), tx1, textBlack.rect() + Point(space, space));
        Display::renderTexture(textYellow, textYellow.rect(), tx2, textYellow.rect() + Point(space, space));

        setSize(tx1.size());
	setPosition(pos);

	setVisible(true);
    }

protected:
    const Texture* textureDisabled(void) const
    {
	return &tx2;
    }

    const Texture* texturePressed(void) const
    {
	return &tx2;
    }

    const Texture* textureReleased(void) const
    {
	return &tx1;
    }
};

class SimpleColorWindow : public Window
{
    Color		backcolor;
    Color		barcolor;
    Rect		toolbar;
    Point		movept;
    Texture		text;

public:
    SimpleColorWindow(const Color & col, const std::string & str, const Point & pos, const Size & sz, Window & win)
	: Window(pos, sz, &win), backcolor(col), barcolor(Color::Silver)
    {
        const FontRender & frs = FontRenderSystem();
        text = Display::renderText(frs, str, Color::Black);
	toolbar = Rect(1, 1, sz.w - 2, 14);
	resetState(FlagModality);
	setState(FlagMouseTracking);
	setVisible(true);
    }

    void renderWindow(void)
    {
	renderClear(backcolor);
	renderColor(barcolor, toolbar);
	renderTexture(text, Point((width() - text.width()) / 2, 1));
    }

protected:
    void mouseFocusEvent(void)
    {
	barcolor = Color(Color::DimGray);
	renderWindow();
    }

    void mouseLeaveEvent(void)
    {
	barcolor = Color(Color::Silver);
	renderWindow();
    }

    bool mousePressEvent(const ButtonEvent & be)
    {
	if(be.isButtonLeft() && (toolbar & be.position()))
	{
	    movept = be.position();
	    setState(FlagPressed);
	}

	DisplayScene::moveTopLayer(*this);
	renderWindow();

	return true;
    }

    bool mouseReleaseEvent(const ButtonEvent & be)
    {
	resetState(FlagPressed);
	return true;
    }

    void mouseTrackingEvent(const Point & pos, u32 buttons)
    {
	if((buttons & ButtonLeft) &&
	    checkState(FlagPressed))
	    setPosition(pos - movept);
    }

    bool mouseMotionEvent(const Point & pos, u32 buttons)
    {
	// disable events for bottom windows
	return (buttons & ButtonLeft) &&
	    checkState(FlagPressed);
    }
};

class InputWidget : public SimpleColorWindow
{
    SWE::LineEdit       lineEdit;

protected:

public:
    InputWidget(const SWE::Rect & area, SWE::Window & win) : SimpleColorWindow(Color::SandyBrown, "InputWidget", area, area, win), lineEdit(this)
    {
        const FontRender & frs = FontRenderSystem();

	lineEdit.setPosition(SWE::Point(4, height() - 4 - frs.lineSkipHeight()));
	lineEdit.setSize(SWE::Size(width() - 8, frs.lineSkipHeight() + 2));
	lineEdit.setText("Hello World!");
	lineEdit.setVisible(true);
    }

    void renderWindow(void) override
    {
	SimpleColorWindow::renderWindow();
	renderColor(SWE::Color::Black, lineEdit.area() - position());
    }
};

class SimpleTextArea : TextArea
{
protected:
    bool keyPressEvent(const KeySym & key) override
    {
	switch(key.keycode())
	{
	    case Key::PAGEUP:
		return scrollUp(visibleItems());
	    case Key::PAGEDOWN:
		return scrollDown(visibleItems());
	    case Key::UP:
		return scrollUp(1);
    	    case Key::DOWN:
		return scrollDown(1);

	    default: break;
	}

	return false;
    }

public:
    SimpleTextArea(const Rect & pos, Window & win) : TextArea(pos, pos, & win)
    {
	const FontRender & frs = FontRenderSystem();

        appendString(frs, "WindowTextScrollArea", Color::Red, AlignCenter).appendSpacer(frs);

	appendString(frs, "1234567890qwertyuiopasdfghjklzxcvbnm", Color::Navy);

	for(int col = Color::Black + 1; col < Color::Transparent; ++col)
	    appendString(frs, ColorIndex(col).toColor().toString(), col);

	setVisible(true);
    }

    void renderWindow(void) override
    {
	renderClear(Color::DarkSlateGray);
	TextArea::renderWindow();
    }
};

class MainWindow : public DisplayWindow
{
    SimpleListBox	listBox;
    SimpleTextButton	button1;
    SimpleCheckBox	button2;
    SimpleTextButton	button3;
    SimpleTextArea	tarea;
    SimpleColorWindow	win1;
    SimpleColorWindow	win2;
    CellInfoBar		listBar1;
    CellInfoBar		listBar2;
    InputWidget		inputLine;
    FontRenderTTF	frs;

public:
    MainWindow(const Color & col, const std::string & file2) : DisplayWindow(col), 
	listBox(Rect(20, 20, 200, 250), *this),
	button1(Point(520, 30), "Test Button1", *this),
	button2(Point(520, 70), "Test Button2", *this),
	button3(Point(520, 110), "Quit Button3", *this),
	tarea(Rect(20, 280, 220, 180), *this),
	win1(Color::DarkSeaGreen, "Test window 1", Point(500, 150), Size(120, 70), *this),
	win2(Color::Plum, "Test Window 2", Point(500, 250), Size(120, 70), *this),
	listBar1(Point(320, 30), { 0, 111, 112, 0 }, SWE::Color::DarkGreen, *this),
	listBar2(Point(320, 80), { 223, 224, 0, 225 }, SWE::Color::DarkOliveGreen, *this),
	inputLine(Rect(250, 300, 150, 50), *this)
    {
	Display::hardwareCursorHide();
	Display::renderCursor(Display::createTexture(file2));

	frs.open("DejaVuSansMono.ttf", 14, RenderBlended);
        frs.setShadedBackground(col);

	button1.setAction(555);
	button1.setToolTip("F1: clicked, F3: disabled");
	button1.setVisible(true);

	VERBOSE("window size: " << sizeof(Window));

	button2.setAction(556);
	button2.setChecked(false);
	button2.setToolTip("unchecked");

	button3.setAction(557);
	button3.setHotKey(Key::ESCAPE);
	button3.setVisible(true);

	listBar1.setLinkedListBar(&listBar2);
	listBar2.setLinkedListBar(&listBar1);

	signalSubscribe(button1, Signal::ButtonReleased);
	signalSubscribe(button1, Signal::ButtonPressed);
	signalSubscribe(button1, Signal::ButtonClicked);

	signalSubscribe(button2, Signal::ButtonReleased);
	signalSubscribe(button2, Signal::ButtonPressed);
	signalSubscribe(button2, Signal::ButtonClicked);
    }

    ~MainWindow()
    {
	Display::hardwareCursorShow();
    }

    void renderWindow(void)
    {
	DisplayWindow::renderWindow();
	renderText(systemFont(), "reder psf system: Test Hyjlpn Swextty!", Color::Yellow, Point(400, 380));
	renderSurface(frs.renderString("render ttf blended: Test Hyjlpn Swextty!", Color::Yellow, RenderBlended, StyleNormal), Point(400, 400));
	renderSurface(frs.renderString("render ttf solid: Test Hyjlpn Swextty!", Color::Yellow, RenderSolid, StyleNormal), Point(400, 420));
	renderSurface(frs.renderString("render ttf shaded: Test Hyjlpn Swextty!", Color::Yellow, RenderShaded, StyleNormal), Point(400, 440));
	renderSurface(frs.renderString("ttf style: Test Hyjlpn Swextty!", Color::Yellow, RenderBlended, StyleBold|StyleItalic|StyleUnderLine), Point(400, 460));
    }

    bool keyPressEvent(const KeySym & key)
    {
	switch(key.keycode())
	{
	    case Key::F1:
		button1.setClicked();
		return true;

	    case Key::F3:
		button1.setDisabled(!button1.isDisabled());
		return true;

	    default: break;
	}

	return false;
    }

    void signalReceive(int sig, const SignalMember* sm)
    {
	switch(sig)
	{
	    case Signal::ButtonReleased:
		if(sm == &button2)
		{
		    button2.setToolTip("checked");
		    VERBOSE("unchecked button3");
		}
		break;

	    case Signal::ButtonPressed:
		if(sm == &button2)
		{
		    button2.setToolTip("unchecked");
		    VERBOSE("checked button2");
		}
		break;

	    case Signal::ButtonClicked:
		if(sm == &button1) VERBOSE("clicked button1");
		break;

	    default: break;
	}
    }

    bool userEvent(int event, void* data)
    {
	switch(event)
	{
	    case 555:
		VERBOSE("EVENT BUTTON1");
		return true;

	    case 556:
		VERBOSE("EVENT button2");
		return true;

	    case 557:
		VERBOSE("EVENT button3");
		setVisible(false);
		return true;

	    default: break;
	}

	return false;
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

	const std::string title = StringFormat("SWE TestGUI: %1").arg(Engine::version());
	const Size geometry(640, 480);

	if(! Display::init(title, geometry, false))
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

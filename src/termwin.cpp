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

#include <algorithm>

#include "engine.h"
#include "termwin.h"

int acs::vline(line_t type)
{
    switch(type)
    {
        case LineThin:    return 0x2502;
        case LineBold:    return 0x2503;
        case LineDouble:  return 0x2551;
        default: break;
    }
    return 179;
}

int acs::hline(line_t type)
{
    switch(type)
    {
        case LineThin:    return 0x2500;
        case LineBold:    return 0x2501;
        case LineDouble:  return 0x2550;
        default: break;
    }
    return 196;
}

int acs::ulcorner(line_t type)
{
    switch(type)
    {
        case LineThin:    return 0x250C;
        case LineBold:    return 0x250F;
        case LineDouble:  return 0x2554;
        default: break;
    }
    return 218;
}

int acs::urcorner(line_t type)
{
    switch(type)
    {
        case LineThin:    return 0x2510;
        case LineBold:    return 0x2513;
        case LineDouble:  return 0x2557;
        default: break;
    }
    return 191;
}

int acs::llcorner(line_t type)
{
    switch(type)
    {
        case LineThin:    return 0x2514;
        case LineBold:    return 0x2517;
        case LineDouble:  return 0x255A;
        default: break;
    }
    return 192;
}

int acs::lrcorner(line_t type)
{
    switch(type)
    {
        case LineThin:    return 0x2518;
        case LineBold:    return 0x251B;
        case LineDouble:  return 0x255D;
        default: break;
    }
    return 217;
}

int acs::ltee(line_t type)
{
    switch(type)
    {
        case LineThin:    return 0x251C;
        case LineBold:    return 0x2523;
        case LineDouble:  return 0x2560;
        default: break;
    }
    return 195;
}

int acs::rtee(line_t type)
{
    switch(type)
    {
        case LineThin:    return 0x2524;
        case LineBold:    return 0x252B;
        case LineDouble:  return 0x2563;
        default: break;
    }
    return 180;
}

int acs::ttee(line_t type)
{
    switch(type)
    {
        case LineThin:    return 0x252C;
        case LineBold:    return 0x2533;
        case LineDouble:  return 0x2566;
        default: break;
    }
    return 194;
}

int acs::btee(line_t type)
{
    switch(type)
    {
        case LineThin:    return 0x2534;
        case LineBold:    return 0x253B;
        case LineDouble:  return 0x2569;
        default: break;
    }
    return 193;
}

int acs::plus(line_t type)
{
    switch(type)
    {
        case LineThin:    return 0x253C;
        case LineBold:    return 0x254B;
        case LineDouble:  return 0x256C;
        default: break;
    }
    return 197;
}

TermWindow::TermWindow() :
    Window(Point(0,0), Display::size(), NULL), fontRender(NULL), align(AlignLeft)
{
}

TermWindow::TermWindow(TermWindow & win) :
    Window(& win), fontRender(win.frs()), align(AlignLeft)
{
    if(! fontRender)
	Engine::except(__FUNCTION__, "fontRender is null");

    if(fontRender->size().isEmpty())
	Engine::except(__FUNCTION__, "font size empty");

    *this << reset::defaults();
}

TermWindow::TermWindow(const FontRender & font) :
    Window(Point(0,0), Display::size(), NULL), fontRender(& font), align(AlignLeft)
{
    if(font.size().isEmpty())
	Engine::except(__FUNCTION__, "font size empty");

    *this << reset::defaults();
    setSize(Display::size());
}

TermWindow::TermWindow(const FontRender & font, Window & win) :
    Window(& win), fontRender(& font), align(AlignLeft)
{
    if(font.size().isEmpty())
	Engine::except(__FUNCTION__, "font size empty");

    *this << reset::defaults();
}

TermWindow::TermWindow(const Point & gfxpt, const Size & gfxsz, const FontRender & font, Window & win) :
    Window(gfxpt, gfxsz, &win), fontRender(& font), align(AlignLeft)
{
    if(font.size().isEmpty())
	Engine::except(__FUNCTION__, "font size empty");

    *this << reset::defaults();
    setSize(gfxsz);
}

void TermWindow::setFontRender(const FontRender & frs)
{
    if(frs.size().isEmpty())
	Engine::except(__FUNCTION__, "font size empty");

    fontRender = & frs;
    setSize(termsz.isEmpty() ? Display::size() : termsz * fontRender->size());
}

void TermWindow::setSize(const Size & sz)
{
    termsz = sz / fontRender->size();

    chars.clear();
    chars.resize(termsz.w * termsz.h, TermCharset(UnicodeColor(0x20, FBColors(fgColor, bgColor))));

    // set dirty area
    std::for_each(chars.begin(), chars.end(), std::bind2nd(std::mem_fun_ref(&BitFlags::set), static_cast<int>(TermCharset::PropDirty)));

    Window::setSize(termsz * fontRender->size());
}

void TermWindow::setCharset(int ch, const ColorIndex & fg, const ColorIndex & bg, int prop)
{
    Rect win(Point(0,0), termsz);

    if(padding())
    {
	win.x = padding.left();
	win.y = padding.top();
	win.w = cols() - win.x - padding.right();
	win.h = rows() - win.y - padding.bottom();
    }

    if(win.x <= curpos.x && curpos.x < win.x + win.w &&
	win.y <= curpos.y && curpos.y < win.y + win.h)
    {
	int pos = index();
	TermCharset & tc = chars[pos];

	if(ch != -1) tc.unicode(ch);

	if(! fg.isTransparent()) tc.fgindex(fg);
	else
	if(! fgColor.isTransparent()) tc.fgindex(fgColor);

	if(! bg.isTransparent()) tc.bgindex(bg);
	else
	if(! bgColor.isTransparent()) tc.bgindex(bgColor);

	if(prop) tc.set(prop);
	tc.set(TermCharset::PropDirty);
    }

    Point curs = cursor();
    *this << cursor::set(curs.x + 1, curs.y);
}

const Point & TermWindow::cursor(void) const
{
    return curpos;
}

bool TermWindow::lineWrap(void) const
{
    return state.check(FlagWrap);
}

int TermWindow::index(const Point & pos) const
{
    return cols() * pos.y + pos.x;
}

int TermWindow::index(void) const
{
    return index(curpos);
}

Point TermWindow::sym2gfx(const Point & sym) const /* coordinate from current win */
{
    return position() + sym * fontRender->size();
}

Point TermWindow::gfx2sym(const Point & gfx) const /* coordinate from current win */
{
    return (gfx - position()) / fontRender->size();
}

Size TermWindow::sym2gfx(const Size & sym) const
{
    return sym * fontRender->size();
}

Size TermWindow::gfx2sym(const Size & gfx) const
{
    Size res = gfx / fontRender->size();
    // fix float values
    if(res.w * fontRender->size().w < gfx.w) res.w += 1;
    if(res.h * fontRender->size().h < gfx.h) res.h += 1;

    return res;
}

Rect TermWindow::sym2gfx(const Rect & sym) const
{
    const Point & sympt = sym;
    const Size & symsz = sym;
    return Rect(sym2gfx(sympt), sym2gfx(symsz));
}

Rect TermWindow::gfx2sym(const Rect & gfx) const
{
    const Point & gfxpt = gfx;
    const Size & gfxsz = gfx;
    return Rect(gfx2sym(gfxpt), gfx2sym(gfxsz));
}

void TermWindow::renderWindow(void)
{
    // set all dirty area
    *this << set::dirty() << set::flush();
}

void TermWindow::renderFlush(void)
{
    Size fontSize = fontRender->size();

    for(auto it = chars.begin(); it != chars.end(); ++it)
    if((*it).check(TermCharset::PropDirty))
    {
	int index = std::distance(chars.begin(), it);
	int posx = (index % cols()) * fontSize.w;
	int posy = (index / cols()) * fontSize.h;
	Color bgcolor = (*it).bgcolor();
	if(! bgcolor.isTransparent())
	    renderColor(bgcolor, Rect(posx, posy, fontSize.w, fontSize.h));
	renderTexture(FontsCache(fontRender).renderCharset((*it).unicode(), (*it).fgcolor()), Point(posx, posy));
	(*it).reset(TermCharset::PropDirty);
    }
}
/*
bool	keyPressHandle(int);
bool	keyReleaseHandle(int);
bool	mousePressHandle(const ButtonEvent &);
bool	mouseReleaseHandle(const ButtonEvent &);
bool	mouseClickHandle(const ButtonsEvent &);
bool	mouseMotionHandle(const Point &);
bool	userHandle(const SDL_UserEvent &);
bool	scrollUpHandle(const Point &);
bool	scrollDownHandle(const Point &);
bool	scrollLeftHandle(const Point &);
bool	scrollRightHandle(const Point &);

bool	keyPressEvent(int) { return false; }
bool	keyReleaseEvent(int) { return false; }
bool	mousePressEvent(const ButtonEvent &) { return false; }
bool	mouseReleaseEvent(const ButtonEvent &) { return false; }
bool	mouseClickEvent(const ButtonsEvent &) { return false; }
void	mouseLeaveEvent(void) {}
bool	mouseMotionEvent(const Point &) { return false; }
bool	userEvent(int, void*) { return false; }
bool	scrollUpEvent(const Point &) { return false; }
bool	scrollDownEvent(const Point &) { return false; }
bool	scrollLeftEvent(const Point &) { return false; }
bool	scrollRightEvent(const Point &) { return false; }
*/

TermWindow & TermWindow::operator<< (const cursor::set & st)
{
    Rect area = Rect(0, 0, cols(), rows());

    if(padding())
    {
	area.x = padding.left();
	area.y = padding.top();
	area.w = cols() - (padding.left() + padding.right());
	area.h = rows() - (padding.top() + padding.bottom());
    }

    curpos.x = st.posx() < area.x ? area.x : st.posx();
    curpos.y = st.posy() < area.y ? area.y : st.posy();

    if(lineWrap() && curpos.x >= area.x + area.w)
    {
	curpos.x = area.x + (curpos.x % area.w);
	curpos.y = curpos.y + curpos.x / area.w;
    }

    return *this;
}

TermWindow & TermWindow::operator<< (const cursor::move & st)
{
    if(st.count())
    {
	Point curs = cursor();

	switch(st.dir())
	{
	    case MoveFirst:		return *this << cursor::set(0, curs.y); break;
	    case MoveLast:		return *this << cursor::set(cols() - 1 , curs.y); break;
	    case MoveUp:		return *this << cursor::set(curs.x, curs.y - st.count()); break;
	    case MoveDown:		return *this << cursor::set(curs.x, curs.y + st.count()); break;
	    case MoveLeft:		return *this << cursor::set(curs.x - st.count(), curs.y); break;
	    case MoveRight:		return *this << cursor::set(curs.x + st.count(), curs.y); break;
	    case MoveUpperLeft:		return *this << cursor::move(MoveUp, st.count()) << cursor::move(MoveLeft, st.count());
	    case MoveUpperRight:	return *this << cursor::move(MoveUp, st.count()) << cursor::move(MoveRight, st.count());
	    case MoveLowerLeft:		return *this << cursor::move(MoveDown, st.count()) << cursor::move(MoveLeft, st.count());
	    case MoveLowerRight:	return *this << cursor::move(MoveDown, st.count()) << cursor::move(MoveRight, st.count());
	    default: break;
	}
    }

    return *this;
}

TermWindow & TermWindow::operator<< (const draw::hline & st)
{
    return *this << fill::charset(st.charset(), Size(st.count(), 1));
}

TermWindow & TermWindow::operator<< (const draw::vline & st)
{
    return *this << fill::charset(st.charset(), Size(1, st.count()));
}

TermWindow & TermWindow::operator<< (const draw::rect & st)
{
    return *this << cursor::set(st.x, st.y) << acs::ulcorner(st.line) << draw::hline(st.w - 2, acs::hline(st.line)) << acs::urcorner(st.line) <<
	cursor::move(MoveLowerLeft) << draw::vline(st.h - 2, acs::vline(st.line)) <<
	cursor::set(st.x, st.y + 1) << draw::vline(st.h - 2, acs::vline(st.line)) <<
        cursor::set(st.x, st.h - 1) << acs::llcorner(st.line) << draw::hline(st.w - 2, acs::hline(st.line)) << acs::lrcorner(st.line);
}

TermWindow & TermWindow::operator<< (int ch)
{
    setCharset(ch);
    return *this;
}

TermWindow & TermWindow::operator<< (const char* st)
{
    return *this << UnicodeString(st ? st : "(null)");
}

TermWindow & TermWindow::operator<< (const std::string & st)
{
    return *this << UnicodeString(st);
}

TermWindow & TermWindow::operator<< (const UnicodeList & st)
{
    int curx = cursor().x;

    for(auto it = st.begin(); it != st.end(); ++it)
    {
        *this << *it;
	*this << cursor::set(curx, cursor().y + 1);
    }

    return *this;
}

TermWindow & TermWindow::operator<< (const UnicodeString & st)
{
    if(0 < st.index('\n'))
	return *this << st.split('\n');

    if(lineWrap())
    {
        size_t wrapsz = cols() - (padding.left() + padding.right());
        if(st.size() > wrapsz)
	    return *this << st.wrap(wrapsz);
    }

    Point cur = cursor();

    if(align == AlignCenter)
        cur.x -= st.size() / 2;
    else
    if(align == AlignRight)
        cur.x -= st.size() - 1;

    *this << cursor::set(cur.x, cur.y);

    for(auto it = st.begin(); it != st.end(); ++it)
        *this << *it;

    return *this;
}

TermWindow & TermWindow::operator<< (const UCString & ucs)
{
/*
    if(0 < st.index('\n'))
	return *this << st.split('\n');

    if(lineWrap())
    {
        size_t wrapsz = cols() - (padding.left() + padding.right());
        if(st.size() > wrapsz)
	    return *this << st.wrap(wrapsz);
    }
*/
    Point cur = cursor();

    if(align == AlignCenter)
        cur.x -= ucs.length() / 2;
    else
    if(align == AlignRight)
        cur.x -= ucs.length() - 1;

    *this << cursor::set(cur.x, cur.y);

    for(size_t pos = 0; pos < ucs.length(); ++pos)
        *this << ucs.at(pos);

    return *this;
}

TermWindow & TermWindow::operator<< (const UnicodeColor & st)
{
    setCharset(st.unicode(), st.fgindex(), st.bgindex());
    return *this;
}

TermWindow & TermWindow::operator<< (const TermCharset & st)
{
    setCharset(st.unicode(), st.fgindex(), st.bgindex(), st.prop());
    return *this;
}

TermWindow & TermWindow::operator<< (const fill::charset & st)
{
    Point start = cursor();

    for(int posy = start.y; posy < start.y + st.height(); ++posy)
    {
	for(int posx = start.x; posx < start.x + st.width(); ++posx)
	{
	    *this << cursor::set(posx, posy);
	    setCharset(st.value());
	}
    }

    return *this;
}

TermWindow & TermWindow::operator<< (const fill::defaults & st)
{
    for(size_t posy = 0; posy < rows(); ++posy)
    {
	for(size_t posx = 0; posx < cols(); ++posx)
	{
	    *this << cursor::set(posx, posy);
	    setCharset(st.unicode(), st.fgindex(), st.bgindex(), st.prop());
	}
    }

    curpos = Point(0, 0);

    return *this;
}

TermWindow & TermWindow::operator<< (const fill::fgcolor & st)
{
    Point start = cursor();

    for(int posy = start.y; posy < start.y + st.height(); ++posy)
    {
	for(int posx = start.x; posx < start.x + st.width(); ++posx)
	{
	    *this << cursor::set(posx, posy);
	    setCharset(-1, st.value());
	}
    }

    return *this;
}

TermWindow & TermWindow::operator<< (const fill::bgcolor & st)
{
    Point start = cursor();

    for(int posy = start.y; posy < start.y + st.height(); ++posy)
    {
	for(int posx = start.x; posx < start.x + st.width(); ++posx)
	{
	    *this << cursor::set(posx, posy);
	    setCharset(-1, Color::Transparent, st.value());
	}
    }

    return *this;
}

TermWindow & TermWindow::operator<< (const fill::colors & st)
{
    Point start = cursor();

    for(int posy = start.y; posy < start.y + st.height(); ++posy)
    {
	for(int posx = start.x; posx < start.x + st.width(); ++posx)
	{
	    *this << cursor::set(posx, posy);
	    setCharset(-1, st.fgindex(), st.bgindex());
	}
    }

    return *this;
}

TermWindow & TermWindow::operator<< (const fill::property & st)
{
    Point start = cursor();

    for(int posy = start.y; posy < start.y + st.height(); ++posy)
    {
	for(int posx = start.x; posx < start.x + st.width(); ++posx)
	{
	    *this << cursor::set(posx, posy);
	    setCharset(-1, Color::Transparent, Color::Transparent, st.value());
	}
    }

    return *this;
}

TermWindow & TermWindow::operator<< (const reset::defaults & st)
{
    curpos = Point(0, 0);
    fgColor = Color::Transparent;
    bgColor = Color::Transparent;
    align = AlignLeft;
    padding = set::padding(0, 0, 0, 0);
    state.reset(FlagWrap);

    return *this;
}

TermWindow & TermWindow::operator<< (const reset::colors & st)
{
    return *this << reset::fgcolor() << reset::bgcolor();
}

TermWindow & TermWindow::operator<< (const reset::fgcolor & st)
{
    fgColor = Color::Transparent;
    return *this;
}

TermWindow & TermWindow::operator<< (const reset::bgcolor & st)
{
    bgColor = Color::Transparent;
    return *this;
}

TermWindow & TermWindow::operator<< (const reset::padding & st)
{
    padding = set::padding(0, 0, 0, 0);
    return *this;
}

TermWindow & TermWindow::operator<< (const reset::wrap & st)
{
    state.reset(FlagWrap);
    return *this;
}

TermWindow & TermWindow::operator<< (const set::dirty & st)
{
    std::for_each(chars.begin(), chars.end(), std::bind2nd(std::mem_fun_ref(&BitFlags::set), static_cast<int>(TermCharset::PropDirty)));
    return *this;
}

TermWindow & TermWindow::operator<< (const set::rn & st)
{
    return *this << cursor::first() << cursor::down();
}

TermWindow & TermWindow::operator<< (const set::flush & st)
{
    renderFlush();
    return *this;
}

TermWindow & TermWindow::operator<< (const set::colors & st)
{
    fgColor = st.fgindex();
    bgColor = st.bgindex();
    return *this;
}

TermWindow & TermWindow::operator<< (const set::fgcolor & st)
{
    fgColor = st.color;
    return *this;
}

TermWindow & TermWindow::operator<< (const set::bgcolor & st)
{
    bgColor = st.color;
    return *this;
}

TermWindow & TermWindow::operator<< (const set::align & st)
{
    align = st();
    return *this;
}

TermWindow & TermWindow::operator<< (const set::padding & st)
{
    padding = st;
    return *this;
}

TermWindow & TermWindow::operator<< (const set::wrap & st)
{
    state.set(FlagWrap);
    return *this;
}




CenteredTerminal::CenteredTerminal(const Size & termsz, const FontRender & font, Window & win)
    : TermWindow(Point(win.size() - font.size() * termsz) / 2, font.size() * termsz, font, win)
{
}

MessageWindow::MessageWindow(const std::string & hdr, const ColorIndex & hcol, const std::string & msg, const ColorIndex & mcol,
    const ColorIndex & back, const ColorIndex & border, const FontRender & font, Window & win) : CenteredTerminal(Size(40, 8), font, win),
    header(hdr), content(msg), headerColor(hcol), contentColor(mcol), backgroundColor(back), borderColor(border)
{
    renderWindow();
    setVisible(true);
}

void MessageWindow::renderWindow(void)
{
    *this << reset::defaults() <<
	fill::defaults(borderColor, backgroundColor) <<
        draw::rect(0, 0, cols(), rows(), LineAscii) <<
	set::fgcolor(headerColor) <<
	cursor::set(cols() / 2, 2) << set::align(AlignCenter) << header <<
	set::fgcolor(contentColor) << 
	cursor::set(cols() / 2, 4) << set::wrap() << content <<
	set::flush();
}

bool MessageWindow::keyPressEvent(int key)
{
    switch(key)
    {
        case Key::AC_BACK:
        case Key::ESCAPE:
            return actionDialogClose();

        default: break;
    }

    return false;
}

bool MessageWindow::mouseClickEvent(const ButtonsEvent &)
{
    return actionDialogClose();
}

bool MessageWindow::actionDialogClose(void)
{
    setVisible(false);
    return true;
}

void DisplayError(const std::string & hdr, const std::string & msg)
{
    if(Display::init("Error", Size(640, 480), false))
    {
        DisplayWindow win1(Color::DarkSlateGray);
	MessageWindow win2(hdr, Color::IndianRed, msg, Color::LightCyan, Color::RoyalBlue, Color::Yellow, systemFont(), win1);

	win2.exec();
    }
}

void MessageTop(const std::string & hdr, const std::string & msg)
{
    Window* win = const_cast<Window*>(Display::window());

    if(win)
    {
	MessageWindow mw(hdr, Color::IndianRed, msg, Color::LightCyan, Color::RoyalBlue, Color::Yellow, systemFont(), *win);
	Display::redraw();
    }
}

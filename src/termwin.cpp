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

TermBase::TermBase() :
    Window(Point(0,0), Display::size(), NULL), fontRender(NULL)
{
    fontRender = & systemFont();

    *this << reset::defaults();
}

TermBase::TermBase(TermBase & win) :
    Window(& win), fontRender(win.frs())
{
    if(! fontRender)
    {
	fontRender = & systemFont();
	ERROR("fontRender is null" << ", " << "used system font");
    }

    if(fontRender->size().isEmpty())
    {
	fontRender = & systemFont();
	ERROR("font size empty" << ", " << "used system font");
    }

    *this << reset::defaults();
}

TermBase::TermBase(const FontRender & font) :
    Window(Point(0,0), Display::size(), NULL), fontRender(& font)
{
    if(fontRender->size().isEmpty())
    {
	fontRender = & systemFont();
	ERROR("font size empty" << ", " << "used system font");
    }

    *this << reset::defaults();
    setSize(Display::size());
}

TermBase::TermBase(const FontRender & font, Window & win) :
    Window(& win), fontRender(& font)
{
    if(fontRender->size().isEmpty())
    {
	fontRender = & systemFont();
	ERROR("font size empty" << ", " << "used system font");
    }

    *this << reset::defaults();
}

TermBase::TermBase(const Point & gfxpt, const Size & gfxsz, const FontRender & font, Window & win) :
    Window(gfxpt, gfxsz, &win), fontRender(& font)
{
    if(fontRender->size().isEmpty())
    {
	fontRender = & systemFont();
	ERROR("font size empty" << ", " << "used system font");
    }

    *this << reset::defaults();
    setSize(gfxsz);
}

void TermBase::setFontRender(const FontRender & frs)
{
    if(frs.size().isEmpty())
    {
	ERROR("font size empty");
    }
    else
    {
	fontRender = & frs;
	setSize(termSize().isEmpty() ? Display::size() : sym2gfx(termSize()));
    }
}

void TermBase::setSize(const Size & sz)
{
    Size tsz = gfx2sym(sz);
    setTermSize(tsz.w, tsz.h);
}

void TermBase::setTermSize(int tmw, int tmh)
{
    termsz.set1(tmw);
    termsz.set2(tmh);
    Window::setSize(sym2gfx(termSize()));
}

/*
void TermBase::setCharset(int ch, const ColorIndex & fg, const ColorIndex & bg, int prop)
{
    Rect win(Point(0,0), termSize());

    if(padding())
    {
	win.x = padding.left();
	win.y = padding.top();
	win.w = cols() - win.x - padding.right();
	win.h = rows() - win.y - padding.bottom();
    }

    if(win.x <= curx() && curx() < win.x + win.w &&
	win.y <= cury() && cury() < win.y + win.h)
    {
	int pos = index();
	UnicodeColor & tc = chars[pos];

	if(ch != -1) tc.unicode(ch);

	if(! fg.isTransparent()) tc.fgindex(fg);
	else
	if(! fgColor().isTransparent()) tc.fgindex(fgColor());

	if(! bg.isTransparent()) tc.bgindex(bg);
	else
	if(! bgColor().isTransparent()) tc.bgindex(bgColor());
    }

    Point curs = cursor();
    *this << cursor::set(curs.x + 1, curs.y);
}

void TermBase::renderFlush(void)
{
    Size fontSize = fontRender->size();

    for(auto it = chars.begin(); it != chars.end(); ++it)
    {
	int index = std::distance(chars.begin(), it);
	int posx = (index % cols()) * fontSize.w;
	int posy = (index / cols()) * fontSize.h;
	Color bgcolor = (*it).bgcolor();
	if(! bgcolor.isTransparent())
	    renderColor(bgcolor, Rect(posx, posy, fontSize.w, fontSize.h));
	renderTexture(FontsCache(fontRender).renderCharset((*it).unicode(), (*it).fgcolor()), Point(posx, posy));
    }
}
*/

bool TermBase::lineWrap(void) const
{
    return checkState(FlagWrap);
}

int TermBase::index(const Point & pos) const
{
    if(0 <= pos.x && 0 <= pos.y &&
	pos.x < cols() && pos.y < rows())
	return cols() * pos.y + pos.x;
    return -1;
}

int TermBase::index(void) const
{
    const Point & cur = cursor();
    return cols() * cur.y + cur.x;
}

Point TermBase::sym2gfx(const Point & sym) const /* coordinate from current win */
{
    return position() + sym * fontRender->size();
}

Point TermBase::gfx2sym(const Point & gfx) const /* coordinate from current win */
{
    return (gfx - position()) / fontRender->size();
}

Size TermBase::sym2gfx(const Size & sym) const
{
    return sym * fontRender->size();
}

Size TermBase::gfx2sym(const Size & gfx) const
{
    Size res = gfx / fontRender->size();
    // fix float values
    if(res.w * fontRender->size().w < gfx.w) res.w += 1;
    if(res.h * fontRender->size().h < gfx.h) res.h += 1;

    return res;
}

Rect TermBase::sym2gfx(const Rect & sym) const
{
    const Point & sympt = sym;
    const Size & symsz = sym;
    return Rect(sym2gfx(sympt), sym2gfx(symsz));
}

Rect TermBase::gfx2sym(const Rect & gfx) const
{
    const Point & gfxpt = gfx;
    const Size & gfxsz = gfx;
    return Rect(gfx2sym(gfxpt), gfx2sym(gfxsz));
}

void TermBase::renderWindow(void)
{
    *this << set::flush();
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

TermBase & TermBase::operator<< (const cursor::set & st)
{
    Rect area = Rect(0, 0, cols(), rows());

    if(padding())
    {
	area.x = padding.left();
	area.y = padding.top();
	area.w = cols() - (padding.left() + padding.right());
	area.h = rows() - (padding.top() + padding.bottom());
    }

    int cx = st.posx() < area.x ? area.x : st.posx();
    int cy = st.posy() < area.y ? area.y : st.posy();

    if(lineWrap() && cx >= area.x + area.w)
    {
	cx = area.x + (cx % area.w);
	cy = cy + cx / area.w;
    }

    setCursorPos(cx, cy);

    return *this;
}

TermBase & TermBase::operator<< (const cursor::move & st)
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

TermBase & TermBase::operator<< (const draw::hline & st)
{
    return *this << fill::charset(st.charset(), Size(st.count(), 1));
}

TermBase & TermBase::operator<< (const draw::vline & st)
{
    return *this << fill::charset(st.charset(), Size(1, st.count()));
}

TermBase & TermBase::operator<< (const draw::rect & st)
{
    return *this << cursor::set(st.x, st.y) << acs::ulcorner(st.line) << draw::hline(st.w - 2, acs::hline(st.line)) << acs::urcorner(st.line) <<
	cursor::move(MoveLowerLeft) << draw::vline(st.h - 2, acs::vline(st.line)) <<
	cursor::set(st.x, st.y + 1) << draw::vline(st.h - 2, acs::vline(st.line)) <<
        cursor::set(st.x, st.h - 1) << acs::llcorner(st.line) << draw::hline(st.w - 2, acs::hline(st.line)) << acs::lrcorner(st.line);
}

TermBase & TermBase::operator<< (int ch)
{
    setCharset(ch);
    return *this;
}

TermBase & TermBase::operator<< (const char* st)
{
    return *this << UnicodeString(st ? st : "(null)");
}

TermBase & TermBase::operator<< (const std::string & st)
{
    return *this << UnicodeString(st);
}

TermBase & TermBase::operator<< (const UnicodeList & st)
{
    int curx = cursor().x;

    for(auto it = st.begin(); it != st.end(); ++it)
    {
        *this << *it;
	*this << cursor::set(curx, cursor().y + 1);
    }

    return *this;
}

TermBase & TermBase::operator<< (const UCStringList & st)
{
    int curx = cursor().x;

    for(auto it = st.begin(); it != st.end(); ++it)
    {
        *this << *it;
	*this << cursor::set(curx, cursor().y + 1);
    }

    return *this;
}

TermBase & TermBase::operator<< (const UnicodeString & st)
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

    if(align() == AlignCenter)
        cur.x -= st.size() / 2;
    else
    if(align() == AlignRight)
        cur.x -= st.size() - 1;

    *this << cursor::set(cur.x, cur.y);

    for(auto it = st.begin(); it != st.end(); ++it)
        *this << *it;

    return *this;
}

TermBase & TermBase::operator<< (const UCString & ucs)
{
    if(0 < ucs.index('\n'))
	return *this << ucs.split('\n');

    if(lineWrap())
    {
        size_t wrapsz = cols() - (padding.left() + padding.right());
        if(ucs.size() > wrapsz)
	    return *this << ucs.wrap(wrapsz);
    }

    Point cur = cursor();

    if(align() == AlignCenter)
        cur.x -= ucs.length() / 2;
    else
    if(align() == AlignRight)
        cur.x -= ucs.length() - 1;

    *this << cursor::set(cur.x, cur.y);

    for(size_t pos = 0; pos < ucs.length(); ++pos)
        *this << ucs.at(pos);

    return *this;
}

TermBase & TermBase::operator<< (const UnicodeColor & st)
{
    setCharset(st.unicode(), st.fgindex(), st.bgindex());
    return *this;
}

TermBase & TermBase::operator<< (const fill::charset & st)
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

TermBase & TermBase::operator<< (const fill::defaults & st)
{
    for(size_t posy = 0; posy < rows(); ++posy)
    {
	for(size_t posx = 0; posx < cols(); ++posx)
	{
	    *this << cursor::set(posx, posy);
	    setCharset(st.unicode(), st.fgindex(), st.bgindex(), st.prop());
	}
    }

    setCursorPos(0, 0);

    return *this;
}

TermBase & TermBase::operator<< (const fill::fgcolor & st)
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

TermBase & TermBase::operator<< (const fill::bgcolor & st)
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

TermBase & TermBase::operator<< (const fill::colors & st)
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

TermBase & TermBase::operator<< (const fill::property & st)
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

TermBase & TermBase::operator<< (const reset::defaults & st)
{
    setCursorPos(0, 0);
    setFGColor(Color::Transparent);
    setBGColor(Color::Transparent);
    setAlign(AlignLeft);
    padding = set::padding(0, 0, 0, 0);
    resetState(FlagWrap);

    return *this;
}

TermBase & TermBase::operator<< (const reset::colors & st)
{
    return *this << reset::fgcolor() << reset::bgcolor();
}

TermBase & TermBase::operator<< (const reset::fgcolor & st)
{
    setFGColor(Color::Transparent);
    return *this;
}

TermBase & TermBase::operator<< (const reset::bgcolor & st)
{
    setBGColor(Color::Transparent);
    return *this;
}

TermBase & TermBase::operator<< (const reset::padding & st)
{
    padding = set::padding(0, 0, 0, 0);
    return *this;
}

TermBase & TermBase::operator<< (const reset::wrap & st)
{
    resetState(FlagWrap);
    return *this;
}

TermBase & TermBase::operator<< (const set::rn & st)
{
    return *this << cursor::first() << cursor::down();
}

TermBase & TermBase::operator<< (const set::flush & st)
{
    renderFlush();
    return *this;
}

TermBase & TermBase::operator<< (const set::colors & st)
{
    setFGColor(st.fgindex());
    setBGColor(st.bgindex());
    return *this;
}

TermBase & TermBase::operator<< (const set::fgcolor & st)
{
    setFGColor(st.color);
    return *this;
}

TermBase & TermBase::operator<< (const set::bgcolor & st)
{
    setBGColor(st.color);
    return *this;
}

TermBase & TermBase::operator<< (const set::align & st)
{
    setAlign(st());
    return *this;
}

TermBase & TermBase::operator<< (const set::padding & st)
{
    padding = st;
    return *this;
}

TermBase & TermBase::operator<< (const set::wrap & st)
{
    setState(FlagWrap);
    return *this;
}

/* TermWindow */
void TermWindow::setTermSize(int tmw, int tmh)
{
    if(rows() != tmh || cols() != tmw)
    {
	chars.clear();
	chars.resize(tmw * tmh, UnicodeColor(0x20, FBColors(fgColor(), bgColor())));
    }

    TermBase::setTermSize(tmw, tmh);
}

void TermWindow::setCharset(int ch, const ColorIndex & fg, const ColorIndex & bg, int prop)
{
    const Rect & termArea = Rect(padding.left(), padding.top(),
			    cols() - (padding.left() + padding.right()), rows() - (padding.top() + padding.bottom()));

    if(termArea & cursor())
    {
	int pos = index();
	UnicodeColor & tc = chars[pos];

	if(ch != -1) tc.unicode(ch);

	if(! fg.isTransparent()) tc.fgindex(fg);
	else
	if(! fgColor().isTransparent()) tc.fgindex(fgColor());

	if(! bg.isTransparent()) tc.bgindex(bg);
	else
	if(! bgColor().isTransparent()) tc.bgindex(bgColor());
    }

    *this << cursor::right(1);
}

void TermWindow::renderSymbol(int symx, int symy)
{
    const Point sympt = Point(symx, symy);
    int pos = index(sympt);

    if(0 <= pos)
    {
	const UnicodeColor & uc = chars[pos];
	const Color & bgcolor = uc.bgcolor();
	const Point relPos = sym2gfx(sympt) - position();

	if(! bgcolor.isTransparent())
	    renderColor(bgcolor, Rect(relPos, sym2gfx(Size(1, 1))));

	renderTexture(FontsCache(fontRender).renderCharset(uc.unicode(), uc.fgcolor()), relPos);
    }
    else
    {
	ERROR("out of range: " << "symx: " << symx << ", " << "symy: " << symy);
    }
}

void TermWindow::renderFlush(void)
{
    for(int symy = 0; symy < rows(); ++symy)
	for(int symx = 0; symx < cols(); ++symx) renderSymbol(symx, symy);
}


/* TermArea */
void TermArea::setTermPos(int symx, int symy)
{
    termpos.set1(symx);
    termpos.set2(symy);
}

void TermArea::setPosition(const Point & pos)
{
    TermWindow* term = static_cast<TermWindow*>(parent());

    if(term)
    {
	Point sympos = term->gfx2sym(pos);
	setTermPos(sympos.x, sympos.y);
    }

    Window::setPosition(pos);
}

void TermArea::setTermArea(int symx, int symy, int cols, int rows)
{
    setTermPos(symx, symy);
    setTermSize(cols, rows);
}

void TermArea::setCharset(int ch, const ColorIndex & fg, const ColorIndex & bg, int prop)
{
    TermWindow* term = static_cast<TermWindow*>(parent());

    if(term)
    {
	const Point savePos = term->cursor();
	const Point & curs = cursor();
	term->setCursorPos(posx() + curs.x, posy() + curs.y);

	ColorIndex fgcol = ! fgColor().isTransparent() ? fgColor() : fg;
	ColorIndex bgcol = ! bgColor().isTransparent() ? bgColor() : bg;

	if(padding())
	{
	    const Rect & termArea = Rect(padding.left(), padding.left(),
			    cols() - (padding.left() + padding.right()), rows() - (padding.left() + padding.bottom()));

	    if(termArea & cursor())
		term->setCharset(ch, fgcol, bgcol, prop);
	}
	else
	    term->setCharset(ch, fgcol, bgcol, prop);

	term->setCursorPos(savePos.x, savePos.y);
	*this << cursor::right(1);
    }
}

void TermArea::renderFlush(void)
{
    TermWindow* term = static_cast<TermWindow*>(parent());

    if(term)
    {
       for(int py = 0; py < rows(); ++py)
           for(int px = 0; px < cols(); ++px)
               term->renderSymbol(posx() + px, posy() + py);
    }
}

/* CenteredTerminal */
CenteredTerminal::CenteredTerminal(int cols, int rows, const FontRender & font, Window & win)
    : TermWindow(Point(win.size() - Size(cols, rows) * font.size()) / 2, Size(cols, rows) * font.size(), font, win)
{
}

/*
#include "termwin_gui.h"
void DisplayError(const std::string & hdr, const std::string & msg)
{
    if(Display::init("Error", Size(640, 480), false))
    {
        DisplayWindow win1(Color::DarkSlateGray);
	TermGUI::MessageBox win2(hdr, msg, TermGUI::ButtonOk, systemFont(), win1);

	win2.exec();
    }
}

void MessageTop(const std::string & hdr, const std::string & msg)
{
    Window* win1 = const_cast<Window*>(Display::window());

    if(win1)
    {
	TermGUI::MessageBox win2(hdr, msg, 0, systemFont(), *win1);
	Display::redraw();
    }
}
*/

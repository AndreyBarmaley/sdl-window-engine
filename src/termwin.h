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

#ifndef _SWE_TERMWIN_
#define _SWE_TERMWIN_

#include "window.h"

enum line_t { LineNone, LineAscii, LineThin, LineBold, LineDouble };
enum move_t { MoveCenter, MoveUp, MoveDown, MoveLeft, MoveRight, MoveUpperLeft, MoveUpperRight, MoveLowerLeft, MoveLowerRight, MoveFirst, MoveLast };

namespace acs
{
    int         vline(line_t);
    int         hline(line_t);
    int         ulcorner(line_t);
    int         urcorner(line_t);
    int         llcorner(line_t);
    int         lrcorner(line_t);
    int         ltee(line_t);
    int         rtee(line_t);
    int         ttee(line_t);
    int         btee(line_t);
    int         plus(line_t);
}


namespace cursor
{
    struct set : public packint2
    {
	set(const Point & pt) : packint2(pt.x, pt.y) {}
	set(int x, int y) : packint2(x, y) {}

	int posx(void) const { return val1(); }
	int posy(void) const { return val2(); }
    };

    struct move : public packint2
    {
	move(move_t dir, int count = 1) : packint2(dir, count) {}

	int dir(void) const { return val1(); }
	int count(void) const { return val2(); }
    };

    struct up : public move
    {
	up(int count = 1) : move(MoveUp, count) {}
    };

    struct down : public move
    {
	down(int count = 1) : move(MoveDown, count) {}
    };

    struct left : public move
    {
	left(int count = 1) : move(MoveLeft, count) {}
    };

    struct right : public move
    {
	right(int count = 1) : move(MoveRight, count) {}
    };

    struct first : public move
    {
	first() : move(MoveFirst) {}
    };

    struct last : public move
    {
	last() : move(MoveLast) {}
    };
}

namespace set
{
    struct fgcolor
    {
	ColorIndex color;

	fgcolor(const ColorIndex & col) : color(col) {}
    };

    struct bgcolor
    {
	ColorIndex color;

	bgcolor(const ColorIndex & col) : color(col) {}
    };

    struct align
    {
	align_t val;

	align(align_t v) : val(v) {}
	align_t operator() (void) const { return val; }
    };

    struct wrap {};

    struct padding : public packint4
    {
        padding() : packint4(0) {}
        padding(int left, int right, int top, int bottom) : packint4(left, right, top, bottom) {}

	int left(void) const { return val1(); }
	int right(void) const { return val2(); }
	int top(void) const { return val3(); }
	int bottom(void) const { return val4(); }
    };

    struct colors : public packint2
    {
	colors(const FBColors & fbc) : packint2(fbc.fg(), fbc.bg()) {}
	colors(const ColorIndex & fg, const ColorIndex & bg) : packint2(fg(), bg()) {}

	ColorIndex fgindex(void) const { return val1(); }
	ColorIndex bgindex(void) const { return val2(); }
    };

    struct rn {};
    struct flush {};
}

namespace reset
{
    struct fgcolor {};
    struct bgcolor {};
    struct colors {};
    struct padding {};
    struct wrap {};
    struct defaults {};
}

namespace fill
{
    struct area : public std::pair<int, packint2>
    {
	area(int val, const Size & sz) : std::pair<int, packint2>(val, packint2(sz.w, sz.h)) {}

	int 		value(void) const { return first; }
	int		width(void) const { return second.val1(); }
	int		height(void) const { return second.val2(); }
    };

    struct charset : public area
    {
	charset(int ch, const Size & sz = Size(1, 1)) : area(ch, sz) {}
    };

    struct space : public charset
    {
	space(int sz = 1) : charset(0x20, Size(sz, 1)) {}
    };

    struct fgcolor : public area
    {
	fgcolor(const ColorIndex & col, const Size & sz = Size(1, 1)) : area(col(), sz) {}
    };

    struct bgcolor : public area
    {
	bgcolor(const ColorIndex & col, const Size & sz = Size(1, 1)) : area(col(), sz) {}
    };

    struct colors : public area
    {
	colors(const FBColors & fbc, const Size & sz = Size(1, 1)) : area(packshort(fbc.bg(), fbc.fg()).value(), sz) {}
	colors(const ColorIndex & fg, const ColorIndex & bg, const Size & sz = Size(1, 1)) : area(packshort(bg(), fg()).value(), sz) {}
	ColorIndex	fgindex(void) const { return ColorIndex(packshort(value()).val2()); }
	ColorIndex	bgindex(void) const { return ColorIndex(packshort(value()).val1()); }
    };

    struct property : public area
    {
	property(int prop, const Size & sz = Size(1, 1)) : area(prop, sz) {}
    };

    struct defaults : public UnicodeColor
    {
	int property;

	defaults(const FBColors & fbc, int ch = 0x20, int prop = 0) : UnicodeColor(ch, fbc), property(prop) {}
	defaults(const ColorIndex & fg, const ColorIndex & bg, int ch = 0x20, int prop = 0) : UnicodeColor(ch, FBColors(fg, bg)), property(prop) {}
	int		prop(void) const { return property; }
    };
}

namespace draw
{
    struct hline : public packint2
    {
	hline(int count, int charset = acs::hline(LineThin)) : packint2(count, charset) {}

	int count(void) const { return val1(); }
	int charset(void) const { return val2(); }
    };

    struct vline : public packint2
    {
	vline(int count, int charset = acs::vline(LineThin)) : packint2(count, charset) {}

	int count(void) const { return val1(); }
	int charset(void) const { return val2(); }
    };

    struct rect : public Rect
    {
	line_t line;

	rect(const Rect & rt, line_t type) : Rect(rt), line(type) {}
	rect(int px, int py, int pw, int ph, line_t type) : Rect(px, py, pw, ph), line(type) {}
    };
}

struct TermPos : packshort
{
    TermPos() {}
    TermPos(int posx, int posy) : packshort(posx, posy) {}
    TermPos(const Point & pt) : packshort(pt.x, pt.y) {}

    int posx(void) const { return val1(); }
    int posy(void) const { return val2(); }

    Point toPoint(void) const { return Point(posx(), posy()); }
};

struct TermSize : packshort
{
    TermSize() {}
    TermSize(int cols, int rows) : packshort(cols, rows) {}
    TermSize(const Size & sz) : packshort(sz.w, sz.h) {}

    int cols(void) const { return val1(); }
    int rows(void) const { return val2(); }

    Size toSize(void) const { return Size(cols(), rows()); }
};

struct TermRect : TermPos, TermSize
{
    TermRect() {}
    TermRect(int posx, int posy, int cols, int rows) : TermPos(posx, posy), TermSize(cols, rows) {}
    TermRect(const Rect & rt) : TermPos(rt.x, rt.y), TermSize(rt.w, rt.h) {}

    Rect toRect(void) const { return Rect(toPoint(), toSize()); }
};

class TermBase : public Window
{
protected:
    const FontRender*		fontRender;
    set::padding		padding;
    packshort			curpos;
    packshort			termsz;
    packshort			defcols;
    packshort			termopt2; /* align, unused */

/*
protected:
    virtual bool        keyPressEvent(int) { return false; }
    virtual bool        keyReleaseEvent(int) { return false; }
    virtual bool        textInputEvent(const std::string &) { return false; }
    virtual bool        mousePressEvent(const ButtonEvent &) { return false; }
    virtual bool        mouseReleaseEvent(const ButtonEvent &) { return false; }
    virtual bool        mouseClickEvent(const ButtonsEvent &) { return false; }
    virtual void        mouseFocusEvent(void) {}
    virtual void        mouseLeaveEvent(void) {}
    virtual void        mouseMotionEvent(const Point &, u32 buttons) {}
    virtual bool        userEvent(int, void*) { return false; }
    virtual bool        scrollUpEvent(const Point &) { return false; }
    virtual bool        scrollDownEvent(const Point &) { return false; }
    virtual bool        scrollLeftEvent(const Point &) { return false; }
    virtual bool        scrollRightEvent(const Point &) { return false; }
    virtual void        tickEvent(u32 ms) {}
    virtual void        renderPresentEvent(u32 ms) {}
    virtual void        displayResizeEvent(const Size &);
    virtual void        windowVisibleEvent(bool);
*/

    // protected: empty font render
    TermBase();
    TermBase(TermBase &);

    bool		lineWrap(void) const;

    int			index(const Point &) const;
    int			index(void) const;



    void		setFGColor(const ColorIndex & col) { defcols.set1(col()); }
    void		setBGColor(const ColorIndex & col) { defcols.set2(col()); }

    ColorIndex		fgColor(void) const { return ColorIndex(defcols.val1()); }
    ColorIndex		bgColor(void) const { return ColorIndex(defcols.val2()); }
    FBColors		colors(void) const { return FBColors(fgColor(), bgColor()); }

    void		setAlign(align_t v) { termopt2.set1(v); }
    int			align(void) const { return termopt2.val1(); }

    void		setUnused8(int v) { termopt2.set2(v); }
    int			unused8(void) const { return termopt2.val2(); }

    const set::padding& paddings(void) const { return padding; }

public:
    TermBase(const FontRender &);
    TermBase(const FontRender &, Window &);
    TermBase(const Point & gfxpt, const Size & gfxsz, const FontRender &, Window &);

    void		setFontRender(const FontRender &);
    void                setSize(const Size &);

    virtual void        setTermSize(int cols, int rows); // max 256 symbols
    void		setCursorPos(int cx, int cy) { curpos.set1(cx); curpos.set2(cy); }
    Point		cursor(void) const { return Point(curpos.val1(), curpos.val2()); }
    virtual void	setCharset(int ch, const ColorIndex & fg = Color::Transparent, const ColorIndex & bg = Color::Transparent, int prop = 0) = 0;
    virtual void	renderFlush(void) = 0;


    inline int		cols(void) const { return termsz.val1(); }
    inline int		rows(void) const { return termsz.val2(); }

    Size		termSize(void) const { return Size(cols(), rows()); }
    const FontRender*	frs(void) const { return fontRender; }

    Point		sym2gfx(const Point &) const;   /* coord transformer: symbol to graphics (parent relative) */
    Point		gfx2sym(const Point &) const;   /* coord transformer: graphics to symbol (parent relative) */
    Size		sym2gfx(const Size &) const;    /* size transformer: symbol to graphics */
    Size		gfx2sym(const Size &) const;    /* size transformer: graphics to symbol */
    Rect		sym2gfx(const Rect &) const;
    Rect		gfx2sym(const Rect &) const;

    TermBase & operator<< (const fill::defaults &);
    TermBase & operator<< (const fill::fgcolor &);
    TermBase & operator<< (const fill::bgcolor &);
    TermBase & operator<< (const fill::colors &);
    TermBase & operator<< (const fill::charset &);
    TermBase & operator<< (const fill::property &);

    TermBase & operator<< (const cursor::set &);
    TermBase& operator<< (const cursor::move &);

    TermBase & operator<< (const set::colors &);
    TermBase & operator<< (const set::fgcolor &);
    TermBase & operator<< (const set::bgcolor &);
    TermBase & operator<< (const set::align &);
    TermBase & operator<< (const set::padding &);
    TermBase & operator<< (const set::wrap &);
    TermBase & operator<< (const set::rn &);
    TermBase & operator<< (const set::flush &);

    TermBase & operator<< (const reset::defaults &);
    TermBase & operator<< (const reset::colors &);
    TermBase & operator<< (const reset::fgcolor &);
    TermBase & operator<< (const reset::bgcolor &);
    TermBase & operator<< (const reset::padding &);
    TermBase & operator<< (const reset::wrap &);

    TermBase & operator<< (const draw::hline &);
    TermBase & operator<< (const draw::vline &);
    TermBase & operator<< (const draw::rect &);

    TermBase & operator<< (int);
    TermBase & operator<< (const char*);
    TermBase & operator<< (const std::string &);
    TermBase & operator<< (const UnicodeString &);
    TermBase & operator<< (const UCString &);
    TermBase & operator<< (const UnicodeList &);
    TermBase & operator<< (const UCStringList &);

    TermBase & operator<< (const UnicodeColor &);

    virtual void        renderWindow(void);
};

class TermWindow : public TermBase
{
protected:
    std::vector<UnicodeColor>	chars;

    TermWindow() {}
    TermWindow(TermBase & term) : TermBase(term) {} // FIXED: remove

public:
    TermWindow(const FontRender & frs) : TermBase(frs) {}
    TermWindow(const FontRender & frs, Window & win) : TermBase(frs, win) {}
    TermWindow(const Point & gfxpos, const Size & gfxsz, const FontRender & frs, Window & win) : TermBase(gfxpos, gfxsz, frs, win) {}

    void		setTermSize(int cols, int row);
    void		setCharset(int ch, const ColorIndex & fg = Color::Transparent, const ColorIndex & bg = Color::Transparent, int prop = 0);

    void		renderSymbol(int symx, int symy);
    void		renderFlush(void);
};

class TermArea : public TermBase
{
protected:
    packshort		termpos;

public:
    TermArea(TermWindow & term) : TermBase(term) {}
    TermArea(int symx, int symy, int cols, int rows, TermWindow & term) : TermBase(term) { setTermArea(symx, symy, cols, rows); }

    void		setTermPos(int symx, int symy);
    void		setPosition(const Point &);
    void		setTermArea(int symx, int symy, int cols, int rows);

    inline int		posx(void) const { return termpos.val1(); }
    inline int		posy(void) const { return termpos.val2(); }

    void		setCharset(int ch, const ColorIndex & fg = Color::Transparent, const ColorIndex & bg = Color::Transparent, int prop = 0);
    void		renderFlush(void);
};

class CenteredTerminal : public TermWindow
{
public:
    CenteredTerminal(int cols, int rows, const FontRender &, Window &);
};

// void DisplayError(const std::string & hdr, const std::string & msg);
// void MessageTop(const std::string & hdr, const std::string & msg);

#endif

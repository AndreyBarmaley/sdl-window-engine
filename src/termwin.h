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
	right(int count = 1) : move(MoveUp, count) {}
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
    struct dirty {};
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

class TermCharset : public UnicodeColor, public BitFlags
{
public:
    enum { PropNone, PropDirty };

    TermCharset() : UnicodeColor(0x20, FBColors(Color::White)), BitFlags(TermCharset::PropDirty) {}
    TermCharset(const UnicodeColor & uc) : UnicodeColor(uc), BitFlags(TermCharset::PropDirty) {}

    int				prop(void) const { return BitFlags::value(); }
    void			set(int v) { BitFlags::set(v); }
};

class TermWindow : public Window
{
protected:
    std::vector<TermCharset>	chars;
    const FontRender*		fontRender;
    Size			termsz;
    ColorIndex			fgColor;
    ColorIndex			bgColor;
    set::padding		padding;
    int				align;
    Point			curpos;
/*
protected:
    virtual bool        keyPressEvent(int) { return false; }
    virtual bool        keyReleaseEvent(int) { return false; }
    virtual bool        textInputEvent(const std::string &) { return false; }
    virtual bool        mousePressEvent(const ButtonEvent &) { return false; }
    virtual bool        mouseReleaseEvent(const ButtonEvent &) { return false; }
    virtual bool        mouseClickEvent(const ButtonsEvent &) { return false; }
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
*/
    // protected: empty font render
    TermWindow();

    void		setCharset(int ch, const ColorIndex & fg = Color::Transparent, const ColorIndex & bg = Color::Transparent, int prop = 0);
    bool		lineWrap(void) const;

    int			index(const Point &) const;
    int			index(void) const;

public:
    TermWindow(TermWindow &);
    TermWindow(const FontRender &);
    TermWindow(const FontRender &, Window &);
    TermWindow(const Point & gfxpt, const Size & gfxsz, const FontRender &, Window &);

    void		setFontRender(const FontRender &);
    void                setSize(const Size &);
    void		renderFlush(void);

    const Point &	cursor(void) const;
    FBColors		colors(void) const { return FBColors(fgColor, bgColor); }
    size_t		cols(void) const { return termsz.w; }
    size_t 		rows(void) const { return termsz.h; }
    const FontRender*	frs(void) const { return fontRender; }


    Point		sym2gfx(const Point &) const;   /* coord transformer: symbol to graphics (parent relative) */
    Point		gfx2sym(const Point &) const;   /* coord transformer: graphics to symbol (parent relative) */
    Size		sym2gfx(const Size &) const;    /* size transformer: symbol to graphics */
    Size		gfx2sym(const Size &) const;    /* size transformer: graphics to symbol */
    Rect		sym2gfx(const Rect &) const;
    Rect		gfx2sym(const Rect &) const;

    TermWindow &        operator<< (const cursor::set &);
    TermWindow &        operator<< (const cursor::move &);

    TermWindow &        operator<< (const fill::defaults &);
    TermWindow &        operator<< (const fill::fgcolor &);
    TermWindow &        operator<< (const fill::bgcolor &);
    TermWindow &        operator<< (const fill::colors &);
    TermWindow &        operator<< (const fill::charset &);
    TermWindow &        operator<< (const fill::property &);

    TermWindow &        operator<< (const set::colors &);
    TermWindow &        operator<< (const set::fgcolor &);
    TermWindow &        operator<< (const set::bgcolor &);
    TermWindow &        operator<< (const set::align &);
    TermWindow &        operator<< (const set::padding &);
    TermWindow &        operator<< (const set::wrap &);
    TermWindow &        operator<< (const set::rn &);
    TermWindow &        operator<< (const set::dirty &);
    TermWindow &        operator<< (const set::flush &);

    TermWindow &        operator<< (const reset::defaults &);
    TermWindow &        operator<< (const reset::colors &);
    TermWindow &        operator<< (const reset::fgcolor &);
    TermWindow &        operator<< (const reset::bgcolor &);
    TermWindow &        operator<< (const reset::padding &);
    TermWindow &        operator<< (const reset::wrap &);

    TermWindow &        operator<< (const draw::hline &);
    TermWindow &        operator<< (const draw::vline &);
    TermWindow &        operator<< (const draw::rect &);

    TermWindow &        operator<< (int);
    TermWindow &        operator<< (const char*);
    TermWindow &        operator<< (const std::string &);
    TermWindow &        operator<< (const UnicodeString &);
    TermWindow &        operator<< (const UCString &);
    TermWindow &        operator<< (const UnicodeList &);

    TermWindow &        operator<< (const UnicodeColor &);
    TermWindow &        operator<< (const TermCharset &);

    virtual void        renderWindow(void);
};

class CenteredTerminal : public TermWindow
{
public:
    CenteredTerminal(const Size & termsz, const FontRender &, Window &);
};

class MessageWindow : public CenteredTerminal
{
    UnicodeString	header;
    UnicodeString	content;

    ColorIndex          headerColor;
    ColorIndex          contentColor;
    ColorIndex          backgroundColor;
    ColorIndex          borderColor;

protected:
    bool                keyPressEvent(int);
    bool                mouseClickEvent(const ButtonsEvent &);
    bool                actionDialogClose(void);

public:
    MessageWindow(const std::string & header, const ColorIndex &, const std::string &, const ColorIndex &, const ColorIndex &, const ColorIndex &, const FontRender &, Window &);

    void                renderWindow(void);
};

void DisplayError(const std::string & hdr, const std::string & msg);
void MessageTop(const std::string & hdr, const std::string & msg);

#endif

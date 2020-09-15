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

#include "swe_display.h"
#include "swe_termwin.h"

namespace SWE
{
    int acs::vline(line_t type)
    {
        switch(type)
        {
            case LineThin:
                return 0x2502;

            case LineBold:
                return 0x2503;

            case LineDouble:
                return 0x2551;

            default:
                break;
        }

        return 179;
    }

    int acs::hline(line_t type)
    {
        switch(type)
        {
            case LineThin:
                return 0x2500;

            case LineBold:
                return 0x2501;

            case LineDouble:
                return 0x2550;

            default:
                break;
        }

        return 196;
    }

    int acs::ulcorner(line_t type)
    {
        switch(type)
        {
            case LineThin:
                return 0x250C;

            case LineBold:
                return 0x250F;

            case LineDouble:
                return 0x2554;

            default:
                break;
        }

        return 218;
    }

    int acs::urcorner(line_t type)
    {
        switch(type)
        {
            case LineThin:
                return 0x2510;

            case LineBold:
                return 0x2513;

            case LineDouble:
                return 0x2557;

            default:
                break;
        }

        return 191;
    }

    int acs::llcorner(line_t type)
    {
        switch(type)
        {
            case LineThin:
                return 0x2514;

            case LineBold:
                return 0x2517;

            case LineDouble:
                return 0x255A;

            default:
                break;
        }

        return 192;
    }

    int acs::lrcorner(line_t type)
    {
        switch(type)
        {
            case LineThin:
                return 0x2518;

            case LineBold:
                return 0x251B;

            case LineDouble:
                return 0x255D;

            default:
                break;
        }

        return 217;
    }

    int acs::ltee(line_t type)
    {
        switch(type)
        {
            case LineThin:
                return 0x251C;

            case LineBold:
                return 0x2523;

            case LineDouble:
                return 0x2560;

            default:
                break;
        }

        return 195;
    }

    int acs::rtee(line_t type)
    {
        switch(type)
        {
            case LineThin:
                return 0x2524;

            case LineBold:
                return 0x252B;

            case LineDouble:
                return 0x2563;

            default:
                break;
        }

        return 180;
    }

    int acs::ttee(line_t type)
    {
        switch(type)
        {
            case LineThin:
                return 0x252C;

            case LineBold:
                return 0x2533;

            case LineDouble:
                return 0x2566;

            default:
                break;
        }

        return 194;
    }

    int acs::btee(line_t type)
    {
        switch(type)
        {
            case LineThin:
                return 0x2534;

            case LineBold:
                return 0x253B;

            case LineDouble:
                return 0x2569;

            default:
                break;
        }

        return 193;
    }

    int acs::plus(line_t type)
    {
        switch(type)
        {
            case LineThin:
                return 0x253C;

            case LineBold:
                return 0x254B;

            case LineDouble:
                return 0x256C;

            default:
                break;
        }

        return 197;
    }

    /* TermBase */
    TermBase::TermBase(const FontRender & font, Window* win)
	: Window(win), fontRender(& font)
    {
        if(fontRender->size().isEmpty())
        {
            fontRender = & systemFont();
            ERROR("font size empty" << ", " << "used system font");
        }

        *this << reset::defaults();
    }

    TermBase::TermBase(const Size & gfxsz, const FontRender & font, Window* win)
	: Window(win), fontRender(& font)
    {
        if(fontRender->size().isEmpty())
        {
            fontRender = & systemFont();
            ERROR("font size empty" << ", " << "used system font");
        }

        setTermSize(gfx2sym(gfxsz));
        *this << reset::defaults();
    }

    TermBase::TermBase(const TermSize & tsz, TermBase & term)
	: Window(& term), fontRender(term.frs())
    {
        setTermSize(tsz);
        *this << reset::defaults();
    }

    TermBase::TermBase(TermBase* term)
	: Window(term), fontRender(term ? term->frs() : nullptr)
    {
        *this << reset::defaults();
    }

    CharsetProperty TermBase::defaultProperty(void) const
    {
        auto term = dynamic_cast<const TermBase*>(parent());
        return term ? term->defaultProperty() : CharsetProperty(RenderSolid, StyleNormal, HintingNormal);
    }

    FBColors TermBase::defaultColors(void) const
    {
        auto term = dynamic_cast<const TermBase*>(parent());
        return term ? term->defaultColors() : FBColors(fgColor(), bgColor());
    }

    void TermBase::setTermPos(const TermPos & tp)
    {
	auto win = parent();
        auto term = dynamic_cast<TermBase*>(win);

        if(term)
        {
	    setPosition(term->sym2gfx(tp));
        }
        else
	if(win)
        {
	    setPosition(sym2gfx(tp));
        }

        termrt.setPos(tp.posx(), tp.posy());
    }

    void TermBase::setTermSize(const TermSize & tsz)
    {
        termrt.setSize(tsz.cols(), tsz.rows());
        Window::setSize(sym2gfx(termSize()));
    }

    void TermBase::setSize(const Size & sz)
    {
        setTermSize(gfx2sym(sz));
    }

    void TermBase::setCursorPos(const TermPos & tp)
    {
        curpos = tp;
    }

    void TermBase::resetCursorPos(void)
    {
        curpos = TermPos();
    }

    const TermPos & TermBase::cursor(void) const
    {
        return curpos;
    }

    size_t TermBase::cols(void) const
    {
	return termSize().cols();
    }

    size_t TermBase::rows(void) const
    {
	return termSize().rows();
    }

    const TermPos & TermBase::termPos(void) const
    {
        return termrt;
    }

    const TermSize & TermBase::termSize(void) const
    {
        return termrt;
    }

    const FontRender* TermBase::frs(void) const
    {
        return fontRender;
    }

    void TermBase::setFGColor(const ColorIndex & col)
    {
        curcols.set1(col());
    }

    void TermBase::setBGColor(const ColorIndex & col)
    {
        curcols.set2(col());
    }
        
    ColorIndex TermBase::fgColor(void) const
    {
        return ColorIndex(curcols.val1());
    }

    ColorIndex TermBase::bgColor(void) const
    {
        return ColorIndex(curcols.val2());
    }

    FBColors TermBase::colors(void) const
    {
        return FBColors(fgColor(), bgColor());
    }

    void TermBase::setAlign(align_t v)
    {
        termopt.set1(v);
    }

    int TermBase::align(void) const
    {
        return termopt.val1();
    }

    void TermBase::setProperty(const CharsetProperty & prop)
    {
        termopt.set2(prop());
    }

    CharsetProperty TermBase::property(void) const
    {
	CharsetProperty prop;
        prop.val = termopt.val2();
	return prop;
    }

    const set::padding & TermBase::paddings(void) const
    {
        return padding;
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
            setSize(0 == rows() && 0 == cols() ? Display::size() : sym2gfx(termSize()));
        }
    }

    bool TermBase::lineWrap(void) const
    {
        return checkState(FlagWrap);
    }

    Point TermBase::sym2gfx(const TermPos & sym) const /* coordinate from current win */
    {
        return position() + sym.toPoint() * fontRender->size();
    }

    TermPos TermBase::gfx2sym(const Point & gfx) const /* coordinate from current win */
    {
        return TermPos((gfx - position()) / fontRender->size());
    }

    Size TermBase::sym2gfx(const TermSize & sym) const
    {
        return sym.toSize() * fontRender->size();
    }

    TermSize TermBase::gfx2sym(const Size & gfx) const
    {
        Size res = gfx / fontRender->size();

        // fix float values
        if(res.w * fontRender->size().w < gfx.w) res.w += 1;

        if(res.h * fontRender->size().h < gfx.h) res.h += 1;

        return TermSize(res);
    }

    Rect TermBase::sym2gfx(const TermRect & sym) const
    {
        const TermPos & sympt = sym;
        const TermSize & symsz = sym;
        return Rect(sym2gfx(sympt), sym2gfx(symsz));
    }

    TermRect TermBase::gfx2sym(const Rect & gfx) const
    {
        const Point & gfxpt = gfx;
        const Size & gfxsz = gfx;
        return TermRect(gfx2sym(gfxpt), gfx2sym(gfxsz));
    }

    void TermBase::renderWindow(void)
    {
        *this << set::flush();
    }

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

        setCursorPos(TermPos(cx, cy));
        return *this;
    }

    TermBase & TermBase::operator<< (const cursor::move & st)
    {
        if(st.count())
        {
            const TermPos & cur = cursor();

            switch(st.dir())
            {
                case MoveFirst:
                    return *this << cursor::set(0, cur.posy());
                    break;

                case MoveLast:
                    return *this << cursor::set(cols() - 1, cur.posy());
                    break;

                case MoveUp:
                    return *this << cursor::set(cur.posx(), cur.posy() - st.count());
                    break;

                case MoveDown:
                    return *this << cursor::set(cur.posx(), cur.posy() + st.count());
                    break;

                case MoveLeft:
                    return *this << cursor::set(cur.posx() - st.count(), cur.posy());
                    break;

                case MoveRight:
                    return *this << cursor::set(cur.posx() + st.count(), cur.posy());
                    break;

                case MoveUpperLeft:
                    return *this << cursor::move(MoveUp, st.count()) << cursor::move(MoveLeft, st.count());

                case MoveUpperRight:
                    return *this << cursor::move(MoveUp, st.count()) << cursor::move(MoveRight, st.count());

                case MoveLowerLeft:
                    return *this << cursor::move(MoveDown, st.count()) << cursor::move(MoveLeft, st.count());

                case MoveLowerRight:
                    return *this << cursor::move(MoveDown, st.count()) << cursor::move(MoveRight, st.count());

                default:
                    break;
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
        return *this << cursor::set(st.posx(), st.posy()) << acs::ulcorner(st.line) << draw::hline(st.cols() - 2, acs::hline(st.line)) << acs::urcorner(st.line) <<
               cursor::set(st.posx(), st.posy() + 1) << draw::vline(st.rows() - 2, acs::vline(st.line)) <<
               cursor::set(st.posx() + st.cols() - 1, st.posy() + 1) << draw::vline(st.rows() - 2, acs::vline(st.line)) <<
               cursor::set(st.posx(), st.posy() + st.rows() - 1) << acs::llcorner(st.line) << draw::hline(st.cols() - 2, acs::hline(st.line)) << acs::lrcorner(st.line);
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
        int curx = cursor().posx();

        for(auto it = st.begin(); it != st.end(); ++it)
        {
            *this << *it;
            *this << cursor::set(curx, cursor().posy() + 1);
        }

        return *this;
    }

    TermBase & TermBase::operator<< (const UCStringList & st)
    {
        int curx = cursor().posx();

        for(auto it = st.begin(); it != st.end(); ++it)
        {
            *this << *it;
            *this << cursor::set(curx, cursor().posy() + 1);
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

        Point cur = cursor().toPoint();

        if(align() == AlignCenter)
            cur.x -= st.size() / 2;
        else if(align() == AlignRight)
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

        Point cur = cursor().toPoint();

        if(align() == AlignCenter)
            cur.x -= ucs.length() / 2;
        else if(align() == AlignRight)
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
        Point start = cursor().toPoint();

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
        for(int posy = 0; posy < rows(); ++posy)
        {
            for(int posx = 0; posx < cols(); ++posx)
            {
                *this << cursor::set(posx, posy);
                setCharset(st.unicode(), st.fgindex(), st.bgindex(), & st.prop());
            }
        }

        resetCursorPos();
        return *this;
    }

    TermBase & TermBase::operator<< (const fill::fgcolor & st)
    {
        Point start = cursor().toPoint();

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
        Point start = cursor().toPoint();

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
        Point start = cursor().toPoint();

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
        Point start = cursor().toPoint();

        for(int posy = start.y; posy < start.y + st.height(); ++posy)
        {
            for(int posx = start.x; posx < start.x + st.width(); ++posx)
            {
                *this << cursor::set(posx, posy);
                setCharset(-1, Color::Transparent, Color::Transparent, & st.toProperty());
            }
        }

        return *this;
    }

    TermBase & TermBase::operator<< (const reset::defaults & st)
    {
        resetCursorPos();
        setFGColor(Color::Transparent);
        setBGColor(Color::Transparent);
        setAlign(AlignLeft);
	setProperty(CharsetProperty());
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

    TermBase & TermBase::operator<< (const reset::align & st)
    {
        setAlign(AlignLeft);
        return *this;
    }

    TermBase & TermBase::operator<< (const reset::property & st)
    {
	setProperty(CharsetProperty());
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

    TermBase & TermBase::operator<< (const set::property & st)
    {
        setProperty(st.prop);
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

#ifdef SWE_WITH_JSON
    JsonObject TermBase::toJson(void) const
    {
        JsonObject res = Window::toJson();
        res.addArray("curpos", JsonPack::point(curpos.toPoint()));
        res.addArray("termsz", JsonPack::size(termSize().toSize()));
        res.addObject("curcols", JsonPack::fbColors(colors()));
        res.addInteger("align", align());

	JsonObject joPadding;
	joPadding.addInteger("left", padding.left());
	joPadding.addInteger("right", padding.right());
	joPadding.addInteger("top", padding.top());
	joPadding.addInteger("bottom", padding.bottom());
        res.addObject("padding", joPadding);

	JsonObject joProp;
	CharsetProperty prop = property();
	joProp.addInteger("render", prop.render());
	joProp.addInteger("style", prop.style());
	joProp.addInteger("hinting", prop.hinting());
        res.addObject("property", joProp);

        return res;
    }
#endif

    /* TermWindow */
    TermWindow::TermWindow(const FontRender & frs, Window* win) : TermBase(frs, win)
    {
        *this << reset::defaults();

	// set full window terminal
	if(win == nullptr)
	{
	    setTermSize(gfx2sym(Display::size()));
	    setVisible(true);
	}
    }

    TermWindow::TermWindow(const Size & gfxsz, const FontRender & frs, Window* win) : TermBase(gfxsz, frs, win)
    {
        chars.resize(rows() * cols(), TermCharset(UnicodeColor(0x20, defaultColors()), CharsetProperty()));
        *this << reset::defaults();
	setVisible(true);
    }

    TermWindow::TermWindow(const TermSize & tsz, TermBase & term) : TermBase(tsz, term)
    {
        chars.resize(rows() * cols(), TermCharset(UnicodeColor(0x20, defaultColors()), CharsetProperty()));
        *this << reset::defaults();
	setVisible(true);
    }

    void TermWindow::displayResizeEvent(const Size & sz, bool sdl)
    {
	// resized sdl window, cols and rows changed, maybe invisible positions
	if(sdl)
	{
            TermSize termsz1 = gfx2sym(sz);
            TermSize termsz2 = minimalTerminalSize();
            Size gfxsz = sym2gfx(termsz1.cols() < termsz2.cols() ||
                                termsz1.rows() < termsz2.rows() ? termsz2 : termsz1);

    	    Display::resize(gfxsz);

	    if(parent())
	    {
		setTermSize(termSize());
		setTermPos(termPos());
	    }
	    else
            {
    		setSize(gfxsz);
                DEBUG("resized, cols: " << cols() << ", rows: " << rows());
            }

            terminalResizeEvent();
	}
	else
	// resized font size, restore terminal size,pos
	{
            Window::setSize(sym2gfx(termSize()));

	    if(parent())
		setTermPos(termPos());

            fontResizeEvent();
	}

	renderWindow();
    }

    FBColors TermWindow::defaultColors(void) const
    {
	return FBColors(Color::Silver, Color::DimGray);
    }

    void TermWindow::setTermSize(const TermSize & tsz)
    {
	TermBase::setTermSize(tsz);
        chars.resize(rows() * cols());
	// geometry changed, reset content
	*this << fill::defaults(defaultColors(), 0x20, defaultProperty()) << cursor::set(0, 0);
    }

    int TermWindow::index(const TermPos & tp) const
    {
        if(tp.posx() < cols() && tp.posy() < rows())
            return cols() * tp.posy() + tp.posx();

        return -1;
    }

    int TermWindow::index(void) const
    {
        const TermPos & cur = cursor();
        return cols() * cur.posy() + cur.posx();
    }

    const TermCharset* TermWindow::charset(const TermPos & tp) const
    {
        int pos = index(tp);

        if(pos < 0 || pos >= static_cast<int>(chars.size()))
        {
            ERROR("out of range: " << ", " << "pos: " << pos << ", " << "size: " << chars.size());
            return nullptr;
        }

        return & chars[pos];
    }

    const TermCharset* TermWindow::charset(void) const
    {
	return charset(cursor());
    }

    void TermWindow::setCharset(int ch, const ColorIndex & fg, const ColorIndex & bg, const CharsetProperty* prop)
    {
        const Rect & termArea = Rect(padding.left(), padding.top(),
                                     cols() - (padding.left() + padding.right()), rows() - (padding.top() + padding.bottom()));

        if(termArea & cursor().toPoint())
        {
            int pos = index();

            if(pos < 0 || pos >= static_cast<int>(chars.size()))
            {
                ERROR("out of range: " << ", " << "pos: " << pos << ", " << "size: " << chars.size());
                return;
            }

            UnicodeColor & tc = chars[pos].charset();

            if(ch != -1) tc.unicode(ch);

	    if(! fg.isTransparent()) tc.fgindex(fg);
            else
            if(! fgColor().isTransparent()) tc.fgindex(fgColor());

	    if(! bg.isTransparent()) tc.bgindex(bg);
            else
            if(! bgColor().isTransparent()) tc.bgindex(bgColor());

	    if(prop) chars[pos].setProperty(*prop);
            else
            if(property().val) chars[pos].setProperty(property());
        }

        *this << cursor::right(1);
    }

    void TermWindow::renderSymbol(int symx, int symy)
    {
        const TermPos sympt(symx, symy);
        int pos = index(sympt);

        if(0 <= pos)
        {
            if(pos >= static_cast<int>(chars.size()))
            {
                ERROR("out of range: " << ", " << "pos: " << pos << ", " << "size: " << chars.size());
                return;
            }

            const UnicodeColor & uc = chars[pos].charset();
            const CharsetProperty & cp = chars[pos].property();
            const Color & bgcolor = uc.bgcolor();
            const Point relPos = sym2gfx(sympt) - position();

            if(! bgcolor.isTransparent())
                renderColor(bgcolor, Rect(relPos, sym2gfx(Size(1, 1))));

            if(uc.unicode() > 0x20 && ! uc.fgcolor().isTransparent())
                renderTexture(FontsCache(fontRender).renderCharset(uc.unicode(), uc.fgcolor(), cp.render(), cp.style(), cp.hinting()), relPos);
        }
        else
            ERROR("out of range: " << "symx: " << symx << ", " << "symy: " << symy);
    }

    void TermWindow::renderFlush(void)
    {
        for(int symy = 0; symy < rows(); ++symy)
            for(int symx = 0; symx < cols(); ++symx) renderSymbol(symx, symy);
    }


    void TermWindow::dumpState(void) const
    {
	Window::dumpState();

        for(int symy = 0; symy < rows(); ++symy)
	{
    	    VERBOSE("row[" << symy << "]");

            for(int symx = 0; symx < cols(); ++symx)
	    {
		const TermCharset* ch = charset(TermPos(symx, symy));
		if(ch)
		{
    		    auto & uc = ch->charset();
    		    auto & prop = ch->property();
        
    		    VERBOSE("col[" << symx << "]: " << SWE::StringFormat("unicode: %1, fgcolor: %2, bgcolor: %3, blend: %4, style: %5, hinting: %6").
                        arg(SWE::String::hex(uc.unicode(), 4)).arg(uc.fgcolor().toString()).arg(uc.bgcolor().toString()).
                        arg(prop.render()).arg(SWE::String::hex(prop.style(), 2)).arg(prop.hinting()));
		}
	    }
	}
    }

#ifdef SWE_WITH_JSON
    JsonObject TermWindow::toJson(void) const
    {
        JsonObject res = TermBase::toJson();
	JsonArray joContent;

        for(int symy = 0; symy < rows(); ++symy)
	{
            for(int symx = 0; symx < cols(); ++symx)
	    {
		const TermCharset* ch = charset(TermPos(symx, symy));
		if(ch)
		{
    		    auto & uc = ch->charset();
    		    auto & prop = ch->property();

		    JsonObject joChar;
		    joChar.addString("unicode", String::hex(uc.unicode(), 4));
		    joChar.addObject("colors", JsonPack::fbColors(uc.colors()));

		    JsonObject joProp;
		    joProp.addInteger("render", prop.render());
		    joProp.addInteger("style", prop.style());
		    joProp.addInteger("hinting", prop.hinting());
    		    joChar.addObject("property", joProp);

		    joContent.addObject(joChar);
		}
	    }
	}

        res.addArray("content", joContent);
	return res;
    }
#endif

    /* TermArea */
    void TermArea::setTermPos(const TermPos & tp)
    {
        termpos = tp;
    }

    void TermArea::setPosition(const Point & pos)
    {
        TermWindow* term = dynamic_cast<TermWindow*>(parent());

        if(term) setCursorPos(term->gfx2sym(pos));

        Window::setPosition(pos);
    }

    void TermArea::setTermArea(const TermRect & tr)
    {
        setTermPos(tr);
        setTermSize(tr);
    }

    void TermArea::setTermArea(int symx, int symy, int cols, int rows)
    {
        setTermPos(TermPos(symx, symy));
        setTermSize(TermSize(cols, rows));
    }

    void TermArea::setCharset(int ch, const ColorIndex & fg, const ColorIndex & bg, const CharsetProperty* prop)
    {
        TermWindow* term = dynamic_cast<TermWindow*>(parent());

        if(term)
        {
            const TermPos savePos = term->cursor();
            const TermPos & cur = cursor();
            term->setCursorPos(termpos + cur);
            ColorIndex fgcol = ! fgColor().isTransparent() ? fgColor() : fg;
            ColorIndex bgcol = ! bgColor().isTransparent() ? bgColor() : bg;

            if(padding())
            {
                const Rect & termArea = Rect(padding.left(), padding.left(),
                                             cols() - (padding.left() + padding.right()), rows() - (padding.left() + padding.bottom()));

                if(termArea & cursor().toPoint())
                    term->setCharset(ch, fgcol, bgcol, prop);
            }
            else
                term->setCharset(ch, fgcol, bgcol, prop);

            term->setCursorPos(savePos);
            *this << cursor::right(1);
        }
    }

    void TermArea::renderFlush(void)
    {
        TermWindow* term = dynamic_cast<TermWindow*>(parent());

        if(term)
        {
            for(int py = 0; py < rows(); ++py)
                for(int px = 0; px < cols(); ++px)
                    term->renderSymbol(posx() + px, posy() + py);
        }
    }

    /* CenteredTerminal */
    CenteredTerminal::CenteredTerminal(const TermSize & tsz, const FontRender & font, Window & win)
        : TermWindow(tsz.toSize() * font.size(), font, & win)
    {
	setPosition((win.size() - size()) / 2);
    }
}

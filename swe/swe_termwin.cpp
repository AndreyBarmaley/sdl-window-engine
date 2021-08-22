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

#include <cmath>
#include <algorithm>

#include "swe_display.h"
#include "swe_display_scene.h"
#include "swe_termwin.h"

namespace SWE
{
    int acs::vline(const LineType & type)
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

        return '|';
    }

    int acs::hline(const LineType & type)
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

        return '-';
    }

    int acs::ulcorner(const LineType & type)
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

        return '+';
    }

    int acs::urcorner(const LineType & type)
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

        return '+';
    }

    int acs::llcorner(const LineType & type)
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

        return '+';
    }

    int acs::lrcorner(const LineType & type)
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

        return '+';
    }

    int acs::ltee(const LineType & type)
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

        return '|';
    }

    int acs::rtee(const LineType & type)
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

        return '|';
    }

    int acs::ttee(const LineType & type)
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

        return '-';
    }

    int acs::btee(const LineType & type)
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

        return '-';
    }

    int acs::plus(const LineType & type)
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

        return '+';
    }

    /* CharState */
    bool CharState::checkState(const Type & st) const
    {
	return state & st;
    }

    void CharState::setState(const Type & st, bool f)
    {
        if(f)
            state |= st;
        else
            state &= ~st;
    }

    void CharState::setAlpha(int val)
    {
	if(val)
	{
	    state &= 0x0F;
	    int alpha = std::round((val & 0xFF) / 17.f);
	    state |= (alpha << 4);
	}
	else
	{
	    state &= 0x0F;
	}
    }

    int CharState::alpha(void) const
    {
	int val = state >> 4;
	return val * 17;
    }

    /* TermBase */
    TermBase::TermBase(Window* win)
        : Window(win), curalign(AlignLeft)
    {
        *this << reset::defaults();
    }

    TermBase::TermBase(const Size & gfxsz, Window* win)
        : Window(win), curalign(AlignLeft)
    {
        *this << reset::defaults();
    }

    TermBase::TermBase(const TermSize & tsz, TermBase & term)
        : Window(& term), curalign(AlignLeft)
    {
        *this << reset::defaults();
    }

    TermBase::TermBase(TermBase* term)
        : Window(term), curalign(AlignLeft)
    {
        *this << reset::defaults();
    }

    CharProperty TermBase::defaultProperty(void) const
    {
        auto term = dynamic_cast<const TermBase*>(parent());
        return term ? term->defaultProperty() : CharProperty(RenderSolid, StyleNormal, HintingNormal);
    }

    FBColors TermBase::defaultColors(void) const
    {
        auto term = dynamic_cast<const TermBase*>(parent());
        return term ? term->defaultColors() : FBColors(fgColor(), bgColor());
    }

    void TermBase::setTermPos(const TermBase & term, const TermPos & tp)
    {
        setPosition(tp.toPoint() * term.frs()->size());
    }

    void TermBase::setTermSize(const TermSize & tsz)
    {
        termsz.setSize(tsz.cols(), tsz.rows());
	terminalResizeEvent();

        if(Display::isMaximizedWindow() && parent() == nullptr && size() == Display::size())
            return;

        // only childs or resize
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

    TermPos TermBase::termPos(const TermBase & term) const
    {
        auto diff = position() - term.position();
        return TermPos(diff / term.frs()->size());
    }

    const TermSize & TermBase::termSize(void) const
    {
        return termsz;
    }

    void TermBase::setFGColor(const ColorIndex & col)
    {
        curcols.setfg(col);
    }

    void TermBase::setBGColor(const ColorIndex & col)
    {
        curcols.setbg(col);
    }
        
    ColorIndex TermBase::fgColor(void) const
    {
        return curcols.fg();
    }

    ColorIndex TermBase::bgColor(void) const
    {
        return curcols.bg();
    }

    const FBColors & TermBase::colors(void) const
    {
        return curcols;
    }

    void TermBase::setBlink(bool f)
    {
	curstate.setState(CharState::Blinked, f);
    }

    bool TermBase::blink(void) const
    {
	return curstate.checkState(CharState::Blinked);
    }

    void TermBase::setInvert(bool f)
    {
	curstate.setState(CharState::Inverted, f);
    }

    bool TermBase::invert(void) const
    {
	return curstate.checkState(CharState::Inverted);
    }

    void TermBase::setFlip(int val, bool f)
    {
	if(CharState::FlipBoth & val)
	    curstate.setState(static_cast<CharState::Type>(CharState::FlipBoth & val), f);
    }

    int TermBase::flip(void) const
    {
	return CharState::FlipBoth & curstate.state;
    }

    void TermBase::setAlpha(int v)
    {
	curstate.setAlpha(v);
    }

    int TermBase::alpha(void) const
    {
	return curstate.alpha();
    }

    void TermBase::setAlign(const AlignType & v)
    {
        curalign = v;
    }

    const AlignType & TermBase::align(void) const
    {
        return curalign;
    }

    void TermBase::setProperty(const CharProperty & prop)
    {
        curprop = prop;
    }

    const CharProperty & TermBase::property(void) const
    {
        return curprop;
    }

    const set::padding & TermBase::paddings(void) const
    {
        return padding;
    }


    bool TermBase::lineWrap(void) const
    {
        return checkState(FlagWrap);
    }

    Point TermBase::sym2gfx(const TermPos & sym) const /* coordinate from current win */
    {
        return position() + sym.toPoint() * frs()->size();
    }

    TermPos TermBase::gfx2sym(const Point & gfx) const /* coordinate from current win */
    {
        return TermPos((gfx - position()) / frs()->size());
    }

    Size TermBase::sym2gfx(const TermSize & sym) const
    {
        return sym.toSize() * frs()->size();
    }

    TermSize TermBase::gfx2sym(const Size & gfx) const
    {
        Size res = gfx / frs()->size();

        // fix float values
        if(res.w * frs()->size().w < gfx.w) res.w += 1;

        if(res.h * frs()->size().h < gfx.h) res.h += 1;

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

    LineType TermBase::systemLine(const LineType & line) const
    {
        return frs() == & systemFont() ? LineType::LineAscii : line;
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
        return *this << cursor::set(st.posx(), st.posy()) << acs::ulcorner(systemLine(st.line)) << draw::hline(st.cols() - 2, acs::hline(systemLine(st.line))) <<
		acs::urcorner(systemLine(st.line)) <<
                cursor::set(st.posx(), st.posy() + 1) << draw::vline(st.rows() - 2, acs::vline(systemLine(st.line))) <<
                cursor::set(st.posx() + st.cols() - 1, st.posy() + 1) << draw::vline(st.rows() - 2, acs::vline(systemLine(st.line))) <<
                cursor::set(st.posx(), st.posy() + st.rows() - 1) << acs::llcorner(systemLine(st.line)) << draw::hline(st.cols() - 2, acs::hline(systemLine(st.line))) <<
		acs::lrcorner(systemLine(st.line));
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
        setProperty(CharProperty());
	setBlink(false);
	setInvert(false);
        setFlip(CharState::FlipBoth, false);
        setAlpha(0);
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
        setProperty(CharProperty());
        return *this;
    }

    TermBase & TermBase::operator<< (const reset::wrap & st)
    {
        resetState(FlagWrap);
        return *this;
    }

    TermBase & TermBase::operator<< (const reset::blink & st)
    {
        setBlink(false);
        return *this;
    }

    TermBase & TermBase::operator<< (const reset::invert & st)
    {
        setInvert(false);
        return *this;
    }

    TermBase & TermBase::operator<< (const reset::alpha & st)
    {
        setAlpha(0);
        return *this;
    }

    TermBase & TermBase::operator<< (const reset::flip & st)
    {
        setFlip(CharState::FlipBoth, false);
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

    TermBase & TermBase::operator<< (const set::blink & st)
    {
        setBlink(true);
        return *this;
    }

    TermBase & TermBase::operator<< (const set::invert & st)
    {
        setInvert(true);
        return *this;
    }

    TermBase & TermBase::operator<< (const set::flip & st)
    {
        setFlip(st.type, true);
        return *this;
    }

    TermBase & TermBase::operator<< (const set::alpha & st)
    {
        setAlpha(st.value);
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
        CharProperty prop = property();
        joProp.addInteger("render", prop.render());
        joProp.addInteger("style", prop.style());
        joProp.addInteger("hinting", prop.hinting());
        res.addObject("property", joProp);

        return res;
    }
#endif

    /* TermWindow */
    TermWindow::TermWindow(const FontRender & frs, Window* win) : TermBase(win), fontRender(& frs)
    {
        *this << reset::defaults();

        // set full window terminal
        if(win == nullptr)
        {
            setTermSize(gfx2sym(Display::size()));
	    setBlinkShow(true);
            setVisible(true);
        }
    }

    TermWindow::TermWindow(const Size & gfxsz, const FontRender & frs, Window* win) : TermBase(gfxsz, win), fontRender(& frs)
    {
        TermBase::setTermSize(gfx2sym(gfxsz));
        chars.resize(rows() * cols(), TermCharset(UnicodeColor(0x20, defaultColors()), CharProperty()));
        *this << reset::defaults();
	setBlinkShow(true);
        setVisible(true);
    }

    TermWindow::TermWindow(const TermSize & tsz, TermBase & term) : TermBase(tsz, term), fontRender(term.frs())
    {
        TermBase::setTermSize(tsz);
        chars.resize(rows() * cols(), TermCharset(UnicodeColor(0x20, defaultColors()), CharProperty()));
        *this << reset::defaults();
	setBlinkShow(true);
        setVisible(true);
    }

    bool TermWindow::blinkShow(void) const
    {
	return checkState(FlagInformed);
    }

    void TermWindow::setBlinkShow(bool f)
    {
	setState(FlagInformed, f);
    }

    const FontRender* TermWindow::frs(void) const
    {
        return fontRender;
    }

    void TermWindow::setFontRender(const FontRender & frs)
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

    void TermWindow::tickEvent(u32 ms)
    {
        if(std::any_of(chars.begin(), chars.end(), [](auto & tc){ return tc.blinked(); } ))
        {
	    if(! blinkShow())
	    {
	        // hide delay
	        if(tickBlink.check(ms, 300))
	        {
		    setBlinkShow(true);
		    renderWindow();
	        }
	    }
	    else
	    // show delay
	    if(tickBlink.check(ms, 700))
	    {
	        setBlinkShow(false);
	        renderWindow();
	    }
        }
    }

    void TermWindow::displayResizeEvent(const Size & winsz)
    {
	// only root win
	if(! parent())
	{
    	    TermSize termsz1 = gfx2sym(winsz);
	    TermSize termsz2 = minimalTerminalSize();

    	    // fix minsize
    	    if(termsz1.cols() < termsz2.cols() ||
                termsz1.rows() < termsz2.rows())
    	    {
        	// repeate display resize
        	Display::resize(sym2gfx(termsz2));
    	    }
    	    else
    	    {
		// newsz incr to chars
        	auto strongwinsz = termsz1.toSize() * frs()->size();
        	if(strongwinsz != winsz)
        	{
            	    // repeate display resize
            	    Display::resize(strongwinsz);
        	}
        	else
        	{
		    // displayResize move to terminalResize
		    setTermSize(termsz1);
        	    renderWindow();
        	}
    	    }
	}
    }

    void TermWindow::fontChangedHandle(void)
    {
        fontChangedEvent();
        auto maxsz = Display::usableBounds();

	// the size is limited, changes the content
        if(Display::isMaximizedWindow() || maxsz.w <= width() || maxsz.h <= height())
        {
	    TermSize termSz = gfx2sym(size());

            DEBUG("resized, cols: " << cols() << ", rows: " << rows());
    	    setTermSize(termSz);

            renderWindow();
        }
        else
        {
            const Size winsz = termSize().toSize() * frs()->size();
            Display::resize(winsz);
        }
    }

    FBColors TermWindow::defaultColors(void) const
    {
        return FBColors(Color::Silver, Color::DimGray);
    }

    void TermWindow::setTermSize(const TermSize & tsz)
    {
        TermBase::setTermSize(tsz);
        size_t newlen = rows() * cols();

        if(chars.size() != newlen)
        {
            chars.resize(newlen);
            // geometry changed, reset content
            *this << fill::defaults(defaultColors(), 0x20, defaultProperty()) << cursor::set(0, 0);
        }
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

    void TermWindow::setCharset(int ch, const ColorIndex & fg, const ColorIndex & bg, const CharProperty* prop)
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

            TermCharset & tc = chars[pos];

            if(ch != -1) tc.setUnicode(ch);

            if(! fg.isTransparent()) tc.setFGIndex(fg);
            else
            if(! fgColor().isTransparent()) tc.setFGIndex(fgColor());

            if(! bg.isTransparent()) tc.setBGIndex(bg);
            else
            if(! bgColor().isTransparent()) tc.setBGIndex(bgColor());

            if(prop) tc.setProperty(*prop);
            else
            if(property().val) tc.setProperty(property());

	    tc.setBlink(blink());
	    tc.setInvert(invert());
	    tc.setFlip(flip());
	    tc.setAlpha(alpha());
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

            TermCharset & tc = chars[pos];
            const CharProperty & cp = tc.property();
            Color bgcolor = tc.colors().bgcolor();
            Color fgcolor = tc.colors().fgcolor();

	    if(tc.inverted())
		std::swap(bgcolor, fgcolor);

            const Point relPos = sym2gfx(sympt) - position();

            if(! bgcolor.isTransparent())
                renderColor(bgcolor, Rect(relPos, sym2gfx(Size(1, 1))));

            if(tc.unicode() > 0x20 &&
		! fgcolor.isTransparent() && (! tc.blinked() || blinkShow()))
            {
                auto tx = FontsCache(fontRender).renderCharset(tc.unicode(), fgcolor, cp.render(), cp.style(), cp.hinting());
                if(0 < tc.alpha())
                    tx.setAlphaMod(tc.alpha());

                renderTexture(tx, relPos, tc.flip());

                if(0 < tc.alpha())
                    tx.setAlphaMod(SDL_ALPHA_OPAQUE);
            }
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
                    auto & prop = ch->property();

                    VERBOSE("col[" << symx << "]: " << SWE::StringFormat("unicode: %1, fgcolor: %2, bgcolor: %3, blend: %4, style: %5, hinting: %6, blinked: %7, inverted: %8, flip: %9, alpha: %10").
                    arg(SWE::String::hex(ch->unicode(), 4)).arg(ch->colors().fgcolor().toString()).arg(ch->colors().bgcolor().toString()).
                    arg(prop.render()).arg(SWE::String::hex(prop.style(), 2)).arg(prop.hinting()).
                    arg(ch->blinked()).arg(ch->inverted()).arg(ch->flip()).arg(ch->alpha()));
                }
            }
        }
    }

#ifdef SWE_WITH_JSON
    JsonObject TermCharset::toJson(void) const
    {
        JsonObject joChar;

        joChar.addString("unicode", String::hex(unicode(), 4));
        joChar.addString("fgcolor", colors().fgcolor().toString());
        joChar.addString("bgcolor", colors().bgcolor().toString());

        joChar.addInteger("render", property().render());
        joChar.addInteger("style", property().style());
        joChar.addInteger("hinting", property().hinting());
        joChar.addBoolean("blinked", blinked());
        joChar.addBoolean("inverted", inverted());
        joChar.addInteger("flip", flip());
        joChar.addInteger("alpha", alpha());

        return joChar;
    }

    JsonObject TermWindow::toJson(void) const
    {
        JsonObject res = TermBase::toJson();
        JsonArray joContent;

        for(int symy = 0; symy < rows(); ++symy)
        {
            for(int symx = 0; symx < cols(); ++symx)
            {
                const TermCharset* ch = charset(TermPos(symx, symy));
                if(ch) joContent.addObject(ch->toJson());
            }
        }

        res.addArray("content", joContent);
        return res;
    }
#endif

    /* CenteredTerminal */
    CenteredTerminal::CenteredTerminal(const TermSize & tsz, const FontRender & font, Window & win)
        : TermWindow(tsz.toSize() * font.size(), font, & win)
    {
        setPosition((win.size() - size()) / 2);
    }
}

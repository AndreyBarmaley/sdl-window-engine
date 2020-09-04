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

#include "swe_window.h"

namespace SWE
{

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

    struct TermSize : packshort
    {
        TermSize() {}
        TermSize(int cols, int rows) : packshort(cols, rows) {}
        TermSize(const Size & sz) : packshort(sz.w, sz.h) {}

        int cols(void) const
        {
            return val1();
        }
        int rows(void) const
        {
            return val2();
        }

        TermSize operator+ (const TermSize & ts) const
        {
            return TermSize(cols() + ts.cols(), rows() + ts.rows());
        }
        TermSize operator- (const TermSize & ts) const
        {
            return TermSize(cols() - ts.cols(), rows() - ts.rows());
        }

        Size toSize(void) const
        {
            return Size(cols(), rows());
        }
    };

    struct TermPos : packshort
    {
        TermPos() {}
        TermPos(int posx, int posy) : packshort(posx, posy) {}
        TermPos(const Point & pt) : packshort(pt.x, pt.y) {}

        int posx(void) const
        {
            return val1();
        }
        int posy(void) const
        {
            return val2();
        }

        TermPos operator+ (const TermPos & tp) const
        {
            return TermPos(posx() + tp.posx(), posy() + tp.posy());
        }
        TermPos operator- (const TermPos & tp) const
        {
            return TermPos(posx() - tp.posx(), posy() - tp.posy());
        }
        TermPos operator+ (const TermSize & ts) const
        {
            return TermPos(posx() + ts.cols(), posy() + ts.rows());
        }
        TermPos operator- (const TermSize & ts) const
        {
            return TermPos(posx() - ts.cols(), posy() - ts.rows());
        }

        Point toPoint(void) const
        {
            return Point(posx(), posy());
        }
    };

    struct TermRect : TermPos, TermSize
    {
        TermRect() {}
        TermRect(int posx, int posy, int cols, int rows) : TermPos(posx, posy), TermSize(cols, rows) {}
        TermRect(const TermPos & tp, const TermSize & ts) : TermPos(tp), TermSize(ts) {}
        TermRect(const Rect & rt) : TermPos(rt.x, rt.y), TermSize(rt.w, rt.h) {}

        Rect toRect(void) const
        {
            return Rect(toPoint(), toSize());
        }
    };

    namespace cursor
    {
        struct set : public packint2
        {
            set(const TermPos & tp) : packint2(tp.posx(), tp.posy()) {}
            set(int x, int y) : packint2(x, y) {}

            int posx(void) const
            {
                return val1();
            }
            int posy(void) const
            {
                return val2();
            }
        };

        struct move : public packint2
        {
            move(move_t dir, int count = 1) : packint2(dir, count) {}

            int dir(void) const
            {
                return val1();
            }
            int count(void) const
            {
                return val2();
            }
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
            align_t operator()(void) const
            {
                return val;
            }
        };

        struct wrap {};

        struct padding : public packint4
        {
            padding() : packint4(0) {}
            padding(int left, int right, int top, int bottom) : packint4(left, right, top, bottom) {}

            int left(void) const
            {
                return val1();
            }
            int right(void) const
            {
                return val2();
            }
            int top(void) const
            {
                return val3();
            }
            int bottom(void) const
            {
                return val4();
            }
        };

        struct colors : public packint2
        {
            colors(const FBColors & fbc) : packint2(fbc.fg(), fbc.bg()) {}
            colors(const ColorIndex & fg, const ColorIndex & bg) : packint2(fg(), bg()) {}

            ColorIndex fgindex(void) const
            {
                return val1();
            }
            ColorIndex bgindex(void) const
            {
                return val2();
            }
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
            area(int val, const TermSize & sz) : std::pair<int, packint2>(val, packint2(sz.cols(), sz.rows())) {}

            int 		value(void) const
            {
                return first;
            }
            int		width(void) const
            {
                return second.val1();
            }
            int		height(void) const
            {
                return second.val2();
            }
        };

        struct charset : public area
        {
            charset(int ch, const TermSize & sz = TermSize(1, 1)) : area(ch, sz) {}
        };

        struct space : public charset
        {
            space(int sz = 1) : charset(0x20, Size(sz, 1)) {}
        };

        struct fgcolor : public area
        {
            fgcolor(const ColorIndex & col, const TermSize & sz = TermSize(1, 1)) : area(col(), sz) {}
        };

        struct bgcolor : public area
        {
            bgcolor(const ColorIndex & col, const TermSize & sz = TermSize(1, 1)) : area(col(), sz) {}
        };

        struct colors : public area
        {
            colors(const FBColors & fbc, const TermSize & sz = TermSize(1, 1)) : area(packshort(fbc.bg(), fbc.fg()).value(), sz) {}
            colors(const ColorIndex & fg, const ColorIndex & bg, const TermSize & sz = TermSize(1, 1)) : area(packshort(bg(), fg()).value(), sz) {}
            ColorIndex	fgindex(void) const
            {
                return ColorIndex(packshort(value()).val2());
            }
            ColorIndex	bgindex(void) const
            {
                return ColorIndex(packshort(value()).val1());
            }
        };

        struct property : public std::pair<CharsetProperty, packint2>
        {
            property(const CharsetProperty & cp, const TermSize & sz = TermSize(1, 1)) :
                std::pair<CharsetProperty, packint2>(cp, packint2(sz.cols(), sz.rows())) {}

            const CharsetProperty & toProperty(void) const
            {
                return first;
            }
            int                     width(void) const
            {
                return second.val1();
            }
            int                     height(void) const
            {
                return second.val2();
            }
        };

        struct defaults : public UnicodeColor
        {
            CharsetProperty property;

            defaults(const FBColors & fbc, int ch = 0x20, int prop = 0) : UnicodeColor(ch, fbc), property(prop) {}
            defaults(const ColorIndex & fg, const ColorIndex & bg, int ch = 0x20, const CharsetProperty & prop = CharsetProperty()) : UnicodeColor(ch, FBColors(fg, bg)), property(prop) {}
            const CharsetProperty  & prop(void) const
            {
                return property;
            }
        };
    }

    namespace draw
    {
        struct hline : public packint2
        {
            hline(int count, int charset = acs::hline(LineThin)) : packint2(count, charset) {}

            int count(void) const
            {
                return val1();
            }
            int charset(void) const
            {
                return val2();
            }
        };

        struct vline : public packint2
        {
            vline(int count, int charset = acs::vline(LineThin)) : packint2(count, charset) {}

            int count(void) const
            {
                return val1();
            }
            int charset(void) const
            {
                return val2();
            }
        };

        struct rect : public TermRect
        {
            line_t line;

            rect(const TermRect & rt, line_t type) : TermRect(rt), line(type) {}
            rect(int px, int py, int pw, int ph, line_t type) : TermRect(px, py, pw, ph), line(type) {}
        };
    }

    struct TermCharset : std::pair<UnicodeColor, CharsetProperty>
    {
        TermCharset() {}

        TermCharset(const UnicodeColor & uc, const CharsetProperty & cp = CharsetProperty())
            : std::pair<UnicodeColor, CharsetProperty>(uc, cp) {}

        void setCharset(const UnicodeColor & uc)
        {
            first = uc;
        }

        void setProperty(const CharsetProperty & cp)
        {
            second = cp;
        }

        const UnicodeColor & charset(void) const
        {
            return first;
        }

        const CharsetProperty &	property(void) const
        {
            return second;
        }

        UnicodeColor & charset(void)
        {
            return first;
        }

        CharsetProperty & property(void)
        {
            return second;
        }
    };

    class TermBase : public Window
    {
    protected:
        const FontRender*	fontRender;
        set::padding		padding;
        TermPos			curpos;
        TermSize		termsz;
        packshort		defcols;
        packshort		termopt2; /* align, unused */

        /*
        protected:
            virtual void        windowMoveEvent(const Point &) {}
            virtual void        windowResizeEvent(const Size &) {}
            virtual void        windowVisibleEvent(bool) {}
            virtual bool        keyPressEvent(const KeySym &) { return false; }
            virtual bool        keyReleaseEvent(const KeySym &) { return false; }
            virtual bool        textInputEvent(const std::string &) { return false; }
            virtual bool        mousePressEvent(const ButtonEvent &) { return false; }
            virtual bool        mouseReleaseEvent(const ButtonEvent &) { return false; }
            virtual bool        mouseClickEvent(const ButtonsEvent &) { return false; }
            virtual void        mouseFocusEvent(void) {}
            virtual void        mouseLeaveEvent(void) {}
            virtual bool        mouseMotionEvent(const Point &, u32 buttons) { return false; }
            virtual bool        userEvent(int, void*) { return false; }
            virtual bool        scrollUpEvent(void) { return false; }
            virtual bool        scrollDownEvent(void) { return false; }
            virtual void        tickEvent(u32 ms) {}
            virtual void        renderPresentEvent(u32 ms) {}
            virtual void        displayResizeEvent(const Size &, bool) {}
            virtual void        displayFocusEvent(bool gain) {}
        */
        virtual void        termResizeEvent(void) {}

        // protected: empty font render
        TermBase();
        TermBase(TermBase &);

        bool		lineWrap(void) const;

        int		index(const Point &) const;
        int		index(void) const;

        void		setFGColor(const ColorIndex &);
        void		setBGColor(const ColorIndex &);

        ColorIndex	fgColor(void) const;
        ColorIndex	bgColor(void) const;
        FBColors	colors(void) const;

        void		setAlign(align_t v);
        int		align(void) const;

        void		setUnused8(int v);
        int		unused8(void) const;

        const set::padding & paddings(void) const;

    public:
        TermBase(const FontRender &);
        TermBase(const FontRender &, Window &);
        TermBase(const Point & gfxpt, const Size & gfxsz, const FontRender &, Window &);

        void		setFontRender(const FontRender &);
        void            setSize(const Size &) override;
        void		setTermSize(const TermSize &);

        void		setCursorPos(const TermPos & tp)
        {
            curpos = tp;
        }
        void		resetCursorPos(void)
        {
            curpos = TermPos();
        }
        const TermPos &	cursor(void) const
        {
            return curpos;
        }

        virtual void setCharset(int ch, const ColorIndex & fg = Color::Transparent, const ColorIndex & bg = Color::Transparent, const CharsetProperty* prop = nullptr) = 0;
        virtual void renderFlush(void) = 0;

        inline int cols(void) const
        {
            return termsz.cols();
        }

        inline int rows(void) const
        {
            return termsz.rows();
        }

        const TermSize & termSize(void) const
        {
            return termsz;
        }

        const FontRender* frs(void) const
        {
            return fontRender;
        }

        Point		sym2gfx(const TermPos &) const;   /* coord transformer: symbol to graphics (parent relative) */
        TermPos		gfx2sym(const Point &) const;   /* coord transformer: graphics to symbol (parent relative) */
        Size		sym2gfx(const TermSize &) const;    /* size transformer: symbol to graphics */
        TermSize	gfx2sym(const Size &) const;    /* size transformer: graphics to symbol */
        Rect		sym2gfx(const TermRect &) const;
        TermRect	gfx2sym(const Rect &) const;

        TermBase & operator<< (const fill::defaults &);
        TermBase & operator<< (const fill::fgcolor &);
        TermBase & operator<< (const fill::bgcolor &);
        TermBase & operator<< (const fill::colors &);
        TermBase & operator<< (const fill::charset &);
        TermBase & operator<< (const fill::property &);

        TermBase & operator<< (const cursor::set &);
        TermBase & operator<< (const cursor::move &);

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

        void		renderWindow(void) override;
	const char*     className(void) const override { return "SWE::TermBase"; }
    };

    class TermWindow : public TermBase
    {
    protected:
        std::vector<TermCharset>	chars;

        TermWindow() {}
        TermWindow(TermBase & term) : TermBase(term) {} // FIXED: remove

        void		termResizeEvent(void) override;

    public:
        TermWindow(const FontRender & frs) : TermBase(frs) {}
        TermWindow(const FontRender & frs, Window & win) : TermBase(frs, win) {}
        TermWindow(const Point & gfxpos, const Size & gfxsz, const FontRender &, Window &);

        const TermCharset*	charset(void) const;

        void		setCharset(int ch, const ColorIndex & fg = Color::Transparent, const ColorIndex & bg = Color::Transparent, const CharsetProperty* prop = nullptr) override;
        void		renderFlush(void) override;

        void		renderSymbol(int symx, int symy);
	const char*     className(void) const override { return "SWE::TermWindow"; }
    };

    class TermArea : public TermBase
    {
    protected:
        TermPos		termpos;

    public:
        TermArea(TermWindow & term) : TermBase(term) {}
        TermArea(int symx, int symy, int cols, int rows, TermWindow & term) : TermBase(term)
        {
            setTermArea(symx, symy, cols, rows);
        }

        void		setTermPos(const TermPos & tp)
        {
            termpos = tp;
        }
        void		setPosition(const Point &) override;

        void		setTermArea(const TermRect &);
        void		setTermArea(int symx, int symy, int cols, int rows);

        inline int	posx(void) const
        {
            return termpos.posx();
        }
        inline int	posy(void) const
        {
            return termpos.posy();
        }

        void		setCharset(int ch, const ColorIndex & fg = Color::Transparent, const ColorIndex & bg = Color::Transparent, const CharsetProperty* prop = nullptr) override;
        void		renderFlush(void) override;
	const char*     className(void) const override { return "SWE::TermArea"; }
    };

    class CenteredTerminal : public TermWindow
    {
    public:
        CenteredTerminal(int cols, int rows, const FontRender &, Window &);
	const char*     className(void) const override { return "SWE::CenteredTerminal"; }
    };

} // SWE
#endif

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

/// @brief пространство SWE
namespace SWE
{
    /// @enum SWE::LineType
    /// @brief перечисление типа символьных линий
    enum LineType
    {
        LineAscii,      ///< ascii линия
        LineThin,       ///< unicode тонкая
        LineBold,       ///< unicode толстая
        LineDouble      ///< unicode двойная
    };

    /// @brief перечисление направления движения курсора в терминале
    enum MoveDirection { MoveCenter, MoveUp, MoveDown, MoveLeft, MoveRight, MoveUpperLeft, MoveUpperRight, MoveLowerLeft, MoveLowerRight, MoveFirst, MoveLast };

    /// @brief пространство символов линий
    namespace acs
    {
	/// @brief код символа "вертикальная линия"
        int         vline(const LineType &);
	/// @brief код символа "горизонтальная линия"
        int         hline(const LineType &);
	/// @brief код символа "верхний левый угол"
        int         ulcorner(const LineType &);
	/// @brief код символа "верхний правый угол"
        int         urcorner(const LineType &);
	/// @brief код символа "нижний левый угол"
        int         llcorner(const LineType &);
	/// @brief код символа "нижний правый угол"
        int         lrcorner(const LineType &);
	/// @brief код символа "левый T"
        int         ltee(const LineType &);
	/// @brief код символа "правый T"
        int         rtee(const LineType &);
	/// @brief код символа "верхний T"
        int         ttee(const LineType &);
	/// @brief код символа "нижний T"
        int         btee(const LineType &);
	/// @brief код символа "+"
        int         plus(const LineType &);
    }

    /// @brief класс двухмерной размерности в терминале
    struct TermSize : packshort
    {
        TermSize() {}
        TermSize(size_t cols, size_t rows) : packshort(cols, rows) {}
        TermSize(const Size & sz) : packshort(sz.w, sz.h) {}
 
        size_t                  cols(void) const { return val1(); }
        size_t                  rows(void) const { return val2(); }
 
        void                    setSize(size_t cols, size_t rows) { set1(cols); set2(rows); }
 
        TermSize                operator+ (const TermSize & ts) const { return TermSize(cols() + ts.cols(), rows() + ts.rows()); }
        TermSize                operator- (const TermSize & ts) const { return TermSize(cols() - ts.cols(), rows() - ts.rows()); }
 
        TermSize &              operator+= (const TermSize & ts) { setSize(cols() + ts.cols(), rows() + ts.rows()); return *this; }
        TermSize &              operator-= (const TermSize & ts) { setSize(cols() - ts.cols(), rows() - ts.rows()); return *this; }
 
        Size                    toSize(void) const { return Size(cols(), rows()); }
    };

    /// @brief класс двухмерной позиции в терминале
    struct TermPos : packshort
    {
        TermPos() {}
        TermPos(int posx, int posy) : packshort(posx, posy) {}
        TermPos(const Point & pt) : packshort(pt.x, pt.y) {}

        int                     posx(void) const { return val1(); }
        int                     posy(void) const { return val2(); }

        void                    setPos(int posx, int posy) { set1(posx); set2(posy); }

        TermPos                 operator+ (const TermPos & tp) const { return TermPos(posx() + tp.posx(), posy() + tp.posy()); }
        TermPos                 operator- (const TermPos & tp) const { return TermPos(posx() - tp.posx(), posy() - tp.posy()); }

        TermPos                 operator+ (const TermSize & ts) const { return TermPos(posx() + ts.cols(), posy() + ts.rows()); }
        TermPos                 operator- (const TermSize & ts) const { return TermPos(posx() - ts.cols(), posy() - ts.rows()); }

        TermPos &               operator+= (const TermPos & tp) { setPos(posx() + tp.posx(), posy() + tp.posy()); return *this; }
        TermPos &               operator-= (const TermPos & tp) { setPos(posx() - tp.posx(), posy() - tp.posy()); return *this; }

        TermPos &               operator+= (const TermSize & ts) { setPos(posx() + ts.cols(), posy() + ts.rows()); return *this; }
        TermPos &               operator-= (const TermSize & ts) { setPos(posx() - ts.cols(), posy() - ts.rows()); return *this; }

        Point                   toPoint(void) const { return Point(posx(), posy()); }
    };

    /// @brief класс прямоугольника в терминале
    struct TermRect : TermPos, TermSize
    {
        TermRect() {}
        TermRect(int posx, int posy, int cols, int rows) : TermPos(posx, posy), TermSize(cols, rows) {}
        TermRect(const TermPos & tp, const TermSize & ts) : TermPos(tp), TermSize(ts) {}
        TermRect(const Rect & rt) : TermPos(rt.x, rt.y), TermSize(rt.w, rt.h) {}

        void                    setRect(int posx, int posy, size_t cols, size_t rows) { setPos(posx, posy); setSize(cols, rows); }
        Rect                    toRect(void) const { return Rect(toPoint(), toSize()); }
    };

    /// @brief класс состояние графического символа
    struct CharState
    {
	u8			state;

        /// @enum CharState::Type
        /// @brief перечисление тип состояний символа
	enum Type
        {
            FlipHorz = 0x01,                    ///< flip по горизонтали
            FlipVert = 0x02,                    ///< flip по вертикали
            FlipBoth = FlipVert | FlipHorz,     ///< flip по горизонтали и вертикали
            Inverted = 0x04,                    ///< инвертировать цвет символа на увет фона
            Blinked = 0x08,                     ///< blink state
            /* AlphaRez = 0xF0 */
        };
	CharState(u8 val = 0) : state(val) {}

	void			setState(const Type &, bool f);
	bool			checkState(const Type &) const;

	void			setAlpha(int);
	int			alpha(void) const;
    };

    /// @brief пространство манипулятор курсора
    namespace cursor
    {
	/// @brief класс манипулятор, установка позиции курсора терминала
        struct set : public packint2
        {
            set(const TermPos & tp) : packint2(tp.posx(), tp.posy()) {}
            set(int x, int y) : packint2(x, y) {}

            int			posx(void) const { return val1(); }
            int			posy(void) const { return val2(); }
        };

	/// @brief класс манипулятор, движение курсора по направлению в терминале
        struct move : public packint2
        {
            move(const MoveDirection & dir, int count = 1) : packint2(dir, count) {}

            MoveDirection	dir(void) const { return static_cast<MoveDirection>(val1()); }
            int			count(void) const { return val2(); }
        };

	/// @brief класс манипулятор, перемещение курсора в терминале вверх
        struct up : public move
        {
            up(int count = 1) : move(MoveUp, count) {}
        };

	/// @brief класс манипулятор, перемещение курсора в терминале вниз
        struct down : public move
        {
            down(int count = 1) : move(MoveDown, count) {}
        };

	/// @brief класс манипулятор, перемещение курсора в терминале налево
        struct left : public move
        {
            left(int count = 1) : move(MoveLeft, count) {}
        };

	/// @brief класс манипулятор, перемещение курсора в терминале направо
        struct right : public move
        {
            right(int count = 1) : move(MoveRight, count) {}
        };

	/// @brief класс манипулятор, перемещение курсора в терминале в позицию "начало строки"
        struct first : public move
        {
            first() : move(MoveFirst) {}
        };

	/// @brief класс манипулятор, перемещение курсора в терминале в позицию "окончание строки"
        struct last : public move
        {
            last() : move(MoveLast) {}
        };
    }

    /// @brief пространство манипулятор set
    namespace set
    {
	/// @brief класс манипулятор, установка состояния set::fgcolor
	/// @see TermBase::operator<< (const set::fgcolor &)
        struct fgcolor
        {
            ColorIndex		color;

            fgcolor(const ColorIndex & col) : color(col) {}
        };

	/// @brief класс манипулятор, установка состояния set::bgcolor
	/// @see TermBase::operator<< (const set::ggcolor &)
        struct bgcolor
        {
            ColorIndex		color;

            bgcolor(const ColorIndex & col) : color(col) {}
        };

	/// @brief класс манипулятор, установка состояния set::align
	/// @see TermBase::operator<< (const set::align &)
        struct align
        {
            AlignType val;

            align(const AlignType & v) : val(v) {}

            const AlignType &	operator()(void) const { return val; }
        };

	/// @brief класс манипулятор, установка состояния set::wrap
	/// @see TermBase::operator<< (const set::wrap &)
        struct wrap {};

	/// @brief класс манипулятор, установка состояния set::padding
	/// @see TermBase::operator<< (const set::padding &)
        struct padding : public packint4
        {
            padding() : packint4(0) {}
            padding(int left, int right, int top, int bottom) : packint4(left, right, top, bottom) {}

            int			left(void) const { return val1(); }
            int			right(void) const { return val2(); }
            int			top(void) const { return val3(); }
            int			bottom(void) const { return val4(); }
        };

	/// @brief класс манипулятор, установка состояния set::colors
	/// @see TermBase::operator<< (const set::colors &)
        struct colors : public packint2
        {
            colors(const FBColors & fbc) : packint2(fbc.fg(), fbc.bg()) {}
            colors(const ColorIndex & fg, const ColorIndex & bg) : packint2(fg(), bg()) {}

            ColorIndex		fgindex(void) const { return val1(); }
            ColorIndex 		bgindex(void) const { return val2(); }
        };

	/// @brief класс манипулятор, установка состояния set::property
	/// @see TermBase::operator<< (const set::property &)
        struct property
        {
	    CharProperty	prop;

	    property(const CharRender & blend, int style = StyleNormal, const CharHinting & hinting = HintingNormal) : prop(blend, style, hinting) {}
            property(const CharProperty & val) : prop(val) {}
        };

	/// @brief класс манипулятор, установка состояния set::blink
        struct blink {};

	/// @brief класс манипулятор, установка состояния set::flip
        struct flip
	{
	    int		type;
	    flip(int val) : type(val) {}
	};

	/// @brief класс манипулятор, установка состояния set::flipvert
        struct flipvert : flip
	{
	    flipvert() : flip(CharState::FlipVert) {}
	};

	/// @brief класс манипулятор, установка состояния set::fliphorz
        struct fliphorz : flip
	{
	    fliphorz() : flip(CharState::FlipHorz) {}
	};

	/// @brief класс манипулятор, установка состояния set::alpha
        struct alpha
	{
	    int		value;
	    alpha(int val) : value(val) {}
	};

	/// @brief класс манипулятор, установка состояния set::invert
        struct invert {};

	/// @brief класс манипулятор, установка состояния set::rn
        struct rn {};

	/// @brief класс манипулятор, установка состояния set::flush
        struct flush {};
    }

    /// @brief пространство манипулятор reset
    namespace reset
    {
	/// @brief класс манипулятор, сброс состояния set::fgcolor
        struct fgcolor {};

	/// @brief класс манипулятор, сброс состояния set::bgcolor
        struct bgcolor {};

	/// @brief класс манипулятор, сброс состояния set::colors
        struct colors {};

	/// @brief класс манипулятор, сброс состояния set::align
        struct align {};

	/// @brief класс манипулятор, сброс состояния set::property
        struct property {};

	/// @brief класс манипулятор, сброс состояния set::padding
        struct padding {};

	/// @brief класс манипулятор, сброс состояния set::wrap
        struct wrap {};

	/// @brief класс манипулятор, сброс состояния set::blink
        struct blink {};

	/// @brief класс манипулятор, сброс состояния set::invert
        struct invert {};

	/// @brief класс манипулятор, сброс состояния set::flip
        struct flip {};

	/// @brief класс манипулятор, сброс состояния set::alpha
        struct alpha {};

	/// @brief класс манипулятор, сброс всех состояний
        struct defaults {};
    }

    /// @brief пространство манипулятор fill
    namespace fill
    {
        /// @protected
        struct area : public std::pair<int, packint2>
        {
            area(int val, const TermSize & sz) : std::pair<int, packint2>(val, packint2(sz.cols(), sz.rows())) {}

            int 		value(void) const { return first; }
            size_t		width(void) const { return second.val1(); }
            size_t		height(void) const { return second.val2(); }
        };

	/// @brief класс манипулятор, заполнение области символом
        /// @details область заполняется от текущей позиции курсора
        struct charset : public area
        {
            charset(int ch, const TermSize & sz = TermSize(1, 1)) : area(ch, sz) {}
        };

	/// @brief класс манипулятор, заполнение строки символом space
        /// @details область заполняется от текущей позиции курсора
        struct space : public charset
        {
            space(int len = 1) : charset(0x20, Size(len, 1)) {}
        };

        /// @brief класс манипулятор, заполнение области цветом символа по умолчанию
        /// @details область заполняется от текущей позиции курсора
        struct fgcolor : public area
        {
            fgcolor(const ColorIndex & col, const TermSize & sz = TermSize(1, 1)) : area(col(), sz) {}
        };

        /// @brief класс манипулятор, заполнение области цветом фона по умолчанию
        /// @details область заполняется от текущей позиции курсора
        struct bgcolor : public area
        {
            bgcolor(const ColorIndex & col, const TermSize & sz = TermSize(1, 1)) : area(col(), sz) {}
        };

        /// @brief класс манипулятор, заполнение области цветом символа и фона по умолчанию
        /// @details область заполняется от текущей позиции курсора
	/// @see FBColors
        struct colors : public area
        {
            colors(const FBColors & fbc, const TermSize & sz = TermSize(1, 1)) : area(packshort(fbc.bg(), fbc.fg()).value(), sz) {}
            colors(const ColorIndex & fg, const ColorIndex & bg, const TermSize & sz = TermSize(1, 1)) : area(packshort(bg(), fg()).value(), sz) {}

            ColorIndex		fgindex(void) const { return ColorIndex(packshort(value()).val2()); }
            ColorIndex		bgindex(void) const { return ColorIndex(packshort(value()).val1()); }
        };

        /// @brief класс манипулятор, заполнение области свойствами символа по умолчанию
        /// @details область заполняется от текущей позиции курсора
        /// @see CharProperty
        struct property : public std::pair<CharProperty, packint2>
        {
            property(const CharProperty & cp, const TermSize & sz = TermSize(1, 1)) :
                std::pair<CharProperty, packint2>(cp, packint2(sz.cols(), sz.rows())) {}

            const CharProperty & toProperty(void) const { return first; }
            size_t		width(void) const { return second.val1(); }
            size_t		height(void) const { return second.val2(); }
        };

        /// @brief класс манипулятор, заполнение области символом, цветом и свойствами по умолчанию
        /// @details область заполняется от текущей позиции курсора
        /// @see CharProperty, FBColors
        struct defaults : public UnicodeColor
        {
            CharProperty	property;

            defaults(const FBColors & fbc, int ch = 0x20, const CharProperty & prop = CharProperty()) : UnicodeColor(ch, fbc), property(prop) {}
            defaults(const ColorIndex & fg, const ColorIndex & bg, int ch = 0x20, const CharProperty & prop = CharProperty()) : UnicodeColor(ch, FBColors(fg, bg)), property(prop) {}

            const CharProperty  & prop(void) const { return property; }
        };
    }

    /// @brief пространство манипулятор draw
    namespace draw
    {
	/// @brief класс манипулятор, рисования горизонтальной линии
        /// @see acs::hline
        struct hline : public packint2
        {
            hline(int count, int charset = acs::hline(LineThin)) : packint2(count, charset) {}

            int			count(void) const { return val1(); }
            int			charset(void) const { return val2(); }
        };

	/// @brief класс манипулятор, рисования вертикальной линии
        /// @see acs::vline
        struct vline : public packint2
        {
            vline(int count, int charset = acs::vline(LineThin)) : packint2(count, charset) {}

            int			count(void) const { return val1(); }
            int			charset(void) const { return val2(); }
        };

	/// @brief класс манипулятор, рисования прямоугольника
        struct rect : public TermRect
        {
            LineType		line;

            rect(const TermRect & rt, const LineType & type = LineThin) : TermRect(rt), line(type) {}
            rect(int px, int py, int pw, int ph, const LineType & type) : TermRect(px, py, pw, ph), line(type) {}
        };
    }

    /// @brief класс терминального символа
    class TermCharset
    {
	// 16 bit + 16 bit
	UnicodeColor		ucol;
	// 8 bit
	CharProperty		prop;
	// 8 bit
	CharState		chst;

    public:
        TermCharset() : chst(0) {}
        TermCharset(const UnicodeColor & uc, const CharProperty & cp = CharProperty(), int cs = 0)
            : ucol(uc), prop(cp), chst(cs) {}

        void			setUnicodeColor(const UnicodeColor & val) { ucol = val; }
        void			setProperty(const CharProperty & val) { prop = val; }
	void			setFGIndex(const ColorIndex & col) { ucol.fgindex(col); }
	void			setBGIndex(const ColorIndex & col) { ucol.bgindex(col); }
	void			setColors(const FBColors & cols) { ucol.colors(cols); }
        void			setUnicode(int sym) { ucol.unicode(sym); }

        const UnicodeColor &	unicodeColor(void) const { return ucol; }
        const FBColors &	colors(void) const { return ucol.colors(); }
        int			unicode(void) const { return ucol.unicode(); }
        const CharProperty &	property(void) const { return prop; }

	// set state
	void			setBlink(bool f) { chst.setState(CharState::Blinked, f); }
	void			setInvert(bool f) { chst.setState(CharState::Inverted, f); }
	void			setFlip(int val) { chst.setState(CharState::FlipBoth, false);
						    if(val & CharState::FlipBoth) 
							chst.setState(static_cast<CharState::Type>(val & CharState::FlipBoth), true); }
	void			setAlpha(int val) { chst.setAlpha(val); }

	bool			blinked(void) const { return chst.checkState(CharState::Blinked); }
	bool			inverted(void) const { return chst.checkState(CharState::Inverted); }
	int			flip(void) const { return CharState::FlipBoth & chst.state; }
	int			alpha(void) const { return chst.alpha(); }

#ifdef SWE_WITH_JSON
        JsonObject      	toJson(void) const;
#endif
    };

    /// @brief базовый класс терминального окна
    class TermBase : public Window
    {
    private:
	    TermBase(const TermBase &) = delete;
	    TermBase &		operator=(const TermBase &) = delete;

    protected:
        set::padding		padding;
        TermPos			curpos;
        TermSize		termsz;
	AlignType		curalign;
        FBColors		curcols;
	CharProperty		curprop;
	CharState		curstate;
	
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
            virtual void        displayResizeEvent(const Size &) {}
            virtual void        displayFocusEvent(bool gain) {}
        */
        bool			lineWrap(void) const;
        LineType                systemLine(const LineType &) const;

        void			setFGColor(const ColorIndex &);
        void			setBGColor(const ColorIndex &);

        ColorIndex		fgColor(void) const;
        ColorIndex		bgColor(void) const;
        const FBColors &	colors(void) const;

        void			setAlign(const AlignType &);
        const AlignType &	align(void) const;

	void			setProperty(const CharProperty &);
	const CharProperty &	property(void) const;

	void			setBlink(bool);
	bool			blink(void) const;

	void			setInvert(bool);
	bool			invert(void) const;

	void			setFlip(int, bool);
	int			flip(void) const;

	void			setAlpha(int);
	int			alpha(void) const;

        const set::padding &	paddings(void) const;

    protected:
        TermBase(TermBase*);

	virtual CharProperty    defaultProperty(void) const;
	virtual FBColors	defaultColors(void) const;
        virtual void            terminalResizeEvent(void) {}

    public:
        TermBase(Window*);
        TermBase(const Size & gfxsz, Window*);
        TermBase(const TermSize &, TermBase &);

        void            	setSize(const Size &) override;
        virtual void		setTermSize(const TermSize &);
	void			setTermPos(const TermBase &, const TermPos &);

        // @protected
        void			setCursorPos(const TermPos &);
        // @protected
        void			resetCursorPos(void);
        const TermPos &		cursor(void) const;

        virtual void		setCharset(int ch, const ColorIndex & fg = Color::Transparent, const ColorIndex & bg = Color::Transparent, const CharProperty* prop = nullptr) = 0;
        virtual void		renderFlush(void) = 0;

	/// @result текуший рендер шрифтов окна
        virtual const FontRender* frs(void) const = 0;

	/// @result количество столбцов в терминале
        size_t                  cols(void) const;
	/// @result количество строк в терминале
        size_t                  rows(void) const;

	/// @result позиция терминала в символах (относительно parent)
        TermPos                 termPos(const TermBase &) const;

	/// @result размер терминала в символах
        const TermSize & 	termSize(void) const;

	/// @brief coordinate transformer: symbol TermPos to graphics Point (parent relative)
        Point			sym2gfx(const TermPos &) const;
	/// @brief coordinate transformer: graphics Point to symbol TermPos (parent relative)
        TermPos			gfx2sym(const Point &) const;
	/// @brief size transformer: symbol TermSize to graphics Size
        Size			sym2gfx(const TermSize &) const;
	/// @brief size transformer: graphics Size to symbol TermSize
        TermSize		gfx2sym(const Size &) const;
	/// @brief rect transformer: symbol TermRect to graphics Rect
        Rect			sym2gfx(const TermRect &) const;
	/// @brief rect transformer: graphics Rect to symbol TermRect
        TermRect		gfx2sym(const Rect &) const;

        TermBase & 		operator<< (const fill::defaults &);
        TermBase & 		operator<< (const fill::fgcolor &);
        TermBase & 		operator<< (const fill::bgcolor &);
        TermBase & 		operator<< (const fill::colors &);
        TermBase & 		operator<< (const fill::charset &);
        TermBase & 		operator<< (const fill::property &);

	/// @brief установить текущую позицию курсора
        TermBase & 		operator<< (const cursor::set &);

	/// @brief подвинуть курсор
        TermBase & 		operator<< (const cursor::move &);

	/// @brief установить цвет символа и цвет фона по умолчанию, для всех последующих действий
	/// @details область действия до вызова reset::colors или reset::defaults и ограниченно до reset::bgcolor или reset::fgcolor
        TermBase & 		operator<< (const set::colors &);

	/// @brief установить цвет символа по умолчанию, для всех последующих действий
	/// @details область действия до вызова reset::fgcolor, reset::colors или reset::defaults
        TermBase & 		operator<< (const set::fgcolor &);

	/// @brief установить цвет фона по умолчанию, для всех последующих действий
	/// @details область действия до вызова reset::bgcolor, reset::colors или reset::defaults
        TermBase & 		operator<< (const set::bgcolor &);

	/// @brief установить выравнивание текста по умолчанию, для всех последующих действий
	/// @details область действия до вызова reset::align или reset::defaults
        TermBase & 		operator<< (const set::align &);

	/// @brief установить отступы по умолчанию, для всех последующих действий
	/// @details область действия до вызова reset::padding или reset::defaults
        TermBase & 		operator<< (const set::padding &);

	/// @brief установить свойства рендера символов по умолчанию, для всех последующих действий
	/// @details область действия до вызова reset::property или reset::defaults
        TermBase & 		operator<< (const set::property &);

	/// @brief установить свойство переноса строк по умолчанию, для всех последующих действий
	/// @details область действия до вызова reset::wrap или reset::defaults
        TermBase & 		operator<< (const set::wrap &);

	/// @brief установить свойство мигания символа по умолчанию, для всех последующих действий
	/// @details область действия до вызова reset::blink или reset::defaults
        TermBase & 		operator<< (const set::blink &);

	/// @brief установить свойство инвертирования цветов по умолчанию, для всех последующих действий
	/// @details область действия до вызова reset::invert или reset::defaults
        TermBase & 		operator<< (const set::invert &);

	/// @brief установить свойство отражения символа по умолчанию, для всех последующих действий
	/// @details область действия до вызова reset::flip или reset::defaults
        TermBase & 		operator<< (const set::flip &);

	/// @brief установить свойство прозрачности символа по умолчанию, для всех последующих действий
	/// @details область действия до вызова reset::alpha или reset::defaults
        TermBase & 		operator<< (const set::alpha &);

	/// @brief перенос текущей строки
        TermBase & 		operator<< (const set::rn &);

	/// @brief синхронизировать внутренний буфера TermWindow с рендером сцены DisplayScene
        TermBase & 		operator<< (const set::flush &);

        TermBase & 		operator<< (const reset::defaults &);
        /// @brief сброс действия работы манипулятора set::colors
        TermBase & 		operator<< (const reset::colors &);
        /// @brief сброс действия работы манипулятора set::fgcolor
        TermBase & 		operator<< (const reset::fgcolor &);
        /// @brief сброс действия работы манипулятора set::bgcolor
        TermBase & 		operator<< (const reset::bgcolor &);
        /// @brief сброс действия работы манипулятора set::padding
        TermBase & 		operator<< (const reset::padding &);
        /// @brief сброс действия работы манипулятора set::align
        TermBase & 		operator<< (const reset::align &);
        /// @brief сброс действия работы манипулятора set::property
        TermBase & 		operator<< (const reset::property &);
        /// @brief сброс действия работы манипулятора set::wrap
        TermBase & 		operator<< (const reset::wrap &);
        /// @brief сброс действия работы манипулятора set::blink
        TermBase & 		operator<< (const reset::blink &);
        /// @brief сброс действия работы манипулятора set::invert
        TermBase & 		operator<< (const reset::invert &);
        /// @brief сброс действия работы манипулятора set::flip
        TermBase & 		operator<< (const reset::flip &);
        /// @brief сброс действия работы манипулятора set::alpha
        TermBase & 		operator<< (const reset::alpha &);

        /// @brief рисование горизонтальной линии из заданного символа
        /// @see draw::hline
        TermBase & 		operator<< (const draw::hline &);
        /// @brief рисование вертикальной линии из заданного символа
        /// @see draw::vline
        TermBase & 		operator<< (const draw::vline &);
        /// @brief рисование прямоугольника
        /// @see draw::rect
        TermBase & 		operator<< (const draw::rect &);

        TermBase & 		operator<< (int);
        TermBase & 		operator<< (const char*);
        TermBase & 		operator<< (const std::string &);
        TermBase & 		operator<< (const UnicodeString &);
        TermBase & 		operator<< (const UCString &);
        TermBase & 		operator<< (const UnicodeList &);
        TermBase & 		operator<< (const UCStringList &);

        TermBase & 		operator<< (const UnicodeColor &);

        void			renderWindow(void) override;
	const char*     	className(void) const override { return "SWE::TermBase"; }
#ifdef SWE_WITH_JSON
        JsonObject      	toJson(void) const override;
#endif
    };

    /// @brief основной класс терминального окна
    class TermWindow : public TermBase
    {
    protected:
        std::vector<TermCharset> chars;
	TickTrigger		tickBlink;
        const FontRender*	fontRender;

    protected:
        TermWindow(TermBase* term) : TermBase(term), fontRender(term->frs()) {}

	bool			blinkShow(void) const;
	void			setBlinkShow(bool);

        int			index(const TermPos &) const;
        int			index(void) const;

        void                    fontChangedHandle(void);

        void                    displayResizeEvent(const Size &) final;
        void			tickEvent(u32 ms) override;

        virtual void            fontChangedEvent(void) {};
        virtual TermSize        minimalTerminalSize(void) const { return TermSize(80, 25); }

        FBColors                defaultColors(void) const override;


    public:
        TermWindow(const FontRender & frs, Window* win);
        TermWindow(const Size & gfxsz, const FontRender & frs, Window* win);
        TermWindow(const TermSize & tsz, TermBase & term);

        const TermCharset*	charset(const TermPos &) const;
        const TermCharset*	charset(void) const;
        void			setCharset(int ch, const ColorIndex & fg = Color::Transparent, const ColorIndex & bg = Color::Transparent, const CharProperty* prop = nullptr) override;
        const FontRender* 	frs(void) const override;

        void			setTermSize(const TermSize &) override;
        void			renderFlush(void) override;

        void			setFontRender(const FontRender &);
        void			renderSymbol(int symx, int symy);
	const char*     	className(void) const override { return "SWE::TermWindow"; }
#ifdef SWE_WITH_JSON
        JsonObject      	toJson(void) const override;
#endif
	void			dumpState(void) const;
    };

    /* FullTerminal */
    class FullTerminal : public TermWindow
    {
    public:
	FullTerminal() : TermWindow(Display::size(), systemFont(), nullptr) {}
        FullTerminal(const FontRender & frs) : TermWindow(frs, nullptr) {}
	const char*     	className(void) const override { return "SWE::FullTerminal"; }
    };

    /* CenteredTerminal */
    class CenteredTerminal : public TermWindow
    {
    public:
        CenteredTerminal(const TermSize &, const FontRender &, Window &);
	const char*     	className(void) const override { return "SWE::CenteredTerminal"; }
    };

} // SWE
#endif

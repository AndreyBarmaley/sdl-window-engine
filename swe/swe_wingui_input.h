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

#ifndef _SWE_GUI_INPUT_
#define _SWE_GUI_INPUT_

#include "swe_wingui.h"

/// @brief пространство SWE
namespace SWE
{
    // LineEdit emit signals: LineEditTextChanged, LineEditCursorChanged, LineEditReturnPressed
    class LineEdit : public WindowToolTipArea
    {
        std::string     	content;
        int             	curpos;

    protected:
        bool            	mouseClickEvent(const ButtonsEvent &) override;
        bool            	keyPressEvent(const KeySym &) override;
        bool            	textInputEvent(const std::string &) override;

	virtual const FontRender & fontRender(void) const;

    public:
        LineEdit(Window*);
        LineEdit(const Size &, Window*);
        LineEdit(const Point &, const Size &, Window*);
        LineEdit(const Point &, const Size &, const std::string &, Window*);

        int             	cursorPosition(void) const;
        int             	cursorPositionAt(const Point &) const;
        void            	clear(void);
        void            	setCursorPosition(int);
        void            	setText(const std::string &);

        const std::string & 	text(void) const;

        void            	renderLineEdit(const Color & textColor, const Color & cursorColor, int cursorHeight, int cursorOffset);
        void            	renderWindow(void) override;

        virtual void    	cursorPositionChanged(int oldpos, int newpos);
        virtual void    	textChanged(const std::string & text);
        virtual void    	returnPressed(void) {}

        const char*             className(void) const override { return "SWE::LineEdit"; }
#ifdef SWE_WITH_JSON
        JsonObject              toJson(void) const override
        {
            JsonObject res = WindowToolTipArea::toJson();
            res.addInteger("curpos", curpos);
            res.addString("text", content);
            return res;
        }
#endif
    };

} // SWE
#endif

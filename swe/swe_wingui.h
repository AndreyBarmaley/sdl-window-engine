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

#ifndef _SWE_WINDOW_GUI_
#define _SWE_WINDOW_GUI_

#include "swe_window.h"

/// @brief пространство SWE
namespace SWE
{
    class WindowToolTipArea : public Window
    {
    protected:
        Texture			tooltip;

    public:
        WindowToolTipArea(Window*);
        WindowToolTipArea(const Size &, Window*);
        WindowToolTipArea(const Point &, const Size &, Window*);

        void			renderToolTip(const std::string &, const FontRender &, const Color & fn, const Color & bg, const Color & rt);
        void			renderToolTip(const StringList &, const FontRender &, const Color & fn, const Color & bg, const Color & rt);
	void			resetToolTip(void) { tooltip.reset(); }

        virtual void            setToolTip(const std::string &);
        virtual void            setToolTip(const std::string &, int width);
        virtual void            setToolTip(const StringList &);

        const Texture & 	tooltipTexture(void) const;

        virtual u32		tooltipTimeout(void) const
	{
	    return 400; // ms
	}

	const char*     	className(void) const override { return "SWE::WindowToolTipArea"; }
#ifdef SWE_WITH_JSON
        JsonObject		toJson(void) const override
	{
	    JsonObject res = Window::toJson();
	    res.addObject("tooltip", tooltip.toJson());
	    return res;
	}
#endif
    };

    /* WindowToolTipIcon */
    class WindowToolTipIcon : public WindowToolTipArea
    {
    protected:
	Texture			icon;

    public:
	WindowToolTipIcon(Window*);
	WindowToolTipIcon(const Point &, const Texture &, const std::string &, Window*);

	void			setTexture(const Texture &);
	void			renderWindow(void) override;

	const char*     	className(void) const override { return "SWE::WindowToolTipIcon"; }
#ifdef SWE_WITH_JSON
        JsonObject		toJson(void) const override
	{
	    JsonObject res = WindowToolTipArea::toJson();
	    res.addObject("icon", icon.toJson());
	    return res;
	}
#endif
    };

} // SWE
#endif

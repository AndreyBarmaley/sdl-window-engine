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

#ifndef _SWE_DISPLAY_
#define _SWE_DISPLAY_

#include "swe_fontset.h"

/// @brief пространство SWE
namespace SWE
{
    class Window;
    class FontRender;
    class UCString;
    class UnicodeString;

    /// @namespace Display
    namespace Display
    {
	/// @brief функция инициализации Display
        bool		init(const std::string &, bool landscape);
        bool		init(const std::string &, const Size & win, bool fullscreen, bool accel = true);
        bool		init(const std::string &, const Size & win, const Size & render, bool fullscreen, bool accel, bool resized);
        bool		resize(const Size &);

        /// @brief функция возвращает размер window
        const Size &	device(void);
        /// @brief функция возвращает максимально возможный регион для window
        Rect            usableBounds(void);

        /// @brief функция возвращает размер render
        const Size &	size(void);

        void            setForceWindowed(bool);
        bool		setFullscreenMode(bool);
        bool		isFullscreenWindow(void);
        bool            isMaximizedWindow(void);

        bool		scaleUsed(void);
        Point		scaleValue(const Point &);

	void		setWindowIcon(const Surface &);
        void		hardwareCursorHide(void);
        void		hardwareCursorShow(void);
        std::list<Size>	hardwareVideoModes(bool landscape);

	/// @brief функция обработки SDL_Event событий
	/// @result не используется
        bool		handleEvents(void);


	void            renderPresent(void);
        void 		renderSurface(const Surface &, const Rect &, Texture &, const Rect &, int flip = FlipNone);
        void 		renderTexture(const Texture &, const Point &);
        void 		renderTexture(const Texture &, const Rect &, Texture &, const Rect &, int flip = FlipNone);

        void		renderClear(const Color &, Texture &);
        void            renderColor(const Color &, Texture &, const Rect &);
        void		renderRect(const Color &, Texture &, const Rect &);
        void		renderLine(const Color &, Texture &, const Point &, const Point &);
        void		renderPoint(const Color &, Texture &, const Point &);
        void		renderPolygon(const Color &, Texture &, const Points &, bool closure = true);

        Rect		renderText(const FontRender &, const UnicodeString &, const Color &, Texture &, const Point &,
				    const AlignType & hAlign, const AlignType & vAlign, bool isHorizontal = true,
				    const CharRender & render = RenderDefault, int style = StyleDefault, const CharHinting & hinting = HintingDefault);

        Rect		renderTextFixed(const FontRender &, const UnicodeString &, const Color &, Texture &, const Point & pos, const Rect & fixed,
				    const AlignType & hAlign, const AlignType & vAlign, bool isHorizontal = true,
				    const CharRender & render = RenderDefault, int style = StyleDefault, const CharHinting & hinting = HintingDefault);

        bool		renderScreenshot(const std::string &);

        Texture &	texture(void);
        void		renderCursor(const Texture &);
        u32		timeStart(void);

        Point		mouseCursorPosition(void);
        u32		mouseButtonState(void);

        Texture		renderRect(const Color & rect, const Color & fill, const Size &, int = 1);
        Texture         renderText(const FontRender &, const UnicodeString &, const Color &, const Color & = Color::transparent());
        Texture         renderText(const FontRender &, const UCString &);

        u32             defaultPixelFormat(void);

	Surface		createSurface(const Texture &);
        Texture		createTexture(const Texture &, int flip = FlipNone);
        Texture		createTexture(const Texture &, const Rect &);
        Texture		createTexture(const Surface &);
        Texture		createTexture(const BinaryBuf &);
        Texture		createTexture(const std::string &);
        Texture		createTexture(const Size &, bool alpha = true);
    } // Display

} // SWE
#endif

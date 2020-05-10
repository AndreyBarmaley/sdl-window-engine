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

#ifndef _SWE_SURFACE_
#define _SWE_SURFACE_

#include <memory>

#include "swe_object.h"
#include "swe_colors.h"
#include "swe_rect.h"

namespace SWE
{

    class BinaryBuf;


#ifdef OLDENGINE
    typedef SDL_Surface SDL_Texture;
    namespace BlendMode
    {
        enum { None, Blend, Add, Mod };
    }
#else
    namespace BlendMode
    {
        enum { None = SDL_BLENDMODE_NONE, Blend = SDL_BLENDMODE_BLEND, Add = SDL_BLENDMODE_ADD, Mod = SDL_BLENDMODE_MOD };
    }
#endif

    class Surface : public ObjectClass
    {
    protected:
        SDL_Surface*	ptr;

    public:
        Surface();
        Surface(SDL_Surface*);
        Surface(const Size &, bool alpha = true);
        Surface(const Surface &);
        Surface(const std::string &);
        Surface(const BinaryBuf &);
        ~Surface();

        Surface &	operator= (const Surface &);
        Surface		copy(void) const;
        static Surface	copy(SDL_Surface*);

        bool		isValid(void) const;
        SDL_Surface*	toSDLSurface(void) const;

        bool            operator== (const Surface & sf) const;
        bool            operator!= (const Surface & sf) const;

        void		setSurface(const Surface &);
        void		reset(void);

        int		width(void) const;
        int		height(void) const;
        Size		size(void) const;
        Rect		rect(void) const;
        int		alphaMod(void) const;
        int		amask(void) const;
        int		flags(void) const;
        Color		colorKey(void) const;
        u32		pixel(const Point &) const;

        u32		mapRGB(const Color &);
        Color		RGBmap(u32) const;

        void		blit(const Rect &, const Rect &, Surface &) const;
        void		fill(const Rect &, const Color &);
        void		clear(const Color & col);
        void		drawPoint(const Point &, const Color &);
        void		drawPixel(const Point &, int);
        bool		save(const std::string &) const;
        void		swap(Surface &);

        void		setColorKey(const Color &);
        void		setAlphaMod(int);
        void		setFlags(int);
        void		resetFlags(int);

	const char*	className(void) const override { return "SWE::Surface"; }
#ifdef WITH_JSON
        JsonObject      toJson(void) const override;
#endif
    };

    const char* blendModeString(int);
    struct SDLTexture;

    class Texture : public ObjectClass
    {
    protected:
        std::shared_ptr<SDLTexture> ptr;

    public:
        Texture() {}
#ifdef OLDENGINE
        Texture(const Surface &);
#endif
        Texture(SDL_Texture*);
	virtual ~Texture() {}

        Texture(const Texture &) = default;
        Texture &	operator=(const Texture &) = default;

        Texture		copy(void) const;

        bool		operator== (const Texture &) const;
        bool		operator!= (const Texture &) const;

#ifdef OLDENGINE
        void		convertToDisplayFormat(void);
#endif

        void		setTexture(const Texture &);
        void		setAlphaMod(int);
        void		setColorMod(const Color &);
        void		setBlendMode(int);

        bool		isValid(void) const;
        SDL_Texture*	toSDLTexture(void) const;

        //Texture             copy(const Rect &) const;
        int		width(void) const;
        int		height(void) const;
        int		alphaMod(void) const;
        Color		colorMod(void) const;
        int		blendMode(void) const;
        Size		size(void) const;
        Rect		rect(void) const;
        void		reset(void);
        void		swap(Texture &);
        void		fill(const Rect &, const Color &);
        bool		save(const std::string &) const;

        std::string	toString(void) const;
        std::string	toStringID(void) const;

	const char*	className(void) const override { return "SWE::Texture"; }
#ifdef WITH_JSON
        JsonObject      toJson(void) const override;
#endif
    };

    class TexturePos : public Texture
    {
    protected:
        Point		pos;

    public:
        TexturePos() {}
        TexturePos(const Texture & tx) : Texture(tx) {}
        TexturePos(const Texture & tx, const Point & pt) : Texture(tx), pos(pt) {}

        TexturePos &	operator= (const Texture & tx);
        void		setPosition(const Point & pt);
        void		setPosition(int posx, int posy);

        const Point &	position(void) const;
        const Texture &	texture(void) const;
        Rect		area(void) const;

        void		swap(TexturePos &);

	const char*	className(void) const override { return "SWE::TexturePos"; }
#ifdef WITH_JSON
        JsonObject      toJson(void) const override;
#endif
    };

} // SWE
#endif

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
    enum { FlipNone = 0, FlipHorizontal = 0x01, FlipVertical = 0x02 };
#else
    enum { FlipNone = SDL_FLIP_NONE, FlipHorizontal = SDL_FLIP_HORIZONTAL, FlipVertical = SDL_FLIP_VERTICAL };
#endif
    enum { Rotate90Degrees = 0x04, Rotate180Degrees = 0x08, Rotate270Degrees = 0x10, FlipRotate = Rotate90Degrees | Rotate180Degrees | Rotate270Degrees };

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
	u32		pixel1(const Point &) const;
	u32		pixel2(const Point &) const;
	u32		pixel3(const Point &) const;
	u32		pixel4(const Point &) const;

	void		draw1(const Point &, u32 pixel);
	void		draw2(const Point &, u32 pixel);
	void		draw3(const Point &, u32 pixel);
	void		draw4(const Point &, u32 pixel);

    protected:
        SDL_Surface*	ptr;

    public:
        Surface();
        Surface(SDL_Surface*);
        Surface(const Size &, bool alpha = true);
        Surface(const Surface &);
        Surface(const std::string &);
        Surface(const BinaryBuf &);
        virtual ~Surface();

        Surface &	operator= (const Surface &);

        static Surface	copy(const Surface &, int flip = FlipNone);
        static Surface	copy(const Surface &, const Rect &);
	static Surface	scale(const Surface &, const Size &);

        bool		isValid(void) const;
        SDL_Surface*	toSDLSurface(void) const;

        bool            operator== (const Surface & sf) const;
        bool            operator!= (const Surface & sf) const;

        void		setSurface(const Surface &);
        void		reset(void);

#ifdef OLDENGINE
	void		convertToDisplayFormat(void);
#endif

        int		width(void) const;
        int		height(void) const;
        Size		size(void) const;
        Rect		rect(void) const;
        int		alphaMod(void) const;
        u32		amask(void) const;
        int		flags(void) const;
        Color		colorKey(void) const;
        u32		pixel(const Point &) const;

        u32		mapRGB(const Color &) const;
        Color		RGBmap(u32) const;

        void		blit(const Rect &, const Rect &, Surface &) const;
        void		fill(const Rect &, const Color &);
        void		clear(const Color & col);
        void		drawPoint(const Point &, const Color &);
        void		drawPixel(const Point &, u32);
        bool		save(const std::string &) const;
        void		swap(Surface &);

        void		setColorKey(const Color &);
        void		setAlphaMod(int);
        void		setFlags(int);
        void		resetFlags(int);

	static Surface	renderGrayScale(const Surface &);
	static Surface	renderSepia(const Surface &);

        std::string	toString(void) const;
	const char*	className(void) const override { return "SWE::Surface"; }
#ifdef WITH_JSON
        JsonObject      toJson(void) const override;
#endif
    };

    class SurfaceRef : public Surface
    {
    public:
	SurfaceRef(SDL_Surface* sf)
	{
    	    if(sf) sf->refcount++;
    	    ptr = sf;
	}
    };

    const char* blendModeString(int);
    //struct SDLTexture;

#ifdef OLDENGINE
    class Texture : public Surface
    {
#else
    class Texture : public ObjectClass
    {
    protected:
        std::shared_ptr<SDL_Texture> ptr;
#endif
    public:
        Texture() {}

#ifdef OLDENGINE
        Texture(const Surface & sf) : Surface(sf) {};
#endif
        Texture(SDL_Texture*);
	virtual ~Texture() {}

#ifndef OLDENGINE
        Texture(const Texture &) = default;
        Texture &	operator=(const Texture &) = default;
#endif

	static Texture	scale(const Texture &, const Size &);
        static Texture	copy(const Texture &, const Rect &);
        static Texture	copy(const Texture &, int flip = FlipNone);

        bool		operator== (const Texture &) const;
        bool		operator!= (const Texture &) const;

        void		setTexture(const Texture &);
        void		setAlphaMod(int);
        void		setColorMod(const Color &);
        void		setBlendMode(int);

        bool		isValid(void) const;
        SDL_Texture*	toSDLTexture(void) const;

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

	static Texture	renderGrayScale(const Texture &);
	static Texture	renderSepia(const Texture &);

        std::string	toString(void) const;
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

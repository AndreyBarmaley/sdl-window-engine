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
#include <sstream>

#include "systems.h"
#include "display.h"
#include "cstring.h"
#include "IMG_savepng.h"
#include "surface.h"

Surface::Surface() : ptr(NULL)
{
}

Surface::Surface(SDL_Surface* sf) : ptr(sf)
{
    if(! ptr)
	ERROR(SDL_GetError());
}

Surface::Surface(const Size & sz, bool alpha) : ptr(NULL)
{
#ifdef OLDENGINE
    int flag = SDL_SWSURFACE;
#else
    int flag = 0;
#endif

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    ptr = SDL_CreateRGBSurface(flag, sz.w, sz.h, 32, 0xff000000, 0x00ff0000, 0x0000ff00, (alpha ? 0x000000ff : 0));
#else
    ptr = SDL_CreateRGBSurface(flag, sz.w, sz.h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, (alpha ? 0xff000000 : 0));
#endif

    if(ptr)
    {
	if(amask())
	{
	    fill(rect(), Color::transparent());
	    //setAlphaMod(SDL_ALPHA_OPAQUE);
	}
	else
	{
	    Color colorKey = Color::colorKey();
	    fill(rect(), colorKey);
	    setColorKey(colorKey);
	    //setAlphaMod(0);
	}
    }
    else
    {
	ERROR(SDL_GetError());
    }
}

Surface::Surface(const Surface & other) : ptr(NULL)
{
    setSurface(other);
}

Surface::Surface(const std::string & file) : ptr(NULL)
{
#ifdef DISABLE_IMAGE
    ptr = SDL_LoadBMP(file.c_str());
#else
    ptr = IMG_Load(file.c_str());
#endif
    if(! ptr)
	ERROR(SDL_GetError());
}

Surface::Surface(const BinaryBuf & raw) : ptr(NULL)
{
    SDL_RWops *rw = SDL_RWFromConstMem(raw.data(), raw.size());
    if(! rw)
	ERROR(SDL_GetError());
#ifdef DISABLE_IMAGE
    ptr = SDL_LoadBMP_RW(rw, 1);
#else
    ptr = IMG_Load_RW(rw, 1);
#endif
    if(! ptr)
	ERROR(SDL_GetError());
}

Surface::~Surface()
{
    reset();
}

Surface & Surface::operator= (const Surface & other)
{
    setSurface(other);

    return *this;
}

void Surface::setSurface(const Surface & other)
{
    reset();

    if(other.ptr)
    {
    	ptr = other.ptr;
	ptr->refcount += 1;
    }
}

Surface Surface::copy(SDL_Surface* ptr)
{
    if(ptr)
        ptr->refcount += 1;
    return Surface(ptr);
}

Surface Surface::copy(void) const
{
    Surface sf;

    if(ptr)
       sf = Surface(SDL_ConvertSurface(ptr, ptr->format, ptr->flags));

    return sf;
}

void Surface::swap(Surface & sf)
{
    std::swap(ptr, sf.ptr);
}

void Surface::reset(void)
{
    if(ptr)
    {
    	if(1 < ptr->refcount)
	{
    	    --ptr->refcount;
	    ptr = NULL;
    	}
	else
    	{
#ifdef OLDENGINE
	    if(ptr != SDL_GetVideoSurface())
    		SDL_FreeSurface(ptr);
#else
    	    SDL_FreeSurface(ptr);
#endif
    	    ptr = NULL;
	}
    }
}

int Surface::width(void) const
{
    return ptr ? ptr->w : 0;
}

int Surface::height(void) const
{
    return ptr ? ptr->h : 0;
}

Size Surface::size(void) const
{
    return Size(width(), height());
}

u32 Surface::mapRGB(const Color & col)
{
    if(ptr)
    {
        return amask() ?
            SDL_MapRGBA(ptr->format, col.r(), col.g(), col.b(), col.a()) :
            SDL_MapRGB(ptr->format, col.r(), col.g(), col.b());
    }

    return 0;
}

Color Surface::RGBmap(u32 val) const
{
    Uint8 r = 0;
    Uint8 g = 0;
    Uint8 b = 0;
    Uint8 a = 0;

    if(ptr)
    {
        if(amask())
            SDL_GetRGBA(val, ptr->format, &r, &g, &b, &a);
        else
            SDL_GetRGB(val, ptr->format, &r, &g, &b);
    }

    return Color(r, g, b, a);
}

int Surface::flags(void) const
{
    return ptr ? ptr->flags : 0;
}

int Surface::amask(void) const
{
    return ptr ? ptr->format->Amask : 0;
}

int Surface::alphaMod(void) const
{
    Uint8 res = 0;

    if(ptr)
    {
#ifdef OLDENGINE
	res = ptr->format->alpha;
#else
        if(0 != SDL_GetSurfaceAlphaMod(ptr, & res))
            ERROR(SDL_GetError());
#endif
    }

    return res;
}

Color Surface::colorKey(void) const
{
    Uint32 key = 0;

    if(ptr)
    {
#ifdef OLDENGINE
	if(ptr->flags & SDL_SRCCOLORKEY)
    	    key = ptr->format->colorkey;
#else
        if(0 != SDL_GetColorKey(ptr, &key))
            ERROR(SDL_GetError());
#endif
    }

    return RGBmap(key);
}

void Surface::setColorKey(const Color & col)
{
    if(ptr)
    {
#ifdef OLDENGINE
	SDL_SetColorKey(ptr, SDL_SRCCOLORKEY, mapRGB(col));
#else
	SDL_SetColorKey(ptr, SDL_TRUE, mapRGB(col));
#endif
    }
}

void Surface::setAlphaMod(int val)
{
    if(ptr)
    {
#ifdef OLDENGINE
	if(0 != SDL_SetAlpha(ptr, (val ? SDL_SRCALPHA : 0), val))
            ERROR(SDL_GetError());
#else
        if(0 != SDL_SetSurfaceAlphaMod(ptr, val))
            ERROR(SDL_GetError());
#endif
    }
}

void Surface::setFlags(int f)
{
    if(ptr) ptr->flags |= f;
}

void Surface::resetFlags(int f)
{
    if(ptr) ptr->flags &= ~f;
}

void Surface::drawPoint(const Point & dpt, const Color & col)
{
    drawPixel(dpt, mapRGB(col));
}

void Surface::drawPixel(const Point & dpt, int val)
{
    if(ptr)
    {
	SDL_Rect dstRect = Rect(dpt, Size(1, 1)).toSDLRect();

	if(0 > SDL_FillRect(ptr, & dstRect, val))
    	    ERROR(SDL_GetError());
    }
}

u32 Surface::pixel(const Point & pt) const
{
    u32 res = 0;

    if(ptr)
    {
	if(pt.x < ptr->w && pt.y < ptr->h)
	{
	    SDL_LockSurface(ptr);
	    const u32* bufp = static_cast<u32*>(ptr->pixels) + pt.y * (ptr->pitch >> 2) + pt.x;
	    res = *bufp;
	    SDL_UnlockSurface(ptr);
	}
	else
	{
	    ERROR("out of range: " << "pos: " << pt.toString() << ", " << "size: " << size().toString());
	}
    }

    return res;
}

void Surface::fill(const Rect & drt, const Color & col)
{
    if(ptr)
    {
	SDL_Rect dstRect = drt.toSDLRect();

	if(0 > SDL_FillRect(ptr, & dstRect, mapRGB(col)))
	    ERROR(SDL_GetError());
    }
}

void Surface::blit(const Rect & srt, const Rect & drt, Surface & dsf) const
{
    if(ptr && dsf.ptr)
    {
	SDL_Rect srcRect = srt.toSDLRect();
	SDL_Rect dstRect = drt.toSDLRect();

	if(0 > SDL_BlitSurface(ptr, & srcRect, dsf.ptr, & dstRect))
	    ERROR(SDL_GetError());
    }
}

bool Surface::save(const std::string & file) const
{
    if(ptr)
    {
#ifdef DISABLE_IMAGE
	return 0 == SDL_SaveBMP(ptr, file.c_str());
#else
 #ifdef OLDENGINE
	return 0 == IMG_SavePNG(file.c_str(), ptr, 7);
 #else
	return 0 == IMG_SavePNG(ptr, file.c_str());
 #endif
#endif
    }
    return false;
}

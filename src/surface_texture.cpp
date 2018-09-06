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
#include "surface.h"

const char* blendModeString(int mode)
{
    switch(mode)
    {
	case BlendMode::None:	return "None";
	case BlendMode::Blend:	return "Blend";
	case BlendMode::Add:	return "Add";
	case BlendMode::Mod:	return "Mod";
	default: break;
    }

    return "Unknown";
}

struct SDLTexture
{
    SDL_Texture*        raw;

    SDLTexture(SDL_Texture* ptr = NULL) : raw(ptr)
    {
    }

    ~SDLTexture()
    {
#ifdef OLDENGINE
	if(raw) SDL_FreeSurface(raw);
#else
	if(raw) SDL_DestroyTexture(raw);
#endif
    }
};

#ifdef OLDENGINE
Texture::Texture(const Surface & sf)
{
    SDL_Surface* src = sf.toSDLSurface();
    if(src)
    {
	ptr = std::make_shared<SDLTexture>(src);
        src->refcount += 1;
    }
}
#else
Texture::Texture(SDL_Texture* tx) : ptr(std::make_shared<SDLTexture>(tx))
{
    if(! tx)
	ERROR(SDL_GetError());
}
#endif


void Texture::reset(void)
{
    ptr.reset();
}

void Texture::setTexture(const Texture & tx)
{
    ptr = tx.ptr;
}

void Texture::setBlendMode(int mode)
{
#ifdef OLDENGINE
#else
    SDL_BlendMode val = static_cast<SDL_BlendMode>(mode);
    if(isValid())
    {
	if(0 != SDL_SetTextureBlendMode(toSDLTexture(), val))
	    ERROR(SDL_GetError());
    }
#endif
}

void Texture::setColorMod(const Color & col)
{
#ifdef OLDENGINE
#else
    if(isValid())
    {
	if(0 != SDL_SetTextureColorMod(toSDLTexture(), col.r(), col.g(), col.b()))
	    ERROR(SDL_GetError());
    }
#endif
}

void Texture::setAlphaMod(int alpha)
{
    if(isValid())
    {
#ifdef OLDENGINE
	Display::createSurface(*this).setAlphaMod(alpha);
#else
	if(0 != SDL_SetTextureAlphaMod(toSDLTexture(), alpha))
	    ERROR(SDL_GetError());
#endif
    }
}

int Texture::blendMode(void) const
{
#ifdef OLDENGINE
    return BlendMode::None;
#else
    SDL_BlendMode val = SDL_BLENDMODE_NONE;

    if(isValid())
    {
	if(0 != SDL_GetTextureBlendMode(toSDLTexture(), &val))
	    ERROR(SDL_GetError());
    }

    return val;
#endif
}

int Texture::alphaMod(void) const
{
    Uint8 res = 0;

    if(isValid())
    {
#ifdef OLDENGINE
	SDL_Texture* tx = toSDLTexture();
	if(tx)
	    res = tx->format->alpha;
#else
	if(0 != SDL_GetTextureAlphaMod(toSDLTexture(), &res))
	    ERROR(SDL_GetError());
#endif
    }

    return res;
}

Color Texture::colorMod(void) const
{
    Uint8 r = 0;
    Uint8 g = 0;
    Uint8 b = 0;

    if(isValid())
    {
#ifdef OLDENGINE
#else
	if(0 != SDL_GetTextureColorMod(toSDLTexture(), &r, &g, &b))
	    ERROR(SDL_GetError());
#endif
    }

    return Color(r, g, b);
}

void Texture::swap(Texture & tx)
{
    ptr.swap(tx.ptr);
}

int Texture::width(void) const
{
    return size().w;
}

int Texture::height(void) const
{
    return size().h;
}

SDL_Texture* Texture::toSDLTexture(void) const
{
    SDLTexture* res = ptr.get();
    return res ? res->raw : NULL;
}

Size Texture::size(void) const
{
    int w = 0;
    int h = 0;
    if(isValid())
    {
#ifdef OLDENGINE
	SDL_Texture* tx = toSDLTexture();
	if(tx)
	{
	    w = tx->w;
	    h = tx->h;
	}
#else
	if(0 != SDL_QueryTexture(toSDLTexture(), NULL, NULL, &w, &h))
	    ERROR(SDL_GetError());
#endif
    }
    return Size(w, h);
}

void Texture::fill(const Rect & drt, const Color & col)
{
    Display::renderColor(col, *this, drt);
}

bool Texture::save(const std::string & file) const
{
    return Display::createSurface(*this).save(file);
}

Texture Texture::copy(void) const
{
    Texture res = Display::createTexture(size());
    Display::renderTexture(*this, rect(), res, rect());

    return res;
}

std::string Texture::toString(void) const
{
    std::ostringstream os;

    if(isValid())
    {
        os <<
            "size" << "(" << size().toString() << "), " <<
            "bpp" << "(" << 32 << "), " <<
            "alphaMod" << "(" << alphaMod() << "), " <<
            "colorMod" << "(" << colorMod().toHexString() << "), " <<
            "blendMode" << "(" << blendModeString(blendMode()) << ")";
    }

    return os.str();
}

std::string Texture::toStringID(void) const
{
    return String::hex64(reinterpret_cast<std::uintptr_t>(toSDLTexture()));
}

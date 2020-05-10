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

#include "swe_systems.h"
#include "swe_display.h"
#include "swe_cstring.h"
#include "swe_surface.h"

#ifdef WITH_JSON
#include "swe_json_ext.h"
#endif

namespace SWE
{
    const char* blendModeString(int mode)
    {
        switch(mode)
        {
            case BlendMode::None:
                return "None";

            case BlendMode::Blend:
                return "Blend";

            case BlendMode::Add:
                return "Add";

            case BlendMode::Mod:
                return "Mod";

            default:
                break;
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

            if(raw && raw != SDL_GetVideoSurface())
                SDL_FreeSurface(raw);

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
#endif

    Texture::Texture(SDL_Texture* tx) : ptr(std::make_shared<SDLTexture>(tx))
    {
        if(! tx)
            ERROR(SDL_GetError());
    }

#ifdef OLDENGINE
    void Texture::convertToDisplayFormat(void)
    {
        if(isValid())
            ptr = std::make_shared<SDLTexture>(SDL_DisplayFormat(toSDLTexture()));
    }
#endif

    void Texture::reset(void)
    {
        ptr.reset();
    }

    bool Texture::operator== (const Texture & tx) const
    {
        return toSDLTexture() == tx.toSDLTexture();
    }

    bool Texture::operator!= (const Texture & tx) const
    {
        return toSDLTexture() != tx.toSDLTexture();
    }

    bool Texture::isValid(void) const
    {
        return toSDLTexture();
    }

    void Texture::setTexture(const Texture & tx)
    {
        ptr = tx.ptr;
    }

    void Texture::setBlendMode(int mode)
    {
#ifdef OLDENGINE
        ERROR("not supported");
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
        ERROR("not supported");
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

            if(0 != SDL_SetAlpha(toSDLTexture(), (alpha ? SDL_SRCALPHA : 0), alpha))
                ERROR(SDL_GetError());

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
            ERROR("not supported");
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

    Rect Texture::rect(void) const
    {
        return Rect(Point(0, 0), size());
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
        return String::pointer(toSDLTexture());
    }

    /* TexturePos */
    TexturePos & TexturePos::operator= (const Texture & tx)
    {
        setTexture(tx);
        return *this;
    }
        
    void TexturePos::setPosition(const Point & pt)
    {
        pos = pt;
    }

    void TexturePos::setPosition(int posx, int posy)
    {
        pos = Point(posx, posy);
    }
        
    const Point & TexturePos::position(void) const
    {
        return pos;
    }
        
    const Texture & TexturePos::texture(void) const
    {
        return *this;
    }
        
    Rect TexturePos::area(void) const
    {
        return Rect(pos, size());
    }

    void TexturePos::swap(TexturePos & tp)
    {
        Texture::swap(tp);
        std::swap(pos, tp.pos);
    }

#ifdef WITH_JSON
    JsonObject Texture::toJson(void) const
    {
	JsonObject res = ObjectClass::toJson();

	if(! isValid())
	{
	    res.addBoolean("valid", false);
	    return res;
	}

	res.addString("self", String::pointer(this));
	res.addBoolean("valid", true);
	res.addArray("size", JsonPack::size(size()));
        res.addInteger("alphaMod", alphaMod());
#ifndef OLDENGINE
        res.addString("colorMod", colorMod().toString());
#endif
        res.addInteger("blendMode", blendMode());
	return res;
    }

    JsonObject TexturePos::toJson(void) const
    {
	JsonObject res = Texture::toJson();
	res.addArray("position", JsonPack::point(position()));
	return res;
    }
#endif
}

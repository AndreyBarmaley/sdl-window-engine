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

#ifdef SWE_WITH_JSON
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

#ifdef SWE_SDL12
    Texture::Texture()
    {
    }

    Texture::Texture(const Surface & sf) : Surface(sf)
    {
	convertToDisplayFormat();
    }

    Texture::Texture(SDL_Texture* tx) : Surface(tx)
    {
	convertToDisplayFormat();
    }
#else
    Texture::Texture() : ptr{nullptr, SDL_DestroyTexture}
    {
    }

    Texture::Texture(const Surface & sf) : ptr{nullptr, SDL_DestroyTexture}
    {
        auto tx = Display::createTexture(sf);
        std::swap(ptr, tx.ptr);
    }

    Texture::Texture(SDL_Texture* tx) : ptr{tx, SDL_DestroyTexture}
    {
        if(! tx)
            ERROR(SDL_GetError());
    }
#endif

    void Texture::reset(void)
    {
#ifdef SWE_SDL12
	Surface::reset();
#else
        ptr.reset();
#endif
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
#ifdef SWE_SDL12
	setSurface(tx);
#else
        ptr = tx.ptr;
#endif
    }

    void Texture::setBlendMode(int mode)
    {
#ifdef SWE_SDL12
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
#ifdef SWE_SDL12
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
#ifdef SWE_SDL12
            Surface::setAlphaMod(alpha);
#else
            if(0 != SDL_SetTextureAlphaMod(toSDLTexture(), alpha))
                ERROR(SDL_GetError());
#endif
        }
    }

    int Texture::blendMode(void) const
    {
#ifdef SWE_SDL12
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
#ifdef SWE_SDL12
	    return Surface::alphaMod();
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
#ifdef SWE_SDL12
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
#ifdef SWE_SDL12
	Surface::swap(tx);
#else
        ptr.swap(tx.ptr);
#endif
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
#ifdef SWE_SDL12
	return toSDLSurface();
#else
        return ptr.get();
#endif
    }

    Size Texture::size(void) const
    {
        int w = 0;
        int h = 0;

        if(isValid())
        {
#ifdef SWE_SDL12
	    return Surface::size();
#else
            if(0 != SDL_QueryTexture(toSDLTexture(), nullptr, nullptr, &w, &h))
                ERROR(SDL_GetError());
#endif
        }

        return Size(w, h);
    }

    void Texture::fill(const Rect & drt, const Color & col)
    {
#ifdef SWE_SDL12
        Surface::fill(drt, col);
#else
        Display::renderColor(col, *this, drt);
#endif
    }

    bool Texture::save(const std::string & file) const
    {
#ifdef SWE_SDL12
	return Surface::save(file);
#else
	return Display::createSurface(*this).save(file);
#endif
    }

    Texture Texture::scale(const Texture & tx, const Size & sz, bool smooth)
    {
#ifdef SWE_SDL12
	return Surface::scale(tx, sz, smooth);
#else
        Texture res = Display::createTexture(sz);
        Display::renderTexture(tx, tx.rect(), res, res.rect(), FlipNone);
        return res;
#endif
    }

    Texture Texture::copy(const Texture & tx, const Rect & rt)
    {
#ifdef SWE_SDL12
	return Surface::copy(tx, rt);
#else
        Texture res = Display::createTexture(rt.toSize());
        Display::renderTexture(tx, rt, res, res.rect(), FlipNone);
        return res;
#endif
    }

    Texture Texture::copy(const Texture & tx, int flip)
    {
#ifdef SWE_SDL12
	return Surface::copy(tx, flip);
#else
        Texture res = Display::createTexture(tx.size());
        Display::renderTexture(tx, tx.rect(), res, res.rect(), flip);
        return res;
#endif
    }

    Texture Texture::renderGrayScale(const Texture & tx)
    {
#ifdef SWE_SDL12
	return Surface::renderGrayScale(tx);
#else
	return Display::createTexture(Surface::renderGrayScale(Display::createSurface(tx)));
#endif
    }

    Texture Texture::renderSepia(const Texture & tx)
    {
#ifdef SWE_SDL12
	return Surface::renderSepia(tx);
#else
	return Display::createTexture(Surface::renderSepia(Display::createSurface(tx)));
#endif
    }

    std::string Texture::toString(void) const
    {
#ifdef SWE_SDL12
	return Surface::toString();
#else
        std::ostringstream os;

        if(isValid())
        {
            os <<
               "size" << "(" << size().toString() << "), " <<
               "bpp" << "(" << 32 << "), " <<
               "alphaMod" << "(" << alphaMod() << "), " <<
               "colorMod" << "(" << colorMod().toHexString() << "), " <<
               "blendMode" << "(" << blendModeString(blendMode()) << ")" <<
	       "refCount" << "(" << ptr.use_count() << ")";
        }

        return os.str();
#endif
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

#ifdef SWE_WITH_JSON
    JsonObject Texture::toJson(void) const
    {
#ifdef SWE_SDL12
	return Surface::toJson();
#else
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
        res.addString("colorMod", colorMod().toString());
        res.addInteger("blendMode", blendMode());
	res.addInteger("refCount", ptr.use_count());
	return res;
#endif
    }

    JsonObject TexturePos::toJson(void) const
    {
	JsonObject res = Texture::toJson();
	res.addArray("position", JsonPack::point(position()));
	return res;
    }
#endif
}

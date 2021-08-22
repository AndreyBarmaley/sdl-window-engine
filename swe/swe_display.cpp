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

#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <list>

#ifdef SWE_SDL12
 #include "SDL_rotozoom.h"
 #include "SDL_gfxPrimitives.h"
#endif

#include "swe_engine.h"
#include "swe_music.h"
#include "swe_display.h"
#include "swe_inputs_keys.h"
#include "swe_surface.h"
#include "swe_fontset.h"
#include "swe_display_scene.h"

namespace SWE
{
    namespace Display
    {
        bool            forceWindowed = false;
        bool		fingerEventEmulation = false;
        const int       fingerMoveDelta = 4;
#ifdef ANDROID
        const int       fingerGestureDelta = 15;
#endif

#ifdef SWE_SDL12
        SDL_Surface*	_window = nullptr;
#else
        SDL_Renderer*   _renderer = nullptr;
        SDL_Window*     _window = nullptr;
        SDL_SpinLock	renderLock = 0;
#endif

        Texture		displayTexture;

        ButtonsEvent	mouseButtons[6]; /* FingerTap, ButtonLeft, ButtonRight, ButtonMiddle, ButtonX1, ButtonX2 */
        int		fingerIndex = 0;

        u32		tickStart = 0;

        Size            winsz;
        Size		rendersz;
        Rect		scale;

        SDL_Event	current;

        bool		createWindow(const std::string &, const Size &, int);
        bool		resizeWindow(const Size &);

        void            closeWindow(void);

        void            redraw(void);

        bool		renderInit(const Size &, bool);
        bool		renderReset(SDL_Texture*);
        bool		isRenderAccelerated(void);
        void            renderPresent(void);
        void		renderCopyEx(const Texture &, const Rect &, Texture &, const Rect &, int);

        void            handleMouseButton(const SDL_MouseButtonEvent &);
        void            handleMouseMotion(const SDL_MouseMotionEvent &);
        void            handleKeyboard(const SDL_KeyboardEvent &);
        void            handleUserEvent(const SDL_UserEvent &);
#ifdef SWE_SDL12
        void            handleMouseWheel(int, int);
#else
        void            handleMouseWheel(const SDL_MouseWheelEvent &);
        void            handleTextInput(const SDL_TextInputEvent &);
        void		handleFingerTap(const SDL_TouchFingerEvent &);
        void		handleFingerMotion(const SDL_TouchFingerEvent &);
#endif
        void            addWindow(Window &);
        void            delWindow(Window &);

        Rect		renderTextFixedHorizontal(const FontRender &, const UnicodeString &, const Color &, Texture &, const Point &, const Rect &, const AlignType &, const AlignType &, const CharRender &, int style, const CharHinting &);
        Rect		renderTextFixedVertical(const FontRender &, const UnicodeString &, const Color &, Texture &, const Point &, const Rect &, const AlignType &, const AlignType &, const CharRender &, int style, const CharHinting &);
        Rect		renderTextHorizontal(const FontRender &, const UnicodeString &, const Color &, Texture &, const Point &, const AlignType &, const AlignType &, const CharRender &, int style, const CharHinting &);
        Rect		renderTextVertical(const FontRender &, const UnicodeString &, const Color &, Texture &, const Point &, const AlignType &, const AlignType &, const CharRender &, int style, const CharHinting &);
    }
}

bool SWE::Display::init(const std::string & title, bool landscape)
{
    auto modes = hardwareVideoModes(landscape);

    if(modes.empty())
    {
        ERROR("video modes empty");
        return false;
    }

    return init(title, modes.back(), true, true);
}

bool SWE::Display::init(const std::string & title, const Size & win, bool fullscreen, bool accel)
{
    if(Systems::isEmbeded() || fullscreen || win.isEmpty())
    {
        Size winsz = SWE::Display::size();
        Size rensz = win;
        DEBUG("window params: " << winsz.toString());
        DEBUG("render params: " << rensz.toString());

        if(winsz.isEmpty())
        {
            // modes sorting landscape
            auto modes = hardwareVideoModes(true);
            winsz = modes.back();
        }

        if(rensz.isEmpty())
            rensz = winsz;
        else
        {
            // rotate win
            if((rensz.w < rensz.h && winsz.w > winsz.h) ||
               (rensz.w > rensz.h && winsz.w < winsz.h))
            {
                std::swap(winsz.w, winsz.h);
                DEBUG("rotate window as render: " << winsz.toString());

                if(winsz < rensz)
                {
                    winsz = rensz;
                    DEBUG("fixed window to render: " << winsz.toString());
                }
            }
        }

        return init(title, winsz, rensz, true, accel, false);
    }

    return init(title, win, win, fullscreen, accel, false);
}

bool SWE::Display::init(const std::string & title, const Size & winsz, const Size & rensz, bool fullscreen, bool accel, bool resized)
{
    closeWindow();
    DEBUG("window params: " << winsz.toString());
    DEBUG("render params: " << rensz.toString());
    // reset old size
    rendersz = Size(0, 0);
#ifdef SWE_SDL12
    int flags = accel ? SDL_SWSURFACE | SDL_HWSURFACE | SDL_DOUBLEBUF : SDL_SWSURFACE;
#else
    int flags = 0;
#endif

    if(Systems::isEmbeded())
        fullscreen = true;

    if(forceWindowed)
        fullscreen = false;

#ifdef SWE_SDL12

    if(fullscreen)
        flags |= SDL_FULLSCREEN;
    else
    if(resized)
        flags |= SDL_RESIZABLE;

#else

    if(fullscreen)
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    else
    if(resized)
        flags |= SDL_WINDOW_RESIZABLE;

#endif
#ifdef ANDROID
    flags |= SDL_WINDOW_FULLSCREEN;
    flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    flags &= ~SDL_WINDOW_RESIZABLE;
#endif

    return createWindow(title, winsz, flags) ? renderInit(rensz, accel) : false;
}

bool SWE::Display::createWindow(const std::string & title, const Size & newsz, int flags)
{
    mouseButtons[0] = ButtonsEvent(FingerTap);
    mouseButtons[1] = ButtonsEvent(ButtonLeft);
    mouseButtons[2] = ButtonsEvent(ButtonRight);
    mouseButtons[3] = ButtonsEvent(ButtonMiddle);
    mouseButtons[4] = ButtonsEvent(ButtonX1);
    mouseButtons[5] = ButtonsEvent(ButtonX2);
    winsz = newsz;
#ifdef SWE_SDL12
    SDL_WM_SetCaption(title.c_str(), nullptr);
    _window = SDL_SetVideoMode(winsz.w, winsz.h, 32, flags);
    winsz.w = _window->w;
    winsz.h = _window->h;
#else
    _window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winsz.w, winsz.h, flags);
    SDL_GetWindowSize(_window, &winsz.w, &winsz.h);
#endif

    if(! _window)
    {
        ERROR(SDL_GetError());
        return false;
    }

    DEBUG("window: " << winsz.w << "x" << winsz.h);

    if(Systems::isEmbeded() && !forceWindowed)
        hardwareCursorHide();

    tickStart = Tools::ticks();
    return true;
}

bool SWE::Display::resizeWindow(const Size & newsz)
{
    if(_window && winsz != newsz)
    {
        bool accel = isRenderAccelerated();
        DEBUG("new sz: " << newsz.toString());
        FontRender::clearCache();
	Size realsz;

#ifdef SWE_SDL12
        _window = SDL_SetVideoMode(newsz.w, newsz.h, _window->format->BitsPerPixel, _window->flags);
	realsz.w = _window->w;
	realsz.h = _window->h;
#else
        SDL_SetWindowSize(_window, newsz.w, newsz.h);
	SDL_GetWindowSize(_window, &realsz.w, &realsz.h);
        DEBUG("real resized: " << realsz.toString());
#endif

        if(renderInit(realsz, accel))
        {
            DisplayScene::textureInvalidHandle();
            DisplayScene::displayResizeHandle(realsz);
            DisplayScene::setDirty(true);
            return true;
        }
    }

    return false;
}

bool SWE::Display::renderInit(const Size & newsz, bool accel)
{
    // save aspect ratio with resize window
    if(rendersz.isEmpty() || rendersz == winsz)
        rendersz = newsz;

    if(displayTexture.isValid())
        displayTexture.reset();

#ifdef SWE_SDL12
    _window = SDL_GetVideoSurface();
    // update winsz
    winsz.w = _window->w;
    winsz.h = _window->h;

    if(winsz != rendersz)
    {
	displayTexture = createTexture(rendersz, true);
	// sdl12 texture default converted
	// displayTexture.convertToDisplayFormat();
	DEBUG("set display to soft surface");
    }
    else
    {
	displayTexture.setSurface(SDL_GetVideoSurface());
	DEBUG("set display to video surface");
    }
#else

    if(_renderer)
    {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }

    _renderer = SDL_CreateRenderer(_window, -1, (accel ? SDL_RENDERER_ACCELERATED : SDL_RENDERER_SOFTWARE));

    if(! _renderer)
    {
        if(accel)
        {
            ERROR(SDL_GetError());
            DEBUG("switch to render software");
            _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_SOFTWARE);
            accel = false;
        }

        if(! _renderer)
        {
            ERROR(SDL_GetError());
            return false;
        }
    }

    if(! renderReset(nullptr))
        return false;

    // update winsz
    SDL_GetWindowSize(_window, &winsz.w, &winsz.h);

    // render info
    if(1)
    {
        SDL_Rect port;
        SDL_RenderGetViewport(_renderer, &port);
        DEBUG("viewport: " << Rect(port).toString());
        const char* hint = SDL_GetHint(SDL_HINT_ORIENTATIONS);
        DEBUG("orientation hint: " << (hint ? hint : "empty"));
    }

    // default: alpha blending enabled
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
    // set displayTexture
    displayTexture = Texture(SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, rendersz.w, rendersz.h));
#endif

    if(! displayTexture.isValid())
    {
        ERROR(SDL_GetError());
        return false;
    }

    renderClear(Color::Black, displayTexture);
    DEBUG("window: " << winsz.w << "x" << winsz.h);
    DEBUG("render: " << rendersz.w << "x" << rendersz.h);
    DEBUG("render: " << (accel ? "hardware" : "software"));
    DEBUG("display texture: " << String::pointer(displayTexture.toSDLTexture()));

    //if(! renderReset(displayTexture.toSDLTexture()))
    //    return false;

    renderReset(nullptr);

    if(winsz != rendersz)
    {
        float scaleX = rendersz.w / static_cast<float>(winsz.w);
        float scaleY = rendersz.h / static_cast<float>(winsz.h);
        float factor = scaleY > scaleX ? scaleY : scaleX;
        DEBUG("scale factor: " << factor);
        scale.w = rendersz.w / factor;
        scale.h = rendersz.h / factor;
        scale.x = (winsz.w - scale.w) / 2;
        scale.y = (winsz.h - scale.h) / 2;
    }
    else
    {
        scale.w = rendersz.w;
        scale.h = rendersz.h;
        scale.x = 0;
        scale.y = 0;
    }

    DisplayScene::setDirty(false);
    return true;
}

void SWE::Display::closeWindow(void)
{
    FontRender::clearCache();
    DisplayScene::sceneDestroy();

    if(displayTexture.isValid())
        displayTexture.reset();

#ifdef SWE_SDL12

    if(_window)
        _window = nullptr;

#else

    if(_renderer)
    {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }

    if(_window)
    {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }

#endif
}

bool SWE::Display::isRenderAccelerated(void)
{
#ifdef SWE_SDL12

    if(_window)
        return _window->flags & SDL_HWACCEL;

#else

    if(_renderer)
    {
        SDL_RendererInfo info;

        if(0 != SDL_GetRendererInfo(_renderer, &info))
        {
            ERROR("get info: " << SDL_GetError());
            return false;
        }

        return info.flags & SDL_RENDERER_ACCELERATED;
    }

#endif
    return false;
}

bool SWE::Display::renderReset(SDL_Texture* target)
{
#ifndef SWE_SDL12

    if(! _renderer)
        return false;

/*
    SDL_RendererInfo info;

    if(0 != SDL_GetRendererInfo(_renderer, & info))
    {
        ERROR("get info: " << SDL_GetError());
        return false;
    }

    if(!(info.flags & SDL_RENDERER_TARGETTEXTURE))
    {
        ERROR("not target texture: " << String::pointer(target));
        return false;
    }
*/

    if(target)
    {   
        int access;
        
        if(0 != SDL_QueryTexture(target, nullptr, & access, nullptr, nullptr))
        {
            ERROR("query texture: " << SDL_GetError());
            return false;
        }

        if(access != SDL_TEXTUREACCESS_TARGET)
        {
            ERROR("not target texture: " << String::pointer(target));
            return false;
        }
    }

    if(0 != SDL_SetRenderTarget(_renderer, target))
    {
        ERROR("set target: " << SDL_GetError());
        return false;
    }

#endif
    return true;
}

void SWE::Display::renderClear(const Color & cl, Texture & dtx)
{
#ifdef SWE_SDL12
    renderColor(cl, dtx, dtx.rect());
#else
    SDL_AtomicLock(& renderLock);
    if(renderReset(dtx.toSDLTexture()))
    {
    	if(0 == SDL_SetRenderDrawColor(_renderer, cl.r(), cl.g(), cl.b(), cl.a()))
    	{
    	    if(0 != SDL_RenderClear(_renderer))
            	ERROR(SDL_GetError());
    	}
    	else
    	    ERROR(SDL_GetError());
    }
    else
    	FIXME(String::pointer(dtx.toSDLTexture()));
    SDL_AtomicUnlock(& renderLock);
#endif
}

void SWE::Display::renderColor(const Color & cl, Texture & dtx, const Rect & drt)
{
#ifdef SWE_SDL12
    boxRGBA(dtx.toSDLSurface(), drt.x, drt.y, drt.x + drt.w - 1, drt.y + drt.h - 1, cl.r(), cl.g(), cl.b(), cl.a());
#else
    SDL_AtomicLock(& renderLock);
    if(renderReset(dtx.toSDLTexture()))
    {
    	if(0 == SDL_SetRenderDrawColor(_renderer, cl.r(), cl.g(), cl.b(), cl.a()))
    	{
    	    const SDL_Rect dstrt = drt.toSDLRect();

    	    if(0 != SDL_RenderFillRect(_renderer, &dstrt))
                ERROR(SDL_GetError());
    	}
    	else
    	    ERROR(SDL_GetError());
    }
    else
    	FIXME(String::pointer(dtx.toSDLTexture()));
    SDL_AtomicUnlock(& renderLock);
#endif
}

SWE::Texture SWE::Display::renderRect(const Color & col, const Color & fill, const Size & sz, int border)
{
    Texture res = createTexture(sz);
    renderClear(fill, res);

    if(col != fill && 0 < border && border < sz.w / 2 && border < sz.h / 2)
    {
        for(int ii = 0; ii < border; ++ii)
            renderRect(col, res, Rect(Point(ii, ii), sz - Size(ii * 2, ii * 2)));
    }

    return res;
}

void SWE::Display::renderRect(const Color & cl, Texture & dtx, const Rect & drt)
{
#ifdef SWE_SDL12
    rectangleRGBA(dtx.toSDLSurface(), drt.x, drt.y, drt.x + drt.w - 1, drt.y + drt.h - 1, cl.r(), cl.g(), cl.b(), cl.a());
#else

    SDL_AtomicLock(& renderLock);
    if(renderReset(dtx.toSDLTexture()))
    {
        if(0 == SDL_SetRenderDrawColor(_renderer, cl.r(), cl.g(), cl.b(), cl.a()))
        {
            const SDL_Rect dstrt = drt.toSDLRect();

            if(0 != SDL_RenderDrawRect(_renderer, &dstrt))
                ERROR(SDL_GetError());

	    // last point (bug)
	    if(0 != SDL_RenderDrawPoint(_renderer, dstrt.x + dstrt.w - 1, dstrt.y + dstrt.h - 1))
                ERROR(SDL_GetError());
        }
        else
            ERROR(SDL_GetError());
    }
    else
        FIXME(String::pointer(dtx.toSDLTexture()));
    SDL_AtomicUnlock(& renderLock);

#endif
}

void SWE::Display::renderPolygon(const Color & cl, Texture & dtx, const Points & v, bool closure)
{
    const Point* pt1 = nullptr;
    const Point* pt2 = nullptr;

    for(size_t it = 0; it < v.size(); ++it)
    {
        pt1 = & v[it];
        pt2 = it + 1 < v.size() ? & v[it + 1] :
              (closure ? & v[0] : nullptr);

        if(pt2) renderLine(cl, dtx, *pt1, *pt2);
    }
}

void SWE::Display::renderLine(const Color & cl, Texture & dtx, const Point & pt1, const Point & pt2)
{
#ifdef SWE_SDL12
    lineRGBA(dtx.toSDLSurface(), pt1.x, pt1.y, pt2.x, pt2.y, cl.r(), cl.g(), cl.b(), cl.a());

#else

    SDL_AtomicLock(& renderLock);
    if(renderReset(dtx.toSDLTexture()))
    {
        if(0 == SDL_SetRenderDrawColor(_renderer, cl.r(), cl.g(), cl.b(), cl.a()))
        {
            if(0 != SDL_RenderDrawLine(_renderer, pt1.x, pt1.y, pt2.x, pt2.y))
                ERROR(SDL_GetError());
        }
        else
            ERROR(SDL_GetError());
    }
    else
        FIXME(String::pointer(dtx.toSDLTexture()));
    SDL_AtomicUnlock(& renderLock);

#endif
}

void SWE::Display::renderPoint(const Color & cl, Texture & dtx, const Point & pt)
{
#ifdef SWE_SDL12
    pixelRGBA(dtx.toSDLSurface(), pt.x, pt.y, cl.r(), cl.g(), cl.b(), cl.a());
#else

    SDL_AtomicLock(& renderLock);
    if(renderReset(dtx.toSDLTexture()))
    {
        if(0 == SDL_SetRenderDrawColor(_renderer, cl.r(), cl.g(), cl.b(), cl.a()))
        {
            if(0 != SDL_RenderDrawPoint(_renderer, pt.x, pt.y))
                ERROR(SDL_GetError());
        }
        else
            ERROR(SDL_GetError());
    }
    else
        FIXME(String::pointer(dtx.toSDLTexture()));
    SDL_AtomicUnlock(& renderLock);

#endif
}

void SWE::Display::renderTexture(const Texture & stx, const Point & dpt)
{
    if(stx.isValid() && stx != displayTexture)
        renderCopyEx(stx, stx.rect(), displayTexture, Rect(dpt, stx.size()), FlipNone);
}

void SWE::Display::renderTexture(const Texture & stx, const Rect & srt, Texture & dtx, const Rect & drt, int flip)
{
    if(stx.isValid() && stx != dtx)
    {
#ifndef SWE_SDL12
        dtx.setBlendMode(BlendMode::None);
#endif
        renderCopyEx(stx, srt, dtx, drt, flip);
#ifndef SWE_SDL12
        dtx.setBlendMode(BlendMode::Blend);
#endif
    }
}

void SWE::Display::renderCursor(const Texture & tx)
{
    DisplayScene::setCursor(tx, Point(0, 0));
}

void SWE::Display::renderCopyEx(const Texture & stx, const Rect & srt, Texture & dtx, const Rect & drt, int flip)
{
#ifdef SWE_SDL12
    if(srt.toSize() != drt.toSize())
    {
	auto stx2 = Texture::copy(stx, srt);
	auto dtx2 = Texture::scale(stx2, drt.toSize());

	if(FlipNone == flip)
	    dtx2.blit(dtx2.rect(), drt, dtx);
	else
	if(flip & (Rotate90Degrees | Rotate180Degrees))
	    Texture::copy(dtx2, flip).blit(dtx2.rect().swapSize(), drt.swapSize(), dtx);
	else
	    Texture::copy(dtx2, flip).blit(dtx2.rect(), drt, dtx);
    }
    else
    {
	if(FlipNone == flip)
	    stx.blit(srt, drt, dtx);
	else
	if(flip & (Rotate90Degrees | Rotate180Degrees))
	    Texture::copy(stx, flip).blit(srt.swapSize(), drt.swapSize(), dtx);
	else
	    Texture::copy(stx, flip).blit(srt, drt, dtx);
    }

    if(dtx == displayTexture)
        DisplayScene::setDirty(true);
#else
    const SDL_Rect srcrt = srt.toSDLRect();
    const SDL_Rect dstrt = drt.toSDLRect();
    SDL_AtomicLock(& renderLock);

    if(renderReset(dtx.toSDLTexture()))
    {
	int angle = 0;

        if(flip & Rotate90Degrees)
	    angle = 90;
        else
        if(flip & Rotate180Degrees)
	    angle = 180;
        else
        if(flip & Rotate270Degrees)
	    angle = 270;

        int res = flip == FlipNone ? SDL_RenderCopy(_renderer, stx.toSDLTexture(), &srcrt, &dstrt) :
                  SDL_RenderCopyEx(_renderer, stx.toSDLTexture(), &srcrt, &dstrt, angle, nullptr, (SDL_RendererFlip) flip);

        if(res)
            ERROR(SDL_GetError() << ", " << String::pointer(stx.toSDLTexture()));

        if(dtx == displayTexture)
            DisplayScene::setDirty(true);
    }
    else
        FIXME(String::pointer(dtx.toSDLTexture()));

    SDL_AtomicUnlock(& renderLock);
#endif
}

void SWE::Display::renderSurface(const Surface & sf, const Rect & srt, Texture & dtx, const Rect & drt, int flip)
{
#ifdef SWE_SDL12

    if(sf.isValid())
        renderTexture(Texture(sf), srt, dtx, drt, flip);

#else

    if(sf.isValid())
        renderTexture(Texture(SDL_CreateTextureFromSurface(_renderer, sf.toSDLSurface())), srt, dtx, drt, flip);

#endif
}

void SWE::Display::renderPresent(void)
{
#ifdef SWE_SDL12

    if(winsz != rendersz)
    {
        SDL_Rect dstrt = scale.toSDLRect();
        float zoomx = dstrt.w / static_cast<float>(rendersz.w);
        float zoomy = dstrt.h / static_cast<float>(rendersz.h);
        SDL_Surface* zoomsf = zoomSurface(displayTexture.toSDLTexture(), zoomx, zoomy, 1);

        if(zoomsf)
        {
            if(0 > SDL_BlitSurface(zoomsf, nullptr, _window, & dstrt))
                ERROR(SDL_GetError() << ", " << String::pointer(displayTexture.toSDLTexture()));

            SDL_FreeSurface(zoomsf);
        }
        else
            ERROR("zoomSurface: " << SDL_GetError());
    }

    if(0 > SDL_Flip(_window))
        ERROR(SDL_GetError());

#else
    SDL_AtomicLock(& renderLock);

    if(renderReset(nullptr))
    {
        if(0 != SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0))
            ERROR("set color: " << SDL_GetError());

        if(0 != SDL_RenderClear(_renderer))
            ERROR("clear: " << SDL_GetError());

        if(winsz != rendersz)
        {
            SDL_Rect dstrt = scale.toSDLRect();

            if(0 != SDL_RenderCopy(_renderer, displayTexture.toSDLTexture(), nullptr, & dstrt))
                ERROR("copy: " << SDL_GetError());
        }
        else if(0 != SDL_RenderCopy(_renderer, displayTexture.toSDLTexture(), nullptr, nullptr))
            ERROR("copy: " << SDL_GetError());

        SDL_RenderPresent(_renderer);
    }

    SDL_AtomicUnlock(& renderLock);
#endif
    DisplayScene::setDirty(false);
}

bool SWE::Display::renderScreenshot(const std::string & file)
{
    return createSurface(displayTexture).save(file);
}

bool SWE::Display::scaleUsed(void)
{
    return winsz != rendersz;
}

SWE::Point SWE::Display::scaleValue(const Point & pt)
{
    return Point((pt.x - scale.x) * rendersz.w / scale.w, (pt.y - scale.y) * rendersz.h / scale.h);
}

const SWE::Size & SWE::Display::device(void)
{
    return winsz;
}

const SWE::Size & SWE::Display::size(void)
{
    return rendersz;
}

u32 SWE::Display::timeStart(void)
{
    return tickStart;
}

SWE::Texture SWE::Display::createTexture(const Surface & sf)
{
#ifdef SWE_SDL12
    return Texture(sf);
#else
    SDL_AtomicLock(& renderLock);
    auto ptr = SDL_CreateTextureFromSurface(_renderer, sf.toSDLSurface());
    SDL_AtomicUnlock(& renderLock);
    return Texture(ptr);
#endif
}

SWE::Texture SWE::Display::createTexture(const BinaryBuf & raw)
{
    SDL_RWops* rw = SDL_RWFromConstMem(raw.data(), raw.size());

    if(! rw)
        ERROR(SDL_GetError());

#ifdef SWE_DISABLE_IMAGE
    return createTexture(Surface(SDL_LoadBMP_RW(rw, 1)));
#else
#ifdef SWE_SDL12
    return Texture(IMG_Load_RW(rw, 1));
#else
    return Texture(IMG_LoadTexture_RW(_renderer, rw, 1));
#endif
#endif
}

SWE::Texture SWE::Display::createTexture(const std::string & file)
{
#ifdef SWE_DISABLE_IMAGE
    return createTexture(Surface(SDL_LoadBMP(file.c_str())));
#else
#ifdef SWE_SDL12
    return Texture(IMG_Load(file.c_str()));
#else
    return Texture(IMG_LoadTexture(_renderer, file.c_str()));
#endif
#endif
}

SWE::Texture SWE::Display::createTexture(const Size & sz, bool alpha)
{
#ifdef SWE_SDL12
    return Texture(Surface(sz, alpha));
#else
    SDL_AtomicLock(& renderLock);
    auto ptr = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, sz.w, sz.h);
    SDL_AtomicUnlock(& renderLock);
    Texture res(ptr);
    renderClear(Color::Transparent, res);
    res.setBlendMode(BlendMode::Blend);
    return res;
#endif
}

SWE::Texture SWE::Display::createTexture(const Texture & tx, const Rect & rt)
{
    Texture res = createTexture(rt.toSize(), false);
    renderTexture(tx, rt, res, res.rect());
    return res;
}

SWE::Texture SWE::Display::createTexture(const Texture & tx, int flip)
{
    Texture res = createTexture(tx.size());
    renderTexture(tx, tx.rect(), res, res.rect(), flip);
    return res;
}

SWE::Texture & SWE::Display::texture(void)
{
    return displayTexture;
}

SWE::Surface SWE::Display::createSurface(const Texture & tx)
{
#ifdef SWE_SDL12
    return Texture::copy(tx);
#else
    SDL_Surface* sf = nullptr;
    Texture tx2 = createTexture(tx, FlipNone);

    SDL_AtomicLock(& renderLock);
    if(renderReset(tx2.toSDLTexture()))
    {
        SDL_Rect srt = tx2.rect().toSDLRect();
        sf = SDL_CreateRGBSurface(0, srt.w, srt.h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

        if(! sf || 0 != SDL_RenderReadPixels(_renderer, & srt, SDL_PIXELFORMAT_ARGB8888, sf->pixels, sf->pitch))
            ERROR(SDL_GetError());
    }
    else
        FIXME(String::pointer(tx2.toSDLTexture()));
    SDL_AtomicUnlock(& renderLock);

    renderReset(nullptr);
    return Surface(sf);
#endif
}

bool SWE::Display::handleEvents(void)
{
    while(SDL_PollEvent(& current))
    {
        switch(current.type)
        {
#ifdef SWE_SDL12

            case SDL_VIDEORESIZE:
                resizeWindow(Size(current.resize.w, current.resize.h));
                break;

            case SDL_ACTIVEEVENT:
                if(current.active.state & SDL_APPACTIVE)
                {
		    DisplayScene::displayFocusHandle(current.active.gain);
                }
                break;

            case SDL_VIDEOEXPOSE:
                // update the screen
                SDL_Flip(_window);
                break;
#else

            case SDL_WINDOWEVENT:
                if(SDL_WINDOWEVENT_EXPOSED == current.window.event)
                    renderPresent();
                else
		if(SDL_WINDOWEVENT_RESIZED == current.window.event)
                    DEBUG("resize: " << current.window.data1 << ", " << current.window.data2);

		if(SDL_WINDOWEVENT_SIZE_CHANGED == current.window.event)
                    resizeWindow(Size(current.window.data1, current.window.data2));
                else
		if(SDL_WINDOWEVENT_FOCUS_GAINED == current.window.event)
		    DisplayScene::displayFocusHandle(true);
                else
		if(SDL_WINDOWEVENT_FOCUS_LOST == current.window.event)
		    DisplayScene::displayFocusHandle(false);
                break;

            case SDL_TEXTINPUT:
                handleTextInput(current.text);
                break;

            case SDL_FINGERDOWN:
            case SDL_FINGERUP:
                handleFingerTap(current.tfinger);
                break;

            case SDL_FINGERMOTION:
                handleFingerMotion(current.tfinger);
                break;
#endif

            case SDL_KEYDOWN:
            case SDL_KEYUP:
                handleKeyboard(current.key);
                break;
                //	    case SDL_TEXTEDITING:
                //		VERBOSE("edit: " << current.edit.text << ", " << current.edit.start);
                //		break;
#if !(defined ANDROID)

            case SDL_MOUSEBUTTONDOWN:
                //DEBUG("DOWN: " << current.button.x << ", " << current.button.y);
                handleMouseButton(current.button);
                break;

            case SDL_MOUSEBUTTONUP:
                //DEBUG("UP: " << current.button.x << ", " << current.button.y);
                handleMouseButton(current.button);
                break;

            case SDL_MOUSEMOTION:
                //DEBUG("MOTION: " << current.button.x << ", " << current.button.y);
                handleMouseMotion(current.motion);
                break;
#ifndef SWE_SDL12

            case SDL_MOUSEWHEEL:
                handleMouseWheel(current.wheel);
                break;
#endif
#endif

            case SDL_USEREVENT:
                handleUserEvent(current.user);
                break;

            case SDL_JOYAXISMOTION:
                FIXME("SDL_JoyAxisMotion");
                break;

            case SDL_JOYBALLMOTION:
                FIXME("SDL_JoyBallMotion");
                break;

            case SDL_JOYHATMOTION:
                FIXME("SDL_JoyHatMotion");
                break;

            case SDL_JOYBUTTONDOWN:
                FIXME("SDL_JoyButtonDown");
                break;

            case SDL_JOYBUTTONUP:
                FIXME("SDL_JoyButtonUp");
                break;

            case SDL_QUIT:
                // DEBUG("SDL_QUIT");
                Engine::except(__FUNCTION__, "SDL_QUIT");
                return false;

            default:
                break;
        }
    }

    return true;
}

// enum { ScrollNone, ScrollUp, ScrollDown, ScrollLeft, ScrollRight };

void SWE::Display::handleKeyboard(const SDL_KeyboardEvent & ev)
{
    KeySym key(ev.keysym);

    switch(ev.type)
    {
        case SDL_KEYDOWN:
            DisplayScene::keyHandle(key, true);
            break;

        case SDL_KEYUP:
            DisplayScene::keyHandle(key, false);
            break;

        default:
            break;
    }

    switch(key.keycode())
    {
        case Key::PAGEUP:
            DisplayScene::scrollHandle(true);
            break;

        case Key::PAGEDOWN:
            DisplayScene::scrollHandle(false);
            break;

        default:
            break;
    }
}

void SWE::Display::handleMouseButton(const SDL_MouseButtonEvent & ev)
{
    ButtonsEvent* coords = nullptr;

    switch(ev.button)
    {
#ifdef SWE_SDL12

        case SDL_BUTTON_WHEELUP:
        case SDL_BUTTON_WHEELDOWN:
            handleMouseWheel(ev.button, ev.type);
            break;
#else

        case SDL_BUTTON_X1:
            coords = &mouseButtons[4];
            break;

        case SDL_BUTTON_X2:
            coords = &mouseButtons[5];
            break;
#endif

        case SDL_BUTTON_LEFT:
            coords = &mouseButtons[1];
            break;

        case SDL_BUTTON_RIGHT:
            coords = &mouseButtons[2];
            break;

        case SDL_BUTTON_MIDDLE:
            coords = &mouseButtons[3];
            break;

        default:
            break;
    }

    if(coords)
    {
        Point real(ev.x, ev.y);

        if(scaleUsed())
            real = scaleValue(real);

        switch(ev.type)
        {
            case SDL_MOUSEBUTTONDOWN:
                coords->setPress(real);
                // generate press
                DisplayScene::mouseButtonHandle(coords->press(), true);
                break;

            case SDL_MOUSEBUTTONUP:
            {
                coords->setRelease(real);
                // generate release
                DisplayScene::mouseButtonHandle(coords->release(), false);
                // generate click
                Point delta = coords->press().position() - coords->release().position();
                const int val = 15;

                if(val > std::abs(delta.x) && val > std::abs(delta.y))
                    DisplayScene::mouseClickHandle(*coords);
                else if(fingerEventEmulation)
                {
                    if(std::abs(delta.x) > std::abs(delta.y))
                        DisplayScene::pushEvent(nullptr, 0 > delta.x ? Signal::GestureFingerRight : Signal::GestureFingerLeft, nullptr);
                    else if(std::abs(delta.x) < std::abs(delta.y))
                        DisplayScene::pushEvent(nullptr, 0 > delta.y ? Signal::GestureFingerDown : Signal::GestureFingerUp, nullptr);
                }

                break;
            }

            default:
                break;
        }
    }
}

void SWE::Display::handleMouseMotion(const SDL_MouseMotionEvent & ev)
{
    Point real(ev.x, ev.y);

    if(scaleUsed())
        real = scaleValue(real);

    DisplayScene::mouseMotionHandle(real, ev.state);

    if(fingerEventEmulation)
    {
        const s32 & dx = ev.xrel;
        const s32 & dy = ev.yrel;

        if(fingerMoveDelta <= std::abs(dx) || fingerMoveDelta <= std::abs(dy))
        {
            if(std::abs(dx) > std::abs(dy))
                DisplayScene::pushEvent(nullptr, 0 < dx ? Signal::FingerMoveRight : Signal::FingerMoveLeft, nullptr);
            else
            if(std::abs(dy) > std::abs(dx))
                DisplayScene::pushEvent(nullptr, 0 < dy ? Signal::FingerMoveDown : Signal::FingerMoveUp, nullptr);
        }
    }
}

#ifdef SWE_SDL12
void SWE::Display::handleMouseWheel(int button, int type)
{
    if(SDL_MOUSEBUTTONUP == type)
    {
        if(SDL_BUTTON_WHEELUP == button)
            DisplayScene::scrollHandle(true);
        else if(SDL_BUTTON_WHEELDOWN == button)
            DisplayScene::scrollHandle(false);
    }
}
#else
void SWE::Display::handleMouseWheel(const SDL_MouseWheelEvent & ev)
{
    if(0 < ev.y)
        DisplayScene::scrollHandle(true);
    else if(0 > ev.y)
        DisplayScene::scrollHandle(false);
}

void SWE::Display::handleTextInput(const SDL_TextInputEvent & ev)
{
    DisplayScene::textInputHandle(ev.text);
}

void SWE::Display::handleFingerTap(const SDL_TouchFingerEvent & ev)
{
    ButtonsEvent & coords = mouseButtons[0];
    const Size & dsz = device();
    Point real(dsz.w * ev.x, dsz.h * ev.y);

    if(scaleUsed())
        real = scaleValue(real);

    switch(ev.type)
    {
        case SDL_FINGERDOWN:
            coords.setPress(real);
            // generate press
            DisplayScene::mouseButtonHandle(coords.press(), true);
            break;

        case SDL_FINGERUP:
        {
            coords.setRelease(real);
            // generate release
            DisplayScene::mouseButtonHandle(coords.release(), true);
#ifdef ANDROID
            // gesture finger event
            Point delta = coords.press().position() - coords.release().position();

            if(fingerGestureDelta > std::abs(delta.x) && fingerGestureDelta > std::abs(delta.y))
                DisplayScene::mouseClickHandle(coords);
            else
            if(std::abs(delta.x) > std::abs(delta.y))
                DisplayScene::pushEvent(nullptr, 0 > delta.x ? Signal::GestureFingerRight : Signal::GestureFingerLeft, nullptr);
            else
            if(std::abs(delta.x) < std::abs(delta.y))
                DisplayScene::pushEvent(nullptr, 0 > delta.y ? Signal::GestureFingerDown : Signal::GestureFingerUp, nullptr);

#else
            // generate click
            DisplayScene::mouseClickHandle(coords);
#endif
        }
        break;

        default:
            break;
    }
}

void SWE::Display::handleFingerMotion(const SDL_TouchFingerEvent & ev)
{
    const Size & dsz = device();
    Point real(dsz.w * ev.x, dsz.h * ev.y);

    if(scaleUsed())
        real = scaleValue(real);

    DisplayScene::mouseMotionHandle(real, FingerTap);
    float dx = dsz.w * ev.dx;
    float dy = dsz.h * ev.dy;

    if(std::fabs(dx) > std::fabs(dy))
        DisplayScene::pushEvent(nullptr, 0 < ev.dx ? Signal::FingerMoveRight : Signal::FingerMoveLeft, nullptr);
    else if(std::fabs(dy) > std::fabs(dx))
        DisplayScene::pushEvent(nullptr, 0 < ev.dy ? Signal::FingerMoveDown : Signal::FingerMoveUp, nullptr);
}
#endif

void SWE::Display::handleUserEvent(const SDL_UserEvent & ev)
{
    DisplayScene::userHandle(ev);
}

void SWE::Display::hardwareCursorHide(void)
{
    SDL_ShowCursor(SDL_DISABLE);
}

void SWE::Display::hardwareCursorShow(void)
{
    SDL_ShowCursor(SDL_ENABLE);
}

SWE::Point SWE::Display::mouseCursorPosition(void)
{
    Point res;
    SDL_GetMouseState(& res.x, & res.y);
    return scaleUsed() ? scaleValue(res) : res;
}

u32 SWE::Display::mouseButtonState(void)
{
    return SDL_GetMouseState(nullptr, nullptr);
}

SWE::Rect SWE::Display::renderTextHorizontal(const FontRender & frs, const UnicodeString & ustr, const Color & col, Texture & dtx, const Point & dpt,
				    const AlignType & hAlign, const AlignType & vAlign,
				    const CharRender & render, int style, const CharHinting & hinting)
{
    Rect fixed = Rect(Point(0, 0), Display::size());
    return renderTextFixedHorizontal(frs, ustr, col, dtx, dpt, fixed, hAlign, vAlign, render, style, hinting);
}

SWE::Rect SWE::Display::renderTextVertical(const FontRender & frs, const UnicodeString & ustr, const Color & col, Texture & dtx, const Point & dpt,
				    const AlignType & hAlign, const AlignType & vAlign,
				    const CharRender & render, int style, const CharHinting & hinting)
{
    Rect fixed = Rect(Point(0, 0), Display::size());
    return renderTextFixedVertical(frs, ustr, col, dtx, dpt, fixed, hAlign, vAlign, render, style, hinting);
}

SWE::Rect SWE::Display::renderTextFixedHorizontal(const FontRender & frs, const UnicodeString & ustr, const Color & col, Texture & dtx, const Point & dpt, const Rect & fixed,
				    const AlignType & hAlign, const AlignType & vAlign,
				    const CharRender & render, int style, const CharHinting & hinting)
{
    Rect res;

    if(frs.isValid() && ustr.size())
    {
        Point dst = dpt;
        const Size usz = frs.unicodeSize(ustr, true);

        if(hAlign == AlignCenter)
            dst.x -= usz.w / 2;
        else if(hAlign == AlignRight)
            dst.x -= usz.w;

        if(vAlign == AlignCenter)
	    // usz.h is lineSkipHeight(), and it is large that char height, little down
            dst.y = dst.y - usz.h / 2 + 2;
        else if(vAlign == AlignBottom)
            dst.y -= usz.h;

        res.x = dst.x;
        res.y = dst.y;
        Rect rtFixed;

        for(auto it = ustr.begin(); it != ustr.end(); ++it) if(*it)
        {
            Texture tx = FontsCache(&frs).renderCharset(*it, col, render, style, hinting);
            if(! tx.isValid()) continue;

            if(Rect::intersection(Rect(fixed.toPoint() + dst, tx.size()), fixed, & rtFixed))
            {
                renderTexture(tx, Rect(Point(0, 0), rtFixed.toSize()), dtx, rtFixed);
                if(res.h < tx.height()) res.h = tx.height();
            }

            dst.x += frs.symbolAdvance(*it);
        }

        res.w = dst.x - res.x;
    }

    return res;
}

SWE::Rect SWE::Display::renderTextFixedVertical(const FontRender & frs, const UnicodeString & ustr, const Color & col, Texture & dtx, const Point & dpt, const Rect & fixed,
				    const AlignType & hAlign, const AlignType & vAlign,
				    const CharRender & render, int style, const CharHinting & hinting)
{
    Rect res;

    if(frs.isValid() && ustr.size())
    {
        Point dst = dpt;
        const Size usz = frs.unicodeSize(ustr, false);

        if(vAlign == AlignCenter)
            dst.y -= usz.h / 2;
        else if(vAlign == AlignBottom)
            dst.y -= usz.h;

        res.x = dst.x;
        res.y = dst.y;

        if(hAlign == AlignCenter)
            res.x -= usz.w / 2;
        else if(hAlign == AlignRight)
            res.x -= usz.w;

        Rect rtFixed;

        for(auto it = ustr.begin(); it != ustr.end(); ++it) if(*it)
        {
            Texture tx = FontsCache(&frs).renderCharset(*it, col, render, style, hinting);
            if(! tx.isValid()) continue;

	    // halign one char
            if(hAlign == AlignCenter)
                dst.x = dpt.x - tx.width() / 2;
            else if(hAlign == AlignRight)
                dst.x = dpt.x - tx.width();

            if(Rect::intersection(Rect(fixed.toPoint() + dst, tx.size()), fixed, & rtFixed))
            {
                renderTexture(tx, Rect(Point(0, 0), rtFixed.toSize()), dtx, rtFixed);
                if(res.w < tx.width()) res.w = tx.width();
            }

	    dst.y += frs.lineSkipHeight();
        }

        res.h = dst.y - res.y;
    }

    return res;
}

SWE::Rect SWE::Display::renderText(const FontRender & frs, const UnicodeString & ustr, const Color & col, Texture & dst, const Point & dpt,
				    const AlignType & hAlign, const AlignType & vAlign, bool isHorizontal,
				    const CharRender & render, int style, const CharHinting & hinting)
{
    return isHorizontal ? renderTextHorizontal(frs, ustr, col, dst, dpt, hAlign, vAlign, render, style, hinting) :
           renderTextVertical(frs, ustr, col, dst, dpt, hAlign, vAlign, render, style, hinting);
}

SWE::Rect SWE::Display::renderTextFixed(const FontRender & frs, const UnicodeString & ustr, const Color & col, Texture & dst, const Point & dpt, const Rect & fixed,
				    const AlignType & hAlign, const AlignType & vAlign, bool isHorizontal,
				    const CharRender & render, int style, const CharHinting & hinting)
{
    return isHorizontal ? renderTextFixedHorizontal(frs, ustr, col, dst, dpt, fixed, hAlign, vAlign, render, style, hinting) :
           renderTextFixedVertical(frs, ustr, col, dst, dpt, fixed, hAlign, vAlign, render, style, hinting);
}

SWE::Texture SWE::Display::renderText(const FontRender & frs, const UCString & ustr)
{
    Texture res;

    if(frs.isValid() && ustr.length())
    {
        Size sz = frs.unicodeSize(ustr.toUnicodeString());
#ifdef SWE_SDL12
        res = createTexture(sz, true);
#else
        res = createTexture(sz);
#endif
        Point dst(0, 0);

        for(size_t pos = 0; pos < ustr.length(); ++pos)
        {
            UnicodeColor us = ustr.at(pos);
            Texture tx = FontsCache(& frs).renderCharset(us.unicode(), us.fgcolor());

            if(tx.isValid())
            {
                const Color bgcolor = us.bgcolor();

                if(! bgcolor.isTransparent())
                    renderColor(bgcolor, res, Rect(dst, tx.size()));

#ifdef SWE_SDL12
		tx.setAlphaMod(0);
#endif

                renderTexture(tx, tx.rect(), res, Rect(dst, tx.size()));
                dst.x += frs.symbolAdvance(us.unicode());

#ifdef SWE_SDL12
		tx.setAlphaMod(SDL_ALPHA_OPAQUE);
#endif
            }
        }
    }

    return res;
}

SWE::Texture SWE::Display::renderText(const FontRender & frs, const UnicodeString & ustr, const Color & fg, const Color & bg)
{
    return renderText(frs, UCString(ustr, FBColors(fg.toColorIndex(), bg.toColorIndex())));
}

bool SWE::Display::resize(const Size & winsz)
{
    return resizeWindow(winsz);
}

std::list<SWE::Size> SWE::Display::hardwareVideoModes(bool landscape)
{
    std::list<Size> result;
#ifdef SWE_SDL12
    SDL_Rect** modes = SDL_ListModes(nullptr, SDL_FULLSCREEN | SDL_HWSURFACE);

    if(modes == (SDL_Rect**) 0)
    {
        ERROR("no modes available");
        return result;
    }
    else if(modes == (SDL_Rect**) - 1)
    {
        ERROR("all resolutions available");
        return result;
    }
    else
    {
        for(int ii = 0; modes[ii]; ++ii)
        {
            Size mode(modes[ii]->w, modes[ii]->h);

            if(landscape)
            {
                if(mode.h > mode.w) std::swap(mode.w, mode.h);
            }
            else
            {
                if(mode.h < mode.w) std::swap(mode.w, mode.h);
            }

            result.push_back(mode);
        }
    }

#else
    SDL_DisplayMode mode;
    int displayIndex = 0;
    int modeIndex = 0;

    while(0 == SDL_GetDisplayMode(displayIndex, modeIndex++, & mode))
    {
        if(landscape)
        {
            if(mode.h > mode.w) std::swap(mode.w, mode.h);
        }
        else
        {
            if(mode.h < mode.w) std::swap(mode.w, mode.h);
        }

        result.push_back(Size(mode.w, mode.h));
    }

#endif
    result.sort();
    result.unique();
    return result;
}

void SWE::Display::setWindowIcon(const Surface & sf)
{
#if SWE_SDL12
    SDL_WM_SetIcon(sf.toSDLSurface(), nullptr);
#else
    SDL_SetWindowIcon(_window, sf.toSDLSurface());
#endif
}

bool SWE::Display::setFullscreenMode(bool f)
{
#if SWE_SDL12
    if(1 == SDL_WM_ToggleFullScreen(SDL_GetVideoSurface()))
        return true;
#else
    if(0 == SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN))
        return true;
#endif
    ERROR(SDL_GetError());
    return false;
}

bool SWE::Display::isFullscreenWindow(void)
{
#ifdef SWE_SDL12
    SDL_Surface* sf = SDL_GetVideoSurface();
    return sf ? sf->flags & SDL_FULLSCREEN : false;
#else

    if(_window)
        return SDL_WINDOW_FULLSCREEN & SDL_GetWindowFlags(_window);

    return false;
#endif
}

#if SWE_SDL12
#if defined(__LINUX__)
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include "SDL_syswm.h"
#endif
#endif

bool SWE::Display::isMaximizedWindow(void)
{
    if(isFullscreenWindow())
        return true;

#if SWE_SDL12
#if defined(__LINUX__)
    SDL_SysWMinfo wm;
    SDL_VERSION(& wm.version);
    SDL_GetWMInfo(& wm);

    Atom wmState = XInternAtom(wm.info.x11.display, "_NET_WM_STATE", False);
    Atom retType;
    int retFormat;
    unsigned long nItems = 0;
    unsigned long bytesAfter = 0;
    unsigned char* properties = nullptr;
    bool res = false;

    auto status = XGetWindowProperty(wm.info.x11.display, wm.info.x11.wmwindow, wmState, 0, ~0L, False, XA_ATOM,
                &retType, &retFormat, &nItems, &bytesAfter, &properties);

    if(status == Success && properties)
    {
	Atom maxVert = XInternAtom(wm.info.x11.display, "_NET_WM_STATE_MAXIMIZED_VERT", False);
	Atom maxHorz = XInternAtom(wm.info.x11.display, "_NET_WM_STATE_MAXIMIZED_HORZ", False);

        auto atoms = reinterpret_cast<Atom*>(properties);
        int maximized = 0;

	for(unsigned int it = 0; it < nItems; ++it)
	{
    	    if(atoms[it] == maxVert)
                maximized |= 1;
	    else
    	    if(atoms[it] == maxHorz)
                maximized |= 2;
	}

        res = maximized == 3;
    }

    if(properties)
	XFree(properties);

    return res;
#else
    FIXME("SDL1.2 not supported maximized flag");
#endif

#else
    if(_window)
    {
        if(SDL_WINDOW_MAXIMIZED & SDL_GetWindowFlags(_window))
            return true;

        return usableBounds().toSize() == device();
    }
#endif

    return false;
}

SWE::Rect SWE::Display::usableBounds(void)
{
#if SWE_SDL12
#if defined(__LINUX__)
    SDL_SysWMinfo wm;
    SDL_VERSION(& wm.version);
    SDL_GetWMInfo(& wm);

    Atom workArea = XInternAtom(wm.info.x11.display, "_NET_WORKAREA",  False);
    Atom retType;
    int retFormat;
    unsigned long nItems = 0;
    unsigned long bytesAfter = 0;
    unsigned char* properties = nullptr;

    auto rootWindow = XRootWindow(wm.info.x11.display, 0);
    Rect res = { 0, 0, 65535, 65535 };

    auto status = XGetWindowProperty(wm.info.x11.display, rootWindow, workArea, 0, ~0L, False, XA_CARDINAL,
                        &retType, &retFormat, &nItems, &bytesAfter, &properties);

    if(status == Success && properties)
    {
        // _NET_WORKAREA, x, y, width, height CARDINAL[][4]/32
        if(3 < nItems)
        {
            auto vals = reinterpret_cast<long*>(properties);
            res.x = vals[0];
            res.y = vals[1];
            res.w = vals[2];
            res.h = vals[3];
        }

    }

    if(properties)
	XFree(properties);

    return res;
#else
    FIXME("SDL1.2 not supported max window size");
#endif

#else
    if(_window)
    {
        SDL_Rect res;
        if(0 == SDL_GetDisplayUsableBounds(0, & res))
        {
            return Rect(res);
        }

        ERROR("get info: " << SDL_GetError());
    }
#endif
    return Rect(0, 0, 65535, 65535);
}

void SWE::Display::setForceWindowed(bool f)
{
    forceWindowed = f;
}

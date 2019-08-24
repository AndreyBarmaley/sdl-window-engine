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
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <list>

#define SWE_VERSION 20190804

#ifdef OLDENGINE
#include "SDL_rotozoom.h"
#endif

#include "display_scene.h"
#include "engine.h"

namespace Engine
{
    void		quit(void);
    void		clearAllSignals(void);
}

namespace Display
{
    bool		debug = false;

#ifdef OLDENGINE
    SDL_Surface*	_window = NULL;
#else
    SDL_Renderer*       _renderer = NULL;
    SDL_Window*         _window = NULL;
    SDL_SpinLock	renderLock = 0;
#endif
    bool		accelval = false;

    Texture		displayTexture;

    ButtonsEvent	mouseButtons[6]; /* FingerTap, ButtonLeft, ButtonRight, ButtonMiddle, ButtonX1, ButtonX2 */
    int			fingerIndex = 0;

    u32			tickStart = 0;

    Size                winsz;
    Size		rendersz;
    Rect		scale;

    SDL_Event		current;

    bool		createWindow(const std::string &, const Size &, int);
    bool		resizeWindow(const Size &, bool);

    void                closeWindow(void);

    void                redraw(void);

    bool		renderInit(const Size &, bool);
    bool		renderReset(SDL_Texture*);
    void                renderPresent(void);
    void		renderCopyEx(const Texture &, const Rect &, Texture &, const Rect &, int);

    void                handleMouseButton(const SDL_MouseButtonEvent &);
    void                handleMouseMotion(const SDL_MouseMotionEvent &);
    void                handleKeyboard(const SDL_KeyboardEvent &);
    void                handleUserEvent(const SDL_UserEvent &);
    void                handleScrollEvent(int, const Point &);
    void                handleFocusEvent(bool);
#ifdef OLDENGINE
    void                handleMouseWheel(int, int);
#else
    void                handleMouseWheel(const SDL_MouseWheelEvent &);
    void                handleTextInput(const SDL_TextInputEvent &);
    void		handleFingerTap(const SDL_TouchFingerEvent &);
    void		handleFingerMotion(const SDL_TouchFingerEvent &);
#endif
    void                addWindow(Window &);
    void                delWindow(Window &);

    Rect		renderTextHorizontal(const FontRender &, const UnicodeString &, const Color &, Texture &, const Point &, int, int);
    Rect		renderTextVertical(const FontRender &, const UnicodeString &, const Color &, Texture &, const Point &, int, int);
}

void Engine::except(const char* func, const char* message)
{
    if(func && message) COUT(String::time() << ": [EXCEPTION]\t" << func << ":  " << message);
#ifndef ANDROID
    throw exception();
#endif
}

bool Engine::init(bool debug)
{
    Display::debug = debug;
    int init_flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER;
    std::srand((u32) std::time(0));

    DEBUG("SDL Window Engine: " << "version: " << version());

    if(SDL_Init(init_flags) != 0)
    {
        ERROR("SDL_Init" << ": " << SDL_GetError());
        return false;
    }
    else
    {
#ifdef OLDENGINE
	const SDL_version* sdlver = SDL_Linked_Version();
	DEBUG("usage " << "SDL" << ", " << "version: " << static_cast<int>(sdlver->major) << "." << static_cast<int>(sdlver->minor) << "." << static_cast<int>(sdlver->patch));
#else
	SDL_version sdlver;
	SDL_GetVersion(&sdlver);
	DEBUG("usage " << "SDL" << ", " << "version: " << static_cast<int>(sdlver.major) << "." << static_cast<int>(sdlver.minor) << "." << static_cast<int>(sdlver.patch) <<
		", revision: " << SDL_GetRevision());
#endif
    }

#ifndef DISABLE_NETWORK
    if(SDLNet_Init() < 0)
    {
        ERROR("SDLNet_Init" << ": " << SDL_GetError());
        return false;
    }
    else
    {
	const SDL_version* sdlver = SDLNet_Linked_Version();
	if(sdlver)
	    DEBUG("usage " << "SDL_net" << ", " << "version: " << static_cast<int>(sdlver->major) << "." << static_cast<int>(sdlver->minor) << "." << static_cast<int>(sdlver->patch));
    }
#endif

    if(IMG_Init(IMG_INIT_PNG) == 0)
    {
        ERROR("IMG_Init" << ": " << SDL_GetError());
        return false;
    }
    else
    {
	const SDL_version* sdlver = IMG_Linked_Version();
	if(sdlver)
	    DEBUG("usage " << "SDL_image" << ", " << "version: " << static_cast<int>(sdlver->major) << "." << static_cast<int>(sdlver->minor) << "." << static_cast<int>(sdlver->patch));
    }

    if(TTF_Init() != 0)
    {
        ERROR("TTF_Init" << ": " << SDL_GetError());
        return false;
    }
    else
    {
	const SDL_version* sdlver = TTF_Linked_Version();
	if(sdlver)
	    DEBUG("usage " << "SDL_ttf" << ", " << "version: " << static_cast<int>(sdlver->major) << "." << static_cast<int>(sdlver->minor) << "." << static_cast<int>(sdlver->patch));
    }

#ifndef DISABLE_AUDIO
#if (SDL_VERSIONNUM(SDL_MIXER_MAJOR_VERSION, SDL_MIXER_MINOR_VERSION, SDL_MIXER_PATCHLEVEL) > SDL_VERSIONNUM(1,2,8))
    if((Mix_Init(MIX_INIT_OGG) & MIX_INIT_OGG) != MIX_INIT_OGG)
    {
        ERROR("MIX_Init" << ": " << Mix_GetError());
        return false;
    }
    else
#endif
    {
	if(0 > Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024))
    	    ERROR("MIX_OpenAudio" << ": " << Mix_GetError());

	const SDL_version* sdlver = Mix_Linked_Version();
	if(sdlver)
	    DEBUG("usage " << "SDL_mixer" << ", " << "version: " << static_cast<int>(sdlver->major) << "." << static_cast<int>(sdlver->minor) << "." << static_cast<int>(sdlver->patch));
    }
#endif

#ifndef ANDROID
#ifndef OLDENGINE
    SDL_StartTextInput();
#endif
#endif
    return true;
}

int Engine::version(void)
{
    return SWE_VERSION;
}

void Engine::setDebugMode(bool f)
{
    Display::debug = f;
}

bool Engine::debugMode(void)
{
    return Display::debug;
}

void Engine::quit(void)
{
    clearAllSignals();
#ifndef ANDROID
#ifndef OLDENGINE
    SDL_StopTextInput();
#endif
#endif
    Display::closeWindow();
#ifndef DISABLE_AUDIO
    Music::reset();
    Mix_CloseAudio();
#if (SDL_VERSIONNUM(SDL_MIXER_MAJOR_VERSION, SDL_MIXER_MINOR_VERSION, SDL_MIXER_PATCHLEVEL) > SDL_VERSIONNUM(1,2,8))
    Mix_Quit();
#endif
#endif
    IMG_Quit();
#ifndef DISABLE_NETWORK
    SDLNet_Quit();
#endif
    TTF_Quit();
    SDL_Quit();
}

bool Display::init(const std::string & title, const Size & win, bool fullscreen, bool accel)
{
    return init(title, win, win, fullscreen, accel, false);
}

bool Display::init(const std::string & title, const Size & win, const Size & render, bool fullscreen, bool accel, bool resized)
{
    closeWindow();

#ifdef OLDENGINE
    int flags = accel ? SDL_SWSURFACE | SDL_HWSURFACE | SDL_DOUBLEBUF : SDL_SWSURFACE;
#else
    int flags = 0;
#endif

    if(Systems::isEmbeded())
	fullscreen = true;

    Size winsz = win;

    if(fullscreen)
    {
	if(win.h > win.w) std::swap(winsz.w, winsz.h);

	auto modes = hardwareVideoModes();
	if(modes.end() == std::find_if(modes.begin(), modes.end(),
            std::bind2nd(std::greater_equal<Size>(), winsz)))
	{
    	    // set max resolution and fullscreen
    	    winsz = modes.back();
	    if(win.h > win.w) std::swap(winsz.w, winsz.h);
	    DEBUG("set win size: " << winsz.w << "x" << winsz.h);
	}
    }

#ifdef OLDENGINE
    if(fullscreen)
        flags |= SDL_FULLSCREEN;
    if(resized)
        flags |= SDL_RESIZABLE;
#else
    if(fullscreen)
	flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    if(resized)
	flags |= SDL_WINDOW_RESIZABLE;
#endif

    return createWindow(title, winsz, flags) ? Display::renderInit(render, accel) : false;
}

bool Display::createWindow(const std::string & title, const Size & newsz, int flags)
{
    mouseButtons[0] = ButtonsEvent(FingerTap);
    mouseButtons[1] = ButtonsEvent(ButtonLeft);
    mouseButtons[2] = ButtonsEvent(ButtonRight);
    mouseButtons[3] = ButtonsEvent(ButtonMiddle);
    mouseButtons[4] = ButtonsEvent(ButtonX1);
    mouseButtons[5] = ButtonsEvent(ButtonX2);

    winsz = newsz;
#ifdef OLDENGINE
    SDL_WM_SetCaption(title.c_str(), NULL);
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

    if(Systems::isEmbeded())
        Display::hardwareCursorHide();

    DEBUG(winsz.w << "x" << winsz.h);
    tickStart = Tools::ticks();

    return true;
}

bool Display::resizeWindow(const Size & newsz, bool sdl)
{
    if(_window && winsz != newsz)
    {
	FontRender::clearCache();

#ifdef OLDENGINE
	_window = SDL_SetVideoMode(newsz.w, newsz.h, _window->format->BitsPerPixel, _window->flags);
#else
	SDL_SetWindowSize(_window, newsz.w, newsz.h);
#endif

	if(renderInit(newsz, accelval))
	{
	    DisplayScene::displayResizeHandle(newsz, sdl);
	    return true;
	}
    }

    return false;
}

bool Display::renderInit(const Size & win, bool accel)
{
    accelval = accel;
    rendersz = win;

    if(displayTexture.isValid())
	displayTexture.reset();

#ifdef OLDENGINE
    _window = SDL_GetVideoSurface();
    winsz.w = _window->w;
    winsz.h = _window->h;

    displayTexture = Display::createTexture(rendersz, false);
    displayTexture.convertToDisplayFormat();
#else
    SDL_GetWindowSize(_window, &winsz.w, &winsz.h);

    if(_renderer)
    {
        SDL_DestroyRenderer(_renderer);
	_renderer = NULL;
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

    DEBUG("render: " << (accel ? "hardware" : "software"));

    if(! renderReset(NULL))
        return false;

    // default: alpha blending enabled
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

    displayTexture = Texture(SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, rendersz.w, rendersz.h));
#endif

    if(! displayTexture.isValid())
    {
        ERROR(SDL_GetError());
        return false;
    }

    renderClear(Color::Black, displayTexture);
    DEBUG(rendersz.w << "x" << rendersz.h);

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

    DisplayScene::setDirty(false);

    return true;
}

void Display::closeWindow(void)
{
    FontRender::clearCache();
    DisplayScene::sceneDestroy();

    if(displayTexture.isValid())
	displayTexture.reset();

#ifdef OLDENGINE
    if(_window)
        _window = NULL;
#else
    if(_renderer)
    {
        SDL_DestroyRenderer(_renderer);
        _renderer = NULL;
    }

    if(_window)
    {
        SDL_DestroyWindow(_window);
        _window = NULL;
    }
#endif
}

bool Display::renderReset(SDL_Texture* target)
{
#ifndef OLDENGINE
    if(! _renderer)
	return false;

    SDL_RendererInfo info;
    if(0 != SDL_GetRendererInfo(_renderer, & info))
    {
        ERROR("get info: " << SDL_GetError());
        return false;
    }

    if(! (info.flags & SDL_RENDERER_TARGETTEXTURE))
    {
        ERROR( "not target texture: " << String::hex64(reinterpret_cast<std::uintptr_t>(target)));
    	return false;
    }

    if(0 != SDL_SetRenderTarget(_renderer, target))
    {
        ERROR("set target: " << SDL_GetError());
        return false;
    }
#endif
    return true;
}

void Display::renderClear(const Color & cl, Texture & dtx)
{
#ifdef OLDENGINE
    renderColor(cl, dtx, dtx.rect());
#else
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
#endif
}

void Display::renderColor(const Color & cl, Texture & dtx, const Rect & drt)
{
#ifdef OLDENGINE
    Display::createSurface(dtx).fill(drt, cl);
#else
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
#endif
}

Texture Display::renderRect(const Color & col, const Color & fill, const Size & sz, int border)
{
    Texture res = createTexture(sz);
    renderClear(fill, res);
    if(res.isValid() && 0 < border && border < sz.w / 2 && border < sz.h / 2)
    {
	for(int ii = 0; ii < border; ++ii)
	    renderRect(col, res, Rect(Point(ii, ii), sz - Size(ii * 2, ii * 2)));
    }
    return res;
}

void Display::renderRect(const Color & cl, Texture & dtx, const Rect & drt)
{
#ifdef OLDENGINE
    Surface dsf = Display::createSurface(dtx);
    int pixel = dsf.mapRGB(cl);

    for(int ox = 0; ox < drt.w; ++ox)
    {
        dsf.drawPixel(drt.toPoint() + Point(ox, 0), pixel);
        dsf.drawPixel(drt.toPoint() + Point(ox, drt.h - 1), pixel);
    }

    for(int oy = 1; oy < drt.h - 1; ++oy)
    {
        dsf.drawPixel(drt.toPoint() + Point(0, oy), pixel);
        dsf.drawPixel(drt.toPoint() + Point(drt.w - 1, oy), pixel);
    }
#else
    if(renderReset(dtx.toSDLTexture()))
    {
	if(0 == SDL_SetRenderDrawColor(_renderer, cl.r(), cl.g(), cl.b(), cl.a()))
	{
    	    const SDL_Rect dstrt = drt.toSDLRect();
    	    if(0 != SDL_RenderDrawRect(_renderer, &dstrt))
        	ERROR(SDL_GetError());
	}
	else
    	    ERROR(SDL_GetError());
    }
#endif
}

void Display::renderPolygon(const Color & cl, Texture & dtx, const Points & v, bool closure)
{
    const Point* pt1 = NULL;
    const Point* pt2 = NULL;

    for(size_t it = 0; it < v.size(); ++it)
    {
        pt1 = & v[it];
        pt2 = it + 1 < v.size() ? & v[it + 1] :
		(closure ? & v[0] : NULL);

	if(pt2) renderLine(cl, dtx, *pt1, *pt2);
    }
}

void Display::renderLine(const Color & cl, Texture & dtx, const Point & pt1, const Point & pt2)
{
#ifdef OLDENGINE
    Points points = Tools::renderLine(pt1, pt2);
    Surface dsf = Display::createSurface(dtx);
    int pixel = dsf.mapRGB(cl);

    for(auto it = points.begin(); it != points.end(); ++it)
        dsf.drawPixel(*it, pixel);
#else
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
#endif
}

void Display::renderPoint(const Color & cl, Texture & dtx, const Point & pt)
{
#ifdef OLDENGINE
    Display::createSurface(dtx).drawPoint(pt, cl);
#else
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
#endif
}

void Display::renderTexture(const Texture & stx, const Point & dpt)
{
    if(stx.isValid() && stx != displayTexture)
	renderCopyEx(stx, stx.rect(), displayTexture, Rect(dpt, stx.size()), FlipNone);
}

void Display::renderTexture(const Texture & stx, const Rect & srt, Texture & dtx, const Rect & drt, int flip)
{
    if(stx.isValid() && stx != dtx)
    {
#ifndef OLDENGINE
	dtx.setBlendMode(BlendMode::None);
#endif
	renderCopyEx(stx, srt, dtx, drt, flip);
#ifndef OLDENGINE
	dtx.setBlendMode(BlendMode::Blend);
#endif
    }
}

void Display::renderCursor(const Texture & tx)
{
    DisplayScene::setCursor(tx, Point(0, 0));
}

#ifdef OLDENGINE
int compat_blit(SDL_Surface* ss, SDL_Rect* srt, SDL_Surface* ds, SDL_Rect* drt)
{
    if(ds != SDL_GetVideoSurface() &&
        // only equal bpp
        ss->format->BitsPerPixel == ds->format->BitsPerPixel &&
        // RGBA -> RGBA
        0 != ss->format->Amask && 0 != ds->format->Amask &&
        (ss->flags & SDL_SRCALPHA))
    {
        int alpha = ss->format->alpha;
        SDL_SetAlpha(ss, 0, 0);
        int res = SDL_BlitSurface(ss, srt, ds, drt);
        SDL_SetAlpha(ss, SDL_SRCALPHA, alpha);
        return res;
    }

    return SDL_BlitSurface(ss, srt, ds, drt);
}
#endif

void Display::renderCopyEx(const Texture & stx, const Rect & srt, Texture & dtx, const Rect & drt, int flip)
{
#ifdef OLDENGINE
    SDL_Rect srcrt = srt.toSDLRect();
    SDL_Rect dstrt = drt.toSDLRect();

    SDL_Surface* src = stx.toSDLTexture();
    SDL_Surface* dst = dtx.toSDLTexture();

    if(0 > SDL_BlitSurface(src, & srcrt, dst, & dstrt))
        ERROR(SDL_GetError() << ", " << stx.toStringID());

    if(dtx == displayTexture)
        DisplayScene::setDirty(true);
#else
    const SDL_Rect srcrt = srt.toSDLRect();
    const SDL_Rect dstrt = drt.toSDLRect();

    SDL_AtomicLock(& renderLock);
    if(renderReset(dtx.toSDLTexture()))
    {
    	int res = flip == FlipNone ? SDL_RenderCopy(_renderer, stx.toSDLTexture(), &srcrt, &dstrt) :
    	    SDL_RenderCopyEx(_renderer, stx.toSDLTexture(), &srcrt, &dstrt, 0, NULL, (SDL_RendererFlip) flip);

	if(res)
	    ERROR(SDL_GetError() << ", " << stx.toStringID());

	if(dtx == displayTexture)
	    DisplayScene::setDirty(true);
    }
    SDL_AtomicUnlock(& renderLock);
#endif
}

void Display::renderSurface(const Surface & sf, const Rect & srt, Texture & dtx, const Rect & drt, int flip)
{
#ifdef OLDENGINE
    if(sf.isValid())
	renderTexture(Texture(sf), srt, dtx, drt, flip);
#else
    if(sf.isValid())
	renderTexture(Texture(SDL_CreateTextureFromSurface(_renderer, sf.toSDLSurface())), srt, dtx, drt, flip);
#endif
}

void Display::renderPresent(void)
{
#ifdef OLDENGINE
    if(winsz != rendersz)
    {
	SDL_Rect dstrt = scale.toSDLRect();
	double zoomx = dstrt.w / static_cast<double>(rendersz.w);
	double zoomy = dstrt.h / static_cast<double>(rendersz.h);

	SDL_Surface* zoomsf = zoomSurface(displayTexture.toSDLTexture(), zoomx, zoomy, 1);

	if(zoomsf)
	{
	    if(0 > SDL_BlitSurface(zoomsf, NULL, _window, & dstrt))
    		ERROR(SDL_GetError() << ", " << displayTexture.toStringID());

	    SDL_FreeSurface(zoomsf);
	}
	else
	{
    	    ERROR("zoomSurface: " << SDL_GetError());
	}
    }
    else
    if(0 > SDL_BlitSurface(displayTexture.toSDLTexture(), NULL, _window, NULL))
        ERROR(SDL_GetError() << ", " << displayTexture.toStringID());

    if(0 > SDL_Flip(_window))
        ERROR(SDL_GetError());
#else
    SDL_AtomicLock(& renderLock);
    if(renderReset(NULL))
    {
	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

	if(0 != SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0))
    	    ERROR("set color: " << SDL_GetError());

	if(0 != SDL_RenderClear(_renderer))
    	    ERROR("clear: " << SDL_GetError());

	if(winsz != rendersz)
	{
	    SDL_Rect dstrt = scale.toSDLRect();
	    if(0 != SDL_RenderCopy(_renderer, displayTexture.toSDLTexture(), NULL, & dstrt))
    		ERROR("copy: " << SDL_GetError());
	}
	else
	if(0 != SDL_RenderCopy(_renderer, displayTexture.toSDLTexture(), NULL, NULL))
    	    ERROR("copy: " << SDL_GetError());

	SDL_RenderPresent(_renderer);
    }
    SDL_AtomicUnlock(& renderLock);
#endif
    DisplayScene::setDirty(false);
}

bool Display::renderScreenshot(const std::string & file)
{
    return createTexture(displayTexture, FlipVertical).save(file);
}

bool Display::scaleUsed(void)
{
    return winsz != rendersz;
}

Point Display::scaleValue(const Point & pt)
{
    return Point((pt.x - scale.x) * rendersz.w / scale.w, (pt.y - scale.y) * rendersz.h / scale.h);
}

const Size & Display::device(void)
{
    return winsz;
}

const Size & Display::size(void)
{
    return rendersz;
}

void Display::redraw(void)
{
    u32 tickCurrent = Tools::ticks();

    if(DisplayScene::isDirty())
    {
	DisplayScene::sceneRedraw();
	renderPresent();
    
	DisplayScene::renderPresentHandle(Tools::ticks() - tickCurrent);
	DisplayScene::setDirty(false);
    }
}

u32 Display::timeStart(void)
{
    return tickStart;
}

Texture Display::createTexture(const Surface & sf)
{
#ifdef OLDENGINE
    return Texture(sf);
#else
    return Texture(SDL_CreateTextureFromSurface(_renderer, sf.toSDLSurface()));
#endif
}

Texture Display::createTexture(const BinaryBuf & raw)
{
    SDL_RWops *rw = SDL_RWFromConstMem(raw.data(), raw.size());
    if(! rw)
        ERROR(SDL_GetError());
#ifdef OLDENGINE
    return Texture(IMG_Load_RW(rw, 1));
#else
    return Texture(IMG_LoadTexture_RW(_renderer, rw, 1));
#endif
}

Texture Display::createTexture(const std::string & file)
{
#ifdef OLDENGINE
    return Texture(IMG_Load(file.c_str()));
#else
    return Texture(IMG_LoadTexture(_renderer, file.c_str()));
#endif
}

Texture Display::createTexture(const Size & sz, bool alpha)
{
#ifdef OLDENGINE
    return Texture(Surface(sz, alpha));
#else
    Texture res(SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, sz.w, sz.h));

    renderClear(Color::Transparent, res);
    res.setBlendMode(BlendMode::Blend);

    return res;
#endif
}

Texture Display::createTexture(const Texture & tx, int flip)
{
    Texture res = Display::createTexture(tx.size());
    Display::renderTexture(tx, tx.rect(), res, res.rect(), flip);
    return res;
}

Texture & Display::texture(void)
{
    return displayTexture;
}

Surface Display::createSurface(const Texture & tx)
{
#ifdef OLDENGINE
    return Surface::copy(tx.toSDLTexture());
#else
    SDL_Surface* sf = NULL;
    Texture tx2 = Display::createTexture(tx, FlipVertical);

    if(renderReset(tx2.toSDLTexture()))
    {
	SDL_Rect srt = tx2.rect().toSDLRect();
	sf = SDL_CreateRGBSurface(0, srt.w, srt.h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

        if(! sf || 0 != SDL_RenderReadPixels(_renderer, & srt, SDL_PIXELFORMAT_ARGB8888, sf->pixels, sf->pitch))
    	    ERROR(SDL_GetError());
    }

    renderReset(NULL);

    return Surface(sf);
#endif
}

bool Display::handleEvents(void)
{
    while(SDL_PollEvent(& current))
    {
        switch(current.type)
        {
#ifdef OLDENGINE
	    case SDL_VIDEORESIZE:
        	Display::resizeWindow(Size(current.resize.w, current.resize.h), true);
		break;

	    case SDL_ACTIVEEVENT:
		if(current.active.state & SDL_APPACTIVE) handleFocusEvent(current.active.gain);
		break;
#else
            case SDL_WINDOWEVENT:
                if(SDL_WINDOWEVENT_EXPOSED == current.window.event)
                    Display::renderPresent();
                else
		if(SDL_WINDOWEVENT_RESIZED == current.window.event)
                    Display::resizeWindow(Size(current.window.data1, current.window.data2), true);
		else
		if(SDL_WINDOWEVENT_FOCUS_GAINED == current.window.event)
		    handleFocusEvent(true);
    		else
		if(SDL_WINDOWEVENT_FOCUS_LOST == current.window.event)
		    handleFocusEvent(false);
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
            case SDL_MOUSEBUTTONUP:
		handleMouseButton(current.button);
		break;

            case SDL_MOUSEMOTION:
		handleMouseMotion(current.motion);
		break;

 #ifndef OLDENGINE
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
		DEBUG("SDL_QUIT");
                // Engine::except(__FUNCTION__, "SDL_QUIT");
		return false;

	    default: break;
	}
    }

    DisplayScene::tickHandle(Tools::ticks() - tickStart);
    return true;
}

enum { ScrollNone, ScrollUp, ScrollDown, ScrollLeft, ScrollRight };

void Display::handleKeyboard(const SDL_KeyboardEvent & ev)
{
    int key = Key::toKey(ev.keysym.sym, ev.keysym.mod);

    DisplayScene::keyDebugHandle(ev);

    switch(ev.type)
    {
        case SDL_KEYDOWN:
            DisplayScene::keyPressHandle(key);
            break;

        case SDL_KEYUP:
            DisplayScene::keyReleaseHandle(key);
            break;

	default: break;
    }

    switch(key)
    {
	case Key::PAGEUP:	handleScrollEvent(ScrollUp, Point(-1, -1)); break;
	case Key::PAGEDOWN:	handleScrollEvent(ScrollDown, Point(-1, -1)); break;
	default: break;
    }
}

void Display::handleMouseButton(const SDL_MouseButtonEvent & ev)
{
    ButtonsEvent* coords = NULL;

    switch(ev.button)
    {
#ifdef OLDENGINE
        case SDL_BUTTON_WHEELUP:
        case SDL_BUTTON_WHEELDOWN:
                handleMouseWheel(ev.button, ev.type);
                break;
#else
	case SDL_BUTTON_X1:	coords = &mouseButtons[4]; break;
	case SDL_BUTTON_X2:	coords = &mouseButtons[5]; break;
#endif
	case SDL_BUTTON_LEFT:	coords = &mouseButtons[1]; break;
	case SDL_BUTTON_RIGHT:	coords = &mouseButtons[2]; break;
	case SDL_BUTTON_MIDDLE:	coords = &mouseButtons[3]; break;
	default: break;
    }

    if(coords)
    {
	Point real(ev.x, ev.y);
	if(Display::scaleUsed())
    	    real = Display::scaleValue(real);

	switch(ev.type)
	{
	    case SDL_MOUSEBUTTONDOWN:
	    	coords->setPress(real);
		// generate press
        	DisplayScene::mousePressHandle(coords->press());
		break;

	    case SDL_MOUSEBUTTONUP:
		coords->setRelease(real);
		// generate release
		DisplayScene::mouseReleaseHandle(coords->release());
		// generate click
		DisplayScene::mouseClickHandle(*coords);
		break;

	    default: break;
	}
    }
}

void Display::handleMouseMotion(const SDL_MouseMotionEvent & ev)
{
    Point real(ev.x, ev.y);
    if(Display::scaleUsed())
    	real = Display::scaleValue(real);

    DisplayScene::mouseMotionHandle(real, ev.state);
}

void Display::handleFocusEvent(bool gain)
{
    DisplayScene::displayFocusHandle(gain);
}

#ifdef OLDENGINE
void Display::handleMouseWheel(int button, int type)
{
    if(SDL_MOUSEBUTTONUP == type)
    {
        int x, y;
        SDL_GetMouseState(& x, & y);

        Point real(x, y);
        if(Display::scaleUsed())
        real = Display::scaleValue(real);

        if(SDL_BUTTON_WHEELUP == button)
            handleScrollEvent(ScrollUp, real);
        else
        if(SDL_BUTTON_WHEELDOWN == button)
            handleScrollEvent(ScrollDown, real);
    }
}
#else
void Display::handleMouseWheel(const SDL_MouseWheelEvent & ev)
{
    int x, y;
    SDL_GetMouseState(& x, & y);

    Point real(x, y);
    if(Display::scaleUsed())
        real = Display::scaleValue(real);

    if(0 < ev.y)
	handleScrollEvent(ScrollUp, real);
    else
    if(0 > ev.y)
	handleScrollEvent(ScrollDown, real);
}

void Display::handleTextInput(const SDL_TextInputEvent & ev)
{
    DisplayScene::textInputHandle(ev.text);
}

void Display::handleFingerTap(const SDL_TouchFingerEvent & ev)
{
    ButtonsEvent & coords = mouseButtons[0];

    const Size & dsz = Display::device();
    Point real(dsz.w * ev.x, dsz.h * ev.y);

    if(Display::scaleUsed())
        real = Display::scaleValue(real);

    switch(ev.type)
    {
        case SDL_FINGERDOWN:
	    coords.setPress(real);
	    // generate press
	    DisplayScene::mousePressHandle(coords.press());
	    break;

        case SDL_FINGERUP:
	    {
		coords.setRelease(real);
		// generate release
		DisplayScene::mouseReleaseHandle(coords.release());
#if (defined ANDROID)
		// fix finger motion
		Point delta = coords.press().position() - coords.release().position();
		const int val = 5;
		if(val > std::abs(delta.x) && val > std::abs(delta.y))
		    DisplayScene::mouseClickHandle(coords);
#else
		// generate click
		DisplayScene::mouseClickHandle(coords);
#endif
	    }
	    break;

	default: break;
    }
}

void Display::handleFingerMotion(const SDL_TouchFingerEvent & ev)
{
    const Size & dsz = Display::device();

    Point real(dsz.w * ev.x, dsz.h * ev.y);

    if(Display::scaleUsed())
        real = Display::scaleValue(real);

    DisplayScene::mouseMotionHandle(real, FingerTap);

    float dx = dsz.w * ev.dx;
    float dy = dsz.h * ev.dy;

    if(std::fabs(dx) > std::fabs(dy))
	handleScrollEvent(dx > 0 ? ScrollLeft : ScrollRight, real);
    else
    if(std::fabs(dy) > std::fabs(dx))
	handleScrollEvent(dy > 0 ? ScrollUp : ScrollDown, real);
}
#endif

void Display::handleUserEvent(const SDL_UserEvent & ev)
{
    DisplayScene::userHandle(ev);
}

void Display::handleScrollEvent(int dir, const Point & pos)
{
    switch(dir)
    {
	case ScrollUp:    DisplayScene::scrollUpHandle(pos); break;
	case ScrollDown:  DisplayScene::scrollDownHandle(pos); break;
	case ScrollLeft:  DisplayScene::scrollLeftHandle(pos); break;
	case ScrollRight: DisplayScene::scrollRightHandle(pos); break;
	default: break;
    }
}

void Display::hardwareCursorHide(void)
{
    SDL_ShowCursor(SDL_DISABLE);
}

void Display::hardwareCursorShow(void)
{
    SDL_ShowCursor(SDL_ENABLE);
}

Point Display::mouseCursorPosition(void)
{
    Point res;
    SDL_GetMouseState(& res.x, & res.y);
    return Display::scaleUsed() ? Display::scaleValue(res) : res;
}

u32 Display::mouseButtonState(void)
{
    return SDL_GetMouseState(NULL, NULL);
}

Rect Display::renderTextHorizontal(const FontRender & frs, const UnicodeString & ustr, const Color & col, Texture & dtx, const Point & dpt, int halign, int valign)
{
    Rect res;

    if(frs.isValid() && ustr.size())
    {
        Point dst = dpt;

        if(halign == AlignCenter)
            dst.x -= frs.unicodeSize(ustr, true).w / 2;
        else
        if(halign == AlignRight)
            dst.x -= frs.unicodeSize(ustr, true).w;

        if(valign == AlignCenter)
            dst.y -= frs.unicodeSize(ustr, true).h / 2;
        else
        if(valign == AlignBottom)
            dst.y -= frs.unicodeSize(ustr, true).h;

        res.x = dst.x;
        res.y = dst.y;

        for(auto it = ustr.begin(); it != ustr.end(); ++it) if(*it)
        {
            Texture tx = FontsCache(&frs).renderCharset(*it, col);

            if(tx.isValid())
            {
                Display::renderTexture(tx, tx.rect(), dtx, Rect(dst, tx.size()));

                if(res.h < tx.height()) res.h = tx.height();
                dst.x += frs.symbolAdvance(*it);
            }
        }

        res.w = dst.x - res.x;
    }

    return res;
}

Rect Display::renderTextVertical(const FontRender & frs, const UnicodeString & ustr, const Color & col, Texture & dtx, const Point & dpt, int halign, int valign)
{
    Rect res;

    if(frs.isValid() && ustr.size())
    {
        Point dst = dpt;

        if(valign == AlignCenter)
            dst.y -= frs.unicodeSize(ustr, false).h / 2;
        else
        if(valign == AlignBottom)
            dst.y -= frs.unicodeSize(ustr, false).h;

        res.x = dst.x;
        res.y = dst.y;

        if(halign == AlignCenter)
            res.x -= frs.unicodeSize(ustr, false).w / 2;
        else
        if(halign == AlignRight)
            res.x -= frs.unicodeSize(ustr, false).w;

        for(auto it = ustr.begin(); it != ustr.end(); ++it) if(*it)
        {
            Texture tx = FontsCache(&frs).renderCharset(*it, col);

            if(tx.isValid())
            {
                if(halign == AlignCenter)
                    dst.x = dpt.x - tx.width() / 2;
                else
                if(halign == AlignRight)
                    dst.x = dpt.x - tx.width();

                Display::renderTexture(tx, tx.rect(), dtx, Rect(dst, tx.size()));

                if(res.w < tx.width()) res.w = tx.width();
                dst.y += tx.height();
            }
        }

        res.h = dst.y - res.y;
    }

    return res;
}

Rect Display::renderText(const FontRender & frs, const UnicodeString & ustr, const Color & col, Texture & dst, const Point & dpt, int halign, int valign, bool horizontal)
{
    return horizontal ? renderTextHorizontal(frs, ustr, col, dst, dpt, halign, valign) :
            renderTextVertical(frs, ustr, col, dst, dpt, halign, valign);
}

Texture Display::renderText(const FontRender & frs, const UCString & ustr)
{
    Texture res;

    if(frs.isValid() && ustr.length())
    {
        Size sz = frs.unicodeSize(ustr.toUnicodeString());

#ifdef OLDENGINE
        res = createTexture(sz, false);
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
                renderTexture(tx, tx.rect(), res, Rect(dst, tx.size()));
                dst.x += frs.symbolAdvance(us.unicode());
            }
        }
    }
 
    return res;
}
    
Texture Display::renderText(const FontRender & frs, const UnicodeString & ustr, const Color & fg, const Color & bg)
{
    return renderText(frs, UCString(ustr, FBColors(fg.toColorIndex(), bg.toColorIndex())));
}

bool Display::resize(const Size & winsz)
{
    return resizeWindow(winsz, false);
}

std::list<Size> Display::hardwareVideoModes(void)
{
    std::list<Size> result;

#ifdef OLDENGINE
    SDL_Rect** modes = SDL_ListModes(NULL, SDL_FULLSCREEN | SDL_HWSURFACE);

    if(modes == (SDL_Rect **) 0)
    {
        ERROR("no modes available");
	return result;
    }
    else
    if(modes == (SDL_Rect **) -1)
    {
        ERROR("all resolutions available");
	return result;
    }
    else
    {
        for(int ii = 0; modes[ii]; ++ii)
	{
	    Size mode(modes[ii]->w, modes[ii]->h);
	    if(mode.h > mode.w) std::swap(mode.w, mode.h);
	    result.push_back(mode);
	}
    }
#else
    SDL_DisplayMode mode;
    int displayIndex = 0;
    int modeIndex = 0;

    while(0 == SDL_GetDisplayMode(displayIndex, modeIndex++, & mode))
    {
	if(mode.h > mode.w) std::swap(mode.w, mode.h);
	result.push_back(Size(mode.w, mode.h));
    }
#endif

    result.sort();
    result.unique();

    return result;
}

/***************************************************************************
 *   Copyright (C) 2018 by public.irkutsk@gmail.com                        *
 *                                                                         *
 *   Part of the SWE (SDL Window Engine) examples:                         *
 *   https://github.com/AndreyBarmaley/sdl-window-engine.git               *
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

#include <clocale>

#include "settings.h"
#include "default_font.h"
#include "mainscreen.h"

bool translationInit(void)
{
    Translation::setStripContext('|');
    const std::string trans = Settings::dataLang(Systems::messageLocale(1).append(".mo"));

    if(Translation::bindDomain(Settings::progDomain(), trans))
    {
        VERBOSE("loaded from: " << trans);
        //Translation::setDomain(Settings::domain());
        return true;
    }

    return false;
}

int main(int argc, char **argv)
{
#ifndef ANDROID
    try
#endif
    {
	translationInit();

	if(! Engine::init())
    	    return -1;

	MainScreen & scr = MainScreen::init(Settings::progName());
	scr.exec();
    }
#ifndef ANDROID
    catch(Engine::exception &)
    {
    }
#endif
    return EXIT_SUCCESS;
}

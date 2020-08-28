/***************************************************************************
 *   Copyright (C) 2020 by SWE team <sdl.window.engine@gmail.com>          *
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

#include <clocale>
#include "mainscreen.h"

int main(int argc, char **argv)
{
    try
    {
        SWE::Systems::setLocale(LC_ALL, "");

	const std::string title = SWE::StringFormat("SWE TestTerm: %1").arg(SWE::Engine::version());

	if(! SWE::Engine::init())
    	    return -1;

	MainScreen & scr = MainScreen::init(title);
	scr.exec();
    }
    catch(SWE::Engine::exception &)
    {
    }
    return EXIT_SUCCESS;
}

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

#include <algorithm>
#include <iterator>
#include <functional>

#include "listitems.h"


/* CellInfoBar */
CellInfoBar::CellInfoBar(const SWE::Point & pos, std::initializer_list<CellInfo> list, int col, SWE::Window & win)
    : LinkedListBar(false, win)
{
    setPosition(pos);

    const int cellsz = 40;
    setSize(list.size() * cellsz, cellsz);

    for(auto it = list.begin(); it != list.end(); ++it)
	addItem(new CellInfoItem(*it, SWE::Size(cellsz, cellsz), col, *this));

    setVisible(true);
}

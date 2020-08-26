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

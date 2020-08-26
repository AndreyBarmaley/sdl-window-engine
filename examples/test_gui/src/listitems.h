#ifndef LISTITEMS_H
#define LISTITEMS_H

#include <initializer_list> 

#include "libswe.h"
////////////////////////// template

template<typename T>
class LinkedListBar : public SWE::ListWidget
{
    LinkedListBar* linkedBar;

protected:
    bool swapItems(SWE::ListWidgetItem* val1, SWE::ListWidgetItem* val2)
    {
	auto item1 = dynamic_cast<T*>(val1);
	auto item2 = dynamic_cast<T*>(val2);
	if(item1 && item2)
	{
	    std::swap(*item1, *item2);
	    return true;
	}
	return false;
    }

public:
    LinkedListBar(bool vertical, SWE::Window & win) : SWE::ListWidget(vertical, & win), linkedBar(NULL)
    {
    }

    void setLinkedListBar(LinkedListBar* link)
    {
	if(this != link) linkedBar = link;
    }

    void itemClicked(SWE::ListWidgetItem* clickedItem, int buttons) override
    {
	if(clickedItem)
	{
    	    // move from bar
    	    if(linkedBar && linkedBar->currentItem())
    	    {
        	swapItems(linkedBar->currentItem(), clickedItem);
        	linkedBar->setCurrentItem(NULL);
        	setCurrentItem(NULL);
    	    }
    	    else
    	    // empty not selected
    	    if(0 == clickedItem->isValid() && currentItem())
        	setCurrentItem(NULL);
	}
    }

    void currentItemChanged(SWE::ListWidgetItem* current, SWE::ListWidgetItem* previous) override
    {
	if(swapItems(current, previous))
    	    setCurrentItem(NULL);
    }
};

//////////////////////////////// content
struct CellInfo
{
    int				val;

    CellInfo(int v = 0) : val(v) {}

    int getValue(void) const { return val; }
    bool isValue(int v) const { return val == v; }
};

class CellInfoItem : public SWE::ListWidgetItem, public CellInfo
{
protected:
    int				color;

public:
    CellInfoItem(const CellInfo & val, const SWE::Size & cellsz, int col, SWE::ListWidget & win)
	: SWE::ListWidgetItem(win), CellInfo(val), color(col)
    {
	setSize(cellsz);
	setVisible(true);
    }

    bool isValid(void) const override
    {
	return getValue();
    }

    void renderWindow(void) override
    {
	std::string str = isValid() ? SWE::String::number(getValue()) : "-";
	renderClear(SWE::Color::Black);
	renderColor(color, SWE::Rect(SWE::Point(1, 1), size() - SWE::Size(2, 2)));

	auto text = SWE::Display::renderText(SWE::FontRenderSystem(), str, SWE::Color::White);
	renderTexture(text, (size() - text.size()) / 2);

	if(isSelected()) renderRect(SWE::Color::Yellow, rect());
    }
};

class CellInfoBar : public LinkedListBar<CellInfo>
{
public:
    CellInfoBar(const SWE::Point &, std::initializer_list<CellInfo>, int col, SWE::Window &);
};
#endif

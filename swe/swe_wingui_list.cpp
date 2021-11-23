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

#include <algorithm>
#include <iterator>

#include "swe_display.h"
#include "swe_wingui_scroll.h"
#include "swe_wingui_list.h"

namespace SWE
{
    /* ListWidgetItem */
    ListWidgetItem::ListWidgetItem(ListWidget & parent) : WindowToolTipArea(& parent)
    {
        setModality(false);
    }

    ListWidgetItem::ListWidgetItem(const SWE::Size & wsz, ListWidget & parent) : WindowToolTipArea(wsz, & parent)
    {
        setModality(false);
    }

    bool ListWidgetItem::isSelected(void) const
    {
	auto list = listWidget();
	return list ? list->currentItem() == this : false;
    }

    ListWidget* ListWidgetItem::listWidget(void) const
    {
	auto list = dynamic_cast<const ListWidget*>(parent());
	return list ? const_cast<ListWidget*>(list) : nullptr;
    }

    void ListWidgetItem::setSelected(bool select)
    {
	auto list = listWidget();
	if(list) list->setCurrentItem(select ? this : nullptr);
    }

    bool ListWidgetItem::mousePressEvent(const ButtonEvent & be)
    {
	auto list = listWidget();
	if(list) list->itemPressed(this, be.button());

        return true;
    }

    bool ListWidgetItem::mouseClickEvent(const ButtonsEvent & be)
    {
	auto list = listWidget();
	if(list)
	{
	    if(be.isButtonLeft())
	    {
    		if(isSelected())
		{
		    list->itemClicked(this, be.press().button());
		    list->itemDoubleClicked(this);
		}
		else
		{
		    setSelected(true);
		    list->itemClicked(this, be.press().button());
		}
	    }
	    else
	    {
		list->itemClicked(this, be.press().button());
	    }
	}

        return true;
    }

    void ListWidgetItem::mouseFocusEvent(void)
    {
	auto list = listWidget();
	if(list) list->itemEntered(this);
    }

    void ListWidgetItem::mouseLeaveEvent(void)
    {
	auto list = listWidget();
	if(list) list->itemEntered(nullptr);
    }


    bool ListWidgetItem::isVisibleNotHidden(void) const
    {
	return ! isHidden() && isVisible();
    }

    /* ListWidget */
    ListWidget::ListWidget(bool vertical, Window* parent) : Window(parent), skipItems(0), curItem(nullptr)
    {
        if(vertical) setState(FlagVertical);
        setModality(false);
    }

    ListWidget::ListWidget(const Size & sz, bool vertical, Window* parent) : Window(sz, parent), skipItems(0), curItem(nullptr)
    {
        if(vertical) setState(FlagVertical);
        setModality(false);
    }

    ListWidget::ListWidget(const Point & pos, const Size & sz, bool vertical, Window* parent) : Window(pos, sz, parent), skipItems(0), curItem(nullptr)
    {
        if(vertical) setState(FlagVertical);
        setModality(false);
    }

    ListWidget::~ListWidget()
    {
	std::for_each(listItems.begin(), listItems.end(), std::default_delete<ListWidgetItem>());
    }

    void ListWidget::addItem(ListWidgetItem* item)
    {
	listItems.push_back(item);
	signalEmit(Signal::ListWidgetChanged);
    }

    void ListWidget::clear(void)
    {
	std::for_each(listItems.begin(), listItems.end(), std::default_delete<ListWidgetItem>());
	listItems.clear();
	curItem = nullptr;
	skipItems = 0;
	signalEmit(Signal::ListWidgetChanged);
    }

    int ListWidget::count(void) const
    {
	return listItems.size();
    }

    ListWidgetItem* ListWidget::currentItem(void) const
    {
	return curItem;
    }

    int ListWidget::currentRow(void) const
    {
	return row(currentItem());
    }

    void ListWidget::insertItem(int row, ListWidgetItem* item)
    {
	if(row < 0 || row >= static_cast<int>(listItems.size()))
	    listItems.push_back(item);
	else
	{
	    auto it = std::next(listItems.begin(), row);
	    listItems.insert(it, item);
	}
	signalEmit(Signal::ListWidgetChanged);
    }

    ListWidgetItem* ListWidget::item(int row) const
    {
	return row < 0 || row >= static_cast<int>(listItems.size()) ?
		nullptr : listItems[row];
    }

    ListWidgetItem* ListWidget::itemAt(const Point & pos) const
    {
	auto it = std::find_if(listItems.begin(), listItems.end(), [&](const ListWidgetItem* item)
	{
	    return item->isVisibleNotHidden() && (item->area() & pos);
	});

	return it != listItems.end() ? *it : nullptr;
    }

    const std::vector<ListWidgetItem*> &
    ListWidget::items(void) const
    {
	return listItems;
    }

    void ListWidget::removeItemWidget(ListWidgetItem* item)
    {
	auto it = std::find(listItems.begin(), listItems.end(), item);
	if(it != listItems.end())
	    pushEventAction(Signal::ListWidgetRemoveItem, this, *it);
    }

    int ListWidget::row(const ListWidgetItem* item) const
    {
	auto it = std::find(listItems.begin(), listItems.end(), item);
	return it == listItems.end() ?
	    -1 : std::distance(listItems.begin(), it);
    }

    int ListWidget::rowTop(void) const
    {
	return skipItems;
    }

    void ListWidget::scrollToItem(const ListWidgetItem* item)
    {
	int pos = row(item);

	if(0 <= pos)
	{
	    int visible = visibleItems();

	    if(pos < skipItems)
	    {
		skipItems = pos;
        	signalEmit(Signal::ListWidgetScrolled);
	    }
	    else
	    if(skipItems + visible - 1 < pos)
	    {
		skipItems = pos;
		if(skipItems > static_cast<int>(listItems.size()) - visible) skipItems = listItems.size() - visible;
    		signalEmit(Signal::ListWidgetScrolled);
	    }
	}
    }

    void ListWidget::setCurrentItem(ListWidgetItem* item)
    {
	ListWidgetItem* previous = curItem;

	if(! item)
	    curItem = nullptr;
	else
	if(listItems.end() != std::find(listItems.begin(), listItems.end(), item))
	    curItem = item;

	if(curItem != previous)
	{
	    currentItemChanged(curItem, previous);
	    setDirty(true);
	}
    }

    void ListWidget::setActiveItem(ListWidgetItem* item)
    {
        setCurrentItem(item);
	auto index = row(item);
	auto visible = visibleItems();

	if(0 <= index && (index < skipItems || index > skipItems + visible))
        {
	    setSkipItems(index + visible < count() ? index : count() - visible);
    	    if(skipItems < 0) skipItems = 0;
	    signalEmit(Signal::ListWidgetScrolled);
	}
    }

    void ListWidget::setSkipItems(int skip)
    {
	skipItems = skip;
    }

    void ListWidget::setCurrentRow(int row)
    {
	setCurrentItem(item(row));
    }

    void ListWidget::sortItems(void)
    {
	std::sort(listItems.begin(), listItems.end(),
	    [](const ListWidgetItem* a, const ListWidgetItem* b){ return *a < *b; });
    }

    ListWidgetItem* ListWidget::frontItem(void)
    {
	return listItems.size() ? listItems.front() : nullptr;
    }

    ListWidgetItem* ListWidget::backItem(void)
    {
	return listItems.size() ? listItems.back() : nullptr;
    }

    ListWidgetItem* ListWidget::takeItem(int row)
    {
	auto it = std::find(listItems.begin(), listItems.end(), item(row));
	if(it != listItems.end())
	{
	    pushEventAction(Signal::ListWidgetTakeItem, this, *it);
	    return *it;
	}
	return nullptr;
    }

    bool ListWidget::isAreaPoint(const Point & pt) const
    {
        return (listArea() + position()) & pt;
    }

    Rect ListWidget::listArea(void) const
    {
	return rect();
    }

    int ListWidget::scrollItems(void) const
    {
	return 1;
    }

    int ListWidget::visibleItems(void) const
    {
	Rect area = listArea();
	Point itemPos = area.toPoint();
	int visible = 0;

	for(auto it = listItems.begin(); it != listItems.end(); ++it)
	{
	    auto item = *it;
	    if(! isHidden())
	    {
		visible++;
    
        	if(isVerticalOrientation())
		{
                    itemPos.y += item->height() + itemSpacer();
		    if(itemPos.y + item->height() > area.y + area.h) break;
                }
		else
		{
                    itemPos.x += item->width() + itemSpacer();
		    if(itemPos.x + item->width() > area.x + area.w) break;
		}
	    }
	}

	return visible;
    }

    bool ListWidget::scrollUp(int rows)
    {
        if(0 < skipItems)
        {
            skipItems -= rows;
	    if(0 > skipItems) skipItems = 0;

            signalEmit(Signal::ListWidgetScrolled);
            setDirty(true);
            return true;
        }
        return false;
    }

    bool ListWidget::scrollDown(int rows)
    {
        int visible = visibleItems();
	int listsz = listItems.size();

	if(visible < listsz &&
	    skipItems + visible < listsz)
	{
	    skipItems += rows;
	    if(skipItems > listsz - visible) skipItems = listItems.size() - visible;

    	    signalEmit(Signal::ListWidgetScrolled);
    	    setDirty(true);
    	    return true;
	}

        return false;
    }

    bool ListWidget::scrollUpEvent(void)
    {
	return scrollUp(scrollItems());
    }

    bool ListWidget::scrollDownEvent(void)
    {
	return scrollDown(scrollItems());
    }

    bool ListWidget::userEvent(int ev, void* ptr)
    {
        if(ev == Signal::FingerMoveUp)
            return scrollUp(scrollItems());
	else
        if(ev == Signal::FingerMoveDown)
            return scrollDown(scrollItems());
	else
	if(ev == Signal::ListWidgetRemoveItem || ev == Signal::ListWidgetTakeItem)
	{
	    auto item = reinterpret_cast<ListWidgetItem*>(ptr);
	    if(item)
	    {
		auto it = std::find(listItems.begin(), listItems.end(), item);
		if(it == listItems.end()) return false;
		if(*it == currentItem())
		    setCurrentItem(nullptr);
		if(ev == Signal::ListWidgetRemoveItem) delete *it;
		listItems.erase(it);
		signalEmit(Signal::ListWidgetChanged);
		return true;
	    }
	}

        return false;
    }

    void ListWidget::signalReceive(int sig, const SignalMember* sm)
    {
        switch(sig)
        {
            case Signal::ScrollBarMoved:
            {
                const ScrollBar* scroll = dynamic_cast<const ScrollBar*>(sm);

                if(scroll)
                {
		    int top = ScrollBar::transformToListTopRow(*scroll, *this);
		    if(0 <= top)
		    {
			skipItems = top;
    			setDirty(true);
		    }
                }
            }
            break;

            default:
                break;
        }
    }

    bool ListWidget::isVerticalOrientation(void) const
    {
        return checkState(FlagVertical);
    }

    void ListWidget::renderVisibleItem(ListWidgetItem* item, int visibleIndex, int visibleItems)
    {
	Point itemPos = listArea().toPoint();
	for(int row = skipItems; row < skipItems + visibleItems; ++row)
	{
	    ListWidgetItem* item2 = ListWidget::item(row);
	    if(item2 == item) break;

    	    if(isVerticalOrientation())
	    {
        	itemPos.y += (item2->height() + itemSpacer());
	    }
	    else
	    {
        	itemPos.x += (item2->width() + itemSpacer());
	    }
	}
	item->setPosition(itemPos);
	item->setHidden(false);
    }

    void ListWidget::renderWindow(void)
    {
        if(listItems.size())
        {
	    int visible = visibleItems();
	    int listsz = listItems.size();

            for(int row = 0; row < listsz; ++row)
            {
		ListWidgetItem* item = ListWidget::item(row);
		if(! item) continue;

	        if(skipItems <= row && row < skipItems + visible)
	        {
		    renderVisibleItem(item, row - skipItems, visible);
	        }
	        else
                {
                    item->setHidden(true);
                }
            }
        }
    }

    /* TextAreaItem */
    TextAreaItem::TextAreaItem(const TexturePos & tp, TextArea & win) : ListWidgetItem(win), content(tp)
    {
        setSize(Size(win.listArea().w, tp.height()));
	setVisible(true);
    }

    void TextAreaItem::renderWindow(void)
    {
        renderTexture(content);
    }

    /* TextArea */
    TextArea & TextArea::appendString(const Texture & tx, const Point & offset)
    {
        addItem(new TextAreaItem(TexturePos(tx, offset), *this));
        return *this;
    }

    TextArea & TextArea::appendString(const FontRender & frs, const UnicodeString & str, const Color & col, int halign, bool wrap)
    {
        if(wrap)
        {
            UCStringList list = frs.splitUCStringWidth(UCString::parseUnicode(str, FBColors(col.toColorIndex())), listArea().w);

            for(auto it = list.begin(); it != list.end(); ++it)
                appendString(frs, *it, halign, false);
        }
        else
            appendString(frs, UCString::parseUnicode(str, FBColors(col.toColorIndex())), halign, false);

        return *this;
    }

    TextArea & TextArea::appendString(const FontRender & frs, const UCString & str, int halign, bool wrap)
    {
        if(wrap)
        {
            UCStringList list = frs.splitUCStringWidth(str, listArea().w);

            for(auto it = list.begin(); it != list.end(); ++it)
                appendString(frs, *it, halign, false);
        }
        else if(str.length())
        {
            Texture tx = Display::renderText(frs, str);
            Point offset = Point(0, 0);
            
            switch(halign)
            {
                case AlignRight:
                    offset.x = listArea().w - tx.width();
                    break;
             
                case AlignCenter:
                    offset.x = (listArea().w - tx.width()) / 2;
                    break;
            
                default:
                    break;
            }
                    
            appendString(tx, offset);
        }
        else
            appendSpacer(frs);

        return *this;
    }

    TextArea & TextArea::appendSpacer(const FontRender & frs)
    {   
        int height = frs.lineSkipHeight();
        addItem(new TextAreaItem(TexturePos(Display::createTexture(Size(listArea().w, height)), Point()), *this));
        return *this;
    }
}

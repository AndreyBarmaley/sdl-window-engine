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

#ifndef _SWE_GUI_LIST_
#define _SWE_GUI_LIST_

#include <vector>

#include "swe_wingui.h"

/// @brief пространство SWE
namespace SWE
{
    // ListWidget emit signals: Signal::ListWidgetChanged, Signal::ListWidgetScrolled
    class ListWidget;

    class ListWidgetItem : public WindowToolTipArea
    {
	friend class ListWidget;
	bool			isVisibleNotHidden(void) const;

    protected:
        bool			mousePressEvent(const ButtonEvent &) override final;
        bool			mouseClickEvent(const ButtonsEvent &) override final;
	void			mouseFocusEvent(void) override final;
	void			mouseLeaveEvent(void) override final;

    public:
        ListWidgetItem(ListWidget &);
        ListWidgetItem(const SWE::Size &, ListWidget &);

        bool			isSelected(void) const;
	ListWidget*		listWidget(void) const;
	void			setSelected(bool);

	virtual bool		isValid(void) const { return true; }
	virtual bool		operator< (const ListWidgetItem & v) const { return this < &v; }
	virtual void		swap(ListWidgetItem*) {}
	const char*             className(void) const override { return "SWE::ListWidgetItem"; }
    };

    class ListWidget : public Window
    {
        std::vector<ListWidgetItem*>
				listItems;
	int			skipItems;
	ListWidgetItem*		curItem;

    protected:
        bool			scrollUpEvent(void) override;
        bool			scrollDownEvent(void) override;
        void			signalReceive(int, const SignalMember*) override;
        bool			userEvent(int, void*) override;

	virtual int		itemSpacer(void) const { return 0; }
	void			setSkipItems(int skip);

    public:
        ListWidget(bool vertical, Window*);
        ListWidget(const Size &, bool vertical, Window*);
        ListWidget(const Point &, const Size &, bool vertical, Window*);
        virtual ~ListWidget();

	void			addItem(ListWidgetItem*);

	int			count(void) const;
	void			clear(void);
	ListWidgetItem*		currentItem(void) const;
	int			currentRow(void) const;

	void			insertItem(int row, ListWidgetItem*);

	ListWidgetItem*		item(int row) const;
	ListWidgetItem*		itemAt(const Point &) const;

	const std::vector<ListWidgetItem*> &
				items(void) const;

	void			removeItemWidget(ListWidgetItem*);
	int 			row(const ListWidgetItem*) const;
	int			rowTop(void) const;

	void			scrollToItem(const ListWidgetItem*);
	void			setActiveItem(ListWidgetItem*);
	void			setCurrentItem(ListWidgetItem*);
	void 			setCurrentRow(int row);
	void			sortItems(void);

	ListWidgetItem*		takeItem(int row);
	ListWidgetItem*		frontItem(void);
	ListWidgetItem*		backItem(void);
	virtual int		visibleItems(void) const;
	virtual int		scrollItems(void) const;
	virtual Rect		listArea(void) const;

	virtual void		currentItemChanged(ListWidgetItem* current, ListWidgetItem* previous) {}
	virtual void		itemEntered(ListWidgetItem*) {}
	virtual void		itemPressed(ListWidgetItem*, int) {}
	virtual void		itemClicked(ListWidgetItem*, int) {}
	virtual void		itemDoubleClicked(ListWidgetItem*) {}

	virtual void		renderWindow(void) override;
	virtual void		renderVisibleItem(ListWidgetItem*, int visibleIndex, int visibleItems);

	bool			isAreaPoint(const Point &) const override;
        bool			isVerticalOrientation(void) const;
        bool			scrollUp(int rows = 1);
        bool			scrollDown(int rows = 1);

	const char*             className(void) const override { return "SWE::ListWidget"; }
#ifdef SWE_WITH_JSON
        JsonObject              toJson(void) const override
        {
            JsonObject res = Window::toJson();
            res.addInteger("size", listItems.size());
            res.addInteger("skipItems", skipItems);
	    JsonArray items;
	    for(auto it = listItems.begin(); it != listItems.end(); ++it)
		items.addObject((*it)->toJson());
            res.addString("curItem", String::pointer(curItem));
	    res.addArray("items", items);
            return res;
        }
#endif
    };

    class TextArea;

    class TextAreaItem : public ListWidgetItem
    {
        TexturePos              content;

    public:
        TextAreaItem(const TexturePos &, TextArea &);
        void renderWindow(void) override;

	const char*             className(void) const override { return "SWE::TextAreaItem"; }
#ifdef SWE_WITH_JSON
        JsonObject              toJson(void) const override
        {
            JsonObject res = ListWidgetItem::toJson();
	    res.addObject("content", content.toJson());
	    return res;
	}
#endif
    };

    class TextArea : public ListWidget
    {
    public:
        TextArea(Window* win) : ListWidget(true, win) {}
        TextArea(const Size & sz, Window* win) : ListWidget(sz, true, win) {}
        TextArea(const Point & pos, const Size & sz, Window* win) : ListWidget(pos, sz, true, win) {}

        TextArea  &       	appendString(const FontRender &, const UnicodeString &, const Color &, int halign = AlignLeft, bool wrap = false);
        TextArea  &       	appendString(const FontRender &, const UCString &, int halign = AlignLeft, bool wrap = false);
        TextArea  &       	appendString(const Texture &, const Point &);
        TextArea  &       	appendSpacer(const FontRender &);

	const char*             className(void) const override { return "SWE::TextArea"; }
    };

} // SWE
#endif

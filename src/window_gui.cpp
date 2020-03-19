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

#include "inputs_keys.h"
#include "display.h"
#include "display_scene.h"
#include "window_gui.h"

WindowTexture::WindowTexture(const Texture & tx, Window* win) : Window(win), texture(tx)
{
    setSize(tx.size());
}

void WindowTexture::setTexture(const Texture & tx)
{
    setSize(tx.size());
    texture = tx;
}

void WindowTexture::renderWindow(void)
{
    renderTexture(texture, Point(0, 0));
}

WindowListItem::WindowListItem(Window & parent) : WindowToolTipArea(& parent)
{
    resetState(FlagModality);
}

bool WindowListItem::isSelected(void) const
{
    return checkState(FlagSelected);
}

WindowListBox::WindowListBox(const Point & pos, const Size & sz, bool vertical, Window & parent) : Window(&parent), indexTop(-1), indexSelected(-1)
{
    if(vertical) setState(FlagVertical);
    resetState(FlagModality);
    setPosition(pos);
    setSize(sz);
}

WindowListBox::WindowListBox(bool vertical, Window & parent) : Window(&parent), indexTop(-1), indexSelected(-1)
{
    if(vertical) setState(FlagVertical);
    resetState(FlagModality);
}

WindowListBox::~WindowListBox()
{
    for(auto it = items.begin(); it != items.end(); ++it)
	delete *it;
}

Rect WindowListBox::itemsArea(void) const
{
    return rect();
}

int WindowListBox::selectedItemIndex(void) const
{
    return indexSelected;
}

int WindowListBox::topItemIndex(void) const
{
    return indexTop;
}

int WindowListBox::itemsCount(void) const
{
    return items.size();
}

int WindowListBox::itemsVisible(void) const
{
    if(items.size())
    {
	Rect area = itemsArea();
	Size itsz = items.front()->size();
	u32 res = isVertical() ? area.h / itsz.h : area.w / itsz.w;
	return res < items.size() ? res : items.size();
    }

    return 0;
}

bool WindowListBox::isVertical(void) const
{
    return checkState(FlagVertical);
}

bool WindowListBox::mouseClickEvent(const ButtonsEvent & coords)
{
    if(coords.isButtonLeft() && items.size())
    {
	Size itsz = items.front()->size();
	int indexClick = indexTop;

	if(isVertical())
	    indexClick += itsz.h ? coords.press().position().y / itsz.h : 0;
	else
	    indexClick += itsz.w ? coords.press().position().x / itsz.w : 0;

	int itemsz = items.size();

	if(indexClick < itemsz && indexSelected != indexClick)
	{
	    indexSelected = indexClick;
	    signalEmit(Signal::ListboxSelectedItem);
	    renderWindow();
	    return true;
	}
    }

    return false;
}

bool WindowListBox::pageUp(void)
{
    if(0 < indexTop)
    {
	indexTop = indexTop - itemsVisible() + 1;
	if(0 > indexTop) indexTop = 0;
	signalEmit(Signal::WindowPageUp);
	renderWindow();
	return true;
    }

    return false;
}

bool WindowListBox::pageDown(void)
{
    int itemsz = items.size();
    int count = itemsVisible();

    if(indexTop < itemsz - count)
    {
	indexTop = indexTop + count - 1;
	if(itemsz - count < indexTop) indexTop = itemsz - count;
	signalEmit(Signal::WindowPageDown);
	renderWindow();
	return true;
    }

    return false;
}

bool WindowListBox::scrollPrev(void)
{
    if(0 < indexTop)
    {
	indexTop--;
	signalEmit(Signal::WindowScrolledPrev);
	renderWindow();
	return true;
    }

    return false;
}

bool WindowListBox::scrollNext(void)
{
    int itemsz = items.size();

    if(indexTop < itemsz - itemsVisible())
    {
	indexTop++;
	signalEmit(Signal::WindowScrolledNext);
	renderWindow();
	return true;
    }

    return false;
}

void WindowListBox::setTopItemIndex(int index)
{
    int itemsz = items.size();

    if(0 <= index && index < itemsz && index != indexTop)
    {
	indexTop = index;

	if(indexTop + itemsVisible() >= itemsz)
	    indexTop = itemsz - itemsVisible();

	signalEmit(Signal::ListboxChangedList);
	renderWindow();
    }
}

void WindowListBox::setSelectedItemIndex(int index)
{
    int itemsz = items.size();

    if(0 <= index && index < itemsz && index != indexSelected)
    {
	indexSelected = index;
	signalEmit(Signal::ListboxSelectedItem);
	renderWindow();
    }
    else
    if(0 > index && index != indexSelected)
    {
	indexSelected = -1;
	signalEmit(Signal::ListboxSelectedItem);
	renderWindow();
    }
}

void WindowListBox::resetSelectedItem(void)
{
    setSelectedItemIndex(-1);
}

void WindowListBox::signalReceive(int sig, const SignalMember* sm)
{
    switch(sig)
    {
	case Signal::ScrollBarScrolledPrev:
	    scrollPrev();
	    break;

	case Signal::ScrollBarScrolledNext:
	    scrollNext();
	    break;

	case Signal::ScrollBarMovedCursor:
	{
	    const WindowScrollBar* scroll = dynamic_cast<const WindowScrollBar*>(sm);
	    if(scroll)
	    {
		int pos = isVertical() ? scroll->cursorPosition().y : scroll->cursorPosition().x;
		setTopItemIndex((itemsCount() - itemsVisible()) * 
		     pos / (scroll->scrollAreaSize() - scroll->cursorSize()));
	    }
	}
	    break;

	default: break;
    }
}

bool WindowListBox::scrollUpEvent(const Point & pos)
{
    return scrollPrev();
}

bool WindowListBox::scrollDownEvent(const Point & pos)
{
    return scrollNext();
}

bool WindowListBox::selectedPrev(void)
{
    if(0 < indexSelected)
    {
	indexSelected--;
	if(indexSelected < indexTop)
	{
	    indexTop = indexSelected;
	    signalEmit(Signal::WindowScrolledPrev);
	}

	signalEmit(Signal::ListboxSelectedItem);
	renderWindow();

	return true;
    }

    return false;
}

bool WindowListBox::selectedNext(void)
{
    int itemsz = items.size();

    if(indexSelected < itemsz - 1)
    {
	indexSelected++;
	if(indexSelected >= indexTop + itemsVisible())
	{
	    indexTop++;
	    signalEmit(Signal::WindowScrolledNext);
	}

	signalEmit(Signal::ListboxSelectedItem);
	renderWindow();

	return true;
    }

    return false;
}

void WindowListBox::renderItems(void) const
{
    if(items.size())
    {
	std::for_each(items.begin(), items.end(), std::bind2nd(std::mem_fun(&Window::setVisible), false));

	const Rect & area = itemsArea();
	Point currentPos = area.toPoint();
	int indexLast = indexTop + itemsVisible();

	for(int it = indexTop; it < indexLast; ++it)
	{
	    if(indexSelected == it)
		items[it]->setState(FlagSelected);
	    else
		items[it]->resetState(FlagSelected);

	    items[it]->setPosition(currentPos);
	    items[it]->setVisible(true);

	    if(isVertical())
		currentPos.y += items[it]->height();
	    else
		currentPos.x += items[it]->width();
	}
    }
}

void WindowListBox::insertItem(WindowListItem* item, bool render)
{
    if(item)
    {
	items.push_back(item);
	if(item->parent() != this)
	    item->setParent(this);
	if(0 > indexTop)
	    indexTop = 0;
	signalEmit(Signal::ListboxChangedList);
	if(render) renderWindow();
    }
}

void WindowListBox::removeAllItems(void)
{
    if(items.size())
    {
	for(auto it = items.begin(); it != items.end(); ++it)
	{
	    (*it)->setVisible(false);
	    delete *it;
	}

	indexTop = -1;
	indexSelected = -1;

	items.clear();
	signalEmit(Signal::ListboxChangedList);
    }
}

bool WindowListBox::removeItem(const WindowId & id)
{
    auto it = std::find_if(items.begin(), items.end(), std::bind2nd(std::mem_fun(&Window::isID), id));

    if(it != items.end())
    {
	int indexDel = std::distance(items.begin(), it);
	items.erase(it);

	if(indexTop > indexDel)
	    indexTop -= 1;

	if(indexSelected > indexDel)
	    indexSelected -= 1;

	(*it)->setVisible(false);
	delete *it;

	signalEmit(Signal::ListboxChangedList);

	return true;
    }

    return false;
}
 
WindowListItem* WindowListBox::findItem(const WindowId & id)
{
    auto it = std::find_if(items.begin(), items.end(), std::bind2nd(std::mem_fun(&Window::isID), id));
    return it != items.end() ? (*it) : NULL;
}

const WindowListItem* WindowListBox::findItemConst(const WindowId & id) const
{
    auto it = std::find_if(items.begin(), items.end(), std::bind2nd(std::mem_fun(&Window::isID), id));
    return it != items.end() ? (*it) : NULL;
}

WindowListItem* WindowListBox::itemIndex(int index)
{
    int itemsz = items.size();
    return 0 <= index && index < itemsz ? items[index] : NULL;
}

const WindowListItem* WindowListBox::itemIndexConst(int index) const
{
    int itemsz = items.size();
    return 0 <= index && index < itemsz ? items[index] : NULL;
}

WindowListItem* WindowListBox::topItem(void)
{
    return itemIndex(indexTop);
}

const WindowListItem* WindowListBox::topItemConst(void) const
{
    return itemIndexConst(indexTop);
}

WindowListItem* WindowListBox::selectedItem(void)
{
    return itemIndex(indexSelected);
}

const WindowListItem* WindowListBox::selectedItemConst(void) const
{
    return itemIndexConst(indexSelected);
}

/* WindowScrollBar */
WindowScrollBar::WindowScrollBar(bool vertical, WindowListBox & win) : Window(&win)
{
    if(vertical) setState(FlagVertical);
    resetState(FlagModality);

    signalSubscribe(win, Signal::WindowPageUp);
    signalSubscribe(win, Signal::WindowPageDown);
    signalSubscribe(win, Signal::WindowScrolledPrev);
    signalSubscribe(win, Signal::WindowScrolledNext);
    signalSubscribe(win, Signal::ListboxChangedList);

    signalSubscribe(*this, Signal::ScrollBarScrolledPrev, win);
    signalSubscribe(*this, Signal::ScrollBarScrolledNext, win);
    signalSubscribe(*this, Signal::ScrollBarMovedCursor, win);
}

WindowScrollBar::WindowScrollBar(const Point & pos, const Size & sz, bool vertical, WindowListBox & win) : Window(&win)
{
    if(vertical) setState(FlagVertical);
    resetState(FlagModality);

    setPosition(pos);
    setSize(sz);

    signalSubscribe(win, Signal::WindowPageUp);
    signalSubscribe(win, Signal::WindowPageDown);
    signalSubscribe(win, Signal::WindowScrolledPrev);
    signalSubscribe(win, Signal::WindowScrolledNext);
    signalSubscribe(win, Signal::ListboxChangedList);

    signalSubscribe(*this, Signal::ScrollBarScrolledPrev, win);
    signalSubscribe(*this, Signal::ScrollBarScrolledNext, win);
    signalSubscribe(*this, Signal::ScrollBarMovedCursor, win);
}

void WindowScrollBar::signalReceive(int sig, const SignalMember* sm)
{
    switch(sig)
    {
	case Signal::WindowPageUp:
	case Signal::WindowPageDown:
	case Signal::WindowScrolledPrev:
	case Signal::WindowScrolledNext:
	case Signal::ListboxChangedList:
	{
	    const WindowListBox* list = dynamic_cast<const WindowListBox*>(sm);
	    if(list) setCursorPosition(*list);
	}
	break;
	default: break;
    }
}

bool WindowScrollBar::isVertical(void) const
{
    return checkState(FlagVertical);
}

Rect WindowScrollBar::scrollArea(void) const
{
    return rect();
}

void WindowScrollBar::setCursorPosition(const WindowListBox & listBox)
{
    setCursorPosition(listBox.itemsCount(), listBox.itemsVisible(), listBox.topItemIndex());
}

void WindowScrollBar::setCursorPosition(u32 count, u32 visible, s32 index)
{
    const Rect & area = scrollArea();
    Point curpos = cursorPosition();
    int freesz = (isVertical() ? area.h : area.w) - cursorSize();
    u32 offset = 0;

    if(0 < freesz)
    {
	if(count <= visible)
	    offset = freesz / 2;
        else
	    offset = index * freesz / (count - visible);

	if(isVertical())
	{
	    curpos.x = area.x;
	    curpos.y = area.y + offset;
	}
	else
	{
	    curpos.x = area.x + offset;
	    curpos.y = area.y;
	}

	textureCursor().setPosition(curpos);
        renderWindow();
    }
}

void WindowScrollBar::windowMoveEvent(const Point &)
{
    const WindowListBox* list = dynamic_cast<const WindowListBox*>(parent());
    if(list) setCursorPosition(*list);
}

void WindowScrollBar::windowResizeEvent(const Size &)
{
    const WindowListBox* list = dynamic_cast<const WindowListBox*>(parent());
    if(list) setCursorPosition(*list);
}

bool WindowScrollBar::mousePressEvent(const ButtonEvent & be)
{
    if(be.isButtonLeft() && (textureCursor().area() & be.position()))
    {
	setState(FlagButtonPressed);
	return true;
    }
    else
    if(be.isButtonLeft() && (scrollArea() & be.position()))
    {
	Point pos1 = be.position();
	Point pos2 = cursorPosition();
	const Rect & localArea = rect();
	int cursz = cursorSize();

	if(isVertical() && pos1.y > localArea.h - cursz)
	    pos1.y = localArea.h - cursz;
	else
	if(! isVertical() && pos1.x > localArea.w - cursz)
	    pos1.x = localArea.w - cursz;

        const WindowListBox* list = dynamic_cast<const WindowListBox*>(parent());
	if(list && list->itemsVisible() < list->itemsCount())
	{
	    textureCursor().setPosition(isVertical() ? Point(pos2.x, pos1.y) : Point(pos1.x, pos2.y));
	    signalEmit(Signal::ScrollBarMovedCursor);
	    renderWindow();
	}

	return true;
    }

    return false;
}

bool WindowScrollBar::mouseReleaseEvent(const ButtonEvent & be)
{
    if(be.isButtonLeft() && (textureCursor().area() & be.position()))
    {
	resetState(FlagButtonPressed);
	return true;
    }
    return false;
}

bool WindowScrollBar::scrollUpEvent(const Point & pos)
{
    return scrollPrev();
}

bool WindowScrollBar::scrollDownEvent(const Point & pos)
{
    return scrollNext();
}

bool WindowScrollBar::scrollPrev(void)
{
    signalEmit(Signal::ScrollBarScrolledPrev);
    return true;
}

bool WindowScrollBar::scrollNext(void)
{
    signalEmit(Signal::ScrollBarScrolledNext);
    return true;
}

bool WindowScrollBar::mouseMotionEvent(const Point & pos1, u32 buttons)
{
    if((buttons & ButtonLeft) &&
	checkState(FlagButtonPressed))
    {
	int cursz = cursorSize();
	const Rect & scroll = scrollArea();

	if((isVertical() && scroll.y < pos1.y && pos1.y < scroll.y + scroll.h - cursz) ||
	    (! isVertical() && scroll.x < pos1.x && pos1.x < scroll.x + scroll.w - cursz))
	{
    	    const WindowListBox* list = dynamic_cast<const WindowListBox*>(parent());
	    if(list && list->itemsVisible() < list->itemsCount())
	    {
		Point pos2 = cursorPosition();
		textureCursor().setPosition(isVertical() ? Point(pos2.x, pos1.y) : Point(pos1.x, pos2.y));
		signalEmit(Signal::ScrollBarMovedCursor);
	    }

	    return true;
	}
    }

    return false;
}

int WindowScrollBar::scrollAreaSize(void) const
{
    return isVertical() ? scrollArea().h : scrollArea().w;
}

int WindowScrollBar::cursorSize(void) const
{
    return isVertical() ? textureCursorConst().height() : textureCursorConst().width();
}

const Point & WindowScrollBar::cursorPosition(void) const
{
    return textureCursorConst().position();
}

/* WindowButton */
WindowButton::WindowButton(Window* win) : WindowToolTipArea(win), hotkey(Key::NONE)
{
    resetState(FlagModality);
    setState(FlagKeyHandle);

    signalSubscribe(*this, Signal::ButtonTimerComplete);
}

WindowButton::WindowButton(const WindowButton & win) : WindowToolTipArea(win), hotkey(win.hotkey)
{
    signalSubscribe(*this, Signal::ButtonTimerComplete);
}


u32 WindowButton::renderButtonComplete(u32 tick, void* ptr)
{
    if(ptr)
    {
	// ptr always WindowButton type
        WindowButton* win = static_cast<WindowButton*>(ptr);
        win->signalEmit(Signal::ButtonTimerComplete);
    }

    return 0;
}

void WindowButton::setInformed(bool f)
{
    setState(FlagButtonInformed, f);
}

void WindowButton::renderWindow(void)
{
    const Texture* tx = NULL;

    if(isDisabled())
	tx = textureDisabled();
    else
    if(isPressed())
	tx = texturePressed();
    else
    if(isFocused() && textureFocused() && textureFocused()->isValid())
	tx = textureFocused();
    else
    if(checkState(FlagButtonInformed) && textureInformed() && textureInformed()->isValid())
	tx = textureInformed();
    else
	tx = textureReleased();

    if(tx && tx->isValid() && ! size().isEmpty())
    {
	Point dst(0, 0);

	if(checkState(FlagButtonCentered))
    	    dst = dst + (size() - tx->size()) / 2;

	renderTexture(*tx, dst);
    }
}

void WindowButton::setPressed(bool f)
{
    if(f && ! isPressed())
    {
	setState(FlagButtonPressed);
	signalEmit(Signal::ButtonPressed);
    }
    else
    if(!f && isPressed())
    {
	resetState(FlagButtonPressed);
	signalEmit(Signal::ButtonReleased);
	if(isAreaPoint(Display::mouseCursorPosition()))
	    setClickedComplete();
    }

    renderWindow();
}

void WindowButton::setReleased(void)
{
    resetState(FlagButtonPressed);
    signalEmit(Signal::ButtonReleased);
    renderWindow();
}

void WindowButton::setClickedComplete(void)
{
    signalEmit(Signal::ButtonClicked);
    if(action() && parent())
    {
	//SignalMember::signalEmit(*parent(), action());
	pushEventAction(action(), parent(), this);
    }
}

void WindowButton::setClicked(void)
{
    if(! checkState(FlagButtonDisabled) &&
	! checkState(FlagButtonPressed))
    {
	setPressed(true);
	SDL_AddTimer(100, renderButtonComplete, this);
    }
}

void WindowButton::setDisabled(bool f)
{
    setState(FlagButtonDisabled, f);
    renderWindow();
}

void WindowButton::setAction(int action)
{
    setResultCode(action);
}

bool WindowButton::isDisabled(void) const
{
    return checkState(FlagButtonDisabled);
}

bool WindowButton::isPressed(void) const
{
    return checkState(FlagButtonPressed);
}

bool WindowButton::isReleased(void) const
{
    return ! isPressed();
}

bool WindowButton::isAction(int val) const
{
    return action() == val;
}

int WindowButton::action(void) const
{
    return resultCode();
}

bool WindowButton::mousePressEvent(const ButtonEvent & st)
{
    if(! checkState(FlagButtonDisabled) && st.isButtonLeft() && ! checkState(FlagButtonPressed))
    {
	setPressed(true);
	return true;
    }
    return false;
}

bool WindowButton::mouseReleaseEvent(const ButtonEvent & st)
{
    if(! checkState(FlagButtonDisabled) && st.isButtonLeft() && checkState(FlagButtonPressed))
    {
	setPressed(false);
	return true;
    }
    return false;
}

void WindowButton::mouseFocusEvent(void)
{
    renderWindow();
}

void WindowButton::mouseLeaveEvent(void)
{
    if(! checkState(FlagButtonDisabled) && checkState(FlagButtonPressed))
	setPressed(false);
    else
	renderWindow();
}

bool WindowButton::keyPressEvent(const KeySym & key)
{
    if(hotkey && hotkey == key.keycode() && ! isDisabled() && isReleased())
    {
        setPressed(true);
	return true;
    }
    return false;
}

bool WindowButton::keyReleaseEvent(const KeySym & key)
{
    if(hotkey == key.keycode() && ! isDisabled() && isPressed())
    {
        setReleased();
        setClickedComplete();
	return true;
    }
    return false;
}

void WindowButton::signalReceive(int sig, const SignalMember* sm)
{
    if(sig == Signal::ButtonTimerComplete && sm == this)
    {
        setReleased();
        setClickedComplete();
    }
}

void WindowButton::setHotKey(const std::string & str)
{
    if(str.size())
	hotkey = Key::toKey(str);
}

void WindowButton::setHotKey(int key)
{
    hotkey = key;
}

bool WindowButton::isHotKey(int val) const
{
    return hotKey() == val;
}

int WindowButton::hotKey(void) const
{
    return hotkey;
}

std::string WindowButton::toString(void) const
{
    return StringFormat("%1, disabled(%2), pressed(%3), released(%4)").arg(Window::toString()).
	    arg(isDisabled() ? 1 : 0).arg(isPressed() ? 1 : 0).arg(isReleased() ? 1 : 0);
}

/* TextureButton */
TextureButton::TextureButton(Window & win) : WindowButton(& win)
{
}

TextureButton::TextureButton(const Texture & tx1, const Texture & tx2, const Point & pos, int action, Window & win)
    : WindowButton(& win)
{
    setPosition(pos);
    setAction(action);
    setSprites(tx1, tx2);
}

const Texture* TextureButton::textureDisabled(void) const
{
    return texturePressed();
}

const Texture* TextureButton::texturePressed(void) const
{
    return &txPress;
}

const Texture* TextureButton::textureReleased(void) const
{
    return &txRelease;
}

void TextureButton::setSprites(const Texture & tx1, const Texture & tx2)
{
    txRelease = tx1;
    txPress = tx2;

    setSize(tx1.size() < tx2.size() ? tx2.size() : tx1.size());

   if(isVisible())
        DisplayScene::setDirty(true);
}

/* WindowCheckBox */
bool WindowCheckBox::mousePressEvent(const ButtonEvent & st)
{
    if(! checkState(FlagButtonDisabled) && st.isButtonLeft())
    {
	setPressed(! checkState(FlagButtonPressed));
        return true;
    }
    return false;
}
    
void WindowCheckBox::mouseLeaveEvent(void)
{
    // skip WindowButton::mouseLeaveEvent
}

bool WindowCheckBox::mouseReleaseEvent(const ButtonEvent & st)
{
    return true;
}

/* WindowToolTipArea */
void WindowToolTipArea::toolTipInit(const std::string & str)
{
    toolTipInit(str, FontRenderSystem(), Color::Black, Color::Wheat, Color::MidnightBlue);
}

void WindowToolTipArea::toolTipInit(const std::string & str, const FontRender & frs, const Color & fncolor, const Color & bgcolor, const Color & rtcolor)
{
    Texture text = Display::renderText(frs, str, fncolor);
    tooltip = Display::renderRect(rtcolor, bgcolor, text.size() + Size(6, 6));

    Display::renderTexture(text, text.rect(), tooltip, Rect(Point(3, 3), text.size()));
}

/* WindowTextAreaItem */
WindowTextAreaItem::WindowTextAreaItem(const TexturePos & tp, Window & win) : WindowListItem(win), content(tp)
{
    setSize(Size(win.width(), tp.height()));
}

void WindowTextAreaItem::renderWindow(void)
{
    renderTexture(content);
}

/* WindowTextArea */
WindowTextArea & WindowTextArea::appendString(const Texture & tx, const Point & offset)
{
    insertItem(new WindowTextAreaItem(TexturePos(tx, offset), *this));
    return *this;
}

WindowTextArea & WindowTextArea::appendString(const FontRender & frs, const UnicodeString & str, const Color & col, int halign, bool wrap)
{
    if(wrap)
    {
	UCStringList list = frs.splitUCStringWidth(UCString::parseUnicode(str, FBColors(col.toColorIndex())), width());
	for(auto it = list.begin(); it != list.end(); ++it)
	    appendString(frs, *it, halign, false);
    }
    else
	appendString(frs, UCString::parseUnicode(str, FBColors(col.toColorIndex())), halign, false);

    return *this;
}

WindowTextArea & WindowTextArea::appendString(const FontRender & frs, const UCString & str, int halign, bool wrap)
{
    if(wrap)
    {
	UCStringList list = frs.splitUCStringWidth(str, width());
	for(auto it = list.begin(); it != list.end(); ++it)
	    appendString(frs, *it, halign, false);
    }
    else
    if(str.length())
    {
	Texture tx = Display::renderText(frs, str);
	Point offset = Point(0, 0);

	switch(halign)
	{
	    case AlignRight:	offset.x = width() - tx.width(); break;
	    case AlignCenter:	offset.x = (width() - tx.width()) / 2; break;
	    default: break;
	}

	appendString(tx, offset);
    }
    else
    {
	appendSpacer(frs);
    }

    return *this;
}

WindowTextArea & WindowTextArea::appendSpacer(const FontRender & frs)
{
    int height = frs.lineSkipHeight();
    insertItem(new WindowTextAreaItem(TexturePos(Display::createTexture(Size(width(), height)), Point()), *this));
    return *this;
}

void WindowTextArea::renderWindow(void)
{
    renderBackground();
    renderItems();
}

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

#ifndef _SWE_WINDOW_GUI_
#define _SWE_WINDOW_GUI_

#include <vector>

#include "window.h"

namespace SWE
{

    class WindowScrollBar;
    struct KeySym;

    class WindowTexture : public Window
    {
    protected:
        Texture			texture;

    public:
        WindowTexture(Window* win) : Window(win) {}
        WindowTexture(const Texture &, Window*);

        void			setTexture(const Texture & tx);
        void			renderWindow(void) override;
    };

    class WindowToolTipArea : public Window
    {
        Texture			tooltip;

    protected:
        const Texture*      	tooltipTexture(void) const override
        {
            return & tooltip;
        }

    public:
        WindowToolTipArea(Window* win) : Window(win) {}
        WindowToolTipArea(const Rect & area, Window & win) : Window(area, area, & win) {}
        WindowToolTipArea(const WindowToolTipArea & win) : Window(win), tooltip(win.tooltip) {}

        void			toolTipInit(const std::string &, const FontRender &, const Color & fn, const Color & bg, const Color & rt);
        void			toolTipInit(const std::string &);
    };

    class WindowListItem : public WindowToolTipArea
    {
    public:
        WindowListItem(Window &);

        bool			isSelected(void) const;
    };

    class WindowListBox : public Window
    {
    protected:
        std::vector<WindowListItem*> items;
        int			indexTop, indexSelected;

        bool			isVertical(void) const;
        virtual Rect		itemsArea(void) const;

    protected:
        bool			mouseClickEvent(const ButtonsEvent &) override;
        bool			scrollUpEvent(const Point &) override;
        bool			scrollDownEvent(const Point &) override;
        bool			userEvent(int, void*) override;
        void			signalReceive(int, const SignalMember*) override;

    public:
        WindowListBox(bool vertical, Window &);
        WindowListBox(const Point &, const Size &, bool vertical, Window &);
        ~WindowListBox();

        void			renderItems(void) const;

        void			insertItem(WindowListItem*, bool render = true);
        bool			removeItem(const WindowId &);
        void			removeAllItems(void);

        WindowListItem*		findItem(const WindowId &);
        const WindowListItem*	findItemConst(const WindowId &) const;

        WindowListItem*		topItem(void);
        const WindowListItem*	topItemConst(void) const;

        WindowListItem*		selectedItem(void);
        const WindowListItem*	selectedItemConst(void) const;

        WindowListItem*		itemIndex(int);
        const WindowListItem*	itemIndexConst(int) const;

        int			itemsCount(void) const;
        int			itemsVisible(void) const;

        void			setTopItemIndex(int);
        int			topItemIndex(void) const;

        void			resetSelectedItem(void);
        void			setSelectedItemIndex(int);
        int			selectedItemIndex(void) const;

        bool			pageUp(void);
        bool			pageDown(void);
        bool			scrollPrev(void);
        bool			scrollNext(void);
        bool			selectedPrev(void);
        bool			selectedNext(void);
    };

    class WindowScrollBar : public Window
    {
    protected:
        virtual TexturePos &	textureCursor(void) = 0;
        virtual const TexturePos &
				textureCursorConst(void) const = 0;
        virtual Rect		scrollArea(void) const;

        bool			isVertical(void) const;

    protected:
        void			windowMoveEvent(const Point &) override;
        void			windowResizeEvent(const Size &) override;
        bool			mousePressEvent(const ButtonEvent &) override;
        bool			mouseReleaseEvent(const ButtonEvent &) override;
        bool			mouseMotionEvent(const Point &, u32 buttons) override;
        bool			scrollUpEvent(const Point &) override;
        bool			scrollDownEvent(const Point &) override;
        void			signalReceive(int, const SignalMember*) override;

    public:
        WindowScrollBar(bool, WindowListBox &);
        WindowScrollBar(const Point &, const Size &, bool vertical, WindowListBox &);

        void			setCursorPosition(const WindowListBox &);
        void			setCursorPosition(u32 count, u32 visible, s32 index);

        const Point 	&	cursorPosition(void) const;
        int			cursorSize(void) const;

        int			scrollAreaSize(void) const;

        bool			scrollPrev(void);
        bool			scrollNext(void);
    };


    class WindowButton : public WindowToolTipArea
    {
        int         hotkey;

    protected:
        virtual const Texture* 	textureFocused(void) const
        {
            return NULL;
        }
        virtual const Texture* 	textureDisabled(void) const
        {
            return texturePressed();
        }
        virtual const Texture* 	texturePressed(void) const
        {
            return NULL;
        }
        virtual const Texture* 	textureReleased(void) const
        {
            return NULL;
        }
        virtual const Texture* 	textureInformed(void) const
        {
            return NULL;    // second sprite, if informed mode
        }

        bool        		mousePressEvent(const ButtonEvent &) override;
        bool        		mouseReleaseEvent(const ButtonEvent &) override;
        void        		mouseLeaveEvent(void) override;
        void        		mouseFocusEvent(void) override;
        bool			keyPressEvent(const KeySym &) override;
        bool			keyReleaseEvent(const KeySym &) override;
        void			signalReceive(int, const SignalMember*) override;

        void			setClickedComplete(void);
        void			setReleased(void);

        static u32		renderButtonComplete(u32 tick, void* ptr);

    public:
        WindowButton(Window* win = NULL);
        WindowButton(const WindowButton &);

        void        		renderWindow(void) override;

        void        		setAction(int);
        void       		setDisabled(bool);
        void			setPressed(bool);
        void			setClicked(void);
        void			setInformed(bool);
        void       		setHotKey(int);
        void       		setHotKey(const std::string &);

        bool        		isAction(int) const;
        bool        		isDisabled(void) const;
        bool       		isPressed(void) const;
        bool        		isReleased(void) const;
        bool        		isHotKey(int) const;

        int         		action(void) const;
        int         		hotKey(void) const;

        std::string		toString(void) const override;
    };

    class WindowCheckBox : public WindowButton
    {
    protected:
        void 			mouseLeaveEvent(void) override;
        bool 			mousePressEvent(const ButtonEvent &) override;
        bool 			mouseReleaseEvent(const ButtonEvent &) override;

    public:
        WindowCheckBox(Window & win) : WindowButton(& win) {}

        void			setChecked(bool f)
        {
            setPressed(f);
        }
        bool			isChecked(void) const
        {
            return isPressed();
        }
    };

    class TextureButton : public WindowButton
    {
        Texture     txRelease, txPress;

    protected:
        const Texture* textureDisabled(void) const;
        const Texture* texturePressed(void) const;
        const Texture* textureReleased(void) const;


    public:
        TextureButton(Window &);
        TextureButton(const Texture &, const Texture &, const Point &, int, Window &);

        void			setSprites(const Texture & release, const Texture & press);
    };

    class WindowTextAreaItem : public WindowListItem
    {
        TexturePos		content;

    public:
        WindowTextAreaItem(const TexturePos &, Window &);
        void renderWindow(void) override;
    };

    class WindowTextArea : public WindowListBox
    {
    protected:
        bool			mouseClickEvent(const ButtonsEvent &) override
        {
            return false;
        }

    public:
        WindowTextArea(Window & win) : WindowListBox(true, win) {}
        WindowTextArea(const Rect & pos, Window & win) : WindowListBox(pos, pos, true, win) {}

        WindowTextArea 	&	appendString(const FontRender &, const UnicodeString &, const Color &, int halign = AlignLeft, bool wrap = false);
        WindowTextArea 	&	appendString(const FontRender &, const UCString &, int halign = AlignLeft, bool wrap = false);
        WindowTextArea 	&	appendString(const Texture &, const Point &);
        WindowTextArea 	&	appendSpacer(const FontRender &);

        void			clear(void)
        {
            removeAllItems();
        }

        virtual void		renderBackground(void) {}
        void			renderWindow(void) override;
    };

} // SWE
#endif

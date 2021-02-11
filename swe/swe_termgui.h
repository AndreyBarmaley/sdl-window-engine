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

#ifndef _SWE_TERMWIN_GUI_
#define _SWE_TERMWIN_GUI_

#ifndef SWE_DISABLE_TERMGUI

#include <array>
#include <memory>
#include "swe_termwin.h"

#ifdef __clang__
#pragma clang diagnostic ignored "-Woverloaded-virtual"
#endif

/// @brief пространство SWE
namespace SWE
{
    /// @brief пространство TermGUI
    namespace TermGUI
    {
	/// @brief Theme colors base class
        class ThemeColors
        {
        public:
	    virtual ~ThemeColors() {}

    	    enum type_t { ColorBackground = 0, ColorBorderLine, ColorHeaderText, ColorBodyText,
    		    ColorButtonBackground, ColorButtonBracket, ColorButtonFirstText, ColorButtonBodyText,
    		    ColorFocusedButtonBackground, ColorFocusedButtonBracket, ColorFocusedButtonFirstText, ColorFocusedButtonBodyText,
    		    ColorInputFieldBackground, ColorFocusedInputFieldBackground, ColorInputFieldText, ColorInputCursor,
		    ColorListSelectedBackground, ColorListSelectedText };

#define ColorsCounts (TermGUI::ThemeColors::ColorListSelectedText + 1)

	    static void		setColor(const type_t & type, const ColorIndex & col);
	    static void		setTheme(const LineType &, std::array<ColorIndex, ColorsCounts>);
	    static void		resetTheme(void);

protected:
            virtual LineType	lineType(void) const;
	    virtual ColorIndex	colorBackground(void) const;
            virtual ColorIndex	colorText(void) const;
            virtual ColorIndex	colorLine(void) const;
        };

	/// @brief класс текстовой метки
        class LabelAction : public TermWindow, public ThemeColors
        {
            UnicodeString 	content;
            int         	hotkey;

        protected:
            LabelAction(TermWindow*);

            bool        	keyPressEvent(const KeySym &) override;
            void        	mouseFocusEvent(void) override;
            void        	mouseLeaveEvent(void) override;
            bool        	mouseClickEvent(const ButtonsEvent &) override;

	    /// @private
            int			parseHotKey(const UnicodeString &) const;
	    /// @private
            void		clickAction(void);
	    /// @private
            void        	renderLabel(void);

	    ColorIndex		colorBackground(void) const override;
            ColorIndex		colorText(void) const override;

	    FBColors        	defaultColors(void) const override
	    {
		return FBColors(colorText(), colorBackground());
	    }

	    /// @private
	    virtual TermSize	labelSize(const UnicodeString &) const;

	    virtual ColorIndex	colorBackground(bool focused) const;
	    virtual ColorIndex	colorText(bool focused) const;
	    virtual ColorIndex	colorHotKey(bool focused) const;

        public:
            LabelAction(const UnicodeString &, int action, TermWindow &);
            LabelAction(const UnicodeString &, int action, const TermPos &, TermWindow &);

            void		setLabel(const UnicodeString &);
            const UnicodeString & label(void) const;
            bool		isLabel(const UnicodeString &) const;

            void		setAction(int);
            int			action(void) const;
            bool		isAction(int) const;

            void		setHotKey(int);
            int			hotKey(void) const;
            bool		isHotKey(int) const;

            void		setHotKeyDisabled(bool);
            bool		isHotKeyDisabled(void) const;

            void		setDisabled(bool);
            bool		isDisabled(void) const;

            void        	setSelected(bool);
            bool        	isSelected(void) const;

            void        	renderWindow(void) override;
	    const char* 	className(void) const override { return "SWE::TermGUI::LabelAction"; }
        };

	/// набор системных меток для TextButton
        enum buttons_t { ButtonOk = 1 << 1, ButtonYes = 1 << 2, ButtonNo = 1 << 3, ButtonCancel = 1 << 4 };

	//// @brief класс текстовой кнопки
        class TextButton : public LabelAction
        {
	protected:
            TextButton(TermWindow* term) : LabelAction(term) {}

	    virtual ColorIndex	colorBracket(bool focused) const;

	    /// @private
            TermSize		labelSize(const UnicodeString &) const override;

        public:
            TextButton(const buttons_t &, TermWindow &);
            TextButton(const UnicodeString &, int action, TermWindow &);
            TextButton(const UnicodeString &, int action, const TermPos &, TermWindow &);

            void		setButton(const buttons_t &);

            void        	renderWindow(void) override;
	    const char* 	className(void) const override { return "SWE::TermGUI::TextButton"; }
        };

	/* LabelActionPtr */
        struct LabelActionPtr : std::unique_ptr<LabelAction>
        {
            LabelActionPtr(LabelAction* ptr)
            {
		reset(ptr);
	    }

            LabelActionPtr(const std::string & str, int action, const TermPos & pos, TermWindow & term)
            {
		reset(new LabelAction(str, action, pos, term));
	    }
        };

	/// @brief класс группы для LabelAction
        class LabelActionGroup : public std::list<LabelActionPtr>
        {
        public:
            LabelActionGroup() {}

            LabelAction*	addLabel(const std::string &, int action, const TermPos &, TermWindow &);
            LabelAction*	addLabel(LabelAction*);

            bool		findLabel(const LabelAction*) const;
            LabelAction*	findLabel(const UnicodeString &) const;
            LabelAction*	findAction(int) const;
            LabelAction*	findHotKey(int) const;
            LabelAction*	findSelected(void) const;
            LabelAction*	findIndex(size_t) const;

            bool		setSelected(const LabelAction*);
            void		resetSelected(const LabelAction* exclude = nullptr);
            void		nextSelected(void);
            void		prevSelected(void);
            void		firstSelected(void);
            void		lastSelected(void);
            bool		isFirstSelected(void) const;
            bool		isLastSelected(void) const;

            int			index(LabelAction*) const;
            size_t		count(void) const { return size(); }
            size_t		rows(void) const;
            size_t		cols(void) const;
        };

	//// @brief класс группы для TextButton
        class ButtonsGroup : public LabelActionGroup
        {
        public:
            ButtonsGroup(int buttons, TermWindow &);
        };

	/// @brief класс пустого текстового окна с заголовком
        class HeaderAreaBox : public TermWindow, public ThemeColors
        {
            UnicodeString	header;

        protected:
            HeaderAreaBox(const UnicodeString & us, TermWindow & term);
            HeaderAreaBox(const UnicodeString & us, const FontRender & frs, Window* win);

            bool                keyPressEvent(const KeySym &) override;
	    LineType		lineType(void) const override;

	    FBColors        	defaultColors(void) const override
	    {
		return FBColors(colorText(), colorBackground());
	    }

	    virtual ColorIndex	colorHeader(void) const;

        public:
            HeaderAreaBox(const UnicodeString &, const TermSize &, TermWindow &);

            void		renderBox(void);
            void		renderWindow(void) override;
	    const char*		className(void) const override { return "SWE::TermGUI::HeaderAreaBox"; }
        };

	/// @brief класс пустого текстового окна с заголовком и группой кнопок
        class ButtonsAreaBox : public HeaderAreaBox
        {
            ButtonsGroup	btnsGroup;

    	    void		initAreaBox(void);

        protected:
    	    ButtonsAreaBox(const UnicodeString & us, int btns, TermWindow & term)
        	: HeaderAreaBox(us, term), btnsGroup(btns, *this)
	    {
		initAreaBox();
    	    }

    	    ButtonsAreaBox(const UnicodeString & us, int btns, const FontRender & frs, Window* win)
        	: HeaderAreaBox(us, frs, win), btnsGroup(btns, *this)
    	    {
		initAreaBox();
    	    }

            void		signalReceive(int, const SignalMember*) override;
            bool                keyPressEvent(const KeySym &) override;

            void		setButtonsPosition(void);
            void		setHotKeyDisabled(bool);
            void		setButtonsSubscribe(int);

        public:
    	    ButtonsAreaBox(const UnicodeString & str, const TermSize & tsz, int btns, TermWindow & term)
        	: HeaderAreaBox(str, tsz, term), btnsGroup(btns, *this)
    	    {
		initAreaBox();
    	    }

	    ButtonsGroup &	buttonsGroup(void);
	    const ButtonsGroup &buttonsGroup(void) const;

	    const char*		className(void) const override { return "SWE::TermGUI::ButtonsAreaBox"; }

            void		renderButtons(void);
            void		renderWindow(void) override;
        };

	/// @brief класс диалога информационного сообщения, с заголовком и группой кнопок
        class MessageBox : protected UnicodeList, public ButtonsAreaBox
        {
        protected:

        public:
            MessageBox(const UnicodeString &, const UnicodeString &, int buttons, TermWindow &);
            MessageBox(const UnicodeString &, const UnicodeString &, int buttons, const FontRender &, Window*);

	    const char*		className(void) const override { return "SWE::TermGUI::MessageBox"; }
            void		renderWindow(void) override;
        };

	/// @brief класс диалога ввода строки текста, с заголовком и группой кнопок
        class InputBox : public ButtonsAreaBox
        {
            std::string		sResult;

        protected:
            bool                keyPressEvent(const KeySym &) override;

            void		setInputFocused(bool);
            bool		checkInputFocused(void) const;

	    virtual ColorIndex	colorFieldText(void) const;
	    virtual ColorIndex	colorFieldCursor(void) const;
            virtual ColorIndex	colorFieldBackground(bool focused) const;

        public:
            InputBox(const UnicodeString &, size_t cols, const std::string & def, TermWindow &);
            InputBox(const UnicodeString &, size_t cols, const std::string & def, const FontRender &, Window*);

            const std::string &	result(void) const
            {
                return sResult;
            }

	    const char*		className(void) const override { return "SWE::TermGUI::InputBox"; }
            void		renderWindow(void) override;
        };

	/// @brief класс диалога выбора из списка значений, с заголовком и группой кнопок
        class ListBox : public ButtonsAreaBox
        {
	    UnicodeList		content;
            std::string		sResult;
            int			selected;
            int			skipped;

            virtual ColorIndex	colorBackground(bool selected) const;
            virtual ColorIndex	colorText(bool selected) const;

        protected:
            bool                keyPressEvent(const KeySym &) override;
            bool		mouseClickEvent(const ButtonsEvent &) override;
            bool		scrollUpEvent(void) override;
            bool		scrollDownEvent(void) override;
            void		windowVisibleEvent(bool) override;

            bool		scrollUpContent(void);
            bool		scrollDownContent(void);

        public:
            ListBox(const UnicodeString &, const UnicodeList &, size_t rows, TermWindow &);
            ListBox(const UnicodeString &, const UnicodeList &, size_t rows, const FontRender &, Window*);

            const std::string &	result(void) const
            {
                return sResult;
            }

	    const char*		className(void) const override { return "SWE::TermGUI::ListBox"; }
            void		renderWindow(void) override;
        };

    } // namespace TermGUI

    class CommandConsole : public TermWindow
    {
        UnicodeList             content;
        std::string             command;
        
    protected:
        FBColors                defaultColors(void) const override;
        bool                    keyPressEvent(const KeySym &) override;
        bool                    textInputEvent(const std::string &) override;

        virtual bool            actionCommand(const std::string &);
        
    public:
        CommandConsole(const Size &, const FontRender &, Window &);
        CommandConsole(const TermSize &, TermWindow &);
        
        void                    renderWindow(void) override;
        
        const std::string &     commandLine(void) const { return command; }
        const UnicodeList &     contentLines(void) const { return content; }
        
        void                    commandLineClear(void);
        void                    contentLinesClear(void);
        
        void                    contentLinesAppend(const std::string &);
        void                    contentLinesAppend(const StringList &);
    };

} // SWE
#endif // SWE_DISABLE_TERMGUI
#endif
